#include "services/Worker.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
extern const uint8_t caBundle[] asm("_binary_data_cert_x509_crt_bundle_bin_start");
Worker* Worker::active_ = nullptr;
bool Worker::begin() {
  queue_ = xQueueCreate(4, sizeof(Request));
  if (!queue_) return false;
  WeatherData initial;
  strlcpy(initial.message, config_.weatherEnabled ? "Aguardando Wi-Fi" : "Clima desativado no painel", sizeof(initial.message));
  state_.weather(initial);
  active_ = this;
  if (xTaskCreatePinnedToCore(task, "hub-services", 12288, this, 1, nullptr, 0) != pdPASS) {
    vQueueDelete(queue_); queue_ = nullptr; active_ = nullptr; return false;
  }
  return true;
}
bool Worker::request(Request request) { return queue_ && xQueueSend(queue_, &request, 0) == pdTRUE; }
void Worker::task(void* context) { static_cast<Worker*>(context)->run(); }
void Worker::onMessage(char*, uint8_t* payload, unsigned int length) {
  if (!active_) return;
  char text[96];
  const size_t count = length < sizeof(text) - 1 ? length : sizeof(text) - 1;
  memcpy(text, payload, count); text[count] = 0;
  uint8_t level = 0;
  if (length <= 384) {
    StaticJsonDocument<512> document;
    if (!deserializeJson(document, payload, length) && document["text"].is<const char*>()) {
      strlcpy(text, document["text"], sizeof(text));
      int parsedLevel = document["level"] | 0;
      level = parsedLevel >= 0 && parsedLevel <= 2 ? parsedLevel : 0;
    }
  }
  for (char* p = text; *p; ++p) if ((uint8_t)*p < 32) *p = ' ';
  active_->state_.message(text);
  active_->state_.alert(text, level);
}
void Worker::run() {
  secure_.setCACertBundle(caBundle);
  secure_.setHandshakeTimeout(8);
  mqtt_.setClient(config_.mqttTLS ? static_cast<Client&>(secure_) : static_cast<Client&>(plain_));
  mqtt_.setServer(config_.mqttHost, config_.mqttPort);
  mqtt_.setCallback(onMessage);
  mqtt_.setSocketTimeout(2);
  mqtt_.setKeepAlive(30);
  if (!mqtt_.setBufferSize(512)) state_.alert("MQTT: memoria insuficiente", 2);
  uint32_t lastMQTT = millis() - 15000, lastTelemetry = 0;
  uint32_t lastWeather = millis() - 600000, lastHA = millis() - 30000;
  for (;;) {
    if (pauseRequested_) {
      mqtt_.disconnect(); plain_.stop(); secure_.stop(); state_.mqtt(false);
      paused_ = true;
      vTaskDelay(pdMS_TO_TICKS(50));
      continue;
    }
    paused_ = false;
    if (WiFi.status() != WL_CONNECTED) {
      if (mqtt_.connected()) mqtt_.disconnect();
      state_.mqtt(false);
      if (config_.weatherEnabled) {
        WeatherData waiting = state_.snapshot().weather;
        if (!waiting.valid) {
          strlcpy(waiting.message, "Aguardando Wi-Fi", sizeof(waiting.message));
          state_.weather(waiting);
        }
      }
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }
    uint32_t now = millis();
    const bool weatherClockReady = time(nullptr) > 1700000000;
    if (config_.weatherEnabled && !weatherClockReady) {
      WeatherData waiting = state_.snapshot().weather;
      if (!waiting.valid) {
        strlcpy(waiting.message, "Aguardando horario NTP", sizeof(waiting.message));
        state_.weather(waiting);
      }
    }
    if (config_.mqttHost[0] && ((config_.modules & (1u << 3)) || (config_.modules & (1u << 4)))) {
      bool clockReady = time(nullptr) > 1700000000;
      if (!mqtt_.connected() && (!config_.mqttTLS || clockReady) && uint32_t(now - lastMQTT) >= 15000) {
        lastMQTT = now;
        char clientID[40]; snprintf(clientID, sizeof(clientID), "ttgo-hub-%012llx", ESP.getEfuseMac());
        bool connected = config_.mqttUser[0]
          ? mqtt_.connect(clientID, config_.mqttUser, config_.mqttPassword)
          : mqtt_.connect(clientID);
        if (connected && !mqtt_.subscribe(config_.mqttTopic)) state_.alert("MQTT: assinatura falhou", 1);
      }
      mqtt_.loop();
      state_.mqtt(mqtt_.connected());
      if (mqtt_.connected() && uint32_t(now - lastTelemetry) >= 30000) {
        lastTelemetry = now;
        char body[128];
        snprintf(body, sizeof(body), "{\"uptime\":%lu,\"heap\":%lu,\"rssi\":%d}",
          (unsigned long)(now / 1000), (unsigned long)ESP.getFreeHeap(), WiFi.RSSI());
        char topic[128]; snprintf(topic, sizeof(topic), "%s/telemetry", config_.mqttTopic);
        mqtt_.publish(topic, body);
      }
    }
    Request requested;
    bool hasRequest = xQueueReceive(queue_, &requested, 0) == pdTRUE;
    if (hasRequest) {
      if (requested == Request::Weather && config_.weatherEnabled && weatherClockReady && (config_.modules & (1u << 1))) {
        lastWeather = now; weather();
      } else if ((requested == Request::HomeAssistant || requested == Request::Toggle) &&
                 config_.haURL[0] && (config_.modules & (1u << 5))) {
        lastHA = now; homeAssistant(requested == Request::Toggle);
      }
    } else if (config_.weatherEnabled && weatherClockReady && (config_.modules & (1u << 1)) &&
               uint32_t(now - lastWeather) >= (state_.snapshot().weather.valid ? 600000u : 15000u)) {
      lastWeather = now; weather();
    } else if (config_.haURL[0] && (config_.modules & (1u << 5)) && uint32_t(now - lastHA) >= 30000) {
      lastHA = now; homeAssistant(false);
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
namespace {
bool beginHTTP(HTTPClient& http, WiFiClient& plain, WiFiClientSecure& secure, const String& url) {
  http.setConnectTimeout(3000); http.setTimeout(5000); http.useHTTP10(true);
  http.setReuse(false);
  if (url.startsWith("https://")) {
    if (time(nullptr) < 1700000000) return false;
    secure.setCACertBundle(caBundle); secure.setHandshakeTimeout(8);
    return http.begin(secure, url);
  }
  return url.startsWith("http://") && http.begin(plain, url);
}
}
void Worker::weather() {
  WeatherData value = state_.snapshot().weather;
  if (!value.valid) {
    strlcpy(value.message, "Consultando Open-Meteo...", sizeof(value.message));
    state_.weather(value);
  }
  WiFiClientSecure secure; WiFiClient plain; HTTPClient http;
  String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(config_.latitude, 4) +
    "&longitude=" + String(config_.longitude, 4) +
    "&current=temperature_2m,weather_code,wind_speed_10m&daily=temperature_2m_max,temperature_2m_min&forecast_days=1&timezone=auto";
  if (!beginHTTP(http, plain, secure, url)) {
    strlcpy(value.message, "Aguardando NTP / TLS", sizeof(value.message)); state_.weather(value); return;
  }
  value.httpCode = http.GET();
  bool parsed = false;
  if (value.httpCode == 200 && http.getSize() <= 8192) {
    StaticJsonDocument<256> filter;
    filter["current"]["temperature_2m"] = true; filter["current"]["weather_code"] = true;
    filter["current"]["wind_speed_10m"] = true;
    filter["daily"]["temperature_2m_max"] = true; filter["daily"]["temperature_2m_min"] = true;
    DynamicJsonDocument document(1536);
    auto error = deserializeJson(document, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(5));
    parsed = !error && document["current"]["temperature_2m"].is<float>() &&
      document["daily"]["temperature_2m_max"][0].is<float>() && document["daily"]["temperature_2m_min"][0].is<float>();
    if (parsed) {
      value.temperature = document["current"]["temperature_2m"];
      value.wind = document["current"]["wind_speed_10m"] | 0.0f;
      value.code = document["current"]["weather_code"] | 0;
      value.maximum = document["daily"]["temperature_2m_max"][0];
      value.minimum = document["daily"]["temperature_2m_min"][0];
      value.updated = millis(); value.valid = true;
      strlcpy(value.message, "Open-Meteo", sizeof(value.message));
    }
  }
  if (!parsed) snprintf(value.message, sizeof(value.message), value.httpCode == 200 ? "Resposta clima invalida (%d)" : "Falha clima: HTTP %d", value.httpCode);
  Serial.printf("Clima: habilitado=1 HTTP=%d dados=%s latitude=%.6f longitude=%.6f\n", value.httpCode, parsed ? "OK" : "FAIL", config_.latitude, config_.longitude);
  http.end(); state_.weather(value);
}
void Worker::homeAssistant(bool toggle) {
  HAData value = state_.snapshot().ha;
  if (!config_.haToken[0] || !config_.haEntity[0]) return;
  String base = config_.haURL;
  while (base.endsWith("/")) base.remove(base.length() - 1);
  String entity = config_.haEntity;
  String domain = entity.substring(0, entity.indexOf('.'));
  if (toggle && domain != "light" && domain != "switch" && domain != "input_boolean") {
    state_.alert("Entidade permite apenas leitura", 1); return;
  }
  WiFiClientSecure secure; WiFiClient plain; HTTPClient http;
  String url = base + (toggle ? "/api/services/" + domain + "/toggle" : "/api/states/" + entity);
  if (!beginHTTP(http, plain, secure, url)) { state_.alert("HA: verifique URL/NTP/TLS", 1); return; }
  http.addHeader("Authorization", String("Bearer ") + config_.haToken);
  if (toggle) {
    http.addHeader("Content-Type", "application/json");
    value.httpCode = http.POST("{\"entity_id\":\"" + entity + "\"}");
    http.end();
    if (value.httpCode == 200) { state_.alert("Comando HA enviado"); homeAssistant(false); }
    else { snprintf(value.state, sizeof(value.state), "Comando falhou: %d", value.httpCode); state_.ha(value); }
    return;
  }
  value.httpCode = http.GET();
  bool parsed = false;
  if (value.httpCode == 200 && http.getSize() <= 32768) {
    StaticJsonDocument<64> filter; filter["state"] = true;
    StaticJsonDocument<256> document;
    auto error = deserializeJson(document, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(8));
    parsed = !error && document["state"].is<const char*>();
    if (parsed) { strlcpy(value.state, document["state"], sizeof(value.state)); value.valid = true; value.updated = millis(); }
  }
  if (!parsed) snprintf(value.state, sizeof(value.state), "Falha HA: HTTP %d", value.httpCode);
  http.end(); state_.ha(value);
}

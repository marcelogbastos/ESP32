#include "network/Portal.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <math.h>
namespace {
String escaped(const char* input) {
  String value(input);
  value.replace("&", "&amp;"); value.replace("<", "&lt;"); value.replace(">", "&gt;");
  value.replace("\"", "&quot;"); value.replace("'", "&#39;"); return value;
}
String field(const char* label, const char* name, const char* value, size_t maximum, bool secret = false) {
  return "<label>" + String(label) + "<input name='" + name + "' maxlength='" + String(maximum) +
    "' type='" + (secret ? "password" : "text") + "' value='" + (secret ? String("") : escaped(value)) + "'></label>";
}
bool validEntity(const String& entity) {
  if (!entity.length()) return true;
  int dots = 0;
  for (size_t i = 0; i < entity.length(); ++i) {
    char c = entity[i];
    if (c == '.') ++dots;
    else if (!isalnum((unsigned char)c) && c != '_') return false;
  }
  return dots == 1 && entity[0] != '.' && !entity.endsWith(".");
}
bool validHost(const String& host) {
  for (size_t i = 0; i < host.length(); ++i)
    if (!isalnum((unsigned char)host[i]) && host[i] != '.' && host[i] != '-') return false;
  return true;
}
bool validURL(const String& url) {
  if (!url.length()) return true;
  int prefix = url.startsWith("https://") ? 8 : url.startsWith("http://") ? 7 : 0;
  if (!prefix || url.length() <= (size_t)prefix) return false;
  for (size_t i = prefix; i < url.length(); ++i) {
    char c = url[i];
    if (!isalnum((unsigned char)c) && c != '.' && c != '-' && c != ':' && c != '/') return false;
  }
  return true;
}
}
bool Portal::authenticate() {
  if (server_.authenticate("admin", config_.adminPassword)) return true;
  server_.requestAuthentication(DIGEST_AUTH, "TTGO Smart Hub", "Autenticacao necessaria"); return false;
}
void Portal::begin() {
  snprintf(apName_, sizeof(apName_), "TTGO-Hub-%04x", (unsigned int)(ESP.getEfuseMac() & 0xffff));
  snprintf(csrf_, sizeof(csrf_), "%08lx%08lx", (unsigned long)esp_random(), (unsigned long)esp_random());
  const char* headers[] = {"X-Hub-Key"}; server_.collectHeaders(headers, 1);
  server_.on("/", HTTP_GET, [this]() { if (authenticate()) form(); });
  server_.on("/save", HTTP_POST, [this]() { if (authenticate()) save(); });
  server_.on("/api/status", HTTP_GET, [this]() {
    if (!authenticate()) return;
    HubSnapshot snapshot = state_.snapshot();
    StaticJsonDocument<768> document;
    document["firmware"] = "ttgo-smart-hub/1.0.1";
    document["uptime"] = millis() / 1000; document["heap"] = ESP.getFreeHeap();
    document["wifi"] = WiFi.status() == WL_CONNECTED;
    document["mqtt"] = snapshot.mqttConnected; document["alerts"] = snapshot.alertCount;
    document["weather_enabled"] = config_.weatherEnabled;
    document["weather_status"] = snapshot.weather.message;
    document["weather_http"] = snapshot.weather.httpCode;
    document["weather_valid"] = snapshot.weather.valid; document["ha_valid"] = snapshot.ha.valid;
    String body; serializeJson(document, body); server_.send(200, "application/json", body);
  });
  server_.on("/api/alert", HTTP_POST, [this]() {
    if (server_.header("X-Hub-Key") != config_.adminPassword) { server_.send(401, "text/plain", "Chave invalida"); return; }
    if (lastAlert_ && uint32_t(millis() - lastAlert_) < 1000) { server_.send(429, "text/plain", "Aguarde 1 segundo"); return; }
    String body = server_.arg("plain");
    if (body.length() > 384) { server_.send(413, "text/plain", "Payload muito grande"); return; }
    StaticJsonDocument<512> document;
    if (deserializeJson(document, body) || !document["text"].is<const char*>() ||
        strlen(document["text"]) == 0 || strlen(document["text"]) > 95) {
      server_.send(400, "text/plain", "Use text com 1 a 95 bytes e level 0 a 2"); return;
    }
    int level = document["level"] | 0;
    if (level < 0 || level > 2) { server_.send(400, "text/plain", "Nivel invalido"); return; }
    state_.alert(document["text"], level); lastAlert_ = millis();
    server_.send(202, "application/json", "{\"accepted\":true}");
  });
  server_.onNotFound([this]() { server_.send(404, "text/plain", "Nao encontrado"); });
}
void Portal::open() {
  opened_ = millis();
  if (active_) return;
  WiFi.mode(WIFI_AP_STA);
  if (!WiFi.softAP(apName_, config_.adminPassword, 1, false, 2)) { state_.alert("Falha ao abrir AP", 2); return; }
  server_.begin(); active_ = true;
  state_.alert("Configuracao aberta por 10 min");
  Serial.printf("Setup: SSID=%s IP=192.168.4.1 usuario=admin senha=%s\n", apName_, config_.adminPassword);
}
void Portal::tick(uint32_t now) {
  if (restartAt_ && int32_t(now - restartAt_) >= 0) ESP.restart();
  if (!active_) return;
  server_.handleClient();
  if (config_.ssid[0] && uint32_t(now - opened_) >= 600000) {
    close();
  }
}
void Portal::close() {
  if (!active_) return;
  server_.stop(); WiFi.softAPdisconnect(true); WiFi.mode(WIFI_STA); active_ = false;
  state_.alert("Painel de configuracao fechado");
}
void Portal::form() {
  String html;
  html.reserve(11000);
  html = "<!doctype html><html lang='pt-BR'><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>TTGO Smart Hub</title><style>body{font:16px system-ui;background:#101827;color:#e5e7eb;max-width:720px;margin:auto;padding:24px}"
    "h1{color:#58d6d0}fieldset{border:1px solid #374151;border-radius:12px;margin:20px 0;padding:18px}label{display:block;margin:12px 0}"
    "input:not([type=checkbox]){box-sizing:border-box;width:100%;padding:10px;background:#1f2937;border:1px solid #4b5563;color:white;border-radius:6px}"
    "button{background:#58d6d0;padding:14px 24px;border:0;border-radius:8px;font-weight:bold}small{color:#aeb7c6}</style>"
    "<h1>TTGO Smart Hub</h1><p>Configure sua placa. Ao salvar, ela reinicia.</p>"
    "<small>Senhas e token em branco preservam os valores atuais. Use este painel em uma rede local confiável.</small>"
    "<form method='post' action='/save'><input type='hidden' name='csrf' value='" + String(csrf_) + "'><fieldset><legend>Rede e relógio</legend>";
  html += field("Wi-Fi SSID", "ssid", config_.ssid, 32);
  html += field("Senha Wi-Fi", "wifiPassword", "", 64, true);
  html += "<label><input type='checkbox' name='openWifi'> Limpar senha Wi-Fi (rede aberta)</label>";
  html += field("Fuso POSIX (Brasil: &lt;-03&gt;3)", "timezone", config_.timezone, 63);
  html += "</fieldset><fieldset><legend>Clima — Open-Meteo</legend><p>As consultas ficam ativas quando o modulo Clima esta habilitado na lista de modulos abaixo.</p>";
  html += field("Latitude", "latitude", String(config_.latitude, 6).c_str(), 24);
  html += field("Longitude", "longitude", String(config_.longitude, 6).c_str(), 24);
  html += "<small>Coordenadas iniciais são um exemplo para São Paulo; ajuste sua localização.</small></fieldset><fieldset><legend>MQTT</legend>";
  html += field("Broker (IP ou hostname; vazio desativa)", "mqttHost", config_.mqttHost, 64);
  html += field("Porta", "mqttPort", String(config_.mqttPort).c_str(), 5);
  html += field("Usuário", "mqttUser", config_.mqttUser, 64);
  html += field("Senha MQTT", "mqttPassword", "", 64, true);
  html += field("Tópico de eventos", "mqttTopic", config_.mqttTopic, 95);
  html += "<label><input type='checkbox' name='mqttTLS'" + String(config_.mqttTLS ? " checked" : "") +
    "> TLS com certificado público válido (normalmente porta 8883)</label></fieldset><fieldset><legend>Home Assistant</legend>";
  html += field("URL base (vazio desativa)", "haURL", config_.haURL, 127);
  html += field("Token de longa duração", "haToken", "", 255, true);
  html += field("Entidade (ex.: switch.lampada)", "haEntity", config_.haEntity, 95);
  html += "<small>HTTP local ou HTTPS com certificado público. Certificados privados exigem incluir sua CA no bundle.</small></fieldset>"
    "<fieldset><legend>Módulos</legend>";
  const char* names[] = {"Dashboard", "Clima", "Wi-Fi Analyzer", "MQTT", "Alertas", "Home Assistant", nullptr, nullptr, "Configuração", "Bluetooth"};
  for (int i = 1; i < 10; ++i) { if (i == 6 || i == 7 || i == 8) continue; html += "<label><input type='checkbox' name='m" + String(i) + "'" +
    String((config_.modules & (1u << i)) ? " checked" : "") + "> " + names[i] + "</label>"; }
  html += "</fieldset><button>Salvar e reiniciar</button></form><p><small>Firmware 1.0.1 · Open-Meteo fornece os dados meteorológicos.</small></p></html>";
  server_.sendHeader("Cache-Control", "no-store"); server_.send(200, "text/html; charset=utf-8", html);
}
void Portal::save() {
  auto reject = [this](const char* message) { server_.send(400, "text/plain; charset=utf-8", message); };
  if (server_.arg("csrf") != csrf_) { server_.send(403, "text/plain", "Reabra o formulario"); return; }
  Config updated = config_;
  struct TextField { const char* name; char* output; size_t size; bool secret; };
  TextField fields[] = {{"ssid", updated.ssid, sizeof(updated.ssid), false},
    {"wifiPassword", updated.wifiPassword, sizeof(updated.wifiPassword), true},
    {"timezone", updated.timezone, sizeof(updated.timezone), false},
    {"mqttHost", updated.mqttHost, sizeof(updated.mqttHost), false},
    {"mqttUser", updated.mqttUser, sizeof(updated.mqttUser), false},
    {"mqttPassword", updated.mqttPassword, sizeof(updated.mqttPassword), true},
    {"mqttTopic", updated.mqttTopic, sizeof(updated.mqttTopic), false},
    {"haURL", updated.haURL, sizeof(updated.haURL), false},
    {"haToken", updated.haToken, sizeof(updated.haToken), true},
    {"haEntity", updated.haEntity, sizeof(updated.haEntity), false}};
  for (const TextField& item : fields) {
    if (!server_.hasArg(item.name)) { reject("Formulario incompleto"); return; }
    String value = server_.arg(item.name);
    if (value.length() >= item.size) { reject("Campo muito longo"); return; }
    for (size_t i = 0; i < value.length(); ++i)
      if ((uint8_t)value[i] < 32 || (uint8_t)value[i] == 127) { reject("Caractere invalido"); return; }
    if (!item.secret || value.length()) strlcpy(item.output, value.c_str(), item.size);
  }
  if (server_.hasArg("openWifi")) updated.wifiPassword[0] = 0;
  if (updated.wifiPassword[0] && strlen(updated.wifiPassword) < 8) { reject("Senha Wi-Fi precisa de 8 a 64 caracteres"); return; }
  if (!updated.timezone[0]) { reject("Preencha o fuso POSIX"); return; }
  if (!validHost(updated.mqttHost) || !validURL(updated.haURL) || !validEntity(updated.haEntity)) {
    reject("Verifique broker, URL e entidade HA"); return;
  }
  if (updated.mqttHost[0] && !updated.mqttTopic[0]) { reject("Preencha o topico MQTT"); return; }
  String port = server_.arg("mqttPort"); char* end = nullptr;
  long parsedPort = strtol(port.c_str(), &end, 10);
  if (!port.length() || *end || parsedPort < 1 || parsedPort > 65535) { reject("Porta invalida"); return; }
  updated.mqttPort = parsedPort;
  String latitude = server_.arg("latitude"), longitude = server_.arg("longitude");
  updated.latitude = strtof(latitude.c_str(), &end);
  if (!latitude.length() || *end || !isfinite(updated.latitude) || updated.latitude < -90 || updated.latitude > 90) {
    reject("Latitude invalida"); return;
  }
  updated.longitude = strtof(longitude.c_str(), &end);
  if (!longitude.length() || *end || !isfinite(updated.longitude) || updated.longitude < -180 || updated.longitude > 180) {
    reject("Longitude invalida"); return;
  }
  updated.mqttTLS = server_.hasArg("mqttTLS");
  updated.modules = (1u << 0) | (1u << 8);
  for (int i = 1; i < 10; ++i) if (i != 6 && i != 7 && i != 8 && server_.hasArg("m" + String(i))) updated.modules |= 1u << i;
  updated.weatherEnabled = (updated.modules & (1u << 1)) != 0;
  if (!store_.save(updated)) { server_.send(500, "text/plain", "Falha NVS; configuracao nao salva"); return; }
  server_.send(200, "text/html; charset=utf-8", "<h1>Salvo</h1><p>A placa vai reiniciar. Reconecte ao Wi-Fi configurado.</p>");
  restartAt_ = millis() + 1500;
}

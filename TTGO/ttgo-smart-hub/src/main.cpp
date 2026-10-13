#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <time.h>
#include "core/Button.h"
#include "core/Config.h"
#include "services/State.h"
#include "services/Worker.h"
#include "network/Portal.h"
#include "display/UI.h"
namespace {
Config config;
ConfigStore store;
SharedState state;
Worker worker(config, state);
Portal portal(config, store, state);
UI ui(config, state, worker, portal);
Button left, right;
uint32_t lastReconnect = 0, lastLog = 0;
bool lastConnected = false, otaStarted = false;
}
void setup() {
  Serial.begin(115200);
  pinMode(35, INPUT); pinMode(0, INPUT_PULLUP);
  left.begin(digitalRead(35) == LOW, millis()); right.begin(digitalRead(0) == LOW, millis());
  bool nvsReady = store.begin(config);
  if (!nvsReady) {
    snprintf(config.adminPassword, sizeof(config.adminPassword), "%08lx%08lx", (unsigned long)esp_random(), (unsigned long)esp_random());
    state.alert("NVS falhou; nao salva configuracoes", 2);
  }
  Serial.printf("Clima config: ativo=%u latitude=%.6f longitude=%.6f\n", config.weatherEnabled, config.latitude, config.longitude);
  bool displayReady = ui.begin();
  WiFi.persistent(false); WiFi.mode(WIFI_STA); WiFi.setHostname("ttgo-smart-hub");
  if (config.ssid[0]) WiFi.begin(config.ssid, config.wifiPassword);
  portal.begin();
  if (!config.ssid[0]) portal.open();
  configTzTime(config.timezone, "pool.ntp.org", "time.cloudflare.com");
  ArduinoOTA.setHostname("ttgo-smart-hub"); ArduinoOTA.setPassword(config.adminPassword);
  ArduinoOTA.onStart([]() { state.alert("Atualizacao OTA em andamento", 1); });
  ArduinoOTA.onError([](ota_error_t error) { Serial.printf("OTA error=%u\n", error); state.alert("Falha na atualizacao OTA", 2); });
  if (!worker.begin()) state.alert("Falha tarefa de rede", 2);
  state.alert("TTGO Smart Hub iniciado");
  Serial.printf("TTGO Smart Hub 1.0.1 chip=%s flash=%lu display=%s NVS=%s\n", ESP.getChipModel(),
    (unsigned long)ESP.getFlashChipSize(), displayReady ? "240x135" : "FAIL", nvsReady ? "OK" : "FAIL");
}
void loop() {
  uint32_t now = millis();
  bool leftDown = digitalRead(35) == LOW, rightDown = digitalRead(0) == LOW;
  ui.input(left.update(leftDown, now), right.update(rightDown, now));
  bool connected = WiFi.status() == WL_CONNECTED;
  if (connected != lastConnected) {
    lastConnected = connected;
    state.alert(connected ? "Wi-Fi conectado" : "Wi-Fi desconectado", connected ? 0 : 1);
    if (connected) { Serial.printf("Wi-Fi IP=%s RSSI=%d\n", WiFi.localIP().toString().c_str(), WiFi.RSSI()); }
  }
  if (!connected && config.ssid[0] && uint32_t(now - lastReconnect) >= 30000) {
    lastReconnect = now; WiFi.reconnect();
  }
  // OTA is exposed only while the physically opened setup portal is active.
  bool otaAllowed = connected && portal.active();
  if (otaAllowed && !otaStarted) { ArduinoOTA.begin(); otaStarted = true; }
  if (!otaAllowed && otaStarted) { ArduinoOTA.end(); otaStarted = false; }
  if (otaStarted) ArduinoOTA.handle();
  portal.tick(now);
  ui.tick(now);
  if (uint32_t(now - lastLog) >= 10000) {
    lastLog = now;
    Serial.printf("uptime=%lu heap=%lu minheap=%lu wifi=%d\n", (unsigned long)(now / 1000),
      (unsigned long)ESP.getFreeHeap(), (unsigned long)ESP.getMinFreeHeap(), WiFi.status());
    HubSnapshot snapshot = state.snapshot();
    Serial.printf("Clima status: ativo=%u ntp=%u HTTP=%d mensagem=%s\n", config.weatherEnabled, time(nullptr) > 1700000000, snapshot.weather.httpCode, snapshot.weather.message);
  }
  delay(1); // Yield to FreeRTOS; no polling waits for Wi-Fi/HTTP/scan.
}

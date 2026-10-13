#include "web/WebControl.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "web/index_html.h"
#if __has_include("secrets.h")
#include "secrets.h"
#endif

namespace {
WebServer server(80);
FaceState* face = nullptr;
const char* apName = "RoboFace";
const char* apPass = "robo1234";

void sendState() {
  char json[96];
  snprintf(json, sizeof json, "{\"face\":%u,\"auto\":%d,\"palette\":%u,\"pupils\":%d}",
           static_cast<unsigned>(face->expression), face->automatic, face->palette, face->pupils);
  server.send(200, "application/json", json);
}
// GET /set?face=N&palette=N&pupils=0|1&auto=0|1 — all parameters optional.
void handleSet() {
  uint32_t now = millis();
  if (server.hasArg("face")) {
    int id = server.arg("face").toInt();
    if (id >= 0 && id < static_cast<int>(Expression::Count)) {
      face->expression = static_cast<Expression>(id);
      face->since = now;
      face->automatic = false;
    }
  }
  if (server.hasArg("palette")) face->palette = constrain(server.arg("palette").toInt(), 0, 4);
  if (server.hasArg("pupils")) face->pupils = server.arg("pupils") == "1";
  if (server.hasArg("auto")) { face->automatic = server.arg("auto") == "1"; face->since = now; }
  sendState();
}
}

namespace WebControl {
void begin(FaceState& state) {
  face = &state;
  bool station = false;
#ifdef WIFI_SSID
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  for (uint32_t t = millis(); WiFi.status() != WL_CONNECTED && millis() - t < 10000;) delay(100);
  station = WiFi.status() == WL_CONNECTED;
#endif
  if (!station) { WiFi.mode(WIFI_AP); WiFi.softAP(apName, apPass); }
  MDNS.begin("robo");
  server.on("/", [] { server.send_P(200, "text/html; charset=utf-8", INDEX_HTML); });
  server.on("/state", sendState);
  server.on("/set", handleSet);
  server.begin();
  Serial.printf("Web: %s / http://%s ou http://robo.local\n",
                station ? "Wi-Fi" : "AP RoboFace (senha robo1234; sem internet, IA indisponivel)",
                (station ? WiFi.localIP() : WiFi.softAPIP()).toString().c_str());
}
void handle() { server.handleClient(); }
}

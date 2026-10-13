#include "network/Network.h"
#include <WiFi.h>
#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif
void Network::begin() {
  configured_ = WIFI_SSID[0] != '\0';
  WiFi.persistent(false);
  WiFi.mode(configured_ ? WIFI_STA : WIFI_OFF);
  if (configured_) { WiFi.begin(WIFI_SSID, WIFI_PASSWORD); lastAttempt_ = millis(); }
}
void Network::tick(uint32_t now) {
  if (configured_ && WiFi.status() != WL_CONNECTED && uint32_t(now - lastAttempt_) >= 30000) {
    lastAttempt_ = now;
    WiFi.reconnect();
  }
}

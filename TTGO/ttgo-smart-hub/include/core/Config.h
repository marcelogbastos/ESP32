#pragma once
#include <Arduino.h>
#include <Preferences.h>
struct Config {
  uint32_t version = 2;
  char ssid[33] = "";
  char wifiPassword[65] = "";
  char adminPassword[25] = "";
  char timezone[64] = "<-03>3";
  bool weatherEnabled = true; // Derived from the Clima module flag; retained for NVS compatibility.
  float latitude = -23.5505f, longitude = -46.6333f;
  char mqttHost[65] = "";
  uint16_t mqttPort = 1883;
  char mqttUser[65] = "";
  char mqttPassword[65] = "";
  char mqttTopic[96] = "ttgo/hub/events";
  bool mqttTLS = false;
  char haURL[128] = "";
  char haToken[256] = "";
  char haEntity[96] = "";
  uint16_t modules = 0x33f; // IDs 6 and 7 retired; keep other persisted IDs stable.
};
class ConfigStore {
 public:
  bool begin(Config& config);
  bool save(const Config& config);
 private:
  Preferences nvs_;
  bool ready_ = false;
};

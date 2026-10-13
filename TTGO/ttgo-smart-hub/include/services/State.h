#pragma once
#include <Arduino.h>
struct WeatherData {
  bool valid = false;
  float temperature = 0, wind = 0, maximum = 0, minimum = 0;
  int code = 0, httpCode = 0;
  uint32_t updated = 0;
  char message[48] = "Configure a localizacao";
};
struct HAData {
  bool valid = false;
  int httpCode = 0;
  char state[48] = "Configure uma entidade";
  uint32_t updated = 0;
};
struct Alert {
  uint32_t created = 0;
  uint8_t level = 0;
  char text[96] = "";
};
struct HubSnapshot {
  WeatherData weather;
  HAData ha;
  bool mqttConnected = false;
  uint32_t mqttReceived = 0;
  char mqttLast[96] = "Aguardando eventos";
  Alert alerts[8]; // Index 0 is newest; bounded memory.
  uint8_t alertCount = 0;
};
class SharedState {
 public:
  HubSnapshot snapshot();
  void weather(const WeatherData& value);
  void ha(const HAData& value);
  void mqtt(bool connected);
  void message(const char* text);
  void alert(const char* text, uint8_t level = 0);
 private:
  portMUX_TYPE lock_ = portMUX_INITIALIZER_UNLOCKED;
  HubSnapshot state_;
};

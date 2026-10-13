#pragma once
#include <Arduino.h>
class Network {
 public:
  void begin();
  void tick(uint32_t now);
  bool configured() const { return configured_; }
 private:
  bool configured_ = false;
  uint32_t lastAttempt_ = 0;
};

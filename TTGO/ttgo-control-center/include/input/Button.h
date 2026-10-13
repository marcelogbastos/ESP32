#pragma once
#include <stdint.h>
class Button {
 public:
  void begin(bool pressed, uint32_t now) { raw_ = stable_ = pressed; changed_ = now; }
  bool update(bool pressed, uint32_t now) {
    if (pressed != raw_) { raw_ = pressed; changed_ = now; }
    if (raw_ != stable_ && uint32_t(now - changed_) >= 30) {
      stable_ = raw_;
      return stable_;
    }
    return false;
  }
 private:
  bool raw_ = false, stable_ = false;
  uint32_t changed_ = 0;
};

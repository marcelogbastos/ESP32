#pragma once
#include <stdint.h>
enum class ButtonEvent : uint8_t { None, Click, Hold };
class Button {
 public:
  void begin(bool down, uint32_t now) {
    raw_ = down_ = down; changed_ = started_ = now; held_ = down;
  }
  ButtonEvent update(bool pressed, uint32_t now) {
    if (pressed != raw_) { raw_ = pressed; changed_ = now; }
    if (raw_ != down_ && uint32_t(now - changed_) >= 30) {
      down_ = raw_;
      if (down_) { started_ = now; held_ = false; }
      else if (!held_) return ButtonEvent::Click;
    }
    if (down_ && !held_ && uint32_t(now - started_) >= 700) {
      held_ = true;
      return ButtonEvent::Hold;
    }
    return ButtonEvent::None;
  }
 private:
  bool raw_ = false, down_ = false, held_ = false;
  uint32_t changed_ = 0, started_ = 0;
};

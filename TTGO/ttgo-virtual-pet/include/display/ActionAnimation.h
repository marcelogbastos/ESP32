#pragma once
#include "pet/Pet.h"
struct AnimationPose {
  bool active = false;
  Action action = Action::Sleep;
  uint8_t frame = 0;
  uint16_t progress = 0; // 0..1000
};
class ActionAnimation {
 public:
  bool start(Action action, uint32_t now) {
    if (action == Action::Sleep || action == Action::Pet || busy(now)) return false;
    action_ = action; started_ = now; running_ = true; return true;
  }
  bool busy(uint32_t now) const { return running_ && uint32_t(now - started_) < duration(); }
  AnimationPose pose(uint32_t now) const {
    AnimationPose value;
    if (!busy(now)) return value;
    uint32_t elapsed = now - started_;
    value.active = true; value.action = action_;
    value.frame = (elapsed / 140) % 8; value.progress = elapsed * 1000 / duration();
    return value;
  }
 private:
  Action action_ = Action::Sleep;
  uint32_t started_ = 0;
  bool running_ = false;
  uint32_t duration() const { return action_ == Action::Clean ? 3360 : 2800; }
};

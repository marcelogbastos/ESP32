#pragma once
#include <stdint.h>
enum class Expression : uint8_t { Normal, Happy, VeryHappy, Laugh, Angry, Furious, Sad, Crying, Surprise, Scared, Sleepy, Sleep, Wink, Curious, Thinking, Suspicious, Confused, Shy, Affectionate, Excited, Bored, Focused, Worried, Mischievous, Count };
class FaceState {
 public:
  Expression expression = Expression::Normal;
  bool automatic = true, pupils = true;
  uint8_t palette = 0;
  uint32_t since = 0;
  void next(uint32_t now) {
    expression = static_cast<Expression>((static_cast<uint8_t>(expression) + 1) % static_cast<uint8_t>(Expression::Count));
    since = now;
  }
  void tick(uint32_t now) { if (automatic && uint32_t(now - since) >= 8000) next(now); }
  uint32_t age(uint32_t now) const { return uint32_t(now - since); }
  // Close and open over 200 ms; the first blink is delayed after an expression change.
  float openness(uint32_t now) const {
    uint32_t phase = age(now) % 4200;
    if (phase < 3900 || phase >= 4100) return 1.0f;
    return phase < 4000 ? 1.0f - (phase - 3900) / 100.0f : (phase - 4000) / 100.0f;
  }
};



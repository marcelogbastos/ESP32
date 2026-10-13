#pragma once
#include <TFT_eSPI.h>
#include "pet/Pet.h"
#include "display/ActionAnimation.h"
enum class View : uint8_t { Home, Care, Stats, Objectives, Customize, Shop, ConfirmReset };
class Screen {
 public:
  bool begin();
  bool animate(Action action, uint32_t now) { return animation_.start(action, now); }
  bool animating(uint32_t now) const { return animation_.busy(now); }
  void draw(const Pet& pet, View view, uint8_t selection, uint8_t focus, const char* message, uint32_t now);
  void brightness(uint8_t value) { ledcWrite(0, value); }
  void affection(uint32_t now) { affectionAt_ = now; affectionate_ = true; }
  void celebrate(uint32_t now) { celebrationAt_ = now; celebrating_ = true; }
 private:
  TFT_eSPI display_;
  TFT_eSprite frame_{&display_};
  bool ready_ = false;
  ActionAnimation animation_;
  uint32_t affectionAt_ = 0, celebrationAt_ = 0;
  bool affectionate_ = false, celebrating_ = false;
  void label(int x, int y, const String& value, uint16_t color = TFT_WHITE, uint8_t font = 1);
  void bar(int x, int y, const char* label, uint8_t value);
  void character(const Pet& pet, uint32_t now);
};

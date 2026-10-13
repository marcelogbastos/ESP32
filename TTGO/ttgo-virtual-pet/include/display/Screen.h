#pragma once
#include <TFT_eSPI.h>
#include "pet/Pet.h"
class Screen {
 public:
  bool begin();
  void draw(const Pet& pet, uint8_t selection, bool stats, bool confirmReset, const char* message, uint32_t now);
 private:
  TFT_eSPI display_;
  TFT_eSprite frame_{&display_};
  bool ready_ = false;
  void label(int x, int y, const String& value, uint16_t color = TFT_WHITE, uint8_t font = 1);
  void bar(int x, int y, const char* label, uint8_t value);
  void character(const Pet& pet, uint32_t now);
};

#pragma once
#include <TFT_eSPI.h>
#include "faces/FaceState.h"
class FaceRenderer {
 public:
  explicit FaceRenderer(TFT_eSprite& frame) : f(frame) {}
  void draw(const FaceState& state, uint32_t now);
 private:
  TFT_eSprite& f;
  void heart(int x, int y, int size, uint16_t color);
  void star(int x, int y, int radius, float angle, uint16_t color);
  void smile(int y, bool sad, uint16_t color);
};

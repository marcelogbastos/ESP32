#pragma once
#include <TFT_eSPI.h>
#include "faces/FaceStyle.h"
class FaceRenderer {
 public:
  explicit FaceRenderer(TFT_eSprite& frame) : f(frame) {}
  void draw(const FaceState& state, uint32_t now);
 private:
  TFT_eSprite& f;
  void curve(int x0,int y0,int cx,int cy,int x1,int y1,uint16_t color);
};

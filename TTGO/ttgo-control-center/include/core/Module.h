#pragma once
#include <TFT_eSPI.h>
struct Module {
  const char* name;
  bool enabled;
  void (*render)(TFT_eSPI&, uint32_t);
};

#pragma once
#include <stdint.h>
namespace Board {
constexpr uint8_t nextButton = 35; // External pull-up on board; ESP32 has none here.
constexpr uint8_t selectButton = 0; // Boot strap: do not hold during reset.
constexpr uint8_t backlight = 4;
constexpr uint16_t width = 240, height = 135;
}

#pragma once
#include <stdint.h>
namespace config {
constexpr uint8_t kNextButtonPin = 35, kActionButtonPin = 0;
constexpr uint32_t kPettingCooldownMs = 30000;
constexpr uint32_t kSaveDelayMs = 3000, kAutosaveMs = 60000;
constexpr uint32_t kDimAfterMs = 45000, kReturnHomeMs = 20000, kMessageMs = 3500;
constexpr uint32_t kFrameMs = 100, kDimFrameMs = 400;
constexpr uint8_t kBrightness = 180, kDimBrightness = 35;
constexpr uint32_t kActiveCpuMhz = 240, kDimCpuMhz = 80;
constexpr uint32_t kEventMinMs = 10UL * 60000, kEventSpreadMs = 10UL * 60000;
}

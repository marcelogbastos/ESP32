#pragma once
#include <stdint.h>
namespace config {
constexpr uint8_t kNextButtonPin = 35, kActionButtonPin = 0;
constexpr uint32_t kPettingCooldownMs = 30000;
constexpr uint32_t kSaveDelayMs = 3000, kAutosaveMs = 60000;
constexpr uint32_t kReturnHomeMs = 20000, kMessageMs = 3500;
constexpr uint32_t kFrameMs = 100;
constexpr uint32_t kEventMinMs = 10UL * 60000, kEventSpreadMs = 10UL * 60000;
}

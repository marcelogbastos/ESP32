#pragma once
#include <Arduino.h>
#include "Config.h"
#ifndef PET_LIGHT_SLEEP
#define PET_LIGHT_SLEEP 0
#endif
#if PET_LIGHT_SLEEP
#include <esp_sleep.h>
#include <driver/gpio.h>
#endif
namespace power {
// Lower CPU clock while the screen is dimmed; the display runs from its own SPI clock.
inline void setLowPower(bool low) { setCpuFrequencyMhz(low ? config::kDimCpuMhz : config::kActiveCpuMhz); }
// Idle wait. With PET_LIGHT_SLEEP the chip sleeps until a button press or the timeout; millis() keeps counting.
inline void idle(uint32_t ms) {
#if PET_LIGHT_SLEEP
  if (ms > 5) {
    gpio_wakeup_enable(static_cast<gpio_num_t>(config::kNextButtonPin), GPIO_INTR_LOW_LEVEL);
    gpio_wakeup_enable(static_cast<gpio_num_t>(config::kActionButtonPin), GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup(); esp_sleep_enable_timer_wakeup(uint64_t(ms) * 1000);
    Serial.flush(); esp_light_sleep_start(); return;
  }
#endif
  delay(1);
}
}

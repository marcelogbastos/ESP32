#pragma once
#include <stdint.h>
// The first five entries map one-to-one to Action values.
enum class MenuItem : uint8_t { Feed, Play, Clean, Sleep, Heal, Game, Stats, Objectives, Customize, Shop, Reset, Count };
constexpr uint8_t kMenuCount = static_cast<uint8_t>(MenuItem::Count);
constexpr bool isCareAction(MenuItem item) { return item <= MenuItem::Heal; }

#pragma once
#include <stdint.h>
enum class Action : uint8_t { Feed, Play, Clean, Sleep, Heal };
enum class Result : uint8_t { Done, Asleep, NoEnergy, NoCoins, Full };
struct PetState {
  uint32_t version = 1;
  uint32_t ageMinutes = 0;
  uint16_t remainderSeconds = 0;
  uint16_t coins = 10;
  uint8_t food = 80, energy = 80, joy = 80, hygiene = 80, health = 100;
  bool sleeping = false;
};
class Pet {
 public:
  const PetState& state() const { return state_; }
  void reset() { state_ = PetState{}; }
  bool restore(const PetState& value) {
    if (value.version != 1 || value.remainderSeconds >= 60 || value.coins > 9999 ||
        value.food > 100 || value.energy > 100 || value.joy > 100 || value.hygiene > 100 || value.health > 100) return false;
    state_ = value; return true;
  }
  uint8_t stage() const { return state_.ageMinutes < 60 ? 0 : state_.ageMinutes < 240 ? 1 : 2; }
  void advance(uint32_t seconds);
  Result act(Action action);
 private:
  PetState state_;
  static uint8_t adjust(uint8_t value, int delta) {
    int next = value + delta; return next < 0 ? 0 : next > 100 ? 100 : next;
  }
  void minute();
};

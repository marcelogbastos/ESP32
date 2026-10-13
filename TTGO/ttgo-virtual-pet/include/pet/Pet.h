#pragma once
#include <stdint.h>
enum class Action : uint8_t { Feed, Play, Clean, Sleep, Heal, Pet };
enum class Result : uint8_t { Done, Asleep, NoEnergy, NoCoins, Full };
enum class Event : uint8_t { FoundCoins, Cuddle, Mess, Count };
enum class Mood : uint8_t { Neutral, Happy, Hungry, Tired, Dirty, Sad, Ill, Sleeping, Affection };
struct LegacyPetState {
  uint32_t version = 1, ageMinutes = 0;
  uint16_t remainderSeconds = 0, coins = 10;
  uint8_t food = 80, energy = 80, joy = 80, hygiene = 80, health = 100;
  bool sleeping = false;
};
static_assert(sizeof(LegacyPetState) == 20, "Legacy ESP32 save layout must remain stable");
struct PetState {
  uint32_t version = 2, ageMinutes = 0;
  uint16_t remainderSeconds = 0, coins = 10;
  uint8_t food = 80, energy = 80, joy = 80, hygiene = 80, health = 100;
  bool sleeping = false;
  uint8_t bond = 20, lifeStage = 0, adultForm = 0, name = 0, color = 0, accessory = 0, owned = 1;
  uint32_t careMinutes = 0, sleepMinutes = 0, plays = 0, day = 0;
  uint8_t feedsToday = 0, playsToday = 0, bathsToday = 0, restToday = 0, claimed = 0;
  uint8_t reserved[3] = {0, 0, 0}; // Explicit padding keeps the saved bytes deterministic.
};
static_assert(sizeof(PetState) == 52, "Saved ESP32 layout must remain stable");
class Pet {
 public:
  const PetState& state() const { return state_; }
  void reset() { state_ = PetState{}; }
  bool restore(const PetState& value);
  bool migrate(const LegacyPetState& value);
  uint8_t stage() const { return state_.lifeStage; }
  Mood mood() const;
  void advance(uint32_t seconds);
  Result act(Action action);
  bool objectivesComplete() const;
  bool claimReward();
  void cycleName() { state_.name = (state_.name + 1) % 8; }
  void cycleColor() { state_.color = (state_.color + 1) % 6; }
  void cycleAccessory();
  Result buy(uint8_t accessory);
  Result canPlayGame() const;
  void finishGame(uint8_t hits);
  bool applyEvent(Event event);
 private:
  PetState state_;
  static uint8_t adjust(uint8_t value, int delta) { int next = value + delta; return next < 0 ? 0 : next > 100 ? 100 : next; }
  void minute();
  void syncDay();
  void addCoins(uint16_t amount) { state_.coins = state_.coins + amount > 9999 ? 9999 : state_.coins + amount; }
};

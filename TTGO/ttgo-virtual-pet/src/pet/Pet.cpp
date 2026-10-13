#include "pet/Pet.h"
namespace {
bool validBase(uint16_t seconds, uint16_t coins, uint8_t food, uint8_t energy, uint8_t joy, uint8_t hygiene, uint8_t health) {
  return seconds < 60 && coins <= 9999 && food <= 100 && energy <= 100 && joy <= 100 && hygiene <= 100 && health <= 100;
}
void increment(uint32_t& value) { if (value < 0xffffffffu) ++value; }
}
bool Pet::restore(const PetState& value) {
  if (value.version != 2 || !validBase(value.remainderSeconds, value.coins, value.food, value.energy, value.joy, value.hygiene, value.health) ||
      value.bond > 100 || value.lifeStage > 2 || value.adultForm > 4 || value.name >= 8 || value.color >= 6 ||
      value.accessory > 3 || value.owned > 15 || !(value.owned & 1) || !(value.owned & (1u << value.accessory)) ||
      value.feedsToday > 2 || value.playsToday > 2 || value.bathsToday > 1 || value.restToday > 5 || value.claimed > 1 ||
      value.careMinutes > value.ageMinutes || value.sleepMinutes > value.ageMinutes) return false;
  state_ = value; syncDay(); return true;
}
bool Pet::migrate(const LegacyPetState& value) {
  if (value.version != 1 || !validBase(value.remainderSeconds, value.coins, value.food, value.energy, value.joy, value.hygiene, value.health)) return false;
  reset();
  state_.ageMinutes = value.ageMinutes; state_.remainderSeconds = value.remainderSeconds;
  state_.coins = value.coins; state_.food = value.food; state_.energy = value.energy;
  state_.joy = value.joy; state_.hygiene = value.hygiene; state_.health = value.health; state_.sleeping = value.sleeping;
  state_.lifeStage = value.ageMinutes >= 240 ? 2 : value.ageMinutes >= 60 ? 1 : 0;
  state_.careMinutes = state_.lifeStage == 2 ? 90 : state_.lifeStage == 1 ? 20 : 0;
  state_.adultForm = state_.lifeStage == 2 ? 4 : 0; state_.day = value.ageMinutes / 1440;
  return true;
}
Mood Pet::mood() const {
  if (state_.sleeping) return Mood::Sleeping;
  if (state_.health < 25) return Mood::Ill;
  if (state_.food < 25) return Mood::Hungry;
  if (state_.hygiene < 25) return Mood::Dirty;
  if (state_.energy < 25) return Mood::Tired;
  if (state_.joy < 25) return Mood::Sad;
  if (state_.bond >= 65) return Mood::Affection;
  return state_.joy >= 70 ? Mood::Happy : Mood::Neutral;
}
void Pet::syncDay() {
  uint32_t day = state_.ageMinutes / 1440;
  if (day != state_.day) {
    state_.day = day; state_.feedsToday = state_.playsToday = state_.bathsToday = state_.restToday = state_.claimed = 0;
  }
}
void Pet::minute() {
  increment(state_.ageMinutes); syncDay();
  state_.food = adjust(state_.food, state_.sleeping ? -1 : -2);
  state_.energy = adjust(state_.energy, state_.sleeping ? 6 : -1);
  state_.joy = adjust(state_.joy, -1); state_.hygiene = adjust(state_.hygiene, -2);
  bool danger = state_.food < 15 || state_.energy < 10 || state_.hygiene < 15;
  if (danger) { state_.health = adjust(state_.health, -3); if (state_.ageMinutes % 5 == 0) state_.bond = adjust(state_.bond, -1); }
  else if (state_.food >= 40 && state_.energy >= 30 && state_.hygiene >= 40) state_.health = adjust(state_.health, 1);
  if (state_.food >= 40 && state_.energy >= 30 && state_.hygiene >= 40 && state_.health >= 50) increment(state_.careMinutes);
  if (state_.sleeping) {
    increment(state_.sleepMinutes);
    if (state_.restToday < 5) ++state_.restToday;
    if (state_.ageMinutes % 5 == 0) state_.bond = adjust(state_.bond, 1);
  }
  if (!state_.lifeStage && state_.ageMinutes >= 60 && state_.careMinutes >= 20) state_.lifeStage = 1;
  if (state_.lifeStage == 1 && state_.ageMinutes >= 240 && state_.careMinutes >= 90) {
    state_.lifeStage = 2;
    state_.adultForm = state_.bond >= 65 ? 1 : state_.sleepMinutes >= 60 ? 2 : state_.plays >= 15 ? 3 : 4;
  }
}
void Pet::advance(uint32_t seconds) {
  uint64_t elapsed = uint64_t(state_.remainderSeconds) + seconds;
  while (elapsed >= 60) { minute(); elapsed -= 60; }
  state_.remainderSeconds = elapsed;
}
Result Pet::act(Action action) {
  if (action == Action::Sleep) { state_.sleeping = !state_.sleeping; return Result::Done; }
  if (state_.sleeping) return Result::Asleep;
  switch (action) {
    case Action::Feed:
      if (state_.food == 100) return Result::Full;
      if (!state_.coins) return Result::NoCoins;
      --state_.coins; state_.food = adjust(state_.food, 20); state_.hygiene = adjust(state_.hygiene, -3);
      if (state_.feedsToday < 2) ++state_.feedsToday; break;
    case Action::Play:
      if (state_.energy < 10) return Result::NoEnergy;
      state_.energy = adjust(state_.energy, -10); state_.joy = adjust(state_.joy, 18); addCoins(2); increment(state_.plays);
      if (state_.playsToday < 2) ++state_.playsToday; break;
    case Action::Clean:
      if (state_.hygiene == 100) return Result::Full;
      state_.hygiene = adjust(state_.hygiene, 30); state_.energy = adjust(state_.energy, -2); state_.bathsToday = 1; break;
    case Action::Heal:
      if (state_.health == 100) return Result::Full;
      if (state_.coins < 2) return Result::NoCoins;
      state_.coins -= 2; state_.health = adjust(state_.health, 25); break;
    case Action::Pet:
      state_.joy = adjust(state_.joy, 3); state_.bond = adjust(state_.bond, 1); return Result::Done;
    case Action::Sleep: break;
  }
  state_.bond = adjust(state_.bond, action == Action::Play ? 3 : 2);
  return Result::Done;
}
bool Pet::objectivesComplete() const { return state_.feedsToday == 2 && state_.playsToday == 2 && state_.bathsToday == 1 && state_.restToday == 5; }
bool Pet::claimReward() {
  if (!objectivesComplete() || state_.claimed) return false;
  state_.claimed = 1; addCoins(8); return true;
}
void Pet::cycleAccessory() {
  do { state_.accessory = (state_.accessory + 1) % 4; } while (!(state_.owned & (1u << state_.accessory)));
}
Result Pet::buy(uint8_t accessory) {
  if (accessory == 0 || accessory > 3) return Result::Full;
  const uint8_t prices[] = {0, 6, 8, 10};
  if (!(state_.owned & (1u << accessory))) {
    if (state_.coins < prices[accessory]) return Result::NoCoins;
    state_.coins -= prices[accessory]; state_.owned |= 1u << accessory;
  }
  state_.accessory = accessory; return Result::Done;
}

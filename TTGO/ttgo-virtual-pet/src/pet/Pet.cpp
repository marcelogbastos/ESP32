#include "pet/Pet.h"
void Pet::minute() {
  if (state_.ageMinutes < 0xffffffffu) ++state_.ageMinutes;
  state_.food = adjust(state_.food, state_.sleeping ? -1 : -2);
  state_.energy = adjust(state_.energy, state_.sleeping ? 6 : -1);
  state_.joy = adjust(state_.joy, -1);
  state_.hygiene = adjust(state_.hygiene, -2);
  bool danger = state_.food < 15 || state_.energy < 10 || state_.hygiene < 15;
  if (danger) state_.health = adjust(state_.health, -3);
  else if (state_.food >= 40 && state_.energy >= 30 && state_.hygiene >= 40) state_.health = adjust(state_.health, 1);
}
void Pet::advance(uint32_t seconds) {
  uint64_t elapsed = uint64_t(state_.remainderSeconds) + seconds;
  while (elapsed >= 60) { minute(); elapsed -= 60; }
  state_.remainderSeconds = elapsed;
}
Result Pet::act(Action action) {
  if (action == Action::Sleep) { state_.sleeping = !state_.sleeping; return Result::Done; }
  // Feeding and bathing remain available during illness so recovery is possible.
  if (state_.sleeping) return Result::Asleep;
  switch (action) {
    case Action::Feed:
      if (state_.food == 100) return Result::Full;
      if (!state_.coins) return Result::NoCoins;
      --state_.coins; state_.food = adjust(state_.food, 20); state_.hygiene = adjust(state_.hygiene, -3); break;
    case Action::Play:
      if (state_.energy < 10) return Result::NoEnergy;
      state_.energy = adjust(state_.energy, -10); state_.joy = adjust(state_.joy, 18);
      state_.coins = state_.coins > 9997 ? 9999 : state_.coins + 2; break;
    case Action::Clean:
      state_.hygiene = adjust(state_.hygiene, 30); state_.energy = adjust(state_.energy, -2); break;
    case Action::Heal:
      if (state_.health == 100) return Result::Full;
      if (state_.coins < 2) return Result::NoCoins;
      state_.coins -= 2; state_.health = adjust(state_.health, 25); break;
    case Action::Sleep: break;
  }
  return Result::Done;
}

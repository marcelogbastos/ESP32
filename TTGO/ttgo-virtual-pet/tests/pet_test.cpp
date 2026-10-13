#include "pet/Pet.h"
#define CHECK(c) do { if (!(c)) return __LINE__; } while(false)
extern "C" int main() {
  Pet pet;
  CHECK(pet.act(Action::Feed) == Result::Done);
  CHECK(pet.state().food == 100 && pet.state().coins == 9);
  CHECK(pet.act(Action::Feed) == Result::Full);
  CHECK(pet.state().coins == 9);
  pet.advance(59); CHECK(pet.state().ageMinutes == 0);
  pet.advance(1); CHECK(pet.state().ageMinutes == 1 && pet.state().food == 98);
  pet.act(Action::Sleep); pet.advance(600);
  CHECK(pet.state().energy == 100 && pet.state().food == 88);
  CHECK(pet.act(Action::Play) == Result::Asleep);
  pet.act(Action::Sleep); CHECK(pet.act(Action::Play) == Result::Done);
  CHECK(pet.state().coins == 11 && pet.state().energy == 90);
  pet.advance(3600); CHECK(pet.stage() == 1);
  pet.advance(10800); CHECK(pet.stage() == 2);
  CHECK(pet.state().health == 0);
  PetState damaged = pet.state(); damaged.health = 101;
  CHECK(!pet.restore(damaged));
  damaged = pet.state(); damaged.remainderSeconds = 60;
  CHECK(!pet.restore(damaged));
  pet.reset(); CHECK(pet.stage() == 0 && pet.state().coins == 10);
  PetState poor = pet.state(); poor.coins = 0; poor.food = 0; poor.energy = 0; poor.health = 0;
  CHECK(pet.restore(poor)); CHECK(pet.act(Action::Feed) == Result::NoCoins);
  CHECK(pet.act(Action::Play) == Result::NoEnergy);
  pet.act(Action::Sleep); pet.advance(120); pet.act(Action::Sleep);
  CHECK(pet.act(Action::Play) == Result::Done); CHECK(pet.state().coins == 2);
  CHECK(pet.act(Action::Feed) == Result::Done); CHECK(pet.act(Action::Clean) == Result::Done);
  PetState sick; sick.health = 60; sick.coins = 1;
  CHECK(pet.restore(sick)); CHECK(pet.act(Action::Heal) == Result::NoCoins);
  CHECK(pet.state().health == 60 && pet.state().coins == 1);
  sick.coins = 2; CHECK(pet.restore(sick)); CHECK(pet.act(Action::Heal) == Result::Done);
  CHECK(pet.state().health == 85 && pet.state().coins == 0);
  CHECK(pet.act(Action::Heal) == Result::NoCoins);
  PetState rich; rich.coins = 9998; CHECK(pet.restore(rich));
  CHECK(pet.act(Action::Play) == Result::Done); CHECK(pet.state().coins == 9999);
  return 0;
}

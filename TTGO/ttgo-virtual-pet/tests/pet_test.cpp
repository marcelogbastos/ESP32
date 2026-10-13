#include "pet/Pet.h"
#include "display/ActionAnimation.h"
#include "pet/MiniGame.h"
#include "util/Codec.h"
#if defined(__wasm__)
extern "C" void* memcpy(void* target, const void* source, unsigned long size) {
  auto* out = static_cast<unsigned char*>(target); auto* in = static_cast<const unsigned char*>(source);
  for (unsigned long i = 0; i < size; ++i) out[i] = in[i]; return target;
}
extern "C" void* memset(void* target, int value, unsigned long size) {
  auto* out = static_cast<unsigned char*>(target);
  for (unsigned long i = 0; i < size; ++i) out[i] = value; return target;
}
#endif
#define CHECK(c) do { if (!(c)) return __LINE__; } while(false)
extern "C" int main() {
  ActionAnimation animation;
  CHECK(!animation.busy(0)); CHECK(!animation.start(Action::Sleep, 0)); CHECK(!animation.start(Action::Pet, 0));
  CHECK(animation.start(Action::Feed, 100)); CHECK(!animation.start(Action::Play, 200));
  CHECK(animation.busy(2899)); CHECK(!animation.busy(2900));
  CHECK(animation.start(Action::Clean, 3000)); CHECK(animation.busy(6359)); CHECK(!animation.busy(6360));
  CHECK(animation.start(Action::Heal, 0xfffffff0u)); CHECK(animation.pose(124).frame == 1); CHECK(!animation.busy(2784));
  Pet pet;
  CHECK(pet.act(Action::Feed) == Result::Done); CHECK(pet.state().food == 100 && pet.state().coins == 9);
  CHECK(pet.act(Action::Feed) == Result::Full); CHECK(pet.state().feedsToday == 1);
  pet.advance(60); CHECK(pet.act(Action::Feed) == Result::Done);
  CHECK(pet.act(Action::Play) == Result::Done); CHECK(pet.act(Action::Play) == Result::Done);
  CHECK(pet.act(Action::Clean) == Result::Done); CHECK(pet.act(Action::Clean) == Result::Full);
  pet.act(Action::Sleep); CHECK(pet.act(Action::Play) == Result::Asleep);
  pet.advance(300); CHECK(pet.state().energy == 87 && pet.objectivesComplete());
  CHECK(pet.claimReward()); CHECK(pet.state().coins == 20); CHECK(!pet.claimReward());
  PetState expired = pet.state(); expired.ageMinutes = 1439; expired.day = 0;
  CHECK(pet.restore(expired)); pet.advance(60); CHECK(pet.state().day == 1 && !pet.state().claimed && !pet.objectivesComplete());
  PetState unhealthy; unhealthy.ageMinutes = 239; unhealthy.careMinutes = 0;
  CHECK(pet.restore(unhealthy)); pet.advance(60); CHECK(pet.stage() == 0); // Age alone cannot cause growth.
  PetState well; well.ageMinutes = 59; well.careMinutes = 20;
  CHECK(pet.restore(well)); pet.advance(60); CHECK(pet.stage() == 1);
  well.ageMinutes = 239; well.lifeStage = 1; well.careMinutes = 90; well.bond = 80;
  CHECK(pet.restore(well)); pet.advance(60); CHECK(pet.stage() == 2 && pet.state().adultForm == 1);
  well.bond = 20; well.sleepMinutes = 60;
  CHECK(pet.restore(well)); pet.advance(60); CHECK(pet.state().adultForm == 2);
  well.sleepMinutes = 0; well.plays = 15;
  CHECK(pet.restore(well)); pet.advance(60); CHECK(pet.state().adultForm == 3);
  PetState mood;
  mood.food = 20; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Hungry);
  mood.food = 80; mood.energy = 20; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Tired);
  mood.hygiene = 20; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Dirty);
  mood.health = 10; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Ill);
  mood.sleeping = true; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Sleeping);
  mood = PetState{}; mood.bond = 80; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Affection);
  mood.joy = 20; CHECK(pet.restore(mood)); CHECK(pet.mood() == Mood::Sad);
  PetState poor; poor.coins = 0; poor.food = 0; poor.energy = 0; poor.health = 0;
  CHECK(pet.restore(poor)); CHECK(pet.act(Action::Feed) == Result::NoCoins); CHECK(pet.act(Action::Play) == Result::NoEnergy);
  pet.act(Action::Sleep); pet.advance(120); pet.act(Action::Sleep);
  CHECK(pet.act(Action::Play) == Result::Done); CHECK(pet.act(Action::Feed) == Result::Done);
  PetState sick; sick.health = 60; sick.coins = 1;
  CHECK(pet.restore(sick)); CHECK(pet.act(Action::Heal) == Result::NoCoins); CHECK(pet.state().health == 60);
  sick.coins = 2; CHECK(pet.restore(sick)); CHECK(pet.act(Action::Heal) == Result::Done); CHECK(pet.state().health == 85);
  pet.reset(); CHECK(pet.buy(3) == Result::Done); CHECK(pet.state().coins == 0 && pet.state().accessory == 3);
  CHECK(pet.buy(3) == Result::Done && pet.state().coins == 0); CHECK(pet.buy(1) == Result::NoCoins);
  pet.cycleAccessory(); CHECK(pet.state().accessory == 0); pet.cycleAccessory(); CHECK(pet.state().accessory == 3);
  for (int i = 0; i < 8; ++i) pet.cycleName(); CHECK(pet.state().name == 0);
  for (int i = 0; i < 6; ++i) pet.cycleColor(); CHECK(pet.state().color == 0);
  LegacyPetState legacy; legacy.ageMinutes = 300; legacy.coins = 47; legacy.food = 42; legacy.sleeping = true;
  CHECK(pet.migrate(legacy)); CHECK(pet.stage() == 2 && pet.state().adultForm == 4);
  CHECK(pet.state().coins == 47 && pet.state().food == 42 && pet.state().sleeping);
  PetState migrated = pet.state(); CHECK(pet.restore(migrated));
  migrated.accessory = 2; CHECK(!pet.restore(migrated)); // Unowned item cannot be loaded.
  migrated = pet.state(); migrated.name = 8; CHECK(!pet.restore(migrated));
  legacy.health = 101; CHECK(!pet.migrate(legacy));
  PetState rich; rich.coins = 9998; CHECK(pet.restore(rich)); pet.act(Action::Play); CHECK(pet.state().coins == 9999);
  // Random events.
  pet.reset(); pet.act(Action::Sleep); CHECK(!pet.applyEvent(Event::FoundCoins)); CHECK(pet.canPlayGame() == Result::Asleep); pet.act(Action::Sleep);
  CHECK(pet.applyEvent(Event::FoundCoins) && pet.state().coins == 13);
  CHECK(pet.applyEvent(Event::Mess) && pet.state().hygiene == 65);
  CHECK(pet.applyEvent(Event::Cuddle) && pet.state().joy == 90 && pet.state().bond == 21);
  CHECK(!pet.applyEvent(Event::Count));
  // Mini-game rewards.
  PetState tired; tired.energy = 7; CHECK(pet.restore(tired)); CHECK(pet.canPlayGame() == Result::NoEnergy);
  pet.finishGame(3); CHECK(pet.state().energy == 7 && pet.state().coins == 10); // Refused game gives nothing.
  pet.reset(); pet.finishGame(3); CHECK(pet.state().energy == 72 && pet.state().joy == 97 && pet.state().coins == 17 && pet.state().plays == 1);
  pet.reset(); pet.finishGame(9); CHECK(pet.state().coins == 17); // Hits are clamped.
  pet.reset(); pet.finishGame(0); CHECK(pet.state().coins == 10 && pet.state().joy == 85);
  // Mini-game rounds: seed 0x0a0a gives targets 30, 30, 20.
  MiniGame game;
  CHECK(!game.active() && !game.finished()); game.start(1000, 0x0a0a);
  CHECK(game.active() && game.round() == 0 && game.target() == 30 && game.width() == 12);
  CHECK(game.cursor(1000) == 0 && game.cursor(1350) == 50 && game.cursor(1700) == 100 && game.cursor(2050) == 50); // Period 1400 ms.
  game.press(1210); CHECK(game.revealing() && game.hits() == 1 && game.lastHit() && game.cursor(9999) == 30);
  game.press(1300); CHECK(game.hits() == 1);
  game.update(2009); CHECK(game.revealing()); game.update(2010); CHECK(!game.revealing() && game.round() == 1 && game.width() == 10);
  game.press(2585); CHECK(game.revealing() && !game.lastHit() && game.hits() == 1); // Round 1 period is 1150 ms; 575 ms is the far end.
  game.update(3385); CHECK(game.round() == 2 && game.target() == 20 && game.width() == 8);
  game.press(3385 + 90); CHECK(game.lastHit() && game.hits() == 2);
  game.update(3475 + 800); CHECK(game.finished() && !game.active() && game.hits() == 2);
  game.stop(); CHECK(!game.finished());
  // Backup text, CRC and reserved bytes.
  uint8_t digits[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  CHECK(codec::crc32(digits, 9) == 0xcbf43926u);
  pet.reset(); pet.act(Action::Play); pet.advance(600); pet.buy(1);
  char text[codec::backupSize(sizeof(PetState))];
  CHECK(codec::encodeBackup(reinterpret_cast<const uint8_t*>(&pet.state()), sizeof(PetState), text, sizeof(text)));
  CHECK(!codec::encodeBackup(reinterpret_cast<const uint8_t*>(&pet.state()), sizeof(PetState), text, sizeof(text) - 1));
  PetState copy;
  CHECK(codec::decodeBackup(text, reinterpret_cast<uint8_t*>(&copy), sizeof(copy)));
  Pet other; CHECK(other.restore(copy) && other.state().coins == pet.state().coins && other.state().ageMinutes == 10 && other.state().accessory == 1);
  text[10] = text[10] == '0' ? '1' : '0'; CHECK(!codec::decodeBackup(text, reinterpret_cast<uint8_t*>(&copy), sizeof(copy)));
  CHECK(!codec::decodeBackup("PET1:00", reinterpret_cast<uint8_t*>(&copy), sizeof(copy)));
  PetState padded; padded.reserved[0] = 7; padded.reserved[2] = 9; CHECK(pet.restore(padded)); CHECK(!pet.state().reserved[0] && !pet.state().reserved[2]);
  return 0;
}

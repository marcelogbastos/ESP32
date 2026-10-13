#include <Arduino.h>
#include <esp_system.h>
#include "Config.h"
#include "pet/Pet.h"
#include "storage/Save.h"
#include "display/Screen.h"
#include "input/Button.h"
#include "console/SerialConsole.h"
#include "power/Power.h"
namespace {
Pet pet; Save save; Screen screen; SerialConsole console; MiniGame game; Button nextButton, actionButton;
View view = View::Home;
uint8_t selection = 0, focus = 0, shop = 1;
bool pendingSave = false, dimmed = false;
uint32_t lastSecond = 0, lastFrame = 0, lastSave = 0, lastAction = 0, messageAt = 0, lastInput = 0, lastPetting = 0, nextEvent = 0;
char message[40] = "";
void say(const char* value, uint32_t now) { strlcpy(message, value, sizeof(message)); messageAt = now; }
void changed(uint32_t now) { pendingSave = true; lastAction = now; }
void goHome() { if (view == View::Game) game.stop(); view = View::Home; }
const char* describe(Result result) {
  switch (result) {
    case Result::Done: return "Feito!";
    case Result::Asleep: return "Acorde primeiro";
    case Result::NoEnergy: return "Preciso dormir";
    case Result::NoCoins: return "Moedas insuficientes";
    case Result::Full: return "Ja esta no maximo";
  }
  return "";
}
void scheduleEvent(uint32_t now) { nextEvent = now + config::kEventMinMs + esp_random() % config::kEventSpreadMs; }
void randomEvent(uint32_t now) {
  scheduleEvent(now);
  Event event = static_cast<Event>(esp_random() % static_cast<uint8_t>(Event::Count));
  if (!pet.applyEvent(event)) return;
  const char* texts[] = {"Achou 3 moedas!", "Ganhou um carinho!", "Fez bagunca..."};
  say(texts[static_cast<uint8_t>(event)], now); changed(now);
  if (event == Event::Cuddle) screen.affection(now);
}
void startGame(uint32_t now) {
  Result result = pet.canPlayGame();
  if (result != Result::Done) { say(describe(result), now); return; }
  game.start(now, esp_random()); view = View::Game;
}
void perform(uint32_t now) {
  MenuItem item = static_cast<MenuItem>(selection);
  switch (item) {
    case MenuItem::Game: startGame(now); return;
    case MenuItem::Stats: view = View::Stats; return;
    case MenuItem::Objectives: view = View::Objectives; return;
    case MenuItem::Customize: view = View::Customize; return;
    case MenuItem::Shop: view = View::Shop; return;
    case MenuItem::Reset: view = View::ConfirmReset; return;
    default: break;
  }
  Action action = static_cast<Action>(selection);
  Result result = pet.act(action);
  if (result == Result::Done) { changed(now); view = View::Home; screen.animate(action, now); }
  else say(describe(result), now);
}
void handleHome(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (left == ButtonEvent::Click) view = View::Care;
  if (right == ButtonEvent::Click && uint32_t(now - lastPetting) >= config::kPettingCooldownMs && pet.act(Action::Pet) == Result::Done) {
    lastPetting = now; changed(now); screen.affection(now);
  }
}
void handleCare(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (left == ButtonEvent::Click) selection = (selection + 1) % kMenuCount;
  if (right == ButtonEvent::Click) perform(now);
}
void handleReset(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (left == ButtonEvent::Click) view = View::Care;
  if (right == ButtonEvent::Click) { pet.reset(); changed(now); selection = 0; view = View::Home; screen.affection(now); }
}
void handleCustomize(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (left == ButtonEvent::Click) focus = (focus + 1) % 3;
  if (right != ButtonEvent::Click) return;
  if (!focus) pet.cycleName(); else if (focus == 1) pet.cycleColor(); else pet.cycleAccessory();
  changed(now);
}
void handleShop(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (left == ButtonEvent::Click) shop = shop % 3 + 1;
  if (right != ButtonEvent::Click) return;
  if (pet.buy(shop) == Result::Done) { changed(now); say("Acessorio equipado", now); }
  else say(describe(Result::NoCoins), now);
}
void handleObjectives(ButtonEvent, ButtonEvent right, uint32_t now) {
  if (right != ButtonEvent::Click) return;
  if (pet.claimReward()) { changed(now); say("Recompensa: +8 moedas", now); }
  else say(pet.state().claimed ? "Recompensa ja recebida" : "Complete os objetivos", now);
}
void handleInput(ButtonEvent left, ButtonEvent right, uint32_t now) {
  if (right == ButtonEvent::Hold && view != View::Game) { view = view == View::Home ? View::Stats : View::Home; message[0] = 0; return; }
  if (left == ButtonEvent::Hold) {
    if (view == View::Home) { pet.act(Action::Sleep); changed(now); }
    else goHome();
    return;
  }
  switch (view) {
    case View::Home: handleHome(left, right, now); break;
    case View::Care: handleCare(left, right, now); break;
    case View::ConfirmReset: handleReset(left, right, now); break;
    case View::Customize: handleCustomize(left, right, now); break;
    case View::Shop: handleShop(left, right, now); break;
    case View::Objectives: handleObjectives(left, right, now); break;
    case View::Game: if (right == ButtonEvent::Click) game.press(now); break;
    case View::Stats: break;
  }
}
void updateGame(uint32_t now) {
  if (view != View::Game) return;
  game.update(now);
  if (!game.finished()) return;
  unsigned hits = game.hits();
  pet.finishGame(hits); goHome(); changed(now);
  char text[40]; snprintf(text, sizeof(text), "%u/%u acertos!", hits, unsigned(MiniGame::kRounds)); say(text, now);
  if (hits) screen.celebrate(now);
}
}
void setup() {
  Serial.begin(115200); pinMode(config::kNextButtonPin, INPUT); pinMode(config::kActionButtonPin, INPUT_PULLUP);
  nextButton.begin(digitalRead(config::kNextButtonPin) == LOW, millis()); actionButton.begin(digitalRead(config::kActionButtonPin) == LOW, millis());
  bool restored = save.begin(pet), displayed = screen.begin();
  lastSecond = lastSave = lastInput = millis(); lastPetting = millis() - config::kPettingCooldownMs; scheduleEvent(millis());
  if (!restored) { say("Falha NVS; ver serial", millis()); view = View::Care; }
  Serial.printf("TTGO Virtual Pet 2.1.0 / display=%s NVS=%s idade=%luh%02lum\n", displayed ? "240x135" : "FAIL", restored ? "OK" : "FAIL",
                (unsigned long)(pet.state().ageMinutes / 60), (unsigned long)(pet.state().ageMinutes % 60));
}
void loop() {
  uint32_t now = millis();
  bool leftDown = digitalRead(config::kNextButtonPin) == LOW, rightDown = digitalRead(config::kActionButtonPin) == LOW;
  ButtonEvent left = nextButton.update(leftDown, now), right = actionButton.update(rightDown, now);
  if (leftDown || rightDown || left != ButtonEvent::None || right != ButtonEvent::None) lastInput = now;
  if (!screen.animating(now)) handleInput(left, right, now);
  updateGame(now);
  if (console.poll(pet, save)) { pendingSave = false; lastSave = now; goHome(); }
  uint32_t seconds = uint32_t(now - lastSecond) / 1000;
  uint8_t oldStage = pet.stage();
  if (seconds) { lastSecond += seconds * 1000; pet.advance(seconds); }
  if (oldStage != pet.stage()) { changed(now); goHome(); screen.celebrate(now); }
  if (int32_t(now - nextEvent) >= 0) { if (view == View::Home) randomEvent(now); else nextEvent = now + 30000; }
  if ((pendingSave && uint32_t(now - lastAction) >= config::kSaveDelayMs) || uint32_t(now - lastSave) >= config::kAutosaveMs) {
    if (!save.write(pet)) { say("Falha ao salvar NVS", now); Serial.println("Pet: falha NVS"); }
    pendingSave = false; lastSave = now;
  }
  uint32_t idle = now - lastInput;
  if (view != View::Home && idle >= config::kReturnHomeMs) { goHome(); message[0] = 0; }
  bool wantDim = idle >= config::kDimAfterMs && !screen.animating(now);
  if (wantDim != dimmed) {
    dimmed = wantDim; screen.brightness(dimmed ? config::kDimBrightness : config::kBrightness); power::setLowPower(dimmed);
  }
  if (message[0] && uint32_t(now - messageAt) > config::kMessageMs) message[0] = 0;
  uint32_t period = dimmed && !screen.animating(now) ? config::kDimFrameMs : config::kFrameMs;
  if (uint32_t(now - lastFrame) >= period) {
    lastFrame = now; screen.draw(pet, view, selection, view == View::Shop ? shop : focus, message, game, now);
  }
  uint32_t sinceFrame = uint32_t(millis() - lastFrame);
  if (dimmed && !screen.animating(now)) power::idle(sinceFrame >= period ? 0 : period - sinceFrame);
  else delay(1);
}

#include <Arduino.h>
#include "pet/Pet.h"
#include "storage/Save.h"
#include "display/Screen.h"
#include "input/Button.h"
namespace {
Pet pet; Save save; Screen screen; Button nextButton, actionButton;
View view = View::Home;
uint8_t selection = 0, focus = 0, shop = 1;
bool pendingSave = false, dimmed = false;
uint32_t lastSecond = 0, lastFrame = 0, lastSave = 0, lastAction = 0, messageAt = 0, lastInput = 0, lastPetting = 0;
char message[40] = "";
void say(const char* value, uint32_t now) { strlcpy(message, value, sizeof(message)); messageAt = now; }
void changed(uint32_t now) { pendingSave = true; lastAction = now; }
void perform(uint32_t now) {
  if (selection >= 5) {
    view = selection == 5 ? View::Stats : selection == 6 ? View::Objectives : selection == 7 ? View::Customize : selection == 8 ? View::Shop : View::ConfirmReset;
    return;
  }
  Result result = pet.act(static_cast<Action>(selection));
  const char* errors[] = {"Feito!", "Acorde primeiro", "Preciso dormir", "Moedas insuficientes", "Ja esta no maximo"};
  if (result == Result::Done) {
    changed(now); view = View::Home; screen.animate(static_cast<Action>(selection), now);
  } else say(errors[static_cast<uint8_t>(result)], now);
}
}
void setup() {
  Serial.begin(115200); pinMode(35, INPUT); pinMode(0, INPUT_PULLUP);
  nextButton.begin(digitalRead(35) == LOW, millis()); actionButton.begin(digitalRead(0) == LOW, millis());
  bool restored = save.begin(pet), displayed = screen.begin();
  lastSecond = lastSave = lastInput = millis(); lastPetting = millis() - 30000;
  if (!restored) { say("Falha NVS; ver serial", millis()); view = View::Care; }
  Serial.printf("TTGO Virtual Pet 2.0.0 / display=%s NVS=%s idade=%lu\n", displayed ? "240x135" : "FAIL", restored ? "OK" : "FAIL", (unsigned long)pet.state().ageMinutes);
}
void loop() {
  uint32_t now = millis();
  bool leftDown = digitalRead(35) == LOW, rightDown = digitalRead(0) == LOW;
  ButtonEvent left = nextButton.update(leftDown, now), right = actionButton.update(rightDown, now);
  if (leftDown || rightDown || left != ButtonEvent::None || right != ButtonEvent::None) lastInput = now;
  if (!screen.animating(now)) {
    if (right == ButtonEvent::Hold) { view = view == View::Home ? View::Stats : View::Home; message[0] = 0; }
    else if (left == ButtonEvent::Hold) {
      if (view == View::Home) { pet.act(Action::Sleep); changed(now); }
      else view = View::Home;
    } else if (view == View::Home) {
      if (left == ButtonEvent::Click) view = View::Care;
      if (right == ButtonEvent::Click && uint32_t(now - lastPetting) >= 30000 && pet.act(Action::Pet) == Result::Done) {
        lastPetting = now; changed(now); screen.affection(now);
      }
    } else if (view == View::Care) {
      if (left == ButtonEvent::Click) selection = (selection + 1) % 10;
      if (right == ButtonEvent::Click) perform(now);
    } else if (view == View::ConfirmReset) {
      if (left == ButtonEvent::Click) view = View::Care;
      if (right == ButtonEvent::Click) { pet.reset(); changed(now); selection = 0; view = View::Home; screen.affection(now); }
    } else if (view == View::Customize) {
      if (left == ButtonEvent::Click) focus = (focus + 1) % 3;
      if (right == ButtonEvent::Click) {
        if (!focus) pet.cycleName(); else if (focus == 1) pet.cycleColor(); else pet.cycleAccessory();
        changed(now);
      }
    } else if (view == View::Shop) {
      if (left == ButtonEvent::Click) shop = shop % 3 + 1;
      if (right == ButtonEvent::Click) {
        Result result = pet.buy(shop);
        if (result == Result::Done) { changed(now); say("Acessorio equipado", now); }
        else say("Moedas insuficientes", now);
      }
    } else if (view == View::Objectives && right == ButtonEvent::Click) {
      if (pet.claimReward()) { changed(now); say("Recompensa: +8 moedas", now); }
      else say(pet.state().claimed ? "Recompensa ja recebida" : "Complete os objetivos", now);
    }
  }
  uint32_t seconds = uint32_t(now - lastSecond) / 1000;
  uint8_t oldStage = pet.stage();
  if (seconds) { lastSecond += seconds * 1000; pet.advance(seconds); }
  if (oldStage != pet.stage()) { changed(now); view = View::Home; screen.celebrate(now); }
  if ((pendingSave && uint32_t(now - lastAction) >= 3000) || uint32_t(now - lastSave) >= 300000) {
    if (!save.write(pet)) { say("Falha ao salvar NVS", now); Serial.println("Pet: falha NVS"); }
    pendingSave = false; lastSave = now;
  }
  uint32_t idle = now - lastInput;
  if (view != View::Home && idle >= 20000) { view = View::Home; message[0] = 0; }
  bool wantDim = idle >= 45000 && !screen.animating(now);
  if (wantDim != dimmed) { dimmed = wantDim; screen.brightness(dimmed ? 35 : 180); }
  if (message[0] && uint32_t(now - messageAt) > 3500) message[0] = 0;
  uint32_t period = dimmed && !screen.animating(now) ? 400 : 100;
  if (uint32_t(now - lastFrame) >= period) {
    lastFrame = now; screen.draw(pet, view, selection, view == View::Shop ? shop : focus, message, now);
  }
  delay(1);
}

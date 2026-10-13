#include <Arduino.h>
#include "pet/Pet.h"
#include "storage/Save.h"
#include "display/Screen.h"
#include "input/Button.h"
namespace {
Pet pet; Save save; Screen screen; Button nextButton, actionButton;
uint8_t selection = 0;
bool stats = false, confirmReset = false, pendingSave = false;
uint32_t lastSecond = 0, lastFrame = 0, lastSave = 0, lastAction = 0, messageAt = 0;
char message[40] = "";
void say(const char* value, uint32_t now) { strlcpy(message, value, sizeof(message)); messageAt = now; }
void perform(uint32_t now) {
  if (selection == 5) { confirmReset = true; return; }
  Result result = pet.act(static_cast<Action>(selection));
  const char* messages[] = {"Feito!", "Acorde o Pip primeiro", "Preciso dormir", "Preciso de moedas", "Ja esta no maximo"};
  say(messages[static_cast<uint8_t>(result)], now);
  if (result == Result::Done) { pendingSave = true; lastAction = now; }
}
}
void setup() {
  Serial.begin(115200); pinMode(35, INPUT); pinMode(0, INPUT_PULLUP);
  nextButton.begin(digitalRead(35) == LOW, millis()); actionButton.begin(digitalRead(0) == LOW, millis());
  bool restored = save.begin(pet);
  bool displayed = screen.begin();
  lastSecond = lastSave = millis();
  if (!restored) say("NVS falhou: nao salva", millis());
  Serial.printf("TTGO Virtual Pet 1.0.0 / display=%s NVS=%s idade=%lu\n", displayed ? "240x135" : "FAIL", restored ? "OK" : "FAIL", (unsigned long)pet.state().ageMinutes);
}
void loop() {
  uint32_t now = millis();
  ButtonEvent left = nextButton.update(digitalRead(35) == LOW, now);
  ButtonEvent right = actionButton.update(digitalRead(0) == LOW, now);
  if (confirmReset) {
    if (left == ButtonEvent::Click || left == ButtonEvent::Hold || right == ButtonEvent::Hold) confirmReset = false;
    else if (right == ButtonEvent::Click) {
      pet.reset(); confirmReset = false; stats = false; selection = 0;
      pendingSave = true; lastAction = now; say("Um novo Pip nasceu!", now);
    }
  } else {
    if (left == ButtonEvent::Click) selection = (selection + 1) % 6;
    if (right == ButtonEvent::Click) perform(now);
    if (right == ButtonEvent::Hold) stats = !stats;
    if (left == ButtonEvent::Hold) {
      pet.act(Action::Sleep); pendingSave = true; lastAction = now;
      say(pet.state().sleeping ? "Boa noite!" : "Bom dia!", now);
    }
  }
  uint32_t seconds = uint32_t(now - lastSecond) / 1000;
  if (seconds) { lastSecond += seconds * 1000; pet.advance(seconds); }
  if ((pendingSave && uint32_t(now - lastAction) >= 3000) || uint32_t(now - lastSave) >= 300000) {
    if (!save.write(pet)) say("Falha ao salvar NVS", now);
    pendingSave = false; lastSave = now;
    Serial.printf("Pet: idade=%lu comida=%u energia=%u saude=%u moedas=%u\n", (unsigned long)pet.state().ageMinutes, pet.state().food, pet.state().energy, pet.state().health, pet.state().coins);
  }
  if (message[0] && uint32_t(now - messageAt) > 3500) message[0] = 0;
  if (uint32_t(now - lastFrame) >= 100) { lastFrame = now; screen.draw(pet, selection, stats, confirmReset, message, now); }
  delay(1);
}

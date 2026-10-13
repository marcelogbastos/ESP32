#include "display/Screen.h"
namespace { constexpr uint16_t bg = 0x0843, panel = 0x1927, teal = 0x4ed9, muted = 0x9cd3; }
bool Screen::begin() {
  display_.init(); display_.setRotation(1); pinMode(4, OUTPUT); digitalWrite(4, HIGH);
  frame_.setColorDepth(8); ready_ = frame_.createSprite(240, 135) != nullptr;
  if (!ready_) { display_.fillScreen(TFT_BLACK); display_.setTextColor(TFT_RED); display_.println("Falha de memoria da tela"); }
  return ready_;
}
void Screen::label(int x, int y, const String& value, uint16_t color, uint8_t font) {
  frame_.setTextColor(color); frame_.drawString(value, x, y, font);
}
void Screen::bar(int x, int y, const char* title, uint8_t value) {
  label(x, y, title, muted); label(x + 65, y, String(value));
  frame_.fillRoundRect(x, y + 10, 87, 5, 2, panel);
  if (value) frame_.fillRect(x, y + 10, value * 87 / 100, 5, value < 25 ? TFT_ORANGE : teal);
}
void Screen::character(const Pet& pet, uint32_t now) {
  const PetState& state = pet.state();
  int size = pet.stage() == 0 ? 34 : pet.stage() == 1 ? 42 : 48;
  int left = 61 - size / 2, top = 49 + (state.sleeping ? 3 : (now / 400) % 2 * 2);
  uint16_t color = pet.stage() == 0 ? TFT_PINK : pet.stage() == 1 ? TFT_SKYBLUE : teal;
  frame_.fillEllipse(62, 98, 31, 4, panel);
  frame_.fillTriangle(left + 1, top + 8, left + 6, top - 10, left + 16, top + 8, color);
  frame_.fillTriangle(left + size - 1, top + 8, left + size - 6, top - 10, left + size - 16, top + 8, color);
  frame_.fillTriangle(left + 5, top + 5, left + 6, top - 4, left + 12, top + 5, TFT_PINK);
  frame_.fillRoundRect(left, top, size, size, 10, color);
  frame_.fillRoundRect(left + size - 1, top + 22, 14, 7, 3, color);
  frame_.fillRoundRect(left + 4, top + size - 2, 10, 6, 2, color);
  frame_.fillRoundRect(left + size - 14, top + size - 2, 10, 6, 2, color);
  int eye = top + size / 3;
  bool blink = now % 4200 < 180 || state.sleeping;
  for (int x : {left + 9, left + size - 13}) {
    if (state.health < 15) {
      frame_.drawLine(x, eye, x + 5, eye + 5, bg); frame_.drawLine(x + 5, eye, x, eye + 5, bg);
    } else if (blink) frame_.drawFastHLine(x, eye + 3, 6, bg);
    else { frame_.fillRoundRect(x, eye, 5, 8, 2, bg); frame_.drawPixel(x + 1, eye + 1, TFT_WHITE); }
  }
  frame_.fillTriangle(59, eye + 10, 65, eye + 10, 62, eye + 13, bg);
  frame_.drawLine(57, eye + 16, 62, eye + (state.joy < 25 ? 14 : 18), bg);
  frame_.drawLine(62, eye + (state.joy < 25 ? 14 : 18), 67, eye + 16, bg);
  if (state.sleeping) label(94, 43 - (now / 500) % 3 * 3, "zZ", muted, 2);
  else if (state.health < 25) label(95, 37, "+", TFT_ORANGE, 2);
  else if (state.joy >= 70) {
    frame_.fillCircle(100, 43, 3, TFT_PINK); frame_.fillCircle(106, 43, 3, TFT_PINK);
    frame_.fillTriangle(97, 43, 109, 43, 103, 51, TFT_PINK);
  }
}
void Screen::draw(const Pet& pet, uint8_t selection, bool stats, bool confirmReset, const char* message, uint32_t now) {
  if (!ready_) return;
  const PetState& state = pet.state();
  frame_.fillSprite(bg); frame_.fillRect(0, 0, 240, 24, panel);
  label(8, 4, "PIP", teal, 2);
  const char* stage[] = {"BEBE", "JOVEM", "ADULTO"};
  label(50, 9, stage[pet.stage()], muted); label(151, 9, String("Moedas ") + state.coins, TFT_YELLOW);
  if (confirmReset) {
    label(14, 38, "Comecar um novo bichinho?", TFT_YELLOW, 2);
    label(14, 67, "O progresso atual sera reiniciado.", muted);
    label(14, 92, "35 cancela / 0 confirma", teal);
  } else if (stats) {
    bar(10, 31, "Comida", state.food); bar(10, 55, "Energia", state.energy); bar(10, 79, "Alegria", state.joy);
    bar(133, 31, "Higiene", state.hygiene); bar(133, 55, "Saude", state.health);
    label(133, 82, String("Idade ") + state.ageMinutes + "m", muted);
    label(133, 98, state.sleeping ? "Dormindo" : "Acordado", teal);
  } else {
    character(pet, now);
    bar(140, 31, "Comida", state.food); bar(140, 54, "Energia", state.energy); bar(140, 77, "Alegria", state.joy);
    label(9, 28, message[0] ? message : state.sleeping ? "Sonhando..." : state.health < 25 ? "Preciso de cuidados" : "Oi! Cuide de mim", state.health < 25 ? TFT_ORANGE : muted);
    label(10, 104, String("Saude ") + state.health + "  Higiene " + state.hygiene, muted);
  }
  if (!confirmReset) {
    const char* actions[] = {"Comida -1 moeda", "Brincar +2 moedas", "Banho", state.sleeping ? "Acordar" : "Dormir", "Remedio -2 moedas", "Novo bichinho"};
    frame_.fillRoundRect(5, 115, 230, 17, 4, panel);
    label(10, 120, String("> ") + actions[selection], teal);
  }
  frame_.pushSprite(0, 0);
}

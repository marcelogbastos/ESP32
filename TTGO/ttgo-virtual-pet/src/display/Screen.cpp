#include "display/Screen.h"
namespace {
constexpr uint16_t bg = 0x0843, panel = 0x1927, teal = 0x4ed9, muted = 0x9cd3;
const char* names[] = {"Pip", "Nino", "Luma", "Bibi", "Kiko", "Tico", "Lilo", "Mimi"};
const char* colors[] = {"Evolucao", "Rosa", "Azul", "Verde", "Amarelo", "Roxo"};
const char* accessories[] = {"Nenhum", "Coleira", "Laco", "Chapeu"};
const char* moods[] = {"Tranquilo", "Feliz", "Com fome", "Cansado", "Sujo", "Triste", "Doente", "Dormindo", "Carinhoso"};
}
bool Screen::begin() {
  display_.init(); display_.setRotation(1); pinMode(4, OUTPUT); digitalWrite(4, HIGH);
  frame_.setColorDepth(8); ready_ = frame_.createSprite(240, 135) != nullptr;
  if (!ready_) { display_.fillScreen(TFT_BLACK); display_.setTextColor(TFT_RED); display_.println("Falha de memoria da tela"); }
  ledcSetup(0, 5000, 8); ledcAttachPin(4, 0); brightness(180);
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
  Mood mood = pet.mood();
  int size = pet.stage() == 0 ? 34 : pet.stage() == 1 ? 42 : 48;
  AnimationPose pose = animation_.pose(now);
  int shift = pose.active && pose.action == Action::Play ? (pose.frame % 4 - 1) * 3 : 0;
  int jump = pose.active && pose.action == Action::Play ? (pose.frame < 4 ? pose.frame : 7 - pose.frame) * 3 : 0;
  int left = 61 - size / 2 + shift, top = 49 + (state.sleeping ? 3 : (now / 400) % 2 * 2) - jump;
  int center = 61 + shift;
  uint16_t color = pet.stage() == 0 ? TFT_PINK : pet.stage() == 1 ? TFT_SKYBLUE : state.adultForm == 1 ? TFT_PINK : state.adultForm == 2 ? TFT_VIOLET : state.adultForm == 3 ? TFT_ORANGE : teal;
  const uint16_t palette[] = {color, TFT_PINK, TFT_SKYBLUE, TFT_GREEN, TFT_YELLOW, TFT_VIOLET};
  color = palette[state.color];
  frame_.fillEllipse(62, 98, 31, 4, panel);
  frame_.fillTriangle(left + 1, top + 8, left + 6, top - 10, left + 16, top + 8, color);
  frame_.fillTriangle(left + size - 1, top + 8, left + size - 6, top - 10, left + size - 16, top + 8, color);
  frame_.fillTriangle(left + 5, top + 5, left + 6, top - 4, left + 12, top + 5, TFT_PINK);
  frame_.fillRoundRect(left, top, size, size, 10, color);
  frame_.fillRoundRect(left + size - 1, top + 22, 14, 7, 3, color);
  frame_.fillRoundRect(left + 4, top + size - 2, 10, 6, 2, color);
  frame_.fillRoundRect(left + size - 14, top + size - 2, 10, 6, 2, color);
  int eye = top + size / 3;
  bool blink = now % 4200 < 180 || mood == Mood::Tired || state.sleeping || (pose.active && pose.action == Action::Clean && pose.frame % 2 == 0);
  for (int x : {left + 9, left + size - 13}) {
    if (state.health < 15) {
      frame_.drawLine(x, eye, x + 5, eye + 5, bg); frame_.drawLine(x + 5, eye, x, eye + 5, bg);
    } else if (blink) frame_.drawFastHLine(x, eye + 3, 6, bg);
    else { frame_.fillRoundRect(x, eye, 5, 8, 2, bg); frame_.drawPixel(x + 1, eye + 1, TFT_WHITE); }
  }
  frame_.fillTriangle(center - 2, eye + 10, center + 4, eye + 10, center + 1, eye + 13, bg);
  if (pose.active && (pose.action == Action::Feed || pose.action == Action::Heal)) {
    frame_.fillEllipse(center + 1, eye + 17, pose.frame % 2 ? 4 : 2, pose.frame % 2 ? 3 : 1, bg);
  } else {
    frame_.drawLine(center - 4, eye + 16, center + 1, eye + (mood == Mood::Sad || mood == Mood::Ill || mood == Mood::Hungry ? 14 : 18), bg);
    frame_.drawLine(center + 1, eye + (mood == Mood::Sad || mood == Mood::Ill || mood == Mood::Hungry ? 14 : 18), center + 6, eye + 16, bg);
  }
  if (pose.active) {
    switch (pose.action) {
      case Action::Feed: {
        // Spoon approaches the mouth repeatedly; crumbs and a food bowl remain visible.
        int approach = pose.frame < 4 ? pose.frame : 7 - pose.frame;
        int x = 104 - approach * 14;
        frame_.drawLine(x + 3, eye + 18, x + 16, eye + 21, muted);
        frame_.fillEllipse(x, eye + 17, 5, 3, TFT_YELLOW);
        frame_.fillCircle(x, eye + 15, 2, TFT_ORANGE);
        frame_.fillRoundRect(18, 89, 17, 9, 3, TFT_SKYBLUE);
        frame_.drawFastHLine(17, 88, 19, TFT_WHITE);
        if (pose.frame >= 3 && pose.frame <= 5) {
          frame_.fillCircle(center - 6, eye + 22 + pose.frame, 1, TFT_YELLOW);
          frame_.fillCircle(center + 7, eye + 24, 1, TFT_YELLOW);
        }
        break;
      }
      case Action::Play: {
        int ballY = 90 - (pose.frame < 4 ? pose.frame : 7 - pose.frame) * 5;
        int ballX = 100 + (pose.frame % 3) * 4;
        frame_.fillCircle(ballX, ballY, 8, TFT_ORANGE);
        frame_.drawCircle(ballX, ballY, 8, TFT_WHITE);
        frame_.drawLine(ballX - 7, ballY, ballX + 7, ballY, bg);
        frame_.drawLine(ballX, ballY - 7, ballX, ballY + 7, bg);
        frame_.drawLine(left - 5, top + 20, left + 1, top + 13, color);
        frame_.drawLine(left + size - 1, top + 13, left + size + 7, top + 20, color);
        label(13, 45 - pose.frame % 3 * 3, "*", TFT_YELLOW, 2);
        break;
      }
      case Action::Clean: {
        frame_.fillRoundRect(28, 86, 69, 13, 5, TFT_SKYBLUE);
        frame_.drawFastHLine(29, 86, 67, TFT_WHITE);
        for (int i = 0; i < 6; ++i) {
          int x = 24 + i * 15;
          int y = 41 + (pose.frame * 5 + i * 7) % 44;
          frame_.drawCircle(x, y, 2 + i % 3, TFT_WHITE);
        }
        for (int i = 0; i < 5; ++i) {
          int x = 42 + i * 9, y = 40 + (pose.frame * 6 + i * 3) % 31;
          frame_.drawFastVLine(x, y, 4, TFT_CYAN);
        }
        frame_.fillRoundRect(45, 34, 38, 5, 2, muted);
        frame_.fillCircle(48, 86, 5, TFT_WHITE);
        frame_.fillCircle(60, 85, 6, TFT_WHITE);
        frame_.fillCircle(74, 87, 5, TFT_WHITE);
        break;
      }
      case Action::Heal: {
        int approach = pose.frame < 4 ? pose.frame : 7 - pose.frame;
        int x = 104 - approach * 14;
        frame_.fillRoundRect(x - 5, eye + 13, 10, 5, 2, TFT_WHITE);
        frame_.fillRect(x, eye + 13, 4, 5, TFT_PINK);
        frame_.fillRoundRect(18, 72, 13, 23, 3, TFT_WHITE);
        frame_.fillRect(20, 68, 9, 5, muted);
        frame_.fillRect(21, 79, 7, 3, TFT_RED);
        frame_.fillRect(23, 77, 3, 7, TFT_RED);
        int sparkleY = 63 - (pose.progress / 100) * 2;
        frame_.drawFastHLine(108, sparkleY, 7, TFT_GREEN);
        frame_.drawFastVLine(111, sparkleY - 3, 7, TFT_GREEN);
        break;
      }
      case Action::Sleep: case Action::Pet: break;
    }
  } else if (state.sleeping) label(94, 43 - (now / 500) % 3 * 3, "zZ", muted, 2);
  else if (state.health < 25) label(95, 37, "+", TFT_ORANGE, 2);
  else if (mood == Mood::Affection || (affectionate_ && uint32_t(now - affectionAt_) < 3000) || mood == Mood::Happy) {
    frame_.fillCircle(100, 43, 3, TFT_PINK); frame_.fillCircle(106, 43, 3, TFT_PINK);
    frame_.fillTriangle(97, 43, 109, 43, 103, 51, TFT_PINK);
  }
  if (!pose.active && mood == Mood::Hungry) {
    frame_.drawRoundRect(19, 86, 18, 9, 3, TFT_ORANGE);
    frame_.drawLine(52, top + size - 10, 57, top + size - 12, bg);
    frame_.drawLine(57, top + size - 12, 63, top + size - 10, bg);
  }
  if (!pose.active && mood == Mood::Dirty) {
    for (int i = 0; i < 3; ++i) frame_.fillCircle(left + 7 + i * 11, top + size - 12, 2, TFT_BROWN);
    frame_.fillCircle(99, 58 + now / 160 % 7, 1, muted);
    frame_.drawLine(96, 56 + now / 160 % 7, 102, 56 + now / 160 % 7, muted);
  }
  if (state.accessory == 1) {
    frame_.fillRect(left + 2, top + size - 10, size - 4, 4, TFT_RED);
    frame_.fillCircle(center + 1, top + size - 7, 3, TFT_YELLOW);
  } else if (state.accessory == 2) {
    frame_.fillTriangle(center - 10, top - 2, center - 10, top + 8, center + 1, top + 3, TFT_VIOLET);
    frame_.fillTriangle(center + 12, top - 2, center + 12, top + 8, center + 1, top + 3, TFT_VIOLET);
  } else if (state.accessory == 3) {
    frame_.fillRect(left + 5, top - 8, size - 10, 9, muted);
    frame_.fillRect(left - 2, top - 1, size + 4, 3, TFT_ORANGE);
  }
  if (celebrating_ && uint32_t(now - celebrationAt_) < 5000) {
    for (int i = 0; i < 5; ++i) label(15 + i * 21, 30 + (now / 100 + i * 8) % 65, "*", TFT_YELLOW, 2);
  }

}
void Screen::draw(const Pet& pet, View view, uint8_t selection, uint8_t focus, const char* message, uint32_t now) {
  if (!ready_) return;
  const PetState& state = pet.state();
  frame_.resetViewport(); frame_.fillSprite(bg);
  if (view == View::Home) {
    // Minimal home: name, situational pet animation and age.
    frame_.setTextColor(teal);
    frame_.drawCentreString(names[state.name], 120, 4, 2);
    frame_.setViewport(58, 0, 124, 135, true);
    character(pet, now); frame_.resetViewport();
    frame_.setTextColor(muted);
    String age = String("Idade: ") + (state.ageMinutes / 60) + "h " + (state.ageMinutes % 60) + "m";
    frame_.drawCentreString(age, 120, 120, 1);
    frame_.pushSprite(0, 0); return;
  }
  frame_.fillRect(0, 0, 240, 24, panel);
  const char* titles[] = {"", "CUIDADOS", "ESTATISTICAS", "OBJETIVOS", "PERSONALIZAR", "LOJINHA", "NOVO BICHINHO"};
  label(8, 4, titles[static_cast<uint8_t>(view)], teal, 2);
  if (view == View::Care) {
    const char* actions[] = {"Comida -1 moeda", "Brincar +2 moedas", "Banho", state.sleeping ? "Acordar" : "Dormir", "Remedio -2 moedas", "Estatisticas", "Objetivos", "Personalizar", "Lojinha", "Novo bichinho"};
    uint8_t start = selection / 4 * 4;
    for (int i = start; i < 10 && i < start + 4; ++i) {
      int y = 29 + (i - start) * 21;
      if (i == selection) frame_.fillRoundRect(5, y, 230, 20, 4, panel);
      label(10, y + 2, String(i == selection ? "> " : "  ") + actions[i], i == selection ? TFT_YELLOW : TFT_WHITE, 2);
    }
  } else if (view == View::Stats) {
    bar(10, 31, "Comida", state.food); bar(10, 55, "Energia", state.energy); bar(10, 79, "Alegria", state.joy);
    bar(133, 31, "Higiene", state.hygiene); bar(133, 55, "Saude", state.health); bar(133, 79, "Vinculo", state.bond);
    label(10, 103, String(names[state.name]) + " " + moods[static_cast<uint8_t>(pet.mood())], teal);
    label(133, 103, String(state.ageMinutes) + "m " + state.coins + " moedas", muted);
  } else if (view == View::Objectives) {
    label(10, 30, String("Dia ligado ") + (state.day + 1), muted);
    label(10, 44, String("Alimentar: ") + state.feedsToday + "/2");
    label(10, 59, String("Brincar: ") + state.playsToday + "/2");
    label(10, 74, String("Banho: ") + state.bathsToday + "/1");
    label(10, 89, String("Dormir: ") + state.restToday + "/5 minutos");
    label(10, 105, state.claimed ? "Recompensa recebida" : "0: resgatar +8 moedas", teal);
  } else if (view == View::Customize) {
    label(10, 36, String(focus == 0 ? "> Nome: " : "  Nome: ") + names[state.name], focus == 0 ? TFT_YELLOW : TFT_WHITE, 2);
    label(10, 60, String(focus == 1 ? "> Cor: " : "  Cor: ") + colors[state.color], focus == 1 ? TFT_YELLOW : TFT_WHITE, 2);
    label(10, 84, String(focus == 2 ? "> Item: " : "  Item: ") + accessories[state.accessory], focus == 2 ? TFT_YELLOW : TFT_WHITE, 2);
  } else if (view == View::Shop) {
    const uint8_t prices[] = {0, 6, 8, 10};
    label(10, 34, String("Moedas: ") + state.coins, muted);
    for (int i = 1; i <= 3; ++i)
      label(10, 49 + (i - 1) * 22, String(i == focus ? "> " : "  ") + accessories[i] + ((state.owned & (1u << i)) ? " (equipar)" : " - " + String(prices[i])), i == focus ? TFT_YELLOW : TFT_WHITE, 2);
  } else if (view == View::ConfirmReset) {
    label(10, 39, "Reiniciar o progresso?", TFT_YELLOW, 2);
    label(10, 73, "35 cancela / 0 confirma", teal);
  }
  if (view != View::ConfirmReset) label(8, 123, message[0] ? message : "35 proximo | 0 OK | segure: voltar", muted);
  frame_.pushSprite(0, 0);
}

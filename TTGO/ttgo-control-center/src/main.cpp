#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <TFT_eSPI.h>
#include "board.h"
#include "input/Button.h"
#include "core/Module.h"
#include "network/Network.h"

namespace {
TFT_eSPI display;
Network network;
Preferences preferences;
Button nextButton, selectButton;
uint8_t selected = 0;
bool inside = false, dirty = true, storageReady = false;
uint32_t lastFrame = 0, lastLog = 0;
uint32_t nextCount = 0, selectCount = 0;
void dashboard(TFT_eSPI& tft, uint32_t now) {
  tft.printf("Uptime: %lu s\nHeap: %lu bytes\n", (unsigned long)(now / 1000), (unsigned long)ESP.getFreeHeap());
  if (WiFi.status() == WL_CONNECTED) {
    tft.printf("Wi-Fi: %d dBm\n", WiFi.RSSI());
    tft.println(WiFi.localIP());
  } else tft.println(network.configured() ? "Wi-Fi: conectando" : "Wi-Fi: sem configuracao");
}
void hardware(TFT_eSPI& tft, uint32_t) {
  tft.printf("GPIO35: %lu  GPIO0: %lu\n", (unsigned long)nextCount, (unsigned long)selectCount);
  tft.println("ST7789 / ESP32 classico");
  tft.println("NVS: " + String(storageReady ? "OK" : "falha"));
  const uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE};
  for (int i = 0; i < 3; ++i) tft.fillRect(8 + i * 76, 90, 70, 18, colors[i]);
}
Module modules[] = {{"Dashboard", true, dashboard}, {"Teste de hardware", true, hardware}};
constexpr uint8_t moduleCount = sizeof(modules) / sizeof(modules[0]);
void render(uint32_t now) {
  display.fillScreen(TFT_BLACK);
  display.drawRect(0, 0, Board::width, Board::height, TFT_DARKGREY);
  display.setTextColor(TFT_CYAN, TFT_BLACK);
  display.setCursor(8, 6, 2);
  display.println(inside ? modules[selected].name : "TTGO Control Center");
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setCursor(8, 30, 2);
  if (inside) modules[selected].render(display, now);
  else for (uint8_t i = 0; i < moduleCount; ++i) {
    if (!modules[i].enabled) continue;
    display.setTextColor(i == selected ? TFT_YELLOW : TFT_WHITE, TFT_BLACK);
    display.printf("%s %s\n", i == selected ? ">" : " ", modules[i].name);
  }
  display.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  display.setCursor(8, 117, 1);
  display.print("35: proximo   0: abrir/voltar");
}
}
void setup() {
  Serial.begin(115200);
  pinMode(Board::nextButton, INPUT);
  pinMode(Board::selectButton, INPUT_PULLUP);
  nextButton.begin(digitalRead(Board::nextButton) == LOW, millis());
  selectButton.begin(digitalRead(Board::selectButton) == LOW, millis());
  display.init();
  display.setRotation(1);
  pinMode(Board::backlight, OUTPUT);
  digitalWrite(Board::backlight, HIGH);
  storageReady = preferences.begin("ttgo", false);
  if (storageReady) selected = preferences.getUChar("module", 0);
  if (selected >= moduleCount || !modules[selected].enabled) selected = 0;
  network.begin();
  Serial.printf("TTGO phase1 / chip=%s rev=%u flash=%lu display=%dx%d NVS=%s\n",
    ESP.getChipModel(), ESP.getChipRevision(), (unsigned long)ESP.getFlashChipSize(),
    display.width(), display.height(), storageReady ? "OK" : "FAIL");
}
void loop() {
  const uint32_t now = millis();
  if (nextButton.update(digitalRead(Board::nextButton) == LOW, now)) {
    ++nextCount;
    if (!inside) {
      for (uint8_t i = 0; i < moduleCount; ++i) {
        selected = (selected + 1) % moduleCount;
        if (modules[selected].enabled) break;
      }
    }
    dirty = true;
  }
  if (selectButton.update(digitalRead(Board::selectButton) == LOW, now)) {
    ++selectCount;
    inside = !inside;
    if (inside && storageReady) preferences.putUChar("module", selected);
    dirty = true;
  }
  network.tick(now);
  if (dirty || (inside && uint32_t(now - lastFrame) >= 1000)) {
    render(now); lastFrame = now; dirty = false;
  }
  if (uint32_t(now - lastLog) >= 10000) {
    lastLog = now;
    Serial.printf("heap=%lu wifi=%d buttons=%lu/%lu\n", (unsigned long)ESP.getFreeHeap(),
      WiFi.status(), (unsigned long)nextCount, (unsigned long)selectCount);
  }
  yield();
}

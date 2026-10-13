#pragma once
#include <TFT_eSPI.h>
#include "core/Config.h"
#include "services/State.h"
#include "services/Worker.h"
#include "network/Portal.h"
#include "modules/BluetoothScanner.h"
#include "core/Button.h"
struct ScanEntry { char ssid[33]; int32_t rssi; uint8_t channel; bool secured; };
class UI {
 public:
  UI(const Config& config, SharedState& state, Worker& worker, Portal& portal)
    : config_(config), state_(state), worker_(worker), portal_(portal), canvas_(&display_) {}
  bool begin();
  void input(ButtonEvent left, ButtonEvent right);
  void tick(uint32_t now);
 private:
  const Config& config_;
  SharedState& state_;
  Worker& worker_;
  Portal& portal_;
  TFT_eSPI display_;
  TFT_eSprite canvas_;
  bool ready_ = false, menu_ = true, dirty_ = true, scanning_ = false;
  uint8_t selected_ = 0, page_ = 0, scanCount_ = 0;
  uint32_t lastFrame_ = 0;
  ScanEntry scans_[24];
  BluetoothScanner bluetooth_;
  bool bluetoothPending_ = false, bluetoothRunning_ = false;
  uint32_t bluetoothRequested_ = 0;
  char bluetoothMessage_[40] = "0: buscar BLE e classico";
  void scanBluetooth();
  char scanMessage_[40] = "0: iniciar busca";
  void enter();
  void scan();
  void render(uint32_t now);
  void text(int x, int y, const String& value, uint16_t color = TFT_WHITE, uint8_t font = 2);
};

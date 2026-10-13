#pragma once
#include <Arduino.h>
#include <atomic>
#include <esp_gap_ble_api.h>
#include <esp_gap_bt_api.h>
struct BluetoothEntry {
  char name[33] = "";
  uint8_t address[6] = {};
  int16_t rssi = -127;
  bool hasRSSI = false, classic = false;
};
struct BluetoothSnapshot {
  BluetoothEntry devices[24];
  uint8_t count = 0;
  bool busy = false;
  char status[40] = "0: buscar BLE e classico";
};
class BluetoothScanner {
 public:
  bool start();
  bool busy() const { return busy_.load(); }
  BluetoothSnapshot snapshot();
 private:
  static BluetoothScanner* active_;
  static void task(void* context);
  static void bleEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* data);
  static void classicEvent(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t* data);
  void run();
  void status(const char* message);
  void add(const BluetoothEntry& entry);
  portMUX_TYPE lock_ = portMUX_INITIALIZER_UNLOCKED;
  BluetoothSnapshot state_;
  std::atomic<bool> busy_{false};
  std::atomic<int> phase_{0}; // 1 BLE pending/running, 2 BLE ended, 3 classic, 4 ended, -1 error.
};

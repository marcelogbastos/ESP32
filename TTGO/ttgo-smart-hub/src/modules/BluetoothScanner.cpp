#include "modules/BluetoothScanner.h"
#include <esp_bt.h>
#include <esp32-hal-bt.h>
#include "modules/Advertising.h"
#include <esp_bt_main.h>
BluetoothScanner* BluetoothScanner::active_ = nullptr;
void BluetoothScanner::status(const char* message) {
  portENTER_CRITICAL(&lock_); strlcpy(state_.status, message, sizeof(state_.status)); portEXIT_CRITICAL(&lock_);
}
BluetoothSnapshot BluetoothScanner::snapshot() {
  portENTER_CRITICAL(&lock_); BluetoothSnapshot copy = state_; portEXIT_CRITICAL(&lock_);
  copy.busy = busy_.load(); return copy;
}
bool BluetoothScanner::start() {
  bool expected = false;
  if (!busy_.compare_exchange_strong(expected, true)) return false;
  portENTER_CRITICAL(&lock_); state_.count = 0; portEXIT_CRITICAL(&lock_);
  status("Iniciando Bluetooth..."); active_ = this;
  if (xTaskCreatePinnedToCore(task, "hub-bluetooth", 4096, this, 1, nullptr, 0) != pdPASS) {
    status("Falha memoria: tarefa BT"); busy_ = false; return false;
  }
  return true;
}
void BluetoothScanner::add(const BluetoothEntry& incoming) {
  portENTER_CRITICAL(&lock_);
  BluetoothEntry entry = incoming;
  int found = -1;
  for (int i = 0; i < state_.count; ++i)
    if (state_.devices[i].classic == entry.classic && !memcmp(state_.devices[i].address, entry.address, 6)) { found = i; break; }
  if (found >= 0) {
    if (!entry.name[0]) strlcpy(entry.name, state_.devices[found].name, sizeof(entry.name));
    if (!entry.hasRSSI) { entry.rssi = state_.devices[found].rssi; entry.hasRSSI = state_.devices[found].hasRSSI; }
    for (int i = found; i + 1 < state_.count; ++i) state_.devices[i] = state_.devices[i + 1];
    --state_.count;
  }
  int position = 0;
  while (position < state_.count && state_.devices[position].rssi >= entry.rssi) ++position;
  if (position < 24) {
    if (state_.count < 24) ++state_.count;
    for (int i = state_.count - 1; i > position; --i) state_.devices[i] = state_.devices[i - 1];
    state_.devices[position] = entry;
  }
  portEXIT_CRITICAL(&lock_);
}
void BluetoothScanner::bleEvent(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t* data) {
  if (!active_) return;
  if (event == ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT) {
    if (data->scan_param_cmpl.status != ESP_BT_STATUS_SUCCESS || esp_ble_gap_start_scanning(5) != ESP_OK) active_->phase_ = -1;
  } else if (event == ESP_GAP_BLE_SCAN_START_COMPLETE_EVT && data->scan_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
    active_->phase_ = -1;
  } else if (event == ESP_GAP_BLE_SCAN_RESULT_EVT) {
    if (data->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
      BluetoothEntry entry;
      memcpy(entry.address, data->scan_rst.bda, 6); entry.rssi = data->scan_rst.rssi; entry.hasRSSI = true;
      advertisingName(entry.name, data->scan_rst.ble_adv, data->scan_rst.adv_data_len);
      advertisingName(entry.name, data->scan_rst.ble_adv + data->scan_rst.adv_data_len, data->scan_rst.scan_rsp_len);
      active_->add(entry);
    } else if (data->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_CMPL_EVT) active_->phase_ = 2;
  }
}
void BluetoothScanner::classicEvent(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t* data) {
  if (!active_) return;
  if (event == ESP_BT_GAP_DISC_STATE_CHANGED_EVT && data->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED) {
    active_->phase_ = 4;
  } else if (event == ESP_BT_GAP_DISC_RES_EVT) {
    BluetoothEntry entry; entry.classic = true; memcpy(entry.address, data->disc_res.bda, 6);
    for (int i = 0; i < data->disc_res.num_prop; ++i) {
      const auto& property = data->disc_res.prop[i];
      if (property.type == ESP_BT_GAP_DEV_PROP_RSSI && property.len >= 1) {
        entry.rssi = *static_cast<int8_t*>(property.val); entry.hasRSSI = true;
      } else if (property.type == ESP_BT_GAP_DEV_PROP_BDNAME) {
        copyRadioName(entry.name, static_cast<uint8_t*>(property.val), property.len);
      } else if (property.type == ESP_BT_GAP_DEV_PROP_EIR) {
        advertisingName(entry.name, static_cast<uint8_t*>(property.val), property.len);
      }
    }
    active_->add(entry);
  }
}
void BluetoothScanner::task(void* context) { static_cast<BluetoothScanner*>(context)->run(); vTaskDelete(nullptr); }
void BluetoothScanner::run() {
  // Reference the Arduino BT HAL so its strong btInUse() keeps the BT pool
  // available at boot; otherwise Arduino irreversibly releases that memory.
  if (btStarted()) { status("Controlador Bluetooth ocupado"); busy_ = false; return; }
  bool success = false;
  esp_bt_controller_config_t configuration = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  esp_err_t error = esp_bt_controller_init(&configuration);
  if (error == ESP_OK) error = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
  if (error == ESP_OK) error = esp_bluedroid_init();
  if (error == ESP_OK) error = esp_bluedroid_enable();
  if (error == ESP_OK) error = esp_ble_gap_register_callback(bleEvent);
  if (error == ESP_OK) error = esp_bt_gap_register_callback(classicEvent);
  if (error == ESP_OK) {
    esp_ble_scan_params_t parameters{};
    parameters.scan_type = BLE_SCAN_TYPE_ACTIVE; parameters.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
    parameters.scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL;
    parameters.scan_interval = 0x50; parameters.scan_window = 0x30;
    parameters.scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE;
    phase_ = 1; status("Buscando BLE (5s)...");
    error = esp_ble_gap_set_scan_params(&parameters);
    uint32_t started = millis();
    while (error == ESP_OK && phase_ == 1 && uint32_t(millis() - started) < 10000) vTaskDelay(pdMS_TO_TICKS(50));
    if (error == ESP_OK && phase_ == 2) {
      phase_ = 3; status("Buscando BT classico...");
      error = esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 4, 0);
      started = millis();
      while (error == ESP_OK && phase_ == 3 && uint32_t(millis() - started) < 15000) vTaskDelay(pdMS_TO_TICKS(50));
      success = error == ESP_OK && phase_ == 4;
      if (phase_ == 3) {
        esp_bt_gap_cancel_discovery();
        started = millis();
        while (phase_ == 3 && uint32_t(millis() - started) < 2000) vTaskDelay(pdMS_TO_TICKS(50));
      }
    } else {
      esp_ble_gap_stop_scanning();
      if (error == ESP_OK) error = ESP_FAIL;
    }
  }
  if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_ENABLED) esp_bluedroid_disable();
  if (esp_bluedroid_get_status() == ESP_BLUEDROID_STATUS_INITIALIZED) esp_bluedroid_deinit();
  if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_ENABLED) esp_bt_controller_disable();
  if (esp_bt_controller_get_status() == ESP_BT_CONTROLLER_STATUS_INITED) esp_bt_controller_deinit();
  char message[40];
  if (error != ESP_OK) snprintf(message, sizeof(message), "Falha Bluetooth: %d", error);
  else snprintf(message, sizeof(message), success ? "%u dispositivos / top 24" : "%u dispositivos / parcial", snapshot().count);
  status(message); busy_ = false;
}

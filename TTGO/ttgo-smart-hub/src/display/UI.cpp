#include "display/UI.h"
#include <WiFi.h>
#include <time.h>
namespace {
constexpr uint16_t background = 0x0843, panel = 0x10e6, accent = 0x4ed9, muted = 0x9cd3;
const char* names[] = {"Dashboard", "Clima", "Wi-Fi Analyzer", "MQTT", "Alertas", "Home Assistant", nullptr, nullptr, "Configuracao", "Bluetooth"};
String clipped(const char* value, size_t size) {
  String result(value);
  for (size_t i = 0; i < result.length(); ++i) if ((uint8_t)result[i] < 32) result.setCharAt(i, ' ');
  if (result.length() > size) result = result.substring(0, size - 2) + "..";
  return result;
}
const char* weatherName(int code) {
  if (code == 0) return "Ceu limpo";
  if (code <= 3) return "Nublado";
  if (code <= 48) return "Neblina";
  if (code <= 67) return "Chuva";
  if (code <= 77) return "Neve";
  if (code <= 86) return "Pancadas";
  return "Tempestade";
}
}
bool UI::begin() {
  display_.init(); display_.setRotation(1);
  pinMode(4, OUTPUT); digitalWrite(4, HIGH);
  canvas_.setColorDepth(8);
  ready_ = canvas_.createSprite(240, 135) != nullptr;
  if (!ready_) { display_.fillScreen(TFT_BLACK); display_.setTextColor(TFT_RED); display_.println("Falha memoria display"); }
  return ready_;
}
void UI::text(int x, int y, const String& value, uint16_t color, uint8_t font) {
  canvas_.setTextColor(color, background); canvas_.drawString(value, x, y, font);
}
void UI::enter() {
  menu_ = false; page_ = 0;
  if (selected_ == 1) worker_.request(Request::Weather);
  if (selected_ == 2 && !scanCount_) scan();
  if (selected_ == 5) worker_.request(Request::HomeAssistant);
}
void UI::input(ButtonEvent left, ButtonEvent right) {
  if (right == ButtonEvent::Hold) { menu_ = true; dirty_ = true; return; }
  if (left == ButtonEvent::Hold) {
    selected_ = 8; menu_ = false; page_ = 0;
    dirty_ = true; return;
  }
  if (menu_) {
    if (left == ButtonEvent::Click) {
      do { selected_ = (selected_ + 1) % 10; } while (!(config_.modules & (1u << selected_)));
    }
    if (right == ButtonEvent::Click) enter();
  } else {
    if (left == ButtonEvent::Click) {
      if (selected_ == 2 || selected_ == 4 || selected_ == 9) ++page_;
    }
    if (right == ButtonEvent::Click) {
      if (selected_ == 1) worker_.request(Request::Weather);
      else if (selected_ == 2) scan();
      else if (selected_ == 5) {
        // Two clicks within the confirmation view are necessary to control HA.
        if (page_ == 1) { if (!worker_.request(Request::Toggle)) state_.alert("Fila HA ocupada", 1); page_ = 0; }
        else page_ = 1;
      } else if (selected_ == 9) scanBluetooth();
      else if (selected_ == 8) {
        if (bluetoothPending_ || bluetoothRunning_) { state_.alert("Aguarde a busca Bluetooth", 1); return; }
        if (portal_.active()) portal_.close(); else portal_.open(); }
    }
  }
  if (left != ButtonEvent::None || right != ButtonEvent::None) dirty_ = true;
}
void UI::scan() {
  if (scanning_) return;
  if (bluetoothPending_ || bluetoothRunning_) { strlcpy(scanMessage_, "Aguarde busca Bluetooth", sizeof(scanMessage_)); return; }
  if (portal_.active()) { strlcpy(scanMessage_, "Feche painel em Configuracao", sizeof(scanMessage_)); return; }
  WiFi.scanDelete();
  int result = WiFi.scanNetworks(true, true, false, 180);
  scanning_ = result == WIFI_SCAN_RUNNING;
  strlcpy(scanMessage_, scanning_ ? "Buscando redes..." : "Falha ao iniciar busca", sizeof(scanMessage_));
}
void UI::scanBluetooth() {
  if (bluetoothPending_ || bluetoothRunning_) return;
  if (portal_.active() || scanning_) { strlcpy(bluetoothMessage_, "Feche painel / aguarde Wi-Fi", sizeof(bluetoothMessage_)); return; }
  worker_.pause(true); bluetoothPending_ = true; bluetoothRequested_ = millis();
  strlcpy(bluetoothMessage_, "Aguardando tarefa de rede...", sizeof(bluetoothMessage_));
}
void UI::tick(uint32_t now) {
  if (bluetoothPending_ && worker_.paused()) {
    bluetoothPending_ = false;
    bluetoothRunning_ = bluetooth_.start();
    if (!bluetoothRunning_) worker_.pause(false);
    page_ = 0; dirty_ = true;
  } else if (bluetoothPending_ && uint32_t(now - bluetoothRequested_) >= 30000) {
    bluetoothPending_ = false; worker_.pause(false);
    strlcpy(bluetoothMessage_, "Rede ocupada; tente novamente", sizeof(bluetoothMessage_)); dirty_ = true;
  }
  if (bluetoothRunning_ && !bluetooth_.busy()) {
    bluetoothRunning_ = false; worker_.pause(false); dirty_ = true;
    strlcpy(bluetoothMessage_, bluetooth_.snapshot().status, sizeof(bluetoothMessage_));
  }
  if (scanning_) {
    int count = WiFi.scanComplete();
    if (count >= 0) {
      scanCount_ = 0;
      for (int i = 0; i < count; ++i) {
        ScanEntry entry;
        strlcpy(entry.ssid, WiFi.SSID(i).length() ? WiFi.SSID(i).c_str() : "(oculta)", sizeof(entry.ssid));
        entry.rssi = WiFi.RSSI(i); entry.channel = WiFi.channel(i);
        entry.secured = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
        uint8_t position = 0;
        while (position < scanCount_ && scans_[position].rssi >= entry.rssi) ++position;
        if (position < 24) {
          if (scanCount_ < 24) ++scanCount_;
          for (int j = scanCount_ - 1; j > position; --j) scans_[j] = scans_[j - 1];
          scans_[position] = entry;
        }
      }
      snprintf(scanMessage_, sizeof(scanMessage_), "%d redes / top %u", count, scanCount_);
      WiFi.scanDelete(); scanning_ = false; page_ = 0; dirty_ = true;
    } else if (count == WIFI_SCAN_FAILED) {
      scanning_ = false; strlcpy(scanMessage_, "Busca falhou; 0: repetir", sizeof(scanMessage_)); dirty_ = true;
    }
  }
  if (ready_ && (dirty_ || uint32_t(now - lastFrame_) >= 1000)) {
    render(now); lastFrame_ = now; dirty_ = false;
  }
}
void UI::render(uint32_t now) {
  HubSnapshot state = state_.snapshot();
  canvas_.fillSprite(background);
  canvas_.fillRect(0, 0, 240, 24, panel);
  text(8, 4, menu_ ? "SMART HUB" : names[selected_], accent);
  canvas_.fillCircle(217, 12, 3, WiFi.status() == WL_CONNECTED ? TFT_GREEN : TFT_ORANGE);
  canvas_.fillCircle(229, 12, 3, state.mqttConnected ? TFT_GREEN : muted);
  if (menu_) {
    uint8_t visible[10], count = 0;
    for (int i = 0; i < 10; ++i) if (config_.modules & (1u << i)) visible[count++] = i;
    uint8_t current = 0;
    while (current < count && visible[current] != selected_) ++current;
    uint8_t start = current / 4 * 4;
    for (uint8_t i = start; i < count && i < start + 4; ++i) {
      int y = 28 + (i - start) * 21;
      if (visible[i] == selected_) canvas_.fillRoundRect(5, y, 230, 20, 4, panel);
      text(11, y + 1, String(visible[i] == selected_ ? "> " : "  ") + names[visible[i]], visible[i] == selected_ ? TFT_YELLOW : TFT_WHITE);
    }
    text(8, 122, "35 proximo  0 abrir  segure35 setup", muted, 1);
  } else {
    switch (selected_) {
      case 0: {
        time_t timestamp = time(nullptr); tm local{};
        bool ready = timestamp > 1700000000 && localtime_r(&timestamp, &local);
        char clock[10] = "--:--", date[32] = "Aguardando NTP";
        if (ready) { strftime(clock, sizeof(clock), "%H:%M:%S", &local); strftime(date, sizeof(date), "%d/%m/%Y", &local); }
        canvas_.setTextSize(2); text(8, 28, clock, TFT_WHITE); canvas_.setTextSize(1);
        text(8, 65, date, muted);
        text(8, 86, WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() + "  " + String(WiFi.RSSI()) + "dBm" : "Wi-Fi desconectado");
        text(8, 108, String("Heap ") + ESP.getFreeHeap() / 1024 + "KB  Alertas " + state.alertCount, muted, 1);
        break;
      }
      case 1:
        if (state.weather.valid) {
          canvas_.setTextSize(2); text(8, 29, String(state.weather.temperature, 1) + " C"); canvas_.setTextSize(1);
          text(8, 65, weatherName(state.weather.code), accent);
          text(8, 86, String("Min ") + String(state.weather.minimum, 1) + "  Max " + String(state.weather.maximum, 1));
          text(8, 108, String("Vento ") + String(state.weather.wind, 1) + "km/h  idade " + ((now - state.weather.updated) / 60000) + "m", muted, 1);
          if (strcmp(state.weather.message, "Open-Meteo")) text(8, 122, clipped(state.weather.message, 35), TFT_ORANGE, 1);
        } else {
          text(8, 40, clipped(state.weather.message, 27), muted);
          if (!config_.weatherEnabled) text(8, 68, "Habilite modulo Clima", TFT_YELLOW, 1);
        }
        break;
      case 2: {
        text(8, 28, scanMessage_, muted, 1);
        uint8_t pages = scanCount_ ? (scanCount_ + 2) / 3 : 1;
        uint8_t first = (page_ % pages) * 3;
        for (uint8_t i = first; i < scanCount_ && i < first + 3; ++i) {
          int y = 42 + (i - first) * 25;
          text(8, y, clipped(scans_[i].ssid, 20), TFT_WHITE, 1);
          text(8, y + 10, String(scans_[i].rssi) + "dBm " + constrain(2 * (scans_[i].rssi + 100), 0, 100) + "% CH" + scans_[i].channel + (scans_[i].secured ? " seg" : " aberto"), accent, 1);
          int bars = scans_[i].rssi > -55 ? 4 : scans_[i].rssi > -67 ? 3 : scans_[i].rssi > -80 ? 2 : 1;
          for (int bar = 0; bar < 4; ++bar) canvas_.fillRect(207 + bar * 6, y + 15 - bar * 3, 4, 4 + bar * 3, bar < bars ? accent : panel);
        }
        break;
      }
      case 3:
        text(8, 32, config_.mqttHost[0] ? clipped(config_.mqttHost, 26) : "Configure o broker", muted);
        text(8, 54, state.mqttConnected ? "MQTT conectado" : "MQTT desconectado", state.mqttConnected ? TFT_GREEN : TFT_ORANGE);
        text(8, 76, String("Eventos: ") + state.mqttReceived);
        text(8, 99, clipped(state.mqttLast, 36), accent, 1);
        break;
      case 4: {
        if (!state.alertCount) { text(8, 42, "Nenhum alerta", muted); break; }
        const Alert& alert = state.alerts[page_ % state.alertCount];
        text(8, 31, String("Alerta ") + (page_ % state.alertCount + 1) + "/" + state.alertCount + "  " + (alert.level == 2 ? "ERRO" : alert.level == 1 ? "AVISO" : "INFO"), alert.level == 2 ? TFT_RED : accent);
        String body(alert.text);
        for (int row = 0; row < 3; ++row) text(8, 58 + row * 16, body.substring(row * 35, (row + 1) * 35), TFT_WHITE, 1);
        text(8, 109, String("Ha ") + ((now - alert.created) / 1000) + "s", muted, 1);
        break;
      }
      case 5:
        text(8, 32, config_.haEntity[0] ? clipped(config_.haEntity, 27) : "Configure uma entidade", muted);
        if (page_ == 1) {
          text(8, 56, "Alternar esta entidade?", TFT_YELLOW);
          text(8, 84, "0 confirma / segure0 cancela", muted, 1);
        } else {
          text(8, 57, clipped(state.ha.state, 27), accent);
          text(8, 88, "0: confirmar comando toggle", muted, 1);
          text(8, 104, String("Atualizado ha ") + ((now - state.ha.updated) / 1000) + "s", muted, 1);
        }
        break;
      case 9: {
        BluetoothSnapshot found = bluetooth_.snapshot();
        text(8, 28, bluetoothPending_ ? bluetoothMessage_ : bluetoothRunning_ || found.count ? found.status : bluetoothMessage_, muted, 1);
        uint8_t pages = found.count ? (found.count + 1) / 2 : 1;
        uint8_t first = (page_ % pages) * 2;
        for (uint8_t i = first; i < found.count && i < first + 2; ++i) {
          const BluetoothEntry& device = found.devices[i];
          int y = 43 + (i - first) * 35;
          char address[20]; snprintf(address, sizeof(address), "%02X:%02X:%02X:%02X:%02X:%02X", device.address[0], device.address[1], device.address[2], device.address[3], device.address[4], device.address[5]);
          text(8, y, clipped(device.name[0] ? device.name : "(sem nome)", 34), TFT_WHITE, 1);
          text(8, y + 12, String(device.classic ? "BT " : "BLE ") + (device.hasRSSI ? String(device.rssi) + "dBm" : "RSSI n/d"), accent, 1);
          text(115, y + 12, address, muted, 1);
          int bars = !device.hasRSSI ? 0 : device.rssi > -55 ? 4 : device.rssi > -67 ? 3 : device.rssi > -80 ? 2 : 1;
          for (int bar = 0; bar < 4; ++bar) canvas_.fillRect(8 + bar * 7, y + 29 - bar * 2, 5, 3 + bar * 2, bar < bars ? accent : panel);
        }
        if (!found.count && !bluetoothRunning_ && !bluetoothPending_) text(8, 59, "0: iniciar busca de dispositivos", muted, 1);
        break;
      }
      case 8:
        text(8, 30, portal_.active() ? portal_.apName() : "0: abrir painel Wi-Fi", accent);
        text(8, 51, "192.168.4.1 / admin", TFT_WHITE, 1);
        text(8, 68, String("Senha: ") + config_.adminPassword, TFT_YELLOW, 1);
        text(8, 87, "AP, painel e OTA: mesma senha", muted, 1);
        text(8, 103, portal_.active() ? "0 fecha / ativo por 10 min*" : "0 abre / 10 min apos configurar", muted, 1);
        break;
    }
    if (selected_ != 1 || !state.weather.valid || !strcmp(state.weather.message, "Open-Meteo")) {
      const char* help = (selected_ == 2 || selected_ == 9) ? "35 pagina / 0 busca | segure0 menu" : selected_ == 4 ? "35 proximo | segure0 menu" :
        selected_ == 1 ? "0 atualizar | segure0 menu" : "Segure0 menu | segure35 setup";
      text(8, 123, help, muted, 1);
    }
  }
  canvas_.pushSprite(0, 0);
}

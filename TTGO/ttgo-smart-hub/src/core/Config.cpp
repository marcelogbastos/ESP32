#include "core/Config.h"
bool ConfigStore::begin(Config& config) {
  ready_ = nvs_.begin("smart-hub", false);
  if (!ready_) return false;
  if (nvs_.getBytesLength("config") == sizeof(Config)) {
    Config loaded;
    if (nvs_.getBytes("config", &loaded, sizeof(loaded)) == sizeof(loaded) && (loaded.version == 1 || loaded.version == 2)) {
      config = loaded;
      if (config.version == 1) {
        config.version = 2; config.modules |= 1u << 9;
        if (!save(config)) return false;
      }
    }
  }
  // Terminate every persisted string, including after a corrupted NVS blob.
  config.ssid[32] = config.wifiPassword[64] = config.adminPassword[24] = '\0';
  config.timezone[63] = config.mqttHost[64] = config.mqttUser[64] = '\0';
  config.mqttPassword[64] = config.mqttTopic[95] = config.haURL[127] = '\0';
  config.haToken[255] = config.haEntity[95] = '\0';
  if (strlen(config.adminPassword) < 12) {
    snprintf(config.adminPassword, sizeof(config.adminPassword), "%08lx%08lx",
      (unsigned long)esp_random(), (unsigned long)esp_random());
    if (!save(config)) return false;
  }
  config.modules &= 0x33f; // Drop retired module flags from existing configurations.
  config.modules |= (1u << 0) | (1u << 8); // Dashboard and setup always available.
  config.weatherEnabled = (config.modules & (1u << 1)) != 0;
  if (config.mqttPort == 0) config.mqttPort = 1883;
  return true;
}
bool ConfigStore::save(const Config& config) {
  return ready_ && nvs_.putBytes("config", &config, sizeof(config)) == sizeof(config);
}

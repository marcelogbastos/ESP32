#include "services/State.h"
HubSnapshot SharedState::snapshot() {
  portENTER_CRITICAL(&lock_); HubSnapshot copy = state_; portEXIT_CRITICAL(&lock_); return copy;
}
void SharedState::weather(const WeatherData& value) {
  portENTER_CRITICAL(&lock_); state_.weather = value; portEXIT_CRITICAL(&lock_);
}
void SharedState::ha(const HAData& value) {
  portENTER_CRITICAL(&lock_); state_.ha = value; portEXIT_CRITICAL(&lock_);
}
void SharedState::mqtt(bool connected) {
  portENTER_CRITICAL(&lock_); state_.mqttConnected = connected; portEXIT_CRITICAL(&lock_);
}
void SharedState::message(const char* text) {
  portENTER_CRITICAL(&lock_);
  ++state_.mqttReceived;
  strlcpy(state_.mqttLast, text, sizeof(state_.mqttLast));
  portEXIT_CRITICAL(&lock_);
}
void SharedState::alert(const char* text, uint8_t level) {
  Alert item;
  item.created = millis(); item.level = level > 2 ? 2 : level;
  strlcpy(item.text, text, sizeof(item.text));
  for (char* p = item.text; *p; ++p) if ((uint8_t)*p < 32) *p = ' ';
  portENTER_CRITICAL(&lock_);
  for (int i = 7; i > 0; --i) state_.alerts[i] = state_.alerts[i - 1];
  state_.alerts[0] = item;
  if (state_.alertCount < 8) ++state_.alertCount;
  portEXIT_CRITICAL(&lock_);
}

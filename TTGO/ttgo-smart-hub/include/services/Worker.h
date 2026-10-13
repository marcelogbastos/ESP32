#pragma once
#include "core/Config.h"
#include "services/State.h"
#include <PubSubClient.h>
#include <atomic>
#include <WiFiClientSecure.h>
enum class Request : uint8_t { Weather, HomeAssistant, Toggle };
class Worker {
 public:
  Worker(const Config& config, SharedState& state) : config_(config), state_(state), mqtt_(plain_) {}
  bool begin();
  bool request(Request request);
  void pause(bool value) { pauseRequested_ = value; }
  bool paused() const { return paused_.load(); }
 private:
  const Config& config_;
  SharedState& state_;
  WiFiClient plain_;
  WiFiClientSecure secure_;
  PubSubClient mqtt_;
  std::atomic<bool> pauseRequested_{false}, paused_{false};
  QueueHandle_t queue_ = nullptr;
  static Worker* active_;
  static void task(void* context);
  static void onMessage(char* topic, uint8_t* payload, unsigned int length);
  void run();
  void weather();
  void homeAssistant(bool toggle);
};

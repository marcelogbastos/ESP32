#pragma once
#include "core/Config.h"
#include "services/State.h"
#include <WebServer.h>
class Portal {
 public:
  Portal(const Config& config, ConfigStore& store, SharedState& state)
    : config_(config), store_(store), state_(state) {}
  void begin();
  void open();
  void close();
  void tick(uint32_t now);
  bool active() const { return active_; }
  const char* apName() const { return apName_; }
 private:
  const Config& config_;
  ConfigStore& store_;
  SharedState& state_;
  WebServer server_{80};
  bool active_ = false;
  uint32_t opened_ = 0, restartAt_ = 0, lastAlert_ = 0;
  char apName_[32] = "", csrf_[17] = "";
  bool authenticate();
  void form();
  void save();
};

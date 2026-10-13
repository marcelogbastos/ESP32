#pragma once
#include <Preferences.h>
#include "pet/Pet.h"
class Save {
 public:
  bool begin(Pet& pet);
  bool write(const Pet& pet);
 private:
  Preferences preferences_;
  bool ready_ = false;
};

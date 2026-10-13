#pragma once
#include <Preferences.h>
#include "pet/Pet.h"
class Save {
 public:
  bool begin(Pet& pet);
  // Skips the flash write when nothing changed since the last write, unless forced.
  bool write(const Pet& pet, bool force = false);
 private:
  Preferences preferences_;
  PetState last_;
  bool ready_ = false, hasLast_ = false;
  bool load(const char* key, const char* crcKey, PetState& value);
};

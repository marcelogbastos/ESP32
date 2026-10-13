#pragma once
#include "pet/Pet.h"
#include "storage/Save.h"
// Line-based serial commands: help, status, dump, restore <PET1:...>.
class SerialConsole {
 public:
  // Returns true when the pet state was replaced by a restore command.
  bool poll(Pet& pet, Save& save);
 private:
  char line_[256];
  size_t length_ = 0;
  bool overflow_ = false;
  bool run(const char* command, Pet& pet, Save& save);
};

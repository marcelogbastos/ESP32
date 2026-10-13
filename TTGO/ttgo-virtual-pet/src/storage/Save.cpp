#include "storage/Save.h"
bool Save::begin(Pet& pet) {
  ready_ = preferences_.begin("virtual-pet", false);
  if (!ready_) return false;
  size_t size = preferences_.getBytesLength("state");
  if (size == sizeof(PetState)) {
    PetState value;
    if (preferences_.getBytes("state", &value, sizeof(value)) != sizeof(value) || !pet.restore(value)) return write(pet);
  } else if (size == sizeof(LegacyPetState)) {
    LegacyPetState legacy;
    if (preferences_.getBytes("state", &legacy, sizeof(legacy)) != sizeof(legacy) || !pet.migrate(legacy)) return false;
    return write(pet);
  } else if (size) return false;
  return true;
}
bool Save::write(const Pet& pet) {
  return ready_ && preferences_.putBytes("state", &pet.state(), sizeof(PetState)) == sizeof(PetState);
}

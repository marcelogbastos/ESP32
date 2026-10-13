#include "storage/Save.h"
bool Save::begin(Pet& pet) {
  ready_ = preferences_.begin("virtual-pet", false);
  if (!ready_) return false;
  if (preferences_.getBytesLength("state") == sizeof(PetState)) {
    PetState value;
    if (preferences_.getBytes("state", &value, sizeof(value)) != sizeof(value) || !pet.restore(value)) return write(pet);
  }
  return true;
}
bool Save::write(const Pet& pet) {
  return ready_ && preferences_.putBytes("state", &pet.state(), sizeof(PetState)) == sizeof(PetState);
}

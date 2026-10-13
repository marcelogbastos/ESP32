#include "storage/Save.h"
#include <string.h>
#include "util/Codec.h"
namespace {
const char* kState = "state";
const char* kStateCrc = "crc";
const char* kBackup = "backup";
const char* kBackupCrc = "bcrc";
uint32_t checksum(const PetState& value) { return codec::crc32(reinterpret_cast<const uint8_t*>(&value), sizeof(value)); }
}
// Reads a saved blob and verifies its checksum. Saves from before the checksum existed have no "crc" key and are accepted.
bool Save::load(const char* key, const char* crcKey, PetState& value) {
  if (preferences_.getBytesLength(key) != sizeof(PetState)) return false;
  if (preferences_.getBytes(key, &value, sizeof(value)) != sizeof(value)) return false;
  return !preferences_.isKey(crcKey) || preferences_.getUInt(crcKey, 0) == checksum(value);
}
bool Save::begin(Pet& pet) {
  ready_ = preferences_.begin("virtual-pet", false);
  if (!ready_) return false;
  size_t size = preferences_.getBytesLength(kState);
  if (size == sizeof(PetState)) {
    PetState value;
    if (load(kState, kStateCrc, value) && pet.restore(value)) {
      last_ = pet.state(); hasLast_ = true;
      // Snapshot of the last good boot state, used when the main copy is damaged.
      preferences_.putBytes(kBackup, &last_, sizeof(last_)); preferences_.putUInt(kBackupCrc, checksum(last_));
      return true;
    }
    Serial.println("Save: estado principal invalido; tentando backup");
    if (load(kBackup, kBackupCrc, value) && pet.restore(value)) { Serial.println("Save: backup restaurado"); return write(pet, true); }
    Serial.println("Save: backup invalido; iniciando novo bichinho");
    return write(pet, true);
  } else if (size == sizeof(LegacyPetState)) {
    LegacyPetState legacy;
    if (preferences_.getBytes(kState, &legacy, sizeof(legacy)) != sizeof(legacy) || !pet.migrate(legacy)) return false;
    return write(pet, true);
  } else if (size) return false;
  return true;
}
bool Save::write(const Pet& pet, bool force) {
  if (!ready_) return false;
  const PetState& state = pet.state();
  if (!force && hasLast_ && memcmp(&state, &last_, sizeof(state)) == 0) return true;
  bool ok = preferences_.putBytes(kState, &state, sizeof(state)) == sizeof(state) && preferences_.putUInt(kStateCrc, checksum(state)) > 0;
  if (ok) { last_ = state; hasLast_ = true; }
  return ok;
}

#include "console/SerialConsole.h"
#include <Arduino.h>
#include <string.h>
#include "util/Codec.h"
bool SerialConsole::poll(Pet& pet, Save& save) {
  bool restored = false;
  while (Serial.available()) {
    char c = char(Serial.read());
    if (c == '\r' || c == '\n') {
      if (overflow_) Serial.println("Console: linha longa demais");
      else if (length_) { line_[length_] = 0; restored |= run(line_, pet, save); }
      length_ = 0; overflow_ = false;
    } else if (length_ < sizeof(line_) - 1) line_[length_++] = c;
    else overflow_ = true;
  }
  return restored;
}
bool SerialConsole::run(const char* command, Pet& pet, Save& save) {
  if (strcmp(command, "help") == 0) {
    Serial.println("Comandos: help | status | dump | restore PET1:<hex>:<crc>");
  } else if (strcmp(command, "status") == 0) {
    const PetState& s = pet.state();
    Serial.printf("idade=%luh%02lum moedas=%u comida=%u energia=%u alegria=%u higiene=%u saude=%u vinculo=%u estagio=%u\n",
                  (unsigned long)(s.ageMinutes / 60), (unsigned long)(s.ageMinutes % 60), s.coins, s.food, s.energy, s.joy, s.hygiene, s.health, s.bond, s.lifeStage);
  } else if (strcmp(command, "dump") == 0) {
    char text[codec::backupSize(sizeof(PetState))];
    codec::encodeBackup(reinterpret_cast<const uint8_t*>(&pet.state()), sizeof(PetState), text, sizeof(text));
    Serial.println(text);
  } else if (strncmp(command, "restore ", 8) == 0) {
    PetState value;
    if (!codec::decodeBackup(command + 8, reinterpret_cast<uint8_t*>(&value), sizeof(value))) Serial.println("Restore: formato ou CRC invalido");
    else if (!pet.restore(value)) Serial.println("Restore: estado rejeitado pelas regras do bichinho");
    else if (!save.write(pet, true)) Serial.println("Restore: falha ao gravar NVS");
    else { Serial.println("Restore: ok"); return true; }
  } else Serial.println("Comando desconhecido; digite help");
  return false;
}

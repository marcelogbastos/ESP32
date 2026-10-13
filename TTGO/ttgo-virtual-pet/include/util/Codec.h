#pragma once
#include <stddef.h>
#include <stdint.h>
namespace codec {
inline uint32_t crc32(const uint8_t* data, size_t size) {
  uint32_t crc = 0xffffffffu;
  for (size_t i = 0; i < size; ++i) {
    crc ^= data[i];
    for (int bit = 0; bit < 8; ++bit) crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
  }
  return ~crc;
}
inline char hexDigit(uint8_t value) { return value < 10 ? char('0' + value) : char('a' + value - 10); }
inline int hexValue(char c) {
  return c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
}
inline size_t length(const char* text) { size_t n = 0; while (text[n]) ++n; return n; }
// "PET1:<hex bytes>:<crc32 hex>" lets a pet be copied through the serial monitor.
constexpr size_t backupSize(size_t bytes) { return 5 + bytes * 2 + 1 + 8 + 1; }
inline bool encodeBackup(const uint8_t* data, size_t size, char* out, size_t capacity) {
  if (capacity < backupSize(size)) return false;
  size_t at = 0;
  for (const char* p = "PET1:"; *p; ++p) out[at++] = *p;
  for (size_t i = 0; i < size; ++i) { out[at++] = hexDigit(data[i] >> 4); out[at++] = hexDigit(data[i] & 15); }
  out[at++] = ':';
  uint32_t crc = crc32(data, size);
  for (int shift = 28; shift >= 0; shift -= 4) out[at++] = hexDigit((crc >> shift) & 15);
  out[at] = 0; return true;
}
inline bool decodeBackup(const char* text, uint8_t* out, size_t size) {
  if (length(text) != backupSize(size) - 1) return false;
  const char* prefix = "PET1:";
  for (size_t i = 0; i < 5; ++i) if (text[i] != prefix[i]) return false;
  size_t at = 5;
  for (size_t i = 0; i < size; ++i) {
    int high = hexValue(text[at++]), low = hexValue(text[at++]);
    if (high < 0 || low < 0) return false;
    out[i] = uint8_t(high << 4 | low);
  }
  if (text[at++] != ':') return false;
  uint32_t crc = 0;
  for (int i = 0; i < 8; ++i) { int digit = hexValue(text[at++]); if (digit < 0) return false; crc = crc << 4 | uint32_t(digit); }
  return crc == crc32(out, size);
}
}

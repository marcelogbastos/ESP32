#pragma once
#include <stdint.h>
#include <stddef.h>
inline void copyRadioName(char* output, const uint8_t* input, size_t length) {
  size_t size = length < 32 ? length : 32;
  size_t written = 0;
  while (written < size && input[written]) {
    output[written] = input[written] < 32 ? ' ' : input[written];
    ++written;
  }
  output[written] = 0;
}
inline void advertisingName(char* output, const uint8_t* bytes, size_t size) {
  size_t offset = 0;
  while (offset < size) {
    uint8_t length = bytes[offset];
    if (!length || offset + 1 + length > size) break;
    uint8_t type = bytes[offset + 1];
    if (length > 1 && (type == 0x09 || (type == 0x08 && !output[0])))
      copyRadioName(output, bytes + offset + 2, length - 1);
    offset += length + 1;
  }
}

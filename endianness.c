#include <stdint.h>

uint32_t reverse_endianness(uint32_t num) {
  uint32_t first_reversal = num << 8 & 0xFF00FF00 | num >> 8 & 0x00FF00FF;
  return first_reversal << 16 | first_reversal >> 16;
}
#include "utils.h"

uint64_t random_uint64() {
  uint64_t u1, u2, u3, u4;
  u1 = (uint64_t)(rand()) & 0xFFFF; u2 = (uint64_t)(rand()) & 0xFFFF;
  u3 = (uint64_t)(rand()) & 0xFFFF; u4 = (uint64_t)(rand()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}
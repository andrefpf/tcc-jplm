#ifndef BINARYTOOLS_H__
#define BINARYTOOLS_H__

#include <cstdint>

namespace BinaryTools {
constexpr bool using_little_endian() {
  if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    return true;
  return false;
}


uint16_t swap_endianess(uint16_t in) {
  return ((in >> 8) & 0x00FF) | ((in << 8) & 0xFF00);
}


uint32_t swap_endianess(uint32_t in) {
  return ((in >> 24) & 0x000000FF) | ((in >> 8) & 0x0000FF00) |
         ((in << 8) & 0x00FF0000) | ((in << 24) & 0xFF000000);
}


uint64_t swap_endianess(uint64_t in) {
  return ((in >> 56) & 0x00000000000000FF) | ((in >> 40) & 0x000000000000FF00) |
         ((in >> 24) & 0x0000000000FF0000) | ((in >> 8)  & 0x00000000FF000000) |
         ((in << 8)  & 0x000000FF00000000) | ((in << 24) & 0x0000FF0000000000) |
         ((in << 40) & 0x00FF000000000000) | ((in << 56) & 0xFF00000000000000);
}

}  // namespace BinaryTools

#endif /* end of include guard: BINARYTOOLS_H__ */
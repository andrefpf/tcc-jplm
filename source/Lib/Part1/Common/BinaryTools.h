#ifndef BINARYTOOLS_H__
#define BINARYTOOLS_H__

#include <cstdint>

namespace BinaryTools {

constexpr bool using_little_endian() {
  if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    return true;
  return false;
}

uint16_t swap_endianess(const uint16_t in);


uint32_t swap_endianess(const uint32_t in);


uint64_t swap_endianess(const uint64_t in);


template<typename T>
T ensure_machines_endianess(const T in) {
  if constexpr (using_little_endian()) {
    return swap_endianess(in);
  }
  return in;
}


}  // namespace BinaryTools

#endif /* end of include guard: BINARYTOOLS_H__ */
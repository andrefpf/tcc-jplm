#ifndef BINARYTOOLS_H__
#define BINARYTOOLS_H__

#include <cstdint>
#include <vector>
#include <algorithm>    // std::reverse

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


template<typename T>
T ensure_big_endian(const T in) {
	return ensure_machines_endianess(in); //works exactly the same...
}

template<typename T>
std::vector<uint8_t> split_in_big_endian_bytes(const T& in) {
	auto n_bytes = sizeof(T); 
    auto bytes = std::vector<uint8_t>();
    if (n_bytes == 1) {
      bytes.push_back(in);
      return bytes;
    }
    bytes.reserve(n_bytes);
    auto ptr_to_byte = reinterpret_cast<const uint8_t*>(&in);

    for(auto i=decltype(n_bytes){0}; i<n_bytes;++i) {
      bytes.push_back(*ptr_to_byte);
      ++ptr_to_byte;
    }

    if constexpr (BinaryTools::using_little_endian()) {
      std::reverse(bytes.begin(), bytes.end());  
    }
    return bytes;
}

}  // namespace BinaryTools

#endif /* end of include guard: BINARYTOOLS_H__ */
#ifndef BINARYTOOLS_H__
#define BINARYTOOLS_H__

#include <algorithm>  // std::reverse
#include <cstdint>
#include <vector>

namespace BinaryTools {

constexpr bool using_little_endian() {
  if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    return true;
  return false;
}


uint8_t swap_endianess(const uint8_t in);


uint16_t swap_endianess(const uint16_t in);


uint32_t swap_endianess(const uint32_t in);


uint64_t swap_endianess(const uint64_t in);


template<typename T, bool big_endian = true>
T ensure_machines_endianess(const T in) {
  if constexpr ((using_little_endian() && big_endian) ||
                (!using_little_endian() && !big_endian)) {
    return swap_endianess(in);
  }
  return in;
}


template<typename T, bool big_endian = false>
T ensure_big_endian(const T in) {
  return ensure_machines_endianess<T, !big_endian>(in);
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

  for (auto i = decltype(n_bytes){0}; i < n_bytes; ++i) {
    bytes.push_back(*ptr_to_byte);
    ++ptr_to_byte;
  }

  if constexpr (BinaryTools::using_little_endian()) {
    std::reverse(bytes.begin(), bytes.end());
  }
  return bytes;
}


template<typename T>
T get_value_from_big_endian_byte_vector(const std::vector<uint8_t>& bytes, const std::size_t pos=0) {
  auto n_bytes = sizeof(T);
  if (n_bytes == 1) {
    return static_cast<T>(bytes.at(pos));
  }

  T out=0;

  auto ptr_to_byte = reinterpret_cast<uint8_t*>(&out);
  if constexpr (BinaryTools::using_little_endian()) {
    ptr_to_byte+=n_bytes-1;
  }

  for (auto i = pos; i < pos+n_bytes; ++i) {
    *ptr_to_byte=bytes.at(i);
    if constexpr (BinaryTools::using_little_endian()) {
      --ptr_to_byte;
    } else {
      ++ptr_to_byte;
    }
  }
  return out;
}


template<typename T>
std::vector<uint8_t>& append_big_endian_bytes(
    std::vector<uint8_t>& byte_list, const T& value) {
  auto value_bytes = split_in_big_endian_bytes(value);
  byte_list.insert(byte_list.end(), value_bytes.begin(), value_bytes.end());
  return byte_list;
}


}  // namespace BinaryTools

#endif /* end of include guard: BINARYTOOLS_H__ */
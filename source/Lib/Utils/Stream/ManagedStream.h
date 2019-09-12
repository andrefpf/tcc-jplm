#ifndef JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__
#define JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include "CommonExceptions.h"

class ManagedStream {
 protected:
  std::ifstream&
      ref_to_stream;  //it may be a good idea to change for a weak ptr
  const uint64_t initial_pos;
  const uint64_t final_pos;

 public:
  ManagedStream(
      std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos);
  ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset);

  ManagedStream get_sub_managed_stream(
      uint64_t initial_pos, uint64_t final_pos);
  ManagedStream get_sub_managed_stream(uint64_t max_offset);

  ManagedStream get_remaining_sub_managed_stream();

  auto get_initial_pos() const noexcept {
    return initial_pos;
  }

  auto get_final_pos() const noexcept {
    return final_pos;
  }


  bool is_valid() const noexcept;
  bool is_valid(uint64_t index) const noexcept;

  uint64_t get_current_pos() const noexcept;


  ManagedStream& rewind();  //! places the stream at initial position


  ManagedStream& forward();  //! places the stream at final position


  ManagedStream& seek(int64_t offset,
      const std::ios_base::seekdir relative_to =
          std::ios_base::beg);  //! place the stream at

  uint64_t tell() const noexcept;


  void dynamic_assert_access_bounds(const uint64_t n) const {
    auto current_position = static_cast<uint64_t>(ref_to_stream.tellg());
    if ((current_position < initial_pos) ||
        (current_position + n > final_pos)) {
      throw ManagedStreamExceptions::OutOfBoundsException(
          n, initial_pos, final_pos, current_position);
    }
  }


  template<size_t N>
  std::vector<std::byte> get_bytes() {
    dynamic_assert_access_bounds(N);
    std::array<std::byte, N> temp_array;
    ref_to_stream.read(reinterpret_cast<char*>(temp_array.data()), N);
    return {std::begin(temp_array), std::end(temp_array)};
  }


  std::vector<std::byte> get_n_bytes(uint64_t n) {
    dynamic_assert_access_bounds(n);
    std::vector<std::byte> temp_vector(n);
    ref_to_stream.read(reinterpret_cast<char*>(temp_vector.data()), n);
    return temp_vector;
  }


  std::byte get_byte() {
    return get_bytes<1>()[0];
  }


  uint64_t get_length() const noexcept;


  ~ManagedStream() = default;
};

#endif /* end of include guard: JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__ */
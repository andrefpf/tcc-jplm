#ifndef MANAGEDSTREAM_H__
#define MANAGEDSTREAM_H__

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>
#include "CommonExceptions.h"

class ManagedStream {
 protected:
  std::ifstream& ref_to_stream;
  const uint64_t initial_pos;
  const uint64_t final_pos;

 public:
  ManagedStream(
      std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos);
  ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset);

  ManagedStream get_sub_managed_stream(
      uint64_t initial_pos, uint64_t final_pos);
  ManagedStream get_sub_managed_stream(uint64_t max_offset);

  uint64_t get_current_pos() const noexcept;


  void rewind();  //! places the stream at initial position


  void forward();  //! places the stream at final position


  template<size_t N>
  std::vector<std::byte> get_bytes() {
    if (static_cast<size_t>(ref_to_stream.tellg()) + N > final_pos) {
      throw ManagedStreamExceptions::OutOfBoundsException(
          N, initial_pos, final_pos, ref_to_stream.tellg());
    }

    std::array<std::byte, N> temp_array;
    ref_to_stream.read(reinterpret_cast<char*>(temp_array.data()), N);
    return {std::begin(temp_array), std::end(temp_array)};
  }


  std::byte get_byte() {
    return get_bytes<1>()[0];
  }

  ~ManagedStream() = default;
};

#endif /* end of include guard: MANAGEDSTREAM_H__ */
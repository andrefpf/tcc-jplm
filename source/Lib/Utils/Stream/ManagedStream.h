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


  bool index_is_valid(uint64_t index) const noexcept;


  uint64_t get_current_pos() const noexcept;


  ManagedStream& rewind();  //! places the stream at initial position


  ManagedStream& forward();  //! places the stream at final position


  ManagedStream& seek(int64_t offset,
      const std::ios_base::seekdir relative_to=std::ios_base::beg);  //! place the stream at


  template<size_t N>
  std::vector<std::byte> get_bytes() {
    auto current_position = static_cast<uint64_t>(ref_to_stream.tellg());
    if ((current_position < initial_pos) ||
        (current_position + N > final_pos)) {
      throw ManagedStreamExceptions::OutOfBoundsException(
          N, initial_pos, final_pos, current_position);
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
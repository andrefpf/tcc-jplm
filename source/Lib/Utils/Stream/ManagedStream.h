#ifndef MANAGEDSTREAM_H__
#define MANAGEDSTREAM_H__

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>
#include "CommonExceptions.h"

class ManagedStream {
 protected:
  std::ifstream& ref_to_stream;
  uint64_t initial_pos;
  uint64_t final_pos;

 public:
  ManagedStream(
      std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos);
  ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset);

  ManagedStream get_sub_managed_stream(
      uint64_t initial_pos, uint64_t final_pos);
  ManagedStream get_sub_managed_stream(uint64_t max_offset);

  uint64_t get_current_pos() const noexcept;
  std::byte get_byte();

  template<int N>
  std::vector<std::byte> get_bytes();

  ~ManagedStream() = default;
};

#endif /* end of include guard: MANAGEDSTREAM_H__ */
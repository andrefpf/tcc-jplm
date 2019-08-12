#ifndef MANAGEDSTREAM_H__
#define MANAGEDSTREAM_H__

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <vector>
#include "CommonExceptions.h"

class ManagedStream {
 protected:
  std::fstream& ref_to_stream;

 public:
  ManagedStream(
      std::fstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos);
  ManagedStream(std::fstream& ref_to_stream, uint64_t max_offset);

  ManagedStream get_sub_managed_stream(
      uint64_t initial_pos, uint64_t final_pos);
  ManagedStream get_sub_managed_stream(uint64_t max_offset);

  void get_current_pos() const noexcept;
  std::byte get_byte();

  template<int N>
  std::vector<std::byte> get_bytes();

  ~ManagedStream();
};

#endif /* end of include guard: MANAGEDSTREAM_H__ */
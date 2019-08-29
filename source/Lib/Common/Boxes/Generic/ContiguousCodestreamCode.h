#ifndef CONTIGUOUSCODESTREAMCODE_H__
#define CONTIGUOUSCODESTREAMCODE_H__

#include <cstddef>  //for std::byte
#include <cstdint>
#include <ostream>

class ContiguousCodestreamCode {
 public:
  virtual uint64_t size() const noexcept = 0;
  virtual ContiguousCodestreamCode* clone() const = 0;

  // virtual bool operator==(const ContiguousCodestreamCode& other) const = 0;
  // virtual bool operator!=(const ContiguousCodestreamCode& other) const = 0;
  virtual bool is_equal(const ContiguousCodestreamCode& other) const = 0;
  virtual std::ostream& write_to(std::ostream& stream) const = 0;

  virtual void push_byte(const std::byte byte) = 0;
  virtual std::byte get_byte_at(const uint64_t pos) const = 0;


  bool operator==(const ContiguousCodestreamCode& other) const {
    return this->is_equal(other);
  }


  bool operator!=(const ContiguousCodestreamCode& other) const {
    return !this->operator==(other);
  }

  //! \todo define a iterator interface
};


std::ostream& operator<<(
    std::ostream& stream, const ContiguousCodestreamCode& code);

#endif /* end of include guard: CONTIGUOUSCODESTREAMCODE_H__ */
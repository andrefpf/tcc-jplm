#ifndef CONTIGUOUSCODESTREAMCODEINMEMORY_H__
#define CONTIGUOUSCODESTREAMCODEINMEMORY_H__

#include <iostream>
#include <vector>
#include "ContiguousCodestreamCode.h"

class ContiguousCodestreamCodeInMemory : public ContiguousCodestreamCode {
 protected:
  std::vector<std::byte> bytes;

 public:
  virtual uint64_t size() const noexcept override {
    return bytes.size();
  }

  ContiguousCodestreamCodeInMemory() = default;


  ContiguousCodestreamCodeInMemory(uint64_t size) {
    bytes.reserve(size);
  }


  ContiguousCodestreamCodeInMemory(const std::vector<std::byte>& bytes)
      : bytes(bytes) {
  }


  ContiguousCodestreamCodeInMemory(std::vector<std::byte>&& bytes)
      : bytes(std::move(bytes)) {
  }


  ContiguousCodestreamCodeInMemory(
      const ContiguousCodestreamCodeInMemory& other)
      : bytes(other.bytes) {
  }


  ContiguousCodestreamCodeInMemory(ContiguousCodestreamCodeInMemory&& other)
      : bytes(std::move(other.bytes)) {
  }


  virtual ~ContiguousCodestreamCodeInMemory() = default;


  virtual void push_byte(const std::byte byte) override {
    bytes.emplace_back(byte);
  }


  virtual std::byte get_byte_at(const uint64_t pos) const override {
    return bytes.at(pos);
  }


  virtual ContiguousCodestreamCodeInMemory* clone() const override {
    return new ContiguousCodestreamCodeInMemory(*this);
  }


  virtual bool is_equal(const ContiguousCodestreamCode& other) const override {
    if (this->size() != other.size()) {
      return false;
    }
    uint64_t counter = 0;
    for (const auto& byte : bytes) {
      if (byte != other.get_byte_at(counter)) {
        return false;
      }
    }
    return true;
  }


  virtual std::ostream& write_to(std::ostream& stream) const override {
    stream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    return stream;
  }
};

#endif /* end of include guard: CONTIGUOUSCODESTREAMCODEINMEMORY_H__ */
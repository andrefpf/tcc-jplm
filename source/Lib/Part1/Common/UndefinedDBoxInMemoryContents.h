#ifndef UNDEFINEDDBOXINMEMORYCONTENTS_H__
#define UNDEFINEDDBOXINMEMORYCONTENTS_H__

#include "InMemoryDBoxContents.h"

class UndefinedDBoxInMemoryContents : public InMemoryDBoxContents {
 public:
 protected:
  std::vector<std::byte> byte_array;

 public:
  UndefinedDBoxInMemoryContents(const std::vector<std::byte>& byte_array)
      : byte_array(byte_array) {
  }


  UndefinedDBoxInMemoryContents(std::vector<std::byte>&& byte_array)
      : byte_array(std::move(byte_array)) {
  }


  UndefinedDBoxInMemoryContents(const UndefinedDBoxInMemoryContents& other)
      : byte_array(other.byte_array) {
  }


  UndefinedDBoxInMemoryContents(UndefinedDBoxInMemoryContents&& other)
      : byte_array(std::move(other.byte_array)) {
  }


  UndefinedDBoxInMemoryContents() = default;


  virtual UndefinedDBoxInMemoryContents* clone() const override {
    return new UndefinedDBoxInMemoryContents(*this);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const UndefinedDBoxInMemoryContents&>(other);
    return *this == cast_other;
  }


  ~UndefinedDBoxInMemoryContents() = default;


  uint64_t size() const noexcept override {
    return byte_array.size();
  }


  bool operator==(const UndefinedDBoxInMemoryContents& other) const {
    return this->byte_array == other.byte_array;
  }


  bool operator!=(const UndefinedDBoxInMemoryContents& other) const {
    return !this->operator==(other);
  }


  void set_bytes(const std::vector<std::byte>&& bytes) {
    byte_array = std::move(bytes);
  }


  void set_bytes(const std::vector<std::byte>& bytes) {
    byte_array = bytes;
  }


  void add_bytes(const std::vector<std::byte>& bytes) {
    byte_array.insert(byte_array.end(), bytes.begin(), bytes.end());
  }


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    return byte_array;
  }
};

#endif /* end of include guard: UNDEFINEDDBOXINMEMORYCONTENTS_H__ */
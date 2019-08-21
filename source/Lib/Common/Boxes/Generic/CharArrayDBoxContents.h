#ifndef CHARARRAYDBOXCONTENTS_H__
#define CHARARRAYDBOXCONTENTS_H__

#include "Lib/Common/Boxes/InMemoryDBoxContents.h"

class CharArrayDBoxContents : public InMemoryDBoxContents {
 protected:
  std::vector<uint8_t> chars;

 public:
  CharArrayDBoxContents(const std::vector<uint8_t>& array) : chars(array) {
  }


  CharArrayDBoxContents(std::vector<uint8_t>&& array)
      : chars(std::move(array)) {
  }


  CharArrayDBoxContents(const CharArrayDBoxContents& other)
      : chars(other.get_const_ref_to_vector()) {
  }


  CharArrayDBoxContents(CharArrayDBoxContents&& other)
      : chars(std::move(other.chars)){};


  ~CharArrayDBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return chars.size();
  }


  virtual CharArrayDBoxContents* clone() const override {
    return new CharArrayDBoxContents(*this);
  }


  const std::vector<uint8_t>& get_const_ref_to_vector() const {
    return chars;
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const CharArrayDBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const CharArrayDBoxContents& other) const {
    return this->chars == other.chars;
  }


  bool operator!=(const CharArrayDBoxContents& other) const {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    for (const auto& value : chars) {
      bytes.emplace_back(std::byte{value});
    }
    return bytes;
  }
};

#endif /* end of include guard: CHARARRAYDBOXCONTENTS_H__ */
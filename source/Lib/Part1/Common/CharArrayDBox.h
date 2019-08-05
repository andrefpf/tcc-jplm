#ifndef CHARARRAYDBOX_H__
#define CHARARRAYDBOX_H__

#include "DBox.h"
#include "InMemoryDBoxContents.h"

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
    return this->chars == cast_other.chars;
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


class CharArrayDBox : public DBox {
 public:
  CharArrayDBox(const std::vector<uint8_t>& array)
      : DBox(std::make_unique<CharArrayDBoxContents>(array)) {
  }


  CharArrayDBox(const CharArrayDBox& other)
      : DBox(std::make_unique<CharArrayDBoxContents>(
            other.get_ref_to_contents())) {
  }


  CharArrayDBox(CharArrayDBox&& other) : DBox(std::move(other)) {
  }


  ~CharArrayDBox() = default;


  CharArrayDBox* clone() const override {
    return new CharArrayDBox(*this);
  }
};

#endif /* end of include guard: CHARARRAYDBOX_H__ */
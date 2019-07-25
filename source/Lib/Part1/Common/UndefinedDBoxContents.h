#ifndef UNDEFINEDDBOXCONTENTS_H__
#define UNDEFINEDDBOXCONTENTS_H__

#include "Box.h"

class UndefinedDBoxContents {
 protected:
  std::vector<uint8_t> byte_array;

 public:
  UndefinedDBoxContents();
  ~UndefinedDBoxContents();

  uint64_t get_size() const noexcept {
    return byte_array.size() * sizeof(uint8_t);
  }

  bool operator==(const UndefinedDBoxContents& other) const {
    return this->byte_array == other.byte_array;
  }

  bool operator!=(const UndefinedDBoxContents& other) const {
    return !this->operator==(other);
  }
};


class UndefinedDBox : public DBox {
 public:
  UndefinedDBox(const UndefinedDBoxContents& contents)
      : DBox(std::make_any<UndefinedDBoxContents>(contents)) {
  }


  UndefinedDBox(const UndefinedDBox& other)
      : DBox(std::make_any<UndefinedDBoxContents>(
            std::any_cast<UndefinedDBoxContents>(other.contents))) {
  }


  ~UndefinedDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<UndefinedDBoxContents>(this->contents).get_size();
  }


  UndefinedDBox* clone() const override {
    return new UndefinedDBox(*this);
  }

  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<UndefinedDBoxContents>(this->get_ref_to_contents()) ==
            std::any_cast<UndefinedDBoxContents>(other.get_ref_to_contents()));
  }
};


#endif /* end of include guard: UNDEFINEDDBOXCONTENTS_H__ */
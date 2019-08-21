#ifndef CHARARRAYDBOX_H__
#define CHARARRAYDBOX_H__

#include "CharArrayDBoxContents.h"
#include "source/Lib/Common/Boxes/DBox.h"

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


  virtual const CharArrayDBoxContents& get_ref_to_contents() const override {
    return static_cast<const CharArrayDBoxContents&>(*contents);
  }
  

  ~CharArrayDBox() = default;


  CharArrayDBox* clone() const override {
    return new CharArrayDBox(*this);
  }
};

#endif /* end of include guard: CHARARRAYDBOX_H__ */
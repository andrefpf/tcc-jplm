#ifndef UNDEFINEDDBOX_H__
#define UNDEFINEDDBOX_H__

#include "source/Lib/Common/Boxes/DBox.h"
#include "UndefinedDBoxContents.h"

class UndefinedDBox : public DBox {
 public:
  UndefinedDBox(const UndefinedDBoxContents& contents)
      : DBox(std::make_unique<UndefinedDBoxContents>(contents)) {
  }


  UndefinedDBox(const UndefinedDBox& other)
      : DBox(std::make_unique<UndefinedDBoxContents>(
            other.get_ref_to_contents())) {
  }


  virtual const UndefinedDBoxContents& get_ref_to_contents() const override {
    return static_cast<const UndefinedDBoxContents&>(*contents);
  }


  virtual ~UndefinedDBox() = default;


  UndefinedDBox* clone() const override {
    return new UndefinedDBox(*this);
  }
};

#endif /* end of include guard: UNDEFINEDDBOX_H__ */
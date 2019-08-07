#ifndef UUIDINFODBOX_H__
#define UUIDINFODBOX_H__

#include "UUIDInfoBoxContents.h"
#include "DBox.h"

class UUIDInfoDBox : public DBox {
 public:
  UUIDInfoDBox(const UUIDInfoBoxContents& contents)
      : DBox(std::make_unique<UUIDInfoBoxContents>(contents)) {
  }


  UUIDInfoDBox(const UUIDInfoDBox& other)
      : DBox(std::make_unique<UUIDInfoBoxContents>(other.get_ref_to_contents())) {
  }


  virtual const UUIDInfoBoxContents& get_ref_to_contents() const override {
    return static_cast<const UUIDInfoBoxContents&>(*contents);
  }


  ~UUIDInfoDBox() = default;


  UUIDInfoDBox* clone() const override {
    return new UUIDInfoDBox(*this);
  }
};

#endif /* end of include guard: UUIDINFODBOX_H__ */
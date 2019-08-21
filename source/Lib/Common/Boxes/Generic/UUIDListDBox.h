#ifndef UUIDLISTDBOX_H__
#define UUIDLISTDBOX_H__

#include "UUIDListBoxContents.h"
#include "source/Lib/Common/Boxes/DBox.h"

class UUIDListDBox : public DBox {
 public:
  UUIDListDBox(const UUIDListBoxContents& contents)
      : DBox(std::make_unique<UUIDListBoxContents>(contents)) {
  }


  UUIDListDBox(const UUIDListDBox& other)
      : DBox(std::make_unique<UUIDListBoxContents>(
            other.get_ref_to_contents())) {
  }


  virtual const UUIDListBoxContents& get_ref_to_contents() const override {
    return static_cast<const UUIDListBoxContents&>(*contents);
  }


  ~UUIDListDBox() = default;


  UUIDListDBox* clone() const override {
    return new UUIDListDBox(*this);
  }
};

#endif /* end of include guard: UUIDLISTDBOX_H__ */
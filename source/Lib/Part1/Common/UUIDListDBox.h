#ifndef UUIDLISTDBOX_H__
#define UUIDLISTDBOX_H__

#include "UUIDListBoxContents.h"
#include "DBox.h"

class UUIDListDBox : public DBox {
 public:
  UUIDListDBox(const UUIDListBoxContents& contents)
      : DBox(std::make_unique<UUIDListBoxContents>(contents)) {
  }


  UUIDListDBox(const UUIDListDBox& other)
      : DBox(std::make_unique<UUIDListBoxContents>(
            other.get_ref_to_contents())) {
  }


  ~UUIDListDBox() = default;


  UUIDListDBox* clone() const override {
    return new UUIDListDBox(*this);
  }
};

#endif /* end of include guard: UUIDLISTDBOX_H__ */
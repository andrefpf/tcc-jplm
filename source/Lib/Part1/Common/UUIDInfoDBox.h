#ifndef UUIDINFODBOX_H__
#define UUIDINFODBOX_H__

#include "UUIDInfoBoxContents.h"
#include "DBox.h"

class UUIDInfoDBox : public DBox {
 public:
  UUIDInfoDBox(const UUIDInfoBoxContents& contents)
      : DBox(std::make_any<UUIDInfoBoxContents>(contents)) {
  }


  UUIDInfoDBox(const UUIDInfoDBox& other)
      : DBox(std::make_any<UUIDInfoBoxContents>(
            std::any_cast<UUIDInfoBoxContents>(other.contents))) {
  }


  ~UUIDInfoDBox() = default;


  UUIDInfoDBox* clone() const override {
    return new UUIDInfoDBox(*this);
  }
};

#endif /* end of include guard: UUIDINFODBOX_H__ */
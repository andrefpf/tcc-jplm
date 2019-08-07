#ifndef UUIDDBOX_H__
#define UUIDDBOX_H__

#include "UUIDBoxContents.h"
#include "DBox.h"

class UUIDDBox : public DBox {
 public:
  UUIDDBox(const UUIDBoxContents& contents)
      : DBox(std::make_unique<UUIDBoxContents>(contents)) {
  }


  UUIDDBox(const UUIDDBox& other)
      : DBox(std::make_unique<UUIDBoxContents>(other.get_ref_to_contents())) {
  }


  ~UUIDDBox() = default;


  UUIDDBox* clone() const override {
    return new UUIDDBox(*this);
  }


  void add_data(const std::vector<uint8_t>& ) { //data
    ///! \todo implement
    // this.contents->
    // std::any_cast<UUIDBoxContents&>(this->contents).add_data(data);
  }

};

#endif /* end of include guard: UUIDDBOX_H__ */
#ifndef CHANNELDEFINITIONDBOX_H__
#define CHANNELDEFINITIONDBOX_H__

#include "DBox.h"
#include "ChannelDefinitionContents.h"

class ChannelDefinitionDBox : public DBox {
 public:
  ChannelDefinitionDBox(const ChannelDefinitionContents& contents)
      : DBox(std::make_unique<ChannelDefinitionContents>(contents)) {
  }


  //check to make sure this works (creating a unique ptr from a reference of base class)
  ChannelDefinitionDBox(const ChannelDefinitionDBox& other)
      : DBox(std::make_unique<ChannelDefinitionContents>(
            other.get_ref_to_contents())) {
  }


  ~ChannelDefinitionDBox() = default;


  ChannelDefinitionDBox* clone() const override {
    return new ChannelDefinitionDBox(*this);
  }
};

#endif /* end of include guard: CHANNELDEFINITIONDBOX_H__ */
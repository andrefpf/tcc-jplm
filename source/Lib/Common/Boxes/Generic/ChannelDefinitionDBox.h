#ifndef CHANNELDEFINITIONDBOX_H__
#define CHANNELDEFINITIONDBOX_H__

#include "source/Lib/Common/Boxes/DBox.h"
#include "ChannelDefinitionContents.h"

class ChannelDefinitionDBox : public DBox {
 public:
  ChannelDefinitionDBox(const ChannelDefinitionContents& contents)
      : DBox(std::make_unique<ChannelDefinitionContents>(contents)) {
  }


  ChannelDefinitionDBox(const ChannelDefinitionDBox& other)
      : DBox(std::make_unique<ChannelDefinitionContents>(
            other.get_ref_to_contents())) {
  }


  virtual const ChannelDefinitionContents& get_ref_to_contents() const override {
    return static_cast<const ChannelDefinitionContents&>(*contents);
  }


  ~ChannelDefinitionDBox() = default;


  ChannelDefinitionDBox* clone() const override {
    return new ChannelDefinitionDBox(*this);
  }
};

#endif /* end of include guard: CHANNELDEFINITIONDBOX_H__ */
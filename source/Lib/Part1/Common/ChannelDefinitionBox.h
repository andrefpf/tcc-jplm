#ifndef JPLM_LIB_PART1_COMMON_CHANNELDEFINITIONBOX_H__
#define JPLM_LIB_PART1_COMMON_CHANNELDEFINITIONBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"


enum class ChannelType : uint16_t {
  ColorImageData = 0,
  Opacity = 1,
  PremultipliedOpacity = 2,
  //3 until 2^16-2 are reserved for ISO use
  NotSpecified = std::numeric_limits<uint16_t>::max(),
};


enum class ChannelAssociation : uint16_t {
  All = 0,
  NotAssociated = std::numeric_limits<uint16_t>::max(),
};

class ChannelDescription {
 protected:
  uint16_t cn;  //channel index;
  uint16_t typ;  //channel type;
  uint16_t asoc;  //channel association;
};


class ChannelDefinitionContents {
 protected:
  std::vector<ChannelDescription> channel_descriptions;

 public:
  ChannelDefinitionContents();
  ~ChannelDefinitionContents();

  uint64_t get_size() const noexcept {
    //2 is from N (number of channel descriptions)
    //3 is from the 3 fields in ChannelDescription
    return 2 + this->channel_descriptions.size() * 3 * sizeof(uint16_t);
  }
};


class ChannelDefinitionDBox : public DBox {
 public:
  ChannelDefinitionDBox(const ChannelDefinitionContents& contents)
      : DBox(std::make_any<ChannelDefinitionContents>(contents)) {
  }


  ChannelDefinitionDBox(const ChannelDefinitionDBox& other)
      : DBox(std::make_any<ChannelDefinitionContents>(
            std::any_cast<ChannelDefinitionContents>(other.contents))) {
  }


  ~ChannelDefinitionDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<ChannelDefinitionContents>(this->contents).get_size();
  }


  ChannelDefinitionDBox* clone() const override {
    return new ChannelDefinitionDBox(*this);
  }
};


class ChannelDefinitionBox : public Box {
 public:
  ChannelDefinitionBox(const ChannelDefinitionContents& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ChannelDefinitionBoxType)),
            ChannelDefinitionDBox(contents)){};
  ~ChannelDefinitionBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_CHANNELDEFINITIONBOX_H__ */
#ifndef CHANNELDEFINITIONCONTENTS_H__
#define CHANNELDEFINITIONCONTENTS_H__

#include "InMemoryDBoxContents.h"
#include "ChannelDescription.h"


class ChannelDefinitionContents : public InMemoryDBoxContents {
 protected:
  std::vector<ChannelDescription> channel_descriptions;

 public:
  ChannelDefinitionContents() = default;

  
  ChannelDefinitionContents(const ChannelDefinitionContents& other) : channel_descriptions(other.channel_descriptions) {}


  ChannelDefinitionContents(ChannelDefinitionContents&& other) : channel_descriptions(std::move(other.channel_descriptions)) {}


  ~ChannelDefinitionContents() = default;


  virtual bool is_equal(const DBoxContents& other) const override {
  	if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const ChannelDefinitionContents&>(other);
    return *this == cast_other;
  }


  virtual ChannelDefinitionContents* clone() const override {
  	return new ChannelDefinitionContents(*this);
  }


  uint64_t size() const noexcept override {
    //2 is from N (number of channel descriptions)
    //3 is from the 3 fields in ChannelDescription
    return 2 + this->channel_descriptions.size() * 3 * sizeof(uint16_t);
  }


  bool operator==(const ChannelDefinitionContents& other) const {
    return this->channel_descriptions == other.channel_descriptions;
  }


  bool operator!=(const ChannelDefinitionContents& other) const {
    return !this->operator==(other);
  }

};

#endif /* end of include guard: CHANNELDEFINITIONCONTENTS_H__ */
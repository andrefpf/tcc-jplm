#ifndef CHANNELDESCRIPTION_H__
#define CHANNELDESCRIPTION_H__

#include <cstdint>
#include <limits>


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

#endif /* end of include guard: CHANNELDESCRIPTION_H__ */
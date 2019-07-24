#ifndef JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__
#define JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__

#include "Box.h"
#include <type_traits>

enum class DefinedBoxesTypes : uint32_t {
  JPEGPlenoSignatureBoxType =   0x6A706C20,
  FileTypeBoxType =             0x66747970,
  JPEGPlenoThumbnailBoxType =   0x6A707468,
  JPEGPlenoHeaderBoxType =      0x6A706C68,
  JPEGPlenoLightFieldBoxType =  0x6A706C66,
  JPEGPlenoPointCloudBoxType =  0x6A707063,
  JPEGPlenoHologramBoxType =    0x6A70686F,
  ImageHeaderBoxType =          0x69686472, //this is from jpeg2000 part 2 (extensions)  
  BitsPerComponentBoxType =     0x62706363, //this is from jpeg2000 part 1  
  ColourSpecificationBoxType =  0x636F6C72, //this is from jpeg2000 part 1  
  ChannelDefinitionBoxType =    0x63646566, //this is from jpeg2000 part 1  
  ContiguousCodestreamBoxType = 0x6A703263, //this is from jpeg2000 part 1  
};

typedef std::underlying_type<DefinedBoxesTypes>::type DefinedBoxesTypesUnderlyingType;



#endif /* end of include guard: JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__ */
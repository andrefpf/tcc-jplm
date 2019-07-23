#ifndef DEFINEDBOXES_H__
#define DEFINEDBOXES_H__

#include "Box.h"
#include <type_traits>

enum class DefinedBoxesTypes : uint32_t {
  JPEGPlenoSignatureBoxType =  0x6A706C20,
  FileTypeBoxType =            0x66747970,
  JPEGPlenoHeaderBoxType =     0x6A706C68,
  JPEGPlenoLightFieldBoxType = 0x6A706C66,
  JPEGPlenoPointCloudBoxType = 0x6A707063,
  JPEGPlenoHologramBoxType =   0x6A70686F,
};

typedef std::underlying_type<DefinedBoxesTypes>::type DefinedBoxesTypesUnderlyingType;



#endif /* end of include guard: DEFINEDBOXES_H__ */
#ifndef COMPRESSIONTYPEIMAGE_H__
#define COMPRESSIONTYPEIMAGE_H__

#include "CompressionType.h"

enum class CompressionTypeImage : compression_type_data {
  JPEG_2000 = 0,
  JPEG = 1,
  JPEG_LS = 2,
  JPEG_XT = 3,
  JPEG_XR = 4,
  JPEG_XS = 5
  //all other values are reserved for ISO use
};

#endif /* end of include guard: COMPRESSIONTYPEIMAGE_H__ */
#ifndef JPEGPLENOSIGNATUREBOX_H__
#define JPEGPLENOSIGNATUREBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"

class JpegPlenoSignatureBox : public Box {
 public:
  //t_box d_box
  JpegPlenoSignatureBox()
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::JPEGPlenoSignatureBoxType)),
            CharArrayDBox({0x0d, 0x0a, 0x87, 0x0a})){};
  ~JpegPlenoSignatureBox() = default;
};

#endif /* end of include guard: JPEGPLENOSIGNATUREBOX_H__ */
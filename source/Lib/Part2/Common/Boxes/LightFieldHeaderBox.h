#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERBOX_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERBOX_H__


#include "Lib/Common/Boxes/Box.h"
#include "LightFieldHeaderDBox.h"


class LightFieldHeaderBox : public Box {
 public:
  constexpr static uint32_t id = 0x6c686472;
  //! \todo: how to check for contradictory information between the LightFieldHeaderBox and the first codestream?

  LightFieldHeaderBox(const LightFieldHeaderContents& contents)
      : Box(TBox(id), LightFieldHeaderDBox(contents)) {
  }


  LightFieldHeaderBox(const LightFieldHeaderBox& other)
      : Box(TBox(id), *other.d_box) {
  }


  ~LightFieldHeaderBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERBOX_H__ */
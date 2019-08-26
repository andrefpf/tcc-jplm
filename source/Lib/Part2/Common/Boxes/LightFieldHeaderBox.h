#ifndef LIGHTFIELDHEADERBOX_H__
#define LIGHTFIELDHEADERBOX_H__


#include "Lib/Common/Boxes/Box.h"
#include "LightFieldHeaderDBox.h"


class LightFieldHeaderBox : public Box {
 public:
  constexpr static uint32_t id = 0x6c686472;


  LightFieldHeaderBox(const LightFieldHeaderCotngents& contents)
      : Box(TBox(id), LightFieldHeaderDBox(contents)) {
  }


  LightFieldHeaderBox(const LightFieldHeaderBox& other)
      : Box(TBox(id), *other.d_box) {
  }


  ~LightFieldHeaderBox() = default;
};

#endif /* end of include guard: LIGHTFIELDHEADERBOX_H__ */
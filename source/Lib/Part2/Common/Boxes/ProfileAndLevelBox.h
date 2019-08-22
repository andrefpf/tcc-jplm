#ifndef PROFILEANDLEVELBOX_H__
#define PROFILEANDLEVELBOX_H__


#include "Lib/Common/Boxes/Box.h"
#include "ProfileAndLevelDBox.h"


class ProfileAndLevelBox : public Box {
 public:
  constexpr static uint32_t id = 0x6a70686f;
  ProfileAndLevelBox(const ProfileAndLevelContents& contents)
      : Box(TBox(id), ProfileAndLevelDBox(contents)) {
  }
  ProfileAndLevelBox(const uint16_t ppih, const uint16_t plev)
      : Box(TBox(id),
            ProfileAndLevelDBox(ProfileAndLevelContents(ppih, plev))) {
  }

  ProfileAndLevelBox(const ProfileAndLevelBox& other)
      : Box(TBox(id), *other.d_box) {
  }

  ~ProfileAndLevelBox() = default;
};

#endif /* end of include guard: PROFILEANDLEVELBOX_H__ */
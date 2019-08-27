#ifndef JPLM_LIB_PART2_COMMON_PROFILEANDLEVELBOX_H__
#define JPLM_LIB_PART2_COMMON_PROFILEANDLEVELBOX_H__


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


  const ProfileAndLevelContents& get_const_ref_to_contents() const {
    return dynamic_cast<const ProfileAndLevelContents&>(
        this->get_ref_to_dbox_contents());
  }


  ~ProfileAndLevelBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_PROFILEANDLEVELBOX_H__ */
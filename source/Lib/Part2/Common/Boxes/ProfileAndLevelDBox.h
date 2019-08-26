#ifndef JPLM_LIB_PART2_COMMON_PROFILEANDLEVELDBOX_H__
#define JPLM_LIB_PART2_COMMON_PROFILEANDLEVELDBOX_H__

#include "ProfileAndLevelContents.h"
#include "source/Lib/Common/Boxes/DBox.h"

class ProfileAndLevelDBox : public DBox {
 public:
  ProfileAndLevelDBox(const ProfileAndLevelContents& contents)
      : DBox(std::move(std::make_unique<ProfileAndLevelContents>(contents))) {
  }


  ProfileAndLevelDBox(const ProfileAndLevelDBox& other)
      : DBox(std::move(std::make_unique<ProfileAndLevelContents>(
                  other.get_ref_to_contents()))) {
  }


  virtual ~ProfileAndLevelDBox() = default;


  virtual const ProfileAndLevelContents& get_ref_to_contents() const override {
    return static_cast<const ProfileAndLevelContents&>(*contents);
  }


  ProfileAndLevelDBox* clone() const override {
    return new ProfileAndLevelDBox(*this);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_PROFILEANDLEVELDBOX_H__ */
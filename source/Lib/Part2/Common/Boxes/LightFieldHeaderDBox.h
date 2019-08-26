#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERDBOX_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERDBOX_H__

#include "Lib/Common/Boxes/DBox.h"
#include "LightFieldHeaderContents.h"


class LightFieldHeaderDBox : public DBox {
 public:
  LightFieldHeaderDBox(const LightFieldHeaderContents& contents)
      : DBox(std::move(std::make_unique<LightFieldHeaderContents>(contents))) {
  }


  LightFieldHeaderDBox(const LightFieldHeaderDBox& other)
      : DBox(std::move(std::make_unique<LightFieldHeaderContents>(
            other.get_ref_to_contents()))) {
  }


  virtual ~LightFieldHeaderDBox() = default;


  virtual const LightFieldHeaderContents& get_ref_to_contents() const override {
    return static_cast<const LightFieldHeaderContents&>(*contents);
  }


  LightFieldHeaderDBox* clone() const override {
    return new LightFieldHeaderDBox(*this);
  }
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDHEADERDBOX_H__ */
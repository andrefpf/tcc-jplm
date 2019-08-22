#ifndef LIGHTFIELDHEADERDBOX_H__
#define LIGHTFIELDHEADERDBOX_H__

#include "LightFieldHeaderContents.h"
#include "source/Lib/Common/Boxes/DBox.h"


class LightfieldHeaderDBox : public DBox {
 public:
  LightfieldHeaderDBox(const LightFieldHeaderContents& contents)
      : DBox(std::make_unique<LightFieldHeaderContents>(contents)) {
  }


  LightfieldHeaderDBox(const LightfieldHeaderDBox& other)
      : DBox(std::make_unique<LightFieldHeaderContents>(
            other.get_ref_to_contents())) {
  }


  virtual ~LightfieldHeaderDBox() = default;


  virtual const LightFieldHeaderContents& get_ref_to_contents() const override {
    return static_cast<const LightFieldHeaderContents&>(*contents);
  }


  LightfieldHeaderDBox* clone() const override {
    return new LightfieldHeaderDBox(*this);
  }
};


#endif /* end of include guard: LIGHTFIELDHEADERDBOX_H__ */
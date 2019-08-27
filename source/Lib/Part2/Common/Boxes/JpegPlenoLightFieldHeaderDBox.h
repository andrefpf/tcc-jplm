#ifndef JPEGPLENOLIGHTFIELDHEADERDBOX_H__
#define JPEGPLENOLIGHTFIELDHEADERDBOX_H__

#include "Lib/Common/Boxes/DBox.h"
#include "JpegPlenoLightFieldHeaderContents.h"

class JpegPlenoLightFieldHeaderDBox : public DBox {
 public:
  JpegPlenoLightFieldHeaderDBox(const JpegPlenoLightFieldHeaderContents& contents)
      : DBox(std::make_unique<JpegPlenoLightFieldHeaderContents>(contents)) {
  }


  JpegPlenoLightFieldHeaderDBox(const JpegPlenoLightFieldHeaderDBox& other)
      : DBox(std::make_unique<JpegPlenoLightFieldHeaderContents>(
            other.get_ref_to_contents())) {
  }


  virtual const JpegPlenoLightFieldHeaderContents& get_ref_to_contents() const override {
    return static_cast<const JpegPlenoLightFieldHeaderContents&>(*contents);
  }


  ~JpegPlenoLightFieldHeaderDBox() = default;


  JpegPlenoLightFieldHeaderDBox* clone() const override {
    return new JpegPlenoLightFieldHeaderDBox(*this);
  }
};

#endif /* end of include guard: JPEGPLENOLIGHTFIELDHEADERDBOX_H__ */
#ifndef JPEGPLENOTHUMBNAILDBOX_H__
#define JPEGPLENOTHUMBNAILDBOX_H__

#include "source/Lib/Common/Boxes/DBox.h"
#include "JpegPlenoThumbnailContents.h"


class JpegPlenoThumbnailDBox : public DBox {
 public:
  JpegPlenoThumbnailDBox(const JpegPlenoThumbnailContents& contents)
      : DBox(std::make_unique<JpegPlenoThumbnailContents>(contents)) {
  }


  JpegPlenoThumbnailDBox(const JpegPlenoThumbnailDBox& other)
      : DBox(std::make_unique<JpegPlenoThumbnailContents>(
            other.get_ref_to_contents())) {
  }


  virtual const JpegPlenoThumbnailContents& get_ref_to_contents() const override {
    return static_cast<const JpegPlenoThumbnailContents&>(*contents);
  }


  ~JpegPlenoThumbnailDBox() = default;


  JpegPlenoThumbnailDBox* clone() const override {
    return new JpegPlenoThumbnailDBox(*this);
  }


  friend void swap(JpegPlenoThumbnailDBox& thumbnail_a, JpegPlenoThumbnailDBox& thumbnail_b) {
    using std::swap;

    swap(thumbnail_a.contents, thumbnail_b.contents);
  }


  JpegPlenoThumbnailDBox& operator=(const JpegPlenoThumbnailDBox& other) {
    if(&other == this)
      return *this;

    JpegPlenoThumbnailDBox temp{other};
    swap(*this, temp);

    return *this;

  }

};

#endif /* end of include guard: JPEGPLENOTHUMBNAILDBOX_H__ */
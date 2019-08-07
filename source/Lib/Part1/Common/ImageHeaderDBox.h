#ifndef IMAGEHEADERDBOX_H__
#define IMAGEHEADERDBOX_H__

#include "DBox.h"
#include "ImageHeaderContents.h"

class ImageHeaderDBox : public DBox {
 public:
  ImageHeaderDBox(const ImageHeaderContents& contents)
      : DBox(std::make_unique<ImageHeaderContents>(contents)) {
  }


  ImageHeaderDBox(const ImageHeaderDBox& other)
      : DBox(std::make_unique<ImageHeaderContents>(
            other.get_ref_to_contents())) {
  }


  virtual const ImageHeaderContents& get_ref_to_contents() const override {
    return static_cast<const ImageHeaderContents&>(*contents);
  }
  

  ~ImageHeaderDBox() = default;


  ImageHeaderDBox* clone() const override {
    return new ImageHeaderDBox(*this);
  }
};

#endif /* end of include guard: IMAGEHEADERDBOX_H__ */
#ifndef IMAGEHEADERBOX_H__
#define IMAGEHEADERBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"


enum class CoderTypeC : uint8_t {
	JPEG_2000=0,
	JPEG=1,
	JPEG_LS=2,
	JPEG_XT=3,
	JPEG_XR=4,
	JPEG_XS=5
};

typedef std::underlying_type<CoderTypeC>::type CoderTypeCUnderlyingType;


class ImageHeaderContents
{
protected:
	uint32_t height;
	uint32_t width;
	uint16_t nc;
	uint8_t bpc;
	CoderTypeC c;
	uint8_t UnkC;
	uint8_t IPR;
public:
	ImageHeaderContents();
	~ImageHeaderContents();

	uint64_t get_size() const noexcept {
		return 2*sizeof(uint32_t)+sizeof(uint16_t)+3*sizeof(uint8_t)+sizeof(CoderTypeCUnderlyingType);
	}
};


class ImageHeaderDBox : public DBox {
 public:
  ImageHeaderDBox(const ImageHeaderContents& contents)
      : DBox(std::make_any<ImageHeaderContents>(contents)) {
  }


  ImageHeaderDBox(const ImageHeaderDBox& other)
      : DBox(std::make_any<ImageHeaderContents>(
            std::any_cast<ImageHeaderContents>(other.contents))) {
  }


  ~ImageHeaderDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<ImageHeaderContents>(this->contents).get_size();
  }


  ImageHeaderDBox* clone() const override {
    return new ImageHeaderDBox(*this);
  }
};


class ImageHeaderBox : public Box {
 public:
  ImageHeaderBox(const ImageHeaderDBox& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ImageHeaderBoxType)),
            contents){};
  ~ImageHeaderBox() = default;
};

#endif /* end of include guard: IMAGEHEADERBOX_H__ */
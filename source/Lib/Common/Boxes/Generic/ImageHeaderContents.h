#ifndef IMAGEHEADERCONTENTS_H__
#define IMAGEHEADERCONTENTS_H__

#include <tuple>  //std::tie
#include "Lib/Part1/Common/CommonExceptions.h"
#include "Lib/Common/Boxes/InMemoryDBoxContents.h"

enum class CoderTypeC : uint8_t {
  JPEG_2000 = 0,
  JPEG = 1,
  JPEG_LS = 2,
  JPEG_XT = 3,
  JPEG_XR = 4,
  JPEG_XS = 5
};


typedef std::underlying_type<CoderTypeC>::type CoderTypeCUnderlyingType;


class ImageHeaderContents : public InMemoryDBoxContents {
 protected:
  uint32_t height;
  uint32_t width;
  uint16_t nc;  //! Number of components
  uint8_t bpc;  //! Bits per component
  CoderTypeC c;  //! Compression type
  uint8_t UnkC;  //! Colourspace Unknown
  uint8_t IPR;  //! Intellectual Property

 public:
  ImageHeaderContents(uint32_t height, uint32_t width,
      uint16_t number_of_channels, uint8_t bits_per_component,
      CoderTypeC coder_type, uint8_t UnkC, uint8_t IPR)
      : height(height), width(width), nc(number_of_channels),
        bpc(bits_per_component), c(coder_type), UnkC(UnkC), IPR(IPR) {
    //should width, height and bpc be checked against 0?
    if ((height == 0) || (width == 0) || (nc == 0) || (bpc == 0)) {
      throw ImageHeaderBoxExceptions::InvalidSizeException(height, width, nc, bpc);
    }
  }


  uint32_t get_height() const noexcept {
    return height;
  }


  uint32_t get_width() const noexcept {
    return width;
  }


  uint16_t get_nc() const noexcept {
    return nc;
  }


  uint16_t get_number_of_channels() const noexcept {
    return get_nc();
  }


  uint8_t get_bpc() const noexcept {
    return bpc;
  }


  uint8_t get_bits_per_component() const noexcept {
    return get_bpc();
  }


  CoderTypeC get_c() const noexcept {
    return c;
  }


  CoderTypeC get_coder_type() const noexcept {
    return get_c();
  }


  bool has_known_color_space() const noexcept {
    if (UnkC == 0) {  // 0, if the colourspace of the image is known
      return true;
    }
    //! \todo Check if should throw exception when value is not 0 or 1
    // Values other than 0 and 1 are reserved for ISO use
    return false;
  }


  bool has_intellectual_property() const noexcept {
    if(UnkC == 0) {
      return false;
    } //1
    return true;
    //! \todo Check if should throw exception when value is not 0 or 1
     // Other values are reserved for ISO use
  }


  virtual ImageHeaderContents* clone() const override {
    return new ImageHeaderContents(*this);
  }


  virtual ~ImageHeaderContents() = default;


  virtual uint64_t size() const noexcept override {
    return 2 * sizeof(uint32_t) + sizeof(uint16_t) + 3 * sizeof(uint8_t) +
           sizeof(CoderTypeCUnderlyingType);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const ImageHeaderContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ImageHeaderContents& other) const {
    return std::tie(this->height, this->width, this->nc, this->bpc, this->c,
               this->UnkC, this->IPR) == std::tie(other.height, other.width,
                                             other.nc, other.bpc, other.c,
                                             other.UnkC, other.IPR);
  }


  bool operator!=(const ImageHeaderContents& other) const {
    return !this->operator==(other);
  }
};

#endif /* end of include guard: IMAGEHEADERCONTENTS_H__ */
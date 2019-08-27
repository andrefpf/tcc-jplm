#ifndef COLOURSPECIFICATIONCONTENTS_H__
#define COLOURSPECIFICATIONCONTENTS_H__

#include <assert.h>
#include <optional>
#include <tuple>  //std::tie
#include "Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "Lib/Part1/Common/BinaryTools.h"

class ICCProfile {
  //not implemented
 public:
  uint64_t size() const noexcept {
    return 0;  //not implemented
  }
};


using enum_cs_field_type = uint32_t;

enum class EnumCS : enum_cs_field_type {
  sRGB = 16,  //sRGB as defined by IEC 61966–2–1
  greyscale =
      17,  //greyscale: A greyscale space where image luminance is related to code values
  // using the sRGB non-linearity given in Eqs. (2) through (4) of IEC 61966–2–1
  // (sRGB) specification
  //
  // other values are reserved for other ISO uses
};

class ColourSpecificationContents : public InMemoryDBoxContents {
 protected:
  // Comments from ISO/ICE 15444-1:2000(E) pg 161
  uint8_t meth;  // specification method, shall be 1 or 2
  //   Specification method. This field specifies the method used by this Colour Specification box to define
  // the colourspace of the decompressed image. This field is encoded as a 1-byte unsigned integer. The
  // value of this field shall be 1 or 2, as defined in Table I-9.
  //
  // 1 means Enumerated Colourspace.
  // 2 means Restricted ICC profile.

  int8_t prec;  // precedence. Reserved for ISO use and value shall be 0;
  //   Precedence. This field is reserved for ISO use and the value shall be set to zero; however, conforming
  // readers shall ignore the value of this field. This field is specified as a signed 1 byte integer.

  uint8_t
      approx;  // Coulourspace approximation; shall be set to 0; other values for ISO use;
  //   Colourspace approximation. This field specifies the extent to which this colour specification
  // method approximates the “correct” definition of the colourspace. The value of this field shall be set to
  // zero; however, conforming readers shall ignore the value of this field. Other values are reserved for
  // other ISO use. This field is specified as 1 byte unsigned integer

  std::optional<EnumCS> enum_cs;  //enumerated colourspace
  //   Enumerated colourspace. This field specifies the colourspace of the image using integer codes. To
  // correctly interpret the colour of an image using an enumerated colourspace, the application must know
  // the definition of that colourspace internally. This field contains a 4-byte big endian unsigned integer
  // value indicating the colourspace of the image. If the value of the METH field is 2, then the EnumCS
  // field shall not exist. Valid EnumCS values for the first colourspace specification box in conforming files
  // are limited to 16 and 17 as defined in Table I-10:

  std::optional<ICCProfile> profile;
  //   This field contains a valid ICC profile, as specified by the ICC Profile Format
  // Specification, which specifies the transformation of the decompressed image data into the PCS. This
  // field shall not exist if the value of the METH field is 1. If the value of the METH field is 2, then the ICC
  // profile shall conform to the Monochrome Input Profile class or the Three-Component Matrix-Based
  // Input Profile class as defined in ICC.1:1998-09.

 public:
  ColourSpecificationContents(EnumCS enum_cs = EnumCS::sRGB)
      : meth(1), prec(0), approx(0), enum_cs(enum_cs) {
  }


  ColourSpecificationContents(ICCProfile profile)
      : meth(2), prec(0), approx(0), profile(profile) {
  }


  virtual ~ColourSpecificationContents() = default;


  virtual ColourSpecificationContents* clone() const override {
    return new ColourSpecificationContents(*this);
  }


  virtual uint64_t size() const noexcept override {
    constexpr auto initial_size = 3 * sizeof(uint8_t);
    if (profile) {
      return initial_size + profile->size();
    }
    assert(enum_cs);
    return initial_size + sizeof(enum_cs_field_type);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ColourSpecificationContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ColourSpecificationContents& other) const {
    //! \todo Add ICCProfile if needed
    return std::tie(meth, prec, approx, enum_cs) ==
           std::tie(other.meth, other.prec, other.approx, other.enum_cs);
  }


  bool operator!=(const ColourSpecificationContents& other) const {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const noexcept override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    bytes.emplace_back(std::byte{meth});
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
    //! \warning There is a narrowing conversion taking place. The standard defines prec as int8. However, it also defines this as 0.
    bytes.emplace_back(std::byte{prec});
#pragma GCC diagnostic pop

    bytes.emplace_back(std::byte{approx});


    // if (profile) {
    //   return initial_size + profile->size();
    // }
    //! \todo implement getting bytes from icc_profile
    assert(!profile);
    assert(enum_cs);

    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<enum_cs_field_type>(*enum_cs));


    return bytes;
  }
};


#endif /* end of include guard: COLOURSPECIFICATIONCONTENTS_H__ */
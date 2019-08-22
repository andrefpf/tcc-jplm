#ifndef LIGHTFIELDHEADERCONTENTS_H__
#define LIGHTFIELDHEADERCONTENTS_H__

#include <tuple>  //std::tie
#include "CompressionTypeLightField.h"
#include "Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "Lib/Part1/Common/BinaryTools.h"


using colour_space_unknown_flag_type = uint8_t;

enum class ColourSpaceUnknownFlag : uint8_t {
  known =
      0,  //the colour space is known and correctly specified in the Colourspace Specification boxes
  unknown = 1,  //the colour space is unknown
  //other values are reserved for ISO use
};

using intelectual_property_flag_type = uint8_t;

enum class IntelectualPropertyFlag : intelectual_property_flag_type {
  has_no_rights_information = 0,  //
  has_rights_information = 1,  //
  //other values are reserved for ISO use
};


class LightFieldHeaderContents : public InMemoryDBoxContents {
 protected:
  uint32_t rows;  //T number of rows of the subaperture view array
  uint32_t columns;  //S number of columns of the subaperture view array
  uint32_t height;  //V subaperture view height
  uint32_t width;  //U subaperture view width
  uint16_t nc;  // number of components
  uint8_t bpc;  // bits per component
  CompressionTypeLightField c;  // compression type (i.e., the mode)
  ColourSpaceUnknownFlag unkc;  //
  IntelectualPropertyFlag ipr;  //
 public:
  LightFieldHeaderContents(uint32_t rows, uint32_t columns, uint32_t height,
      uint32_t width, uint16_t nc, uint8_t bpc, CompressionTypeLightField c,
      ColourSpaceUnknownFlag unkc, IntelectualPropertyFlag ipr)
      : rows(rows), columns(columns), height(height), width(width), nc(nc),
        bpc(bpc), c(c), unkc(unkc), ipr(ipr) {
  }

  virtual ~LightFieldHeaderContents() = default;


  virtual LightFieldHeaderContents* clone() const override {
    return new LightFieldHeaderContents(*this);
  }


  uint64_t size() const noexcept override {
    return 2 * sizeof(uint16_t);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const LightFieldHeaderContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const LightFieldHeaderContents& other) const noexcept {
    return (std::tie(this->rows, this->columns, this->height, this->width,
                this->nc, this->bpc, this->c, this->unkc, this->ipr) ==
            std::tie(other.rows, other.columns, other.height, other.width,
                other.nc, other.bpc, other.c, other.unkc, other.ipr));
  }


  bool operator!=(const LightFieldHeaderContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    BinaryTools::append_big_endian_bytes(bytes, rows);
    BinaryTools::append_big_endian_bytes(bytes, columns);
    BinaryTools::append_big_endian_bytes(bytes, height);
    BinaryTools::append_big_endian_bytes(bytes, width);
    BinaryTools::append_big_endian_bytes(bytes, nc);
    BinaryTools::append_big_endian_bytes(bytes, bpc);

    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<compression_type_data>(c));
    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<colour_space_unknown_flag_type>(unkc));
    BinaryTools::append_big_endian_bytes(
        bytes, static_cast<intelectual_property_flag_type>(ipr));

    return bytes;
  }
}

#endif /* end of include guard: LIGHTFIELDHEADERCONTENTS_H__ */
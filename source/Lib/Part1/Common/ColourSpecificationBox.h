#ifndef JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__
#define JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__

#include <stdexcept>
#include "Box.h"
#include "DefinedBoxes.h"

class ICCProfile {
  //not implemented
 public:
  uint64_t get_size() {
    return 0;  //not implemented
  }
};

class ColourSpecificationContents {
 protected:
  uint8_t meth;  //specification method, shall be 1 or 2
  int8_t prec;  //precedence. Reserved for ISO use and value shall be 0;
  uint8_t approx;  //Coulourspace approximation; shall be set to 0; other values for ISO use;
  std::optional<uint16_t> EnumCS;  //enumerated colourspace
  std::optional<ICCProfile> profile;

 public:
  ColourSpecificationContents(uint16_t EnumCS) : meth(1), prec(0), approx(0), EnumCS(EnumCS) {}
  ColourSpecificationContents(ICCProfile profile) : meth(2), prec(0), approx(0), profile(profile) {}

  ~ColourSpecificationContents();

  uint64_t get_size() {
    constexpr auto initial_size = 3 * sizeof(uint8_t);
    if (EnumCS) {
      return initial_size + sizeof(uint16_t);
    }
    if (profile) {
      return initial_size + profile->get_size();
    }
    throw std::runtime_error(
        "Either EnumCS or ICCProfile must be present in "
        "ColourSpecificationBox");
  }
};


class ColourSpecificationDBox : public DBox {
 public:
  ColourSpecificationDBox(const ColourSpecificationContents& contents)
      : DBox(std::make_any<ColourSpecificationContents>(contents)) {
  }


  ColourSpecificationDBox(const ColourSpecificationDBox& other)
      : DBox(std::make_any<ColourSpecificationContents>(
            std::any_cast<ColourSpecificationContents>(other.contents))) {
  }
  

  ~ColourSpecificationDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<ColourSpecificationContents>(this->contents)
        .get_size();
  }


  ColourSpecificationDBox* clone() const override {
    return new ColourSpecificationDBox(*this);
  }
};


class ColourSpecificationBox : public Box {
 public:
  ColourSpecificationBox(const std::vector<uint8_t>& bits_per_component_vector)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ColourSpecificationBoxType)),
            CharArrayDBox(bits_per_component_vector)){};
  ~ColourSpecificationBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_COLOURSPECIFICATIONBOX_H__ */
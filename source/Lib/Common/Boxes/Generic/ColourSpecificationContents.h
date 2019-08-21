#ifndef COLOURSPECIFICATIONCONTENTS_H__
#define COLOURSPECIFICATIONCONTENTS_H__

#include <optional>
#include <tuple>  //std::tie
#include "source/Lib/Common/Boxes/InMemoryDBoxContents.h"
#include <assert.h>

class ICCProfile {
  //not implemented
 public:
  uint64_t size() const noexcept {
    return 0;  //not implemented
  }
};

class ColourSpecificationContents : public InMemoryDBoxContents {
 protected:
  uint8_t meth;  //specification method, shall be 1 or 2
  int8_t prec;  //precedence. Reserved for ISO use and value shall be 0;
  uint8_t
      approx;  //Coulourspace approximation; shall be set to 0; other values for ISO use;
  std::optional<uint16_t> EnumCS;  //enumerated colourspace
  std::optional<ICCProfile> profile;

 public:
  ColourSpecificationContents(uint16_t EnumCS)
      : meth(1), prec(0), approx(0), EnumCS(EnumCS) {
  }


  ColourSpecificationContents(ICCProfile profile)
      : meth(2), prec(0), approx(0), profile(profile) {
  }


  ~ColourSpecificationContents();

  virtual ColourSpecificationContents* clone() const override {
    return new ColourSpecificationContents(*this);
  }


  virtual uint64_t size() const noexcept override {
    constexpr auto initial_size = 3 * sizeof(uint8_t);
    if (profile) {
      return initial_size + profile->size();
    }
    assert(EnumCS);
    return initial_size + sizeof(uint16_t);
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
    return std::tie(meth, prec, approx, EnumCS) ==
           std::tie(other.meth, other.prec, other.approx, other.EnumCS);
  }


  bool operator!=(const ColourSpecificationContents& other) const {
    return !this->operator==(other);
  }
};


#endif /* end of include guard: COLOURSPECIFICATIONCONTENTS_H__ */
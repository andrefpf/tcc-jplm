#ifndef PROFILEANDLEVELCONTENTS_H__
#define PROFILEANDLEVELCONTENTS_H__


#include <tuple>  //std::tie
#include "source/Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "source/Lib/Part1/Common/BinaryTools.h"

class ProfileAndLevelContents : public InMemoryDBoxContents {
 protected:
  uint16_t ppih;  // profile of the codestream
  uint16_t plev;  // level of the codestream
 public:
  ProfileAndLevelContents(uint16_t ppih, uint16_t plev)
      : ppih(ppih), plev(plev) {
  }

  virtual ~ProfileAndLevelContents() = default;


  virtual ProfileAndLevelContents* clone() const override {
    return new ProfileAndLevelContents(*this);
  }


  uint64_t size() const noexcept override {
    return 2 * sizeof(uint16_t);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ProfileAndLevelContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ProfileAndLevelContents& other) const noexcept {
    return (
        std::tie(this->ppih, this->plev) == std::tie(other.ppih, other.plev));
  }


  bool operator!=(const ProfileAndLevelContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    BinaryTools::append_big_endian_bytes(bytes, ppih);
    BinaryTools::append_big_endian_bytes(bytes, plev);
    return bytes;
  }
}

#endif /* end of include guard: PROFILEANDLEVELCONTENTS_H__ */
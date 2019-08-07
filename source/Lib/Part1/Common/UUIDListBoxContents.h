#ifndef UUIDLISTBOXCONTENTS_H__
#define UUIDLISTBOXCONTENTS_H__


#include "UniversalUniqueIdentifier.h"
#include "InMemoryDBoxContents.h"


class UUIDListBoxContents : public InMemoryDBoxContents {
 protected:
  std::vector<UniversalUniqueIdentifier> id;

 public:
  UUIDListBoxContents() = default;


  virtual UUIDListBoxContents* clone() const override {
    return new UUIDListBoxContents(*this);
  }


  ~UUIDListBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return 2 + id.size() * 16;
    //2 for NU (Number of UUID) + 16 for each uuid on the list
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const UUIDListBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const UUIDListBoxContents& other) const {
    return (this->id == other.id);
  }


  bool operator!=(const UUIDListBoxContents& other) const {
    return !this->operator==(other);
  }


  uint16_t get_nu() const noexcept {
    return id.size();
  }
};

#endif /* end of include guard: UUIDLISTBOXCONTENTS_H__ */
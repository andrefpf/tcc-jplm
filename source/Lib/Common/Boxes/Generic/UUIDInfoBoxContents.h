#ifndef UUIDINFOBOXCONTENTS_H__
#define UUIDINFOBOXCONTENTS_H__


#include "DataEntryURLBox.h"
#include "source/Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "UUIDListBox.h"
#include "UniversalUniqueIdentifier.h"


class UUIDInfoBoxContents : public InMemoryDBoxContents {
 protected:
  UUIDListBox u_list;
  DataEntryURLBox de;

 public:
  UUIDInfoBoxContents() = default;

  // implemented copy constructors in UUIDListBox, DataEntryURLBox
  UUIDInfoBoxContents(const UUIDInfoBoxContents& other)
      : u_list(other.u_list), de(other.de) {
  }


  UUIDInfoBoxContents(UUIDInfoBoxContents&& other)
      : u_list(std::move(other.u_list)), de(std::move(other.de)) {
  }


  virtual UUIDInfoBoxContents* clone() const override {
    return new UUIDInfoBoxContents(*this);
  }


  ~UUIDInfoBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return u_list.size() + de.size();
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const UUIDInfoBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const UUIDInfoBoxContents& other) const {
    return (this->u_list == other.u_list) && (this->de == other.de);
  }


  bool operator!=(const UUIDInfoBoxContents& other) const {
    return !this->operator==(other);
  }
};

#endif /* end of include guard: UUIDINFOBOXCONTENTS_H__ */
#ifndef UUIDBOXCONTENTS_H__
#define UUIDBOXCONTENTS_H__

//UUID

#include "Lib/Common/Boxes/InMemoryDBoxContents.h"
#include "UniversalUniqueIdentifier.h"

class UUIDBoxContents : public InMemoryDBoxContents {
 protected:
  UniversalUniqueIdentifier id;
  std::vector<uint8_t> data;

 public:
  UUIDBoxContents() = default;


  virtual UUIDBoxContents* clone() const override {
    return new UUIDBoxContents(*this);
  }


  virtual ~UUIDBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return id.size() + data.size() * sizeof(uint8_t);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const UUIDBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const UUIDBoxContents& other) const {
    return (this->id == other.id) && (this->data == other.data);
  }


  bool operator!=(const UUIDBoxContents& other) const {
    return !this->operator==(other);
  }


  void add_data(const std::vector<uint8_t>& data_to_add) {
    data.reserve(data_to_add.size());
    data.insert(data.end(), data_to_add.begin(), data_to_add.end());
  }
};


#endif /* end of include guard: UUIDBOXCONTENTS_H__ */
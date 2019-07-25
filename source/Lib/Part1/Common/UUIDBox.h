#ifndef UUIDBOX_H__
#define UUIDBOX_H__

#include "Box.h"

//UUID

struct my_uint48_t {
  uint16_t hi;
  uint64_t lo;
};


class UniversalUniqueIdentifier {
 protected:
  my_uint48_t node;
  uint8_t clock_seq_low;
  uint8_t clock_seq_hi_and_reserved;
  uint16_t time_hi_and_version;
  uint16_t time_mid;
  uint32_t time_low;

 public:
  UniversalUniqueIdentifier();
  ~UniversalUniqueIdentifier();

  bool operator==(const UniversalUniqueIdentifier& other) const {
    return std::tie(this->node, this->clock_seq_low,
               this->clock_seq_hi_and_reserved, this->time_hi_and_version,
               this->time_mid, this->time_low) ==
           std::tie(other.node, other.clock_seq_low,
               other.clock_seq_hi_and_reserved, other.time_hi_and_version,
               other.time_mid, other.time_low);
  }


  bool operator!=(const UniversalUniqueIdentifier& other) const {
    return !this->operator==(other);
  }

  uint64_t get_size() const noexcept {
    return 16;
    //16 = 128 bits
  }
};


class UUIDBoxContents {
 protected:
  UniversalUniqueIdentifier id;
  std::vector<uint8_t> data;

 public:
  UUIDBoxContents();
  ~UUIDBoxContents();

  uint64_t get_size() const noexcept {
    return id.get_size() + data.size() * sizeof(uint8_t);
  }

  bool operator==(const UUIDBoxContents& other) const {
    return (this->id == other.id) && (this->data == other.data);
  }


  bool operator!=(const UUIDBoxContents& other) const {
    return !this->operator==(other);
  }
};


class UUIDBox : public DBox {
 public:
  UUIDBox(const UUIDBoxContents& contents)
      : DBox(std::make_any<UUIDBoxContents>(contents)) {
  }


  UUIDBox(const UUIDBox& other)
      : DBox(std::make_any<UUIDBoxContents>(
            std::any_cast<UUIDBoxContents>(other.contents))) {
  }


  ~UUIDBox() = default;


  uint64_t get_size() const noexcept override {
    return std::any_cast<UUIDBoxContents>(this->contents).get_size();
  }


  UUIDBox* clone() const override {
    return new UUIDBox(*this);
  }

  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<UUIDBoxContents>(this->get_ref_to_contents()) ==
            std::any_cast<UUIDBoxContents>(other.get_ref_to_contents()));
  }
};


#endif /* end of include guard: UUIDBOX_H__ */
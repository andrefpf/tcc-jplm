#ifndef UNIVERSALUNIQUEIDENTIFIER_H__
#define UNIVERSALUNIQUEIDENTIFIER_H__

#include <tuple>

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

  uint64_t size() const noexcept {
    return 16;
    //16 = 128 bits
  }
};

#endif /* end of include guard: UNIVERSALUNIQUEIDENTIFIER_H__ */
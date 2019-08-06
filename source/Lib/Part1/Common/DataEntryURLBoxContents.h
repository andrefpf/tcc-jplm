#ifndef DATAENTRYURLBOXCONTENTS_H__
#define DATAENTRYURLBOXCONTENTS_H__

#include "InMemoryDBoxContents.h"
#include <tuple> //std::tie
#include "BinaryTools.h"

class DataEntryURLBoxContents : public InMemoryDBoxContents {
 protected:
  uint8_t vers;  //version number
  BinaryTools::uint24_t flag;  //flags
  std::string loc;  //location (the url)

 public:
  DataEntryURLBoxContents() = default;


  virtual DataEntryURLBoxContents* clone() const override {
  	return new DataEntryURLBoxContents(*this);
  }


  ~DataEntryURLBoxContents() = default;


  virtual uint64_t size() const noexcept override {
    return 4 + loc.size() + 1;
    //4 for ver and location + the size of the string + the null termination char
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const DataEntryURLBoxContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const DataEntryURLBoxContents& other) const {
    return std::tie(this->vers, this->flag, this->loc) ==
           std::tie(other.vers, other.flag, other.loc);
  }


  bool operator!=(const DataEntryURLBoxContents& other) const {
    return !this->operator==(other);
  }


  uint8_t get_version_number() const noexcept {
    return vers;
  }


  BinaryTools::uint24_t get_flag() const noexcept {
    return flag;
  }


  const char* get_location() const noexcept {
    return loc.c_str();
  }
};

#endif /* end of include guard: DATAENTRYURLBOXCONTENTS_H__ */
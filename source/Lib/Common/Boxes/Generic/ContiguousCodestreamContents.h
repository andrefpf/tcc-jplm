#ifndef CONTIGUOUSCODESTREAMCONTENTS_H__
#define CONTIGUOUSCODESTREAMCONTENTS_H__

#include <memory>
#include "Lib/Common/Boxes/SuperBoxDBoxContents.h"
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"


class ContiguousCodestreamContents : public SuperBoxDBoxContents {
  //not implemented yet...
 protected:
  std::unique_ptr<ContiguousCodestreamCode> code;

 public:
  ContiguousCodestreamContents(const ContiguousCodestreamCode& code)
      : code(std::unique_ptr<ContiguousCodestreamCode>(code.clone())) {
  }


  ContiguousCodestreamContents(std::unique_ptr<ContiguousCodestreamCode>&& code)
      : code(std::move(code)) {
  }


  ContiguousCodestreamContents(const ContiguousCodestreamContents& other)
      : code(std::unique_ptr<ContiguousCodestreamCode>(other.code->clone())) {
  }


  virtual uint64_t size() const noexcept override {
    return code->size();
  }


  virtual ContiguousCodestreamContents* clone() const override {
    return new ContiguousCodestreamContents(*this);
  }


  virtual bool is_equal(const DBoxContents& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const ContiguousCodestreamContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const ContiguousCodestreamContents& other) const {
    return *(this->code) == *(other.code);
  }


  bool operator!=(const ContiguousCodestreamContents& other) const {
    return !this->operator==(other);
  }


  std::ostream& write_to(std::ostream& stream) const final {
    stream << *code;
    return stream;
  }
};

#endif /* end of include guard: CONTIGUOUSCODESTREAMCONTENTS_H__ */
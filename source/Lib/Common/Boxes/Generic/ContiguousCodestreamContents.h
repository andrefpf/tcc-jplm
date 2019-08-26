#ifndef CONTIGUOUSCODESTREAMCONTENTS_H__
#define CONTIGUOUSCODESTREAMCONTENTS_H__

#include <memory>
#include "Lib/Common/Boxes/SuperBoxDBoxContents.h"


class Code {
public:
  virtual uint64_t size() const noexcept = 0;
  virtual Code* clone() const = 0;

  virtual bool operator==(const Code& other) const = 0;
  virtual bool operator!=(const Code& other) const = 0;
};

std::ostream& operator<<(std::ostream& stream, const Code& code);


class ContiguousCodestreamContents : public SuperBoxDBoxContents {
  //not implemented yet...
 protected:
  std::unique_ptr<Code> code;

 public:
  ContiguousCodestreamContents(const Code& code) : code(std::unique_ptr<Code>(code.clone())) {
  }

  
  ContiguousCodestreamContents(std::unique_ptr<Code>&& code) : code(std::move(code)) {
  }


  ContiguousCodestreamContents(const ContiguousCodestreamContents& other) : code(std::unique_ptr<Code>(other.code->clone())) {
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
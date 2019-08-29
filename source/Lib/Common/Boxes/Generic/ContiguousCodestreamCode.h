#ifndef CONTIGUOUSCODESTREAMCODE_H__
#define CONTIGUOUSCODESTREAMCODE_H__

#include <cstdint>
#include <ostream>

class ContiguousCodestreamCode {
 public:
  virtual uint64_t size() const noexcept = 0;
  virtual ContiguousCodestreamCode* clone() const = 0;

  virtual bool operator==(const ContiguousCodestreamCode& other) const = 0;
  virtual bool operator!=(const ContiguousCodestreamCode& other) const = 0;
  virtual std::ostream& write_to(std::ostream& stream) const = 0;
};


std::ostream& operator<<(std::ostream& stream, const ContiguousCodestreamCode& code);

#endif /* end of include guard: CONTIGUOUSCODESTREAMCODE_H__ */
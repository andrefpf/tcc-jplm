#ifndef DBOXCONTENTS_H__
#define DBOXCONTENTS_H__

#include <stdexcept>
#include <vector>
#include <cstddef>

class DBoxContents {
  //for now this is a interface
 public:
  DBoxContents() = default;
  ~DBoxContents() = default;

  virtual uint64_t size() const noexcept = 0;

  virtual DBoxContents* clone() const = 0; ///! \todo implement

  virtual bool is_equal(const DBoxContents& other) const = 0;

  virtual std::ostream& write_to(std::ostream& stream) const = 0;
};

#endif /* end of include guard: DBOXCONTENTS_H__ */
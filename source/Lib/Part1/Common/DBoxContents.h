#ifndef DBOXCONTENTS_H__
#define DBOXCONTENTS_H__

#include <stdexcept>
#include <vector>
#include <cstddef>

class DBoxContents {
  //for now this is only a helper class
 public:
  DBoxContents() = default;
  ~DBoxContents() = default;

  virtual std::vector<std::byte> get_bytes() const {
    throw std::runtime_error(
        "Not implemented yet (ColourSpecificationContents.get_bytes).");
  }
};

#endif /* end of include guard: DBOXCONTENTS_H__ */
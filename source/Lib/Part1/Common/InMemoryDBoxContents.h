#ifndef INMEMORYDBOXCONTENTS_H__
#define INMEMORYDBOXCONTENTS_H__


#include <iostream>
#include "DBoxContents.h"

class InMemoryDBoxContents : public DBoxContents {
 public:
  InMemoryDBoxContents() = default;


  virtual ~InMemoryDBoxContents() {
  }


  virtual std::vector<std::byte> get_bytes() const {
    throw std::runtime_error(
        "Not implemented yet (get_bytes).");
  }


  virtual std::ostream& write_to(std::ostream& stream) const final;
};


std::ostream& operator<<(
    std::ostream& stream, const InMemoryDBoxContents& d_box);


#endif /* end of include guard: INMEMORYDBOXCONTENTS_H__ */
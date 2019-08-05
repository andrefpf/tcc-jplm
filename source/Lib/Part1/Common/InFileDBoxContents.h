#ifndef INFILEDBOXCONTENTS_H__
#define INFILEDBOXCONTENTS_H__

#include <iostream>
#include "DBoxContents.h"

class InFileDBoxContents : public DBoxContents {
 protected:
  std::fstream file;

 public:
  InFileDBoxContents(const std::string& filename)
      : file(filename.c_str(), std::ios::in | std::ios::out) {
  }
  virtual std::ostream& write_to(std::ostream& stream) const final;
};

#endif /* end of include guard: INFILEDBOXCONTENTS_H__ */
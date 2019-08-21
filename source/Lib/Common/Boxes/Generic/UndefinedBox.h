#ifndef UNDEFINEDBOX_H__
#define UNDEFINEDBOX_H__

#include "Lib/Common/Boxes/Box.h"
#include "UndefinedDBox.h"

class UndefinedBox : public Box {
 public:
  UndefinedBox(const TBox& type, const UndefinedDBoxContents& contents)
      : Box(type, UndefinedDBox(contents)) {
  }


  UndefinedBox(const UndefinedBox& other)
      : Box(TBox(other.t_box), *other.d_box) {
  }


  ~UndefinedBox() = default;
};

#endif /* end of include guard: UNDEFINEDBOX_H__ */
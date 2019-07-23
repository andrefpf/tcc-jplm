/*
* @Author: Ismael Seidel
* @Date:   2019-07-23 15:37:58
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-07-23 16:27:51
*/

#include "Box.h"


auto Box::get_lbox() const noexcept {
  return this->l_box;
}


auto Box::get_tbox() const noexcept {
  return this->t_box;
}


auto Box::get_xlbox() const noexcept {
  return this->xl_box;
}


auto Box::get_dbox() const noexcept {
  return this->d_box;
}


std::ostream& operator<<(std::ostream& stream, const Box& box) {
  stream << box.get_lbox() << box.get_tbox();
  auto xlbox = box.get_xlbox();
  if (xlbox) {
    stream << xlbox.value();
  }
  // stream << box.get_dbox();
  return stream;
}
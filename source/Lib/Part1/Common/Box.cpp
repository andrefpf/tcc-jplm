/*
* @Author: Ismael Seidel
* @Date:   2019-07-23 15:37:58
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-07-23 18:01:49
*/

#include "Box.h"

LBox Box::get_lbox() const noexcept {
  return this->l_box;
}


TBox Box::get_tbox() const noexcept {
  return this->t_box;
}


std::optional<XLBox> Box::get_xlbox() const noexcept {
  return this->xl_box;
}


std::unique_ptr<DBox> Box::get_dbox() const noexcept {
  return std::unique_ptr<DBox>(this->d_box->clone());
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
/*
* @Author: Ismael Seidel
* @Date:   2019-08-05 11:32:31
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-05 12:35:59
*/

#include "InMemoryDBoxContents.h"


std::ostream& InMemoryDBoxContents::write_to(std::ostream& stream) const {
  auto bytes = this->get_bytes();
  stream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
  return stream;
}

std::ostream& operator<<(
    std::ostream& stream, const InMemoryDBoxContents& d_box_contents) {
  return d_box_contents.write_to(stream);
}
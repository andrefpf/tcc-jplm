/*
* @Author: Ismael Seidel
* @Date:   2019-08-05 11:59:15
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-05 12:36:04
*/


#include "InFileDBoxContents.h"


std::ostream& InFileDBoxContents::write_to(std::ostream& stream) const {
  stream << file.rdbuf();
  return stream;
}
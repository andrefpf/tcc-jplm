/*
* @Author: Ismael Seidel
* @Date:   2019-08-29 13:18:45
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 13:20:35
*/

#include "ContiguousCodestreamCode.h"

std::ostream& operator<<(std::ostream& stream, const ContiguousCodestreamCode& code) {
  return code.write_to(stream);
}
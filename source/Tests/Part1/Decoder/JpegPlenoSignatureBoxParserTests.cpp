/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:21:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-14 17:52:55
*/

#include "Lib/Common/Boxes/BoxParser.h"

int main() {
  BoxParser::get_instance().parse(0x000000);
  std::cout << "Main" << std::endl;
  return 0;
}
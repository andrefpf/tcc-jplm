/*
* @Author: Ismael Seidel
* @Date:   2019-07-30 12:45:42
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-07-30 13:26:30
*/

#include "BinaryTools.h"


uint16_t BinaryTools::swap_endianess(const uint16_t in) {
  return ((in >> 8) & 0x00FF) | ((in << 8) & 0xFF00);
}


uint32_t BinaryTools::swap_endianess(const uint32_t in) {
  return ((in >> 24) & 0x000000FF) | ((in >> 8) & 0x0000FF00) |
         ((in << 8) & 0x00FF0000) | ((in << 24) & 0xFF000000);
}


uint64_t BinaryTools::swap_endianess(const uint64_t in) {
  return ((in >> 56) & 0x00000000000000FF) | ((in >> 40) & 0x000000000000FF00) |
         ((in >> 24) & 0x0000000000FF0000) | ((in >> 8)  & 0x00000000FF000000) |
         ((in << 8)  & 0x000000FF00000000) | ((in << 24) & 0x0000FF0000000000) |
         ((in << 40) & 0x00FF000000000000) | ((in << 56) & 0xFF00000000000000);
}
/*
* @Author: Ismael Seidel
* @Date:   2019-08-27 17:17:09
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-27 17:28:19
*/

#include "ProfileAndLevelBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::ProfileAndLevelBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto ppih = box_parser_helper.get_next<uint16_t>();
  auto plev = box_parser_helper.get_next<uint16_t>();
  auto profile_and_level_box = std::make_unique<ProfileAndLevelBox>(ppih, plev);
  return profile_and_level_box;
}

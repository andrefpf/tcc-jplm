/*
* @Author: Ismael Seidel
* @Date:   2019-08-19 14:39:46
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-19 15:41:01
*/

#include "FileTypeBoxParser.h"

//! \todo how to adere to the following restriction? All files shall contain one and only one FileTypeBox
//

// 4 4 4 4 4

std::unique_ptr<Box> JPLMBoxParser::FileTypeBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto brand = box_parser_helper.get_next<uint32_t>();
  auto minor_version = box_parser_helper.get_next<uint32_t>();

  auto compatibility_list = std::vector<uint32_t>();
  auto lenght = (box_parser_helper.get_data_lenght() - sizeof(brand) -
                  sizeof(minor_version))/sizeof(uint32_t);
  compatibility_list.reserve(lenght);

  for(auto i=decltype(lenght){0}; i < lenght; ++i) {
    compatibility_list.emplace_back(box_parser_helper.get_next<uint32_t>());  	
  }

  auto file_type_box = std::make_unique<FileTypeBox>(
      FileTypeContents(brand, minor_version, std::move(compatibility_list)));

  return file_type_box;
}
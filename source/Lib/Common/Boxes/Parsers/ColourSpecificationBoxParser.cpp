/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 10:34:36
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 10:46:29
*/

#include "ColourSpecificationBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::ColourSpecificationBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto meth = box_parser_helper.get_next<uint8_t>();
  auto prec = box_parser_helper.get_next<int8_t>();
  auto approx = box_parser_helper.get_next<uint8_t>();


  if (meth == 1) {
    auto enum_cs = box_parser_helper.get_next<enum_cs_field_type>();
    auto colour_specification_box =
        std::make_unique<ColourSpecificationBox>(ColourSpecificationContents(meth, prec, approx, EnumCS{enum_cs}));

    return colour_specification_box;
  }

  //! \todo implement ICCProfile parsing (needs a ICC Profile implementation available)
  if (meth == 2) {
    // throw BoxParsersExceptions::ColourSpecificationBoxExceptions::ICCProfileIsNotSupportedYetException();
  // 	auto icc_profile = BoxParserRegistry::get_instance().parse<ICCProfile>(box_parser_helper.get_remaining_stream());
  // 	auto colour_specification_box =
  //       std::make_unique<ColourSpecificationBox>(ColourSpecificationContents(meth, prec, approx, icc_profile));

  //   return colour_specification_box;
  }

  //throw (the only options are 1 and 2 for meth)
  return nullptr;
}
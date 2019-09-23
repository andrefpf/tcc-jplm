/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     BoxParserRegistry.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */


#ifndef JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__
#define JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__

#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include "Lib/Common/Boxes/Box.h"
#include "Lib/Common/Boxes/Parsers/ColourSpecificationBoxParser.h"
#include "Lib/Common/Boxes/Parsers/ContiguousCodestreamBoxParser.h"
#include "Lib/Part1/Decoder/Boxes/FileTypeBoxParser.h"
//! [Include the parser header]
#include "Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h"
//! [Include the parser header]
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/LightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/ProfileAndLevelBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserRegistry {
  using ParsedBox = std::unique_ptr<Box>;
  using ParsingFunction = std::function<ParsedBox(BoxParserHelperBase&)>;

 private:
  void register_known_parsers();


  BoxParserRegistry() {
    register_known_parsers();
  }


  ~BoxParserRegistry() = default;

 public:
  static BoxParserRegistry& get_instance();


  static std::map<t_box_id_type, ParsingFunction>& get_ref_to_parser_map();


  ParsedBox parse(ManagedStream&& managed_stream) const;


  template<class ParsingBox, bool required = true>
  std::unique_ptr<ParsingBox> parse(ManagedStream&& managed_stream) const {
    // std::cout << "ManagedStream is at: " << managed_stream.tell() << std::endl;
    // std::cout << "ManagedStream is limited to: " << managed_stream.get_length() << std::endl;
    // std::cout << typeid(ParsingBox).name() << std::endl;
    auto box_parser_helper =
        BoxParserHelper<ParsingBox, required>(managed_stream);
    // std::cout << "created helper here <>" << std::endl;
    if constexpr (!required) {
      if (!box_parser_helper.is_a_box_with_id(ParsingBox::id)) {
        //this means that the current box being read is not of the type ParsingBox
        //therefore, it can rewind the stream to allow for a next box to read again the l and t boxes
        managed_stream.rewind();
        return nullptr;
      }
    }
    auto parsed_box = parse(box_parser_helper);
    if (parsed_box) {  //i.e., the unique_ptr is not null
      return std::unique_ptr<ParsingBox>(
          static_cast<ParsingBox*>(parsed_box.release()));
    }
    return nullptr;
  }


  ParsedBox parse(BoxParserHelperBase& box_parser_helper) const;


  template<class ParserClass>
  void register_parser() {
    auto& map = BoxParserRegistry::get_ref_to_parser_map();
    const auto id = ParserClass::ParsingBox::id;
    auto parsing_function = ParserClass::parse;
    // std::cout << "Registered 0x" << std::hex << std::setfill('0')
    //           << std::setw(8) << id << std::endl;
    map[id] = parsing_function;
  }


  


};

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__ */


/*! \page parser_info How to create a new box parser
  \tableofcontents
  This page will introduce briefly how to implement a new box parser.
  \section sec Creating a new box parser
  Let us assume that we are to implement a parser for the Jpeg Pleno Signature Box.
  There will be a JpegPlenoSignatureBox class defined. 
  In our case, it will be at Lib/Part1/Common/Boxes/JpegPlenoSignatureBox.h

  The parser class JpegPlenoSignatureBoxParser will be defined in the header Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h 
  and its implementation will be at Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.cpp .

  To create a new parser class, the class <b>MUST</b> have two mandatory items:
   -# The name of the box to be parsed.
    - \snippet Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h ParsingBox definition with type of the box to be parsed
   -# The parser method with a standard signature.
    - \snippet Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h Parse function definition
    
  \subsection subsection1 Parser implementation
  
  To implement the parser, you have access to a BoxParserHelperBase. Such helper has the ability to provide access to the data in the stream.
  Also, you have access to the BoxParserRegistry, and thus you can parse another box from the stream (in the case of a super box being parsed).
  
  Let us first see how to implement a parser for a common box, like the FileTypeBox. 
  To obtain values for fields of a box, we can call the get_next method of BoxParserHelperBase, specifying the type of the data we want to get.
  After obtaining all the required data from a box, a box containing that data must be instanciated and returned. See the example below: 

  \snippet Lib/Part1/Decoder/Boxes/FileTypeBoxParser.cpp Parsing a file type box 

  Notice that the size of the data contained in the box is obtained using box_parser_helper.get_data_lenght().
  
  Now, let us see how to get a box, in the case it is needed within a superbox.

  \snippet Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldHeaderBoxParser.cpp Parsing a box within a super box

  Notice that the parsing call used to obtain light_field_header_box and bits_per_component_box are different. 
  The fisrt is a mandatory box. Thus, if the parser does not find such box in the stream in the current position, it throws an exception. 
  On the other hand, the second box is not mandatory and thus the parse method must be called using false as the last template parameter. 


  \subsection subsection2 Registering the parser
  Include the parser header in the BoxParserRegistry header: 

  \snippet Lib/Common/Boxes/Parsers/BoxParserRegistry.h Include the parser header

  Finally, to allow this parser to be used during decoding, it is necessary to make it visible.
  To do so, it is only necessary to include a line in BoxParserRegistry::register_known_parsers() implementation: 
  \snippet Lib/Common/Boxes/Parsers/BoxParserRegistry.cpp Registering the parser

  By doing so, the JpegPlenoSignatureBoxParser parse method will be used whenever a JpegPlenoSignatureBox is found in the bitstream;
*/

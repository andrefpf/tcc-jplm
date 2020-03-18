/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     JPLFileParser.h
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-05
 */
#ifndef JPLM_LIB_PART1_JPLFILEPARSER_H
#define JPLM_LIB_PART1_JPLFILEPARSER_H


#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Utils/Stream/ManagedStream.h"


class JPLFileParser {
 protected:
  const BoxParserRegistry& parser = BoxParserRegistry::get_instance();
  const std::string filename;
  const uint64_t file_size;
  std::ifstream if_stream;
  ManagedStream managed_stream;
  std::unique_ptr<JpegPlenoSignatureBox> temp_signature;
  std::unique_ptr<FileTypeBox> temp_file_type;
  std::map<uint32_t, std::vector<std::unique_ptr<Box>>> temp_decoded_boxes;


  std::unique_ptr<FileTypeBox> decode_boxes_until_a_file_type_box_is_found();

  uint64_t decode_boxes();


  std::unique_ptr<JpegPlenoSignatureBox> get_signature_box();


  std::unique_ptr<FileTypeBox> get_file_type_box();

 public:
  JPLFileParser(const std::string& filename);


  virtual ~JPLFileParser();
};


#endif  //JPLM_LIB_PART1_JPLFILEPARSER_H

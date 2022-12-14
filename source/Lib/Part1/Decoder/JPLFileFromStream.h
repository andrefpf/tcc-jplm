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

/** \file     JPLFileFromStream.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__
#define JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <magic_enum.hpp>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part1/Common/JPLFile.h"
#include "Lib/Part1/Decoder/CommonExceptions.h"
#include "Lib/Part1/Decoder/JPLFileParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class JPLFileFromStream : public JPLFileParser, public JPLFile {
 private:
  struct ConstrainedBoxIndex {
    std::optional<uint64_t> thumbnail_box_position = std::nullopt;
    std::optional<uint64_t> xml_box_with_catalog_position = std::nullopt;
    uint64_t file_type_box_position = 0;
    uint64_t fist_plenoptic_box_position = std::numeric_limits<uint64_t>::max();
    uint64_t last_plenoptic_box_position = 0;
    uint64_t number_of_plenoptic_elements = 0;
  };

  ConstrainedBoxIndex get_constrained_box_index() const;

 protected:
  uint64_t number_of_decoded_boxes = 0;


  /**
   * @brief      Checks if the parsed codestream is well formed according to JPEG Pleno Part 1.
   */
  void check_boxes_constraints();


  /**
   * @brief      Populates the lightfield codestreams list within the JPLFile
   */
  void populate_light_field_codestreams();


  /**
   * @brief      Populates the point clouds codestreams list within the JPLFile
   * 
   * \todo Implement for Point Cloud Boxes...
   */
  void populate_point_cloud_codestreams();


  /**
   * @brief      Populates the hologram codestreams list within the JPLFile
   */
  void populate_hologram_codestreams();


  /**
   * @brief      Populates the list of codestreams within the JPLFile with Lightfield, Point Clouds or Hologram codestream boxes
   */
  void populate_codestreams_list();


  /**
   * @brief      Populates the files of the JPLFile with the ones readed from the stream
   */
  void populate_jpl_fields();

 public:
  /**
   * @brief      Constructs a new instance.
   *
   * @param[in]  filename  The filename
   */
  JPLFileFromStream(const std::string& filename);


  /**
   * @brief      Gets the number of decoded boxes.
   *
   * @return     The number of decoded boxes.
   */
  uint64_t get_number_of_decoded_boxes();


  /**
   * @brief      Destroys the object.
   */
  virtual ~JPLFileFromStream() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_JPLFILEFROMSTREAM_H__ */
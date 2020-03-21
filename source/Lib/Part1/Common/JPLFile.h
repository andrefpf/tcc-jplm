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

/** \file     JPLFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-25
 */


#ifndef JPLM_LIB_PART1_COMMON_JPLFILE_H__
#define JPLM_LIB_PART1_COMMON_JPLFILE_H__

#include "Lib/Common/Boxes/Generic/IntellectualPropertyBox.h"
#include "Lib/Common/Boxes/Generic/UUIDBox.h"
#include "Lib/Common/Boxes/Generic/UUIDInfoBox.h"
#include "Lib/Common/Boxes/Generic/UUIDListBox.h"
#include "Lib/Common/Boxes/Generic/XMLBox.h"
#include "Lib/Part1/Common/Boxes/FileTypeBox.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoCodestreamBox.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoFileTypeContents.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoSignatureBox.h"
#include "Lib/Part1/Common/Boxes/JpegPlenoThumbnailBox.h"


class JPLFile {
 protected:
  std::unique_ptr<JpegPlenoSignatureBox> jpeg_pleno_signature_box;
  std::unique_ptr<FileTypeBox> file_type_box;  //this is the FileTypeBox
  std::unique_ptr<XMLBox> xml_box_with_catalog;
  std::unique_ptr<JpegPlenoThumbnailBox> jpeg_pleno_thumbnail_box;
  std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>
      jpeg_pleno_codestreams;  //optional
  std::optional<IntellectualPropertyBox> ipr_box;
  // std::optional<std::vector<XMLBox>> xml_boxes; //boxes??
  std::optional<std::vector<std::unique_ptr<UUIDBox>>> uuid_boxes;
  std::optional<std::vector<std::unique_ptr<UUIDInfoBox>>> uuid_info_boxes;

 public:
  JPLFile(const FileTypeBox& file_type_box);


  JPLFile(const JpegPlenoSignatureBox& jpeg_pleno_signature_box =
              JpegPlenoSignatureBox(),
      const FileTypeBox& file_type_box = FileTypeBox(
          JpegPlenoFileTypeContents()));


  JPLFile(JpegPlenoSignatureBox&& jpeg_pleno_signature_box,
      FileTypeBox&& file_type_box);


  JPLFile(std::unique_ptr<JpegPlenoSignatureBox>&& jpeg_pleno_signature_box,
      std::unique_ptr<FileTypeBox>&& file_type_box);


  virtual ~JPLFile() = default;


  JPLFile& add_codestream_box(
      std::unique_ptr<JpegPlenoCodestreamBox>&& codestream_box);


  JPLFile& add_thumbnail_box(const JpegPlenoThumbnailBox& thumbnail_box);


  bool has_thumbnail() const noexcept;


  bool has_codestream() const noexcept;


  std::size_t number_of_codestreams() const noexcept;


  std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
  get_reference_to_codestreams() noexcept;


  const std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
  get_reference_to_codestreams() const noexcept;


  JpegPlenoSignatureBox get_jpeg_pleno_signature_box() const noexcept;


  FileTypeBox get_file_type_box() const noexcept;


  std::size_t size() const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const JPLFile& jpl_file);
};


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_JPLFILE_H__ */
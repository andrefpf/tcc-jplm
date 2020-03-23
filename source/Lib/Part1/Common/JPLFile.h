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
#include "Lib/Part1/Common/CatalogGenerator.h"

class JPLFile {
 protected:
  std::unique_ptr<JpegPlenoSignatureBox> jpeg_pleno_signature_box;
  std::unique_ptr<FileTypeBox> file_type_box;  //this is the FileTypeBox
  //the xml_box_with_catalog is mutable as it need to be updated before any operation involving it
  mutable std::unique_ptr<XMLBox> xml_box_with_catalog;
  std::unique_ptr<JpegPlenoThumbnailBox> jpeg_pleno_thumbnail_box;
  std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>
      jpeg_pleno_codestreams;  //optional
  std::optional<IntellectualPropertyBox> ipr_box;
  std::optional<std::vector<std::unique_ptr<XMLBox>>> xml_boxes;
  std::optional<std::vector<std::unique_ptr<UUIDBox>>> uuid_boxes;
  std::optional<std::vector<std::unique_ptr<UUIDInfoBox>>> uuid_info_boxes;

 public:
  /**
   * @brief      Constructs a new instance of JPLFile.
   *
   * @param[in]  file_type_box  The file type box
   */
  JPLFile(const FileTypeBox& file_type_box);


  /**
   * @brief      Constructs a new instance of JPLFile.
   *
   * @param[in]  jpeg_pleno_signature_box  The jpeg pleno signature box
   * @param[in]  file_type_box             The file type box
   */
  JPLFile(const JpegPlenoSignatureBox& jpeg_pleno_signature_box =
              JpegPlenoSignatureBox(),
      const FileTypeBox& file_type_box = FileTypeBox(
          JpegPlenoFileTypeContents()));


  /**
   * @brief      Constructs a new instance of JPLFile.
   *
   * @param      jpeg_pleno_signature_box  The jpeg pleno signature box
   * @param      file_type_box             The file type box
   */
  JPLFile(JpegPlenoSignatureBox&& jpeg_pleno_signature_box,
      FileTypeBox&& file_type_box);


  /**
   * @brief      Constructs a new instance of JPLFile.
   *
   * @param      jpeg_pleno_signature_box  The jpeg pleno signature box
   * @param      file_type_box             The file type box
   */
  JPLFile(std::unique_ptr<JpegPlenoSignatureBox>&& jpeg_pleno_signature_box,
      std::unique_ptr<FileTypeBox>&& file_type_box);


  /**
   * @brief      Destroys the object.
   */
  virtual ~JPLFile() = default;


  /**
   * @brief      Adds a codestream box to this JPLFile.
   *
   * @param      codestream_box  The codestream box
   *
   * @return     A reference to this object
   */
  JPLFile& add_codestream_box(
      std::unique_ptr<JpegPlenoCodestreamBox>&& codestream_box);


  /**
   * @brief      Adds a thumbnail box to this JPLFile.
   *
   * @param[in]  thumbnail_box  The thumbnail box
   *
   * @return     A reference to this object
   */
  JPLFile& add_thumbnail_box(const JpegPlenoThumbnailBox& thumbnail_box);


  /**
   * @brief      Determines if the file contains a thumbnail box.
   *
   * @return     True if thumbnail, False otherwise.
   */
  bool has_thumbnail() const noexcept;


  /**
   * @brief      Determines if the file contains at least one codestream.
   *
   * @return     True if codestream, False otherwise.
   */
  bool has_codestream() const noexcept;


  /**
   * @brief      Gets the number of codestreams in the JPLFile
   *
   * @return     The number of codestreams
   */
  std::size_t number_of_codestreams() const noexcept;


  /**
   * @brief      Gets the reference to codestreams.
   *
   * @return     The reference to codestreams.
   */
  std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
  get_reference_to_codestreams() noexcept;


  /**
   * @brief      Gets the reference to codestreams.
   *
   * @return     The reference to codestreams.
   */
  const std::vector<std::unique_ptr<JpegPlenoCodestreamBox>>&
  get_reference_to_codestreams() const noexcept;


  /**
   * @brief      Gets the jpeg pleno signature box.
   *
   * @return     The jpeg pleno signature box.
   */
  JpegPlenoSignatureBox get_jpeg_pleno_signature_box() const noexcept;


  /**
   * @brief      Gets the file type box.
   *
   * @return     The file type box.
   */
  FileTypeBox get_file_type_box() const noexcept;


  /**
   * @brief      Gets the size (in bytes) of the JPLFile
   *
   * @return     Size in bytes
   */
  std::size_t size() const noexcept;


  void enable_catalog() const noexcept;


  friend std::ostream& operator<<(std::ostream& os, const JPLFile& jpl_file);
};


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_JPLFILE_H__ */
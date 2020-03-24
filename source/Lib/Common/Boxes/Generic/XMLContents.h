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

/** \file     XMLContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-20
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_XML_CONTENTS_H
#define JPLM_LIB_COMMON_BOXES_GENERIC_XML_CONTENTS_H

#include <algorithm>  //for std::transform
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"


class XMLContents : public InMemoryDBox {
 protected:
  std::string contents = "";

 public:
  /**
   * @brief      Constructs a new instance having empty content.
   */
  XMLContents() = default;


  /**
   * @brief      Constructs a new instance by copying the contents string.
   *
   * @param[in]  contents  The contents
   */
  XMLContents(const std::string& contents) : contents(contents) {
  }


  /**
   * @brief      Constructs a new instance by obtaining the ownership of the contents string.
   *
   * @param      contents  The contents
   */
  XMLContents(std::string&& contents) : contents(std::move(contents)) {
  }


  /**
   * @brief      Creates a new instance of the object with same properties than original.
   *
   * @return     Copy of this object.
   */
  XMLContents* clone() const override;


  /**
   * @brief      Destroys the object.
   */
  virtual ~XMLContents() = default;


  /**
   * @brief      Gets the size of the XML contents (in bytes)
   *
   * @return     Size (in bytes)
   */
  uint64_t size() const noexcept override;


  /**
   * @brief      Determines whether the specified other is equal.
   *
   * @param[in]  other  The other
   *
   * @return     True if the specified other is equal, False otherwise.
   */
  bool is_equal(const DBox& other) const override;


  /**
   * @brief      Equality operator.
   *
   * @param[in]  other  The other
   *
   * @return     The result of the equality
   */
  bool operator==(const XMLContents& other) const;


  /**
   * @brief      Inequality operator.
   *
   * @param[in]  other  The other
   *
   * @return     The result of the inequality
   */
  bool operator!=(const XMLContents& other) const;


  /**
   * @brief      Gets the string with contents.
   *             
   *             The XML contains basically a string with a XML code.
   *             \todo Validate the XML contained in the string
   * @return     The string with contents.
   */
  const std::string& get_string_with_contents() const noexcept;


  /**
   * @brief      Gets the bytes.
   *
   * @return     The bytes.
   */
  virtual std::vector<std::byte> get_bytes() const noexcept override;


  /**
   * @brief      Sets the contents by copy.
   *
   * @param[in]  new_content  The new content
   */
  void set_contents(const std::string& new_content);


  /**
   * @brief      Sets the contents by assuming the ownership (move).
   *
   * @param      new_content  The new content
   */
  void set_contents(std::string&& new_content);
};

#endif  // JPLM_LIB_COMMON_BOXES_GENERIC_XML_CONTENTS_H
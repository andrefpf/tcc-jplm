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

/** \file     InMemoryDBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-05
 */


#ifndef JPLM_LIB_COMMON_GENERIC_INMEMORYDBOX_H__
#define JPLM_LIB_COMMON_GENERIC_INMEMORYDBOX_H__


#include <iostream>
#include "Lib/Common/Boxes/DBox.h"

class InMemoryDBox : public DBox {
 public:
  InMemoryDBox() = default;


  virtual ~InMemoryDBox() = default;

  /**
   * \brief      Gets a ordered vector with the bytes to be written into the stream.
   *
   * \return     The vector of bytes.
   */
  virtual std::vector<std::byte> get_bytes() const {
    throw std::runtime_error(
        "Not implemented yet (get_bytes).");
  }


  /**
   * \brief      Writes the bytes of this DBox to the stream.
   *
   * \param      stream  The stream
   *
   * \return     A reference to the same stream that whas passed as parameter
   */
  std::ostream& write_to(std::ostream& stream) const final;
};


std::ostream& operator<<(
    std::ostream& stream, const InMemoryDBox& d_box);


#endif /* end of include guard: JPLM_LIB_COMMON_GENERIC_INMEMORYDBOX_H__ */
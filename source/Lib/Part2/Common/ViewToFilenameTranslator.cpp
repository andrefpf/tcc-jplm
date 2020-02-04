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

/** \file     ViewToFilenameTranslator.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-04
 */

#include "ViewToFilenameTranslator.h"


/**
 * @brief      Checks if a position is within the valid interval defined by the file name (0 to 999)
 *
 * @param[in]  position  The position (t, s)
 */
void ViewToFilenameTranslator::check_for_overflow(
    const std::pair<std::size_t, std::size_t>& position) const {
  const auto& [t, s] = position;
  if (t > 999) {
    throw ViewToFilenameTranslatorExceptions::Char3OverflowException();
  }
  if (s > 999) {
    throw ViewToFilenameTranslatorExceptions::Char3OverflowException();
  }
}


/**
 * @brief      Gets the name from position in the format S_T with three characteres for each coordinate.
 *
 * @param[in]  position  The position, a pair (T, S)
 *
 * @return     The name from position, "S_T".
 */
std::string ViewToFilenameTranslator::view_position_to_filename(
    const std::pair<std::size_t, std::size_t>& position) const {
  check_for_overflow(position);
  const auto& [t, s] = position;
  std::ostringstream string_stream;
  string_stream << std::setw(3) << std::setfill('0') << s << '_' << std::setw(3)
                << std::setfill('0') << t << file_extension;
  return string_stream.str();
}
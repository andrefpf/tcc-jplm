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

/** \file     BoxParserHelper.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-15
 */

#ifndef JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPER_H
#define JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPER_H

#include "Lib/Utils/Stream/BinaryTools.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "Lib/Common/Boxes/Parsers/BoxParserHelperBase.h"
#include "Lib/Common/Boxes/Parsers/CommonExceptions.h"


template<class ParsingBox, bool required=true>
class BoxParserHelper : public BoxParserHelperBase {
 protected:
  void check_t_box_value() {
    if(!this->is_a_box_with_id(ParsingBox::id)) {
      throw BoxParserExceptions::WrongTBoxValueException(
          this->t_box_value_in_stream, ParsingBox::id);
    }
  }


 public:
  BoxParserHelper(ManagedStream& stream) : BoxParserHelperBase(stream) {
    if constexpr (required) {
      check_t_box_value();
    }
  }


  virtual ~BoxParserHelper() = default;
};


#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPER_H__ */
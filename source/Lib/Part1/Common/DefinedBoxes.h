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


/** \file     DefinedBoxesTypes.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */

#ifndef JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__
#define JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__

#include "source/Lib/Common/Boxes/Box.h"
#include <type_traits>

enum class DefinedBoxesTypes : uint32_t {
  JPEGPlenoSignatureBoxType =   0x6A706C20,
  FileTypeBoxType =             0x66747970,
  JPEGPlenoThumbnailBoxType =   0x6A707468,
  JPEGPlenoHeaderBoxType =      0x6A706C68,
  JPEGPlenoLightFieldBoxType =  0x6A706C66,
  JPEGPlenoPointCloudBoxType =  0x6A707063,
  JPEGPlenoHologramBoxType =    0x6A70686F,
  ImageHeaderBoxType =          0x69686472, //this is from jpeg2000 part 2 (extensions)  
  BitsPerComponentBoxType =     0x62706363, //this is from jpeg2000 part 1  
  ColourSpecificationBoxType =  0x636F6C72, //this is from jpeg2000 part 1  
  ChannelDefinitionBoxType =    0x63646566, //this is from jpeg2000 part 1  
  ContiguousCodestreamBoxType = 0x6A703263, //this is from jpeg2000 part 1  
  IntellectualPropertyBoxType = 0x6A703269, //this is from jpeg2000 part 1 
  UUIDBoxType =                 0x75756964, //this is from jpeg2000 part 1 
  UUIDInfoBoxType =             0x75696E66, //this is from jpeg2000 part 1 
  UUIDListBoxType =             0x756C7374, //this is from jpeg2000 part 1 
  DataEntryURLBoxType =         0x75726C20, //this is from jpeg2000 part 1 
};


typedef std::underlying_type<DefinedBoxesTypes>::type DefinedBoxesTypesUnderlyingType;


namespace DefinedBoxes {

  template<DefinedBoxesTypes type>
  constexpr DefinedBoxesTypesUnderlyingType get_value() {
    return static_cast<DefinedBoxesTypesUnderlyingType>(type);
  }

}


#endif /* end of include guard: JPLM_LIB_PART1_COMMON_DEFINEDBOXES_H__ */
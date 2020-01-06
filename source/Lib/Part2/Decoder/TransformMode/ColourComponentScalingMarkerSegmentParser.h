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

/** \file     ColourComponentScalingMarkerSegmentParser.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-06
 */

#ifndef COLOURCOMPONENTSCALINGMARKERSEGMENTPARSER_H__
#define COLOURCOMPONENTSCALINGMARKERSEGMENTPARSER_H__

#include "Lib/Part2/Common/TransformMode/ColourComponentScalingMarkerSegment.h"
#include "Lib/Part2/Decoder/TransformMode/MarkerSegmentHelper.h"

namespace ColourComponentScalingMarkerSegmentParser {
ColourComponentScalingMarkerSegment get_codestream_pointer_set_marker_segment(
    const ContiguousCodestreamCode& codestream_code) {
  auto SLscc_from_codestream_code =
      MarkerSegmentHelper::get_next<uint8_t>(codestream_code);
  if (SLscc_from_codestream_code != ColourComponentScalingMarkerSegment::SLscc) {
    //throw error
  }

  auto Lscc_from_codestream_code =
      MarkerSegmentHelper::get_next<uint16_t>(codestream_code);
  

  if ((Lscc_from_codestream_code != 6) && (Lscc_from_codestream_code != 8)) {
    //throw error
  }

  auto more_than_256_colour_components = false;

  if(Lscc_from_codestream_code == 8) {
  	more_than_256_colour_components = true;
  }


  std::size_t colour_component_index = 0; //temporary initialization


  if(more_than_256_colour_components) {
  	colour_component_index = MarkerSegmentHelper::get_next<uint16_t>(codestream_code);
  } else {
	colour_component_index = MarkerSegmentHelper::get_next<uint8_t>(codestream_code);
  }


  auto Spscc = MarkerSegmentHelper::get_next<uint16_t>(codestream_code);


  return ColourComponentScalingMarkerSegment(more_than_256_colour_components, colour_component_index, Spscc);

}

}


#endif /* end of include guard: COLOURCOMPONENTSCALINGMARKERSEGMENTPARSER_H__ */
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

/** \file     LightFieldContigurationMarkerSegmentParser.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-10-16
 */

#ifndef LIGHTFIELDCONTIGURATIONMARKERSEGMENTPARSER_H__
#define LIGHTFIELDCONTIGURATIONMARKERSEGMENTPARSER_H__

#include "Lib/Part2/Decoder/TransformMode/MarkerSegmentHelper.h"
#include "Lib/Part2/Common/TransformMode/LightFieldConfigurationMarkerSegment.h"


namespace LightFieldContigurationMarkerSegmentParser {
	LightFieldConfigurationMarkerSegment get_light_field_configuration_marker_segment(const ContiguousCodestreamCode& codestream_code) {
		[[maybe_unused]] auto SLlfc_from_codestream_code = codestream_code.get_next_byte();
		std::cout << "SLlfc_from_codestream_code: " << std::to_integer<int>(SLlfc_from_codestream_code) << std::endl;
	  	[[maybe_unused]] auto Llfc_from_codestream_code = MarkerSegmentHelper::get_next<uint16_t>(codestream_code);
	  	std::cout << "Llfc_from_codestream_code: " << Llfc_from_codestream_code << std::endl;
	  	std::cout << "I'm here" << std::endl;
	  	auto ROWS = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "ROWS: " << ROWS << std::endl;
	  	auto COLUMNS = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "COLUMNS: " << COLUMNS << std::endl;
	  	auto HEIGHT = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "HEIGHT: " << HEIGHT << std::endl;
	  	auto WIDTH = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "WIDTH: " << WIDTH << std::endl;
	  	auto lightfield_dimension = LightfieldDimension<uint32_t>(ROWS, COLUMNS, HEIGHT, WIDTH);
	  	auto NC = MarkerSegmentHelper::get_next<uint16_t>(codestream_code);
	  	std::cout << "NC: " << NC << std::endl;
	  	auto Ssiz_vector = std::vector<ComponentSsizParameter>();
	  	Ssiz_vector.reserve(NC);
	  	for(auto i=decltype(NC){0};i<NC;++i) {
	  		std::cout << "i: " << i << "(" << NC << ")" << std::endl;
	  		Ssiz_vector.emplace_back(ComponentSsizParameter(MarkerSegmentHelper::get_next<uint8_t>(codestream_code)));
	  	}
	  	std::cout << "out of loop" << std::endl;
	  	[[maybe_unused]] auto N_4D = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "N_4D: " << N_4D << std::endl;
	  	auto BLOCK_SIZE_t = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "BLOCK_SIZE_t: " << BLOCK_SIZE_t << std::endl;
	  	auto BLOCK_SIZE_s = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "BLOCK_SIZE_s: " << BLOCK_SIZE_s << std::endl;
	  	auto BLOCK_SIZE_v = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "BLOCK_SIZE_v: " << BLOCK_SIZE_v << std::endl;
	  	auto BLOCK_SIZE_u = MarkerSegmentHelper::get_next<uint32_t>(codestream_code);
	  	std::cout << "BLOCK_SIZE_u: " << BLOCK_SIZE_u << std::endl;
	  	auto block_dimension = LightfieldDimension<uint32_t>(BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u);
	  	auto max_bitplane = std::vector<uint8_t>();
	  	max_bitplane.reserve(NC);
	  	for(auto i=decltype(NC){0};i<NC;++i) {
	  		max_bitplane.emplace_back(MarkerSegmentHelper::get_next<uint8_t>(codestream_code));
	  	}
	  	auto TRNC = MarkerSegmentHelper::get_next<uint8_t>(codestream_code);

		return LightFieldConfigurationMarkerSegment(lightfield_dimension, Ssiz_vector, block_dimension, max_bitplane, TRNC);
	}

}

#endif /* end of include guard: LIGHTFIELDCONTIGURATIONMARKERSEGMENTPARSER_H__ */
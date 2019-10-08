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

/** \file     MuleEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef MULEENCODER_H__
#define MULEENCODER_H__


#include "App/TemporaryCommonTransformMode/MuleCodec.h"

#include "Lib/Part2/Encoder/TransformMode/Hierarchical4DEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/TransformPartition.h"
#include <memory>


class MuleEncoder : public MuleCodec
{
private:
	Hierarchical4DEncoder hierarchical_4d_encoder;
	TransformPartition tp;

    // void open_raw_lightfield();
public:
	MuleEncoder(ParameterHandler handler);
	~MuleEncoder() = default;
	
	void setup_hierarchical_4d_encoder();
	void write_initial_data_to_encoded_file();

	std::unique_ptr<ContiguousCodestreamCode> get_contigous_codestream_code();
	
	virtual void run_for_block_4d(const uint32_t channel,
      const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) override;
};

#endif /* end of include guard: MULEENCODER_H__ */
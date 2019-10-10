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

/** \file     Hierarchical4DCodec.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-04-11
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_HIERARCHICAL4DCODEC_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_HIERARCHICAL4DCODEC_H__

#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "Lib/Part2/Common/TransformMode/ProbabilityModel.h"
#include "Lib/Part2/Common/TransformMode/ABACCodec.h"
#include <vector>
#include <array>

enum class PartitionFlag {transform=0, spatialSplit=1, viewSplit=2}; //Table C.30
enum class HexadecaTreeFlag {lowerBitPlane=0, splitBlock=1, zeroBlock=2}; //Table C.44

class Hierarchical4DCodec
{
protected:
	static constexpr auto MINIMUM_BITPLANE_PRECISION=5;
	static constexpr auto BITPLANE_BYPASS=-1;
	static constexpr auto BITPLANE_BYPASS_FLAGS=-1;
	static constexpr auto SYMBOL_PROBABILITY_MODEL_INDEX=1;
	static constexpr auto SEGMENTATION_PROB_MODEL_INDEX=32;
	static constexpr auto number_of_probability_models=161;
    std::array<ProbabilityModel, number_of_probability_models> probability_models;
    int mPGMScale;
    uint8_t superior_bit_plane = 30;
    uint8_t inferior_bit_plane = 0;
    uint32_t mTransformLength_t, mTransformLength_s, mTransformLength_v, mTransformLength_u;
public:
    auto get_number_of_elements_in_transform() const {
        return mTransformLength_t*mTransformLength_s*mTransformLength_v*mTransformLength_u;
    }

    void set_level_shift(int value);
    int get_level_shift() const;

    void set_inferior_bit_plane(uint8_t value);
    uint8_t get_inferior_bit_plane() const;

    void set_superior_bit_plane(uint8_t value);
    uint8_t get_superior_bit_plane() const;

    void set_lightfield_dimension(const LightfieldDimension<uint32_t>& dimension);
    void set_transform_dimension(const LightfieldDimension<uint32_t>& dimension);
    void set_minimum_transform_dimension(const LightfieldDimension<uint32_t>& dimension);


    int mSegmentationFlagProbabilityModelIndex = SEGMENTATION_PROB_MODEL_INDEX;
    int mSymbolProbabilityModelIndex = SYMBOL_PROBABILITY_MODEL_INDEX;
    Block4D mSubbandLF;

    //LightfieldDimension<uint32_t> transform_dimension;
    uint32_t mMinimumTransformLength_t, mMinimumTransformLength_s, mMinimumTransformLength_v, mMinimumTransformLength_u;
    //LightfieldDimension<uint32_t> minimum_transform_dimension;
    uint32_t mNumberOfVerticalViews, mNumberOfHorizontalViews;
    uint32_t mNumberOfViewLines, mNumberOfViewColumns;
    //LightfieldDimension<uint32_t> lightfield_dimension;
    Hierarchical4DCodec() = default;
    virtual ~Hierarchical4DCodec() = default;
    virtual void reset_probability_models();


};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_HIERARCHICAL4DCODEC_H__ */
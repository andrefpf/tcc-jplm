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

/** \file     ThreeChannelBlock4DHolder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef THREECHANNELBLOCK4DHOLDER_H__
#define THREECHANNELBLOCK4DHOLDER_H__

#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "Lib/Part2/Common/TransformMode/LightField.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "assert.h"

class YCbCrBlock4DHolder;
class YCoCgBlock4DHolder;
class RGBBlock4DHolder;

class ThreeChannelBlock4DHolder
{
protected:
	int scale;
public:
	std::tuple<Block4D&, Block4D&, Block4D&> channels;
	std::array<Block4D*, 3> channels_ptr_array;
	ThreeChannelBlock4DHolder(Block4D& channel_0, Block4D& channel_1, Block4D& channel_2, int scale):
	scale(scale), channels(channel_0, channel_1, channel_2), channels_ptr_array{&channel_0, &channel_1, &channel_2} {};
	~ThreeChannelBlock4DHolder() = default;

	void copy_to(ThreeChannelBlock4DHolder& dest) const;
	void shift_uv_planes(int shift, int position_t, int position_s) const;
	void clip(int min, int max) const;
	void set_blocks_to_zero() const;
	void add_constant_to_pels(int value) const;
	void set_block_dimensions(int length_t, int length_s, int length_v, int length_u) const;
	void resize_blocks(int length_t, int length_s, int length_v, int length_u) const;
	void convert_to(ThreeChannelBlock4DHolder& dest_color_space) const;
	const std::array<Block4D*, 3>& as_ptr_array() const {return channels_ptr_array;};

	virtual void convert_to(RGBBlock4DHolder& dest_color_space) const = 0;
	virtual void convert_to(YCbCrBlock4DHolder& dest_color_space) const = 0;
	virtual void convert_to(YCoCgBlock4DHolder& dest_color_space) const = 0;

	virtual ColorSpaces::ColorSpace get_my_type() const = 0;

	void get_data_from_lightfield(LightField& lightfield, int verticalView, int horizontalView, int viewLine, int viewColumn);
	void set_data_into_lightfield(LightField& lightfield, int verticalView, int horizontalView, int viewLine, int viewColumn);
};

class RGBBlock4DHolder : public ThreeChannelBlock4DHolder
{
public:
	RGBBlock4DHolder(Block4D& r_block, Block4D& g_block, Block4D& b_block, int scale) : 
	ThreeChannelBlock4DHolder(r_block, g_block, b_block, scale) {};
	~RGBBlock4DHolder() = default;

	void convert_to(RGBBlock4DHolder& dest_color_space) const override {copy_to(dest_color_space);};
	void convert_to(YCbCrBlock4DHolder& dest_color_space) const override;
	void convert_to(YCoCgBlock4DHolder& dest_color_space) const override;

	ColorSpaces::ColorSpace get_my_type() const override {return ColorSpaces::ColorSpace::RGB;};
};

class YCbCrBlock4DHolder : public ThreeChannelBlock4DHolder
{
public:
	YCbCrBlock4DHolder(Block4D& y_block, Block4D& cb_block, Block4D& cr_block, int scale) : 
	ThreeChannelBlock4DHolder(y_block, cb_block, cr_block, scale) {};
	~YCbCrBlock4DHolder() = default;

	void convert_to(RGBBlock4DHolder& dest_color_space) const override;
	void convert_to(YCbCrBlock4DHolder& dest_color_space) const override {copy_to(dest_color_space);};
	void convert_to(YCoCgBlock4DHolder& dest_color_space) const override;

	ColorSpaces::ColorSpace get_my_type() const override {return ColorSpaces::ColorSpace::BT601;};
};

class YCoCgBlock4DHolder : public ThreeChannelBlock4DHolder
{
public:
	YCoCgBlock4DHolder(Block4D& y_block, Block4D& co_block, Block4D& cg_block, int scale) : 
	ThreeChannelBlock4DHolder(y_block, co_block, cg_block, scale) {};
	~YCoCgBlock4DHolder() = default;
	
	void convert_to(RGBBlock4DHolder& dest_color_space) const override;
	void convert_to(YCbCrBlock4DHolder& dest_color_space) const override;
	void convert_to(YCoCgBlock4DHolder& dest_color_space) const override {copy_to(dest_color_space);};

	ColorSpaces::ColorSpace get_my_type() const override {return ColorSpaces::ColorSpace::YCoCg;};
};

#endif /* end of include guard: THREECHANNELBLOCK4DHOLDER_H__ */
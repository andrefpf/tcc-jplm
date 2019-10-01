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

/** \file     MuleEncoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

/*
* @Author: Ismael Seidel
* @Date:   2019-03-26 16:31:36
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-14 16:56:54
*/
#include "MuleEncoder.h"

MuleEncoder::MuleEncoder(ParameterHandler handler) : 
	MuleCodec(handler),
	tp(	parameter_handler.min_transform_length_t,
		parameter_handler.min_transform_length_s,
		parameter_handler.min_transform_length_v,
		parameter_handler.min_transform_length_u) {
        
    auto dimension = LightfieldDimension<std::size_t>(parameter_handler.number_of_vertical_views, 
                parameter_handler.number_of_horizontal_views, 
                3, 3);
    auto config = LightfieldIOConfiguration(
        parameter_handler.decoded_lightfield.string(),
        dimension
        );

    raw_lightfield = std::make_unique<LightFieldTransformMode<>>(config);

	tp.mPartitionData.set_dimension(parameter_handler.transform_length_t,
									parameter_handler.transform_length_s,
									parameter_handler.transform_length_v,
									parameter_handler.transform_length_u);
 	setup_hierarchical_4d_encoder();

 	setup_transform_coefficients(true); 	

	open_encoded_lightfield("wb");
	write_initial_data_to_encoded_file();

    initialize_extension_lenghts();

	fclose(encoded_file_pointer);
    encoded_file_pointer=nullptr;
}

MuleEncoder::~MuleEncoder() {
	hierarchical_4d_encoder.finish();
    std::ofstream ofs(parameter_handler.encoded_lightfield.string(),std::ofstream::binary | std::ofstream::out | std::ofstream::app);
    ofs << hierarchical_4d_encoder.get_ref_to_codestream_code();
}

void MuleEncoder::encode() {

    const auto& [T, S, V, U] = raw_lightfield->get_dimensions<uint32_t>();

    auto BLOCK_SIZE_t = parameter_handler.transform_length_t;
    auto BLOCK_SIZE_s = parameter_handler.transform_length_s;
    auto BLOCK_SIZE_v = parameter_handler.transform_length_v;
    auto BLOCK_SIZE_u = parameter_handler.transform_length_u;

    
    int32_t level_shift = -std::pow(2, raw_lightfield->get_views_bpp())/2; //

    for(auto t = 0; t < T; t += BLOCK_SIZE_t) {
        auto used_size_t = (t + BLOCK_SIZE_t > T)? T%BLOCK_SIZE_t : BLOCK_SIZE_t;
        for(auto s = 0; s < S; s += BLOCK_SIZE_s) {
            auto used_size_s = (s + BLOCK_SIZE_s > S)? S%BLOCK_SIZE_s : BLOCK_SIZE_s;
            for(auto v = 0; v < V; v += BLOCK_SIZE_v) {
                auto used_size_v = (v + BLOCK_SIZE_v > V)? V%BLOCK_SIZE_v : BLOCK_SIZE_v;
                for(auto u = 0; u < U; u += BLOCK_SIZE_u) {
                    auto used_size_u = (u + BLOCK_SIZE_u > U)? U%BLOCK_SIZE_u : BLOCK_SIZE_u;

                    
                    if(parameter_handler.verbose)
                        printf("transforming the 4D block at position (%d %d %d %d)\n", t, s, v, u);
                    
                    //if (parameter_handler.extension_method == SHRINK_TO_FIT) {
                    //    rgb_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
                    //    spectral_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
                    //}
                    

                    auto size = LightfieldDimension<uint32_t>(used_size_t, used_size_s, used_size_v, used_size_u);
                    
                    for(auto color_channel_index=0;color_channel_index<3;++color_channel_index) {
                        auto block_4d = raw_lightfield->get_block_4D_from(color_channel_index, {t, s, v, u}, size);


                        block_4d+=level_shift;
                        // std::cout << block_4d;

                        hierarchical_4d_encoder.reset_probability_models();
                        tp.rd_optimize_transform(block_4d, hierarchical_4d_encoder, parameter_handler.lambda);
                        tp.encode_partition(hierarchical_4d_encoder, parameter_handler.lambda);
                    }
                    // exit(0);
                }
            }
        }
    }
}

void MuleEncoder::setup_hierarchical_4d_encoder() {

    hierarchical_4d_encoder.mTransformLength_t = parameter_handler.transform_length_t;
    hierarchical_4d_encoder.mTransformLength_s = parameter_handler.transform_length_s;
    hierarchical_4d_encoder.mTransformLength_v = parameter_handler.transform_length_v;
    hierarchical_4d_encoder.mTransformLength_u = parameter_handler.transform_length_u;
    hierarchical_4d_encoder.create_temporary_buffer(hierarchical_4d_encoder.mTransformLength_u);
    hierarchical_4d_encoder.mMinimumTransformLength_t = parameter_handler.min_transform_length_t;
    hierarchical_4d_encoder.mMinimumTransformLength_s = parameter_handler.min_transform_length_s;
    hierarchical_4d_encoder.mMinimumTransformLength_v = parameter_handler.min_transform_length_v;
    hierarchical_4d_encoder.mMinimumTransformLength_u = parameter_handler.min_transform_length_u;
    // std::tie(
    //     hierarchical_4d_encoder.mNumberOfVerticalViews, 
    //     hierarchical_4d_encoder.mNumberOfHorizontalViews, 
    //     hierarchical_4d_encoder.mNumberOfViewLines, 
    //     hierarchical_4d_encoder.mNumberOfViewColumns) = raw_lightfield->get_dimensions<int>();
    const auto& [T, S, V, U] = raw_lightfield->get_dimensions<uint32_t>();
    hierarchical_4d_encoder.mNumberOfVerticalViews = T;
    hierarchical_4d_encoder.mNumberOfHorizontalViews = S;
    hierarchical_4d_encoder.mNumberOfViewLines = V;
    hierarchical_4d_encoder.mNumberOfViewColumns = U;

    hierarchical_4d_encoder.mPGMScale = std::pow(2, raw_lightfield->get_views_bpp())-1;
}

void write_int16_into_file(int value, FILE* fp) {
	fwrite(&value, 2, 1, fp);
}

void MuleEncoder::write_initial_data_to_encoded_file() {//read_initial_data_from_encoded_file
	write_int16_into_file(hierarchical_4d_encoder.mSuperiorBitPlane, encoded_file_pointer);
    
    //writes the maximum transform sizes
    write_int16_into_file(hierarchical_4d_encoder.mTransformLength_t, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mTransformLength_s, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mTransformLength_v, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mTransformLength_u, encoded_file_pointer);
    
    //writes the minimum transform sizes
    write_int16_into_file(hierarchical_4d_encoder.mMinimumTransformLength_t, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mMinimumTransformLength_s, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mMinimumTransformLength_v, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mMinimumTransformLength_u, encoded_file_pointer);
    

    //writes the number of views of the lightfield
    write_int16_into_file(hierarchical_4d_encoder.mNumberOfVerticalViews, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mNumberOfHorizontalViews, encoded_file_pointer);
    
    //writes the number of lines and columns of each view
    write_int16_into_file(hierarchical_4d_encoder.mNumberOfViewLines, encoded_file_pointer);
    write_int16_into_file(hierarchical_4d_encoder.mNumberOfViewColumns, encoded_file_pointer);
    
    //writes the bit precision of each component of the pixels of the views
    write_int16_into_file(hierarchical_4d_encoder.mPGMScale, encoded_file_pointer);

    //writes the extension_method to file...
    // write_int16_into_file(parameter_handler.extension_method, encoded_file_pointer);
}

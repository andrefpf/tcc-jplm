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

/** \file     Hierarchical4DDecoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#include "Hierarchical4DDecoder.h"

void Hierarchical4DDecoder::start() {
    entropy_decoder.start();  //initializes entropy decoder
    reset_probability_models();   
}

void Hierarchical4DDecoder::decode_block(int position_t, int position_s, int position_v, int position_u, int length_t, int length_s, int length_v, int length_u, int bitplane) {
    
    if(bitplane < inferior_bit_plane) {
        return;
    }
    
    if(length_t*length_s*length_v*length_u == 1) { //perhaps & instead of *?
        mSubbandLF.mPixel[position_t][position_s][position_v][position_u] = decode_coefficient(bitplane);
        return;
    }

    switch (decode_segmentation_flag(bitplane)) {
        case HexadecaTreeFlag::lowerBitPlane: 
            decode_block(position_t, position_s, position_v, position_u, length_t, length_s, length_v, length_u, bitplane-1);
            break;
        case HexadecaTreeFlag::splitBlock: {
            int half_length_t = (length_t > 1) ? length_t/2 : 1;
            int half_length_s = (length_s > 1) ? length_s/2 : 1;
            int half_length_v = (length_v > 1) ? length_v/2 : 1;
            int half_length_u = (length_u > 1) ? length_u/2 : 1;
            
            int number_of_subdivisions_t = (length_t > 1) ? 2 : 1;
            int number_of_subdivisions_s = (length_s > 1) ? 2 : 1;
            int number_of_subdivisions_v = (length_v > 1) ? 2 : 1;
            int number_of_subdivisions_u = (length_u > 1) ? 2 : 1;
            
            for(auto t = 0; t < number_of_subdivisions_t; t++) {
                for(auto s = 0; s < number_of_subdivisions_s; s++) {
                    for(auto v = 0; v < number_of_subdivisions_v; v++) {
                        for(auto u = 0; u < number_of_subdivisions_u; u++) {    
                            int new_position_t = position_t+t*half_length_t;
                            int new_position_s = position_s+s*half_length_s;
                            int new_position_v = position_v+v*half_length_v;
                            int new_position_u = position_u+u*half_length_u;
                            
                            int new_length_t = (t == 0) ? half_length_t : (length_t-half_length_t);
                            int new_length_s = (s == 0) ? half_length_s : (length_s-half_length_s);
                            int new_length_v = (v == 0) ? half_length_v : (length_v-half_length_v);
                            int new_length_u = (u == 0) ? half_length_u : (length_u-half_length_u);
                                               
                            decode_block(new_position_t, new_position_s, new_position_v, new_position_u, new_length_t, new_length_s, new_length_v, new_length_u, bitplane);
                       }
                    }
                }
            }
            break;
        }
        case HexadecaTreeFlag::zeroBlock: {
            for(auto t = 0; t < length_t; t++) {
                for(auto s = 0; s < length_s; s++) {
                    for(auto v = 0; v < length_v; v++) {
                        for(auto u = 0; u < length_u; u++) {
                            mSubbandLF.mPixel[position_t+t][position_s+s][position_v+v][position_u+u] = 0;
                        }
                    }
                }
            }
            break;
        }
        default:
            std::cerr << "Invalid segmentation flag." << std::endl;
            exit(3);
    }
}

int Hierarchical4DDecoder::decode_coefficient(int bitplane) {
    int magnitude = 0;
    int bit;
    int bit_position=bitplane;
    
    for(bit_position = bitplane; bit_position >= inferior_bit_plane; bit_position--) {
        magnitude = magnitude << 1;
        bit = entropy_decoder.decode_bit(probability_models[bit_position+mSymbolProbabilityModelIndex]);
        if(bit_position > BITPLANE_BYPASS) 
            probability_models[bit_position+mSymbolProbabilityModelIndex].update(bit);            
        if(bit == 1) {
            magnitude++;
         }            
    }
    magnitude = magnitude << inferior_bit_plane;
    if(magnitude > 0) {
        magnitude += (1 << inferior_bit_plane)/2;
        if(entropy_decoder.decode_bit(probability_models[0]) == 1) {
            magnitude = -magnitude;
        }
    }
    return(magnitude);
}

HexadecaTreeFlag Hierarchical4DDecoder::decode_segmentation_flag(int bitplane) {
    int lsb=0;
    int msb = entropy_decoder.decode_bit(probability_models[(bitplane<<1)+mSegmentationFlagProbabilityModelIndex]);
    if(bitplane > BITPLANE_BYPASS_FLAGS) 
        probability_models[(bitplane<<1)+mSegmentationFlagProbabilityModelIndex].update(msb);
    if(msb == 0) {
        lsb = entropy_decoder.decode_bit(probability_models[(bitplane<<1)+1+mSegmentationFlagProbabilityModelIndex]);
        if(bitplane > BITPLANE_BYPASS_FLAGS) 
            probability_models[(bitplane<<1)+1+mSegmentationFlagProbabilityModelIndex].update(lsb);
        if (lsb) {//01 = 1 = splitBlock
            return HexadecaTreeFlag::splitBlock;
        }//00
        return HexadecaTreeFlag::lowerBitPlane;
    } //10
    return HexadecaTreeFlag::zeroBlock;
}

PartitionFlag Hierarchical4DDecoder::decode_partition_flag(void) {
    if (!entropy_decoder.decode_bit(probability_models[0])) //=0
        return PartitionFlag::transform;
    //1
    if(entropy_decoder.decode_bit(probability_models[0])) //1
        return PartitionFlag::viewSplit; //11
    return PartitionFlag::spatialSplit; //10
}

int Hierarchical4DDecoder::decode_integer(int precision) {
    int integerValue = 0;
    for(int n = 0; n < precision; n++) {
        auto bit = entropy_decoder.decode_bit(probability_models[0]);
        integerValue = (integerValue << 1) + bit;
    }
    return integerValue;
}

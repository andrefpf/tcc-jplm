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

/** \file     ABACDecoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo Bresciani 
 *  \date     2019-09-26
 */

#include "ABACDecoder.h"
#include <iostream>

void ABACDecoder::start() {

    if(codestream_code.peek_next_byte() == std::byte{0xff}) {
        [[maybe_unused]] auto byte = codestream_code.get_next_byte();
    }
    if(codestream_code.peek_next_byte() == std::byte{0xa4}) {
        [[maybe_unused]] auto byte = codestream_code.get_next_byte();
    }

    number_of_bits_in_byte = 0;  
    mLow = 0;
    mHigh = MAXINT;    
    mTag = 0;
    for(auto n = decltype(INTERVAL_PRECISION){0}; n < INTERVAL_PRECISION; n++) {
        mTag = mTag << 1;
        int bit = ReadBitFromFile();
        if(bit == 1)
            mTag = mTag | SET_LSB_MASK;
    }
}

int ABACDecoder::decode_bit(const ProbabilityModel& mPmodel) {
    unsigned long int acumFreq_0 = mPmodel.get_frequency_of_zeros();
    unsigned long int acumFreq_1 = mPmodel.get_frequency_of_ones();
    int bitDecoded;
    unsigned long int threshold = (((mTag - mLow + 1) * acumFreq_1 - 1)/(mHigh - mLow + 1));
    unsigned long int length_0 = (((mHigh - mLow + 1) * acumFreq_0)/acumFreq_1);

    if(threshold < acumFreq_0) {
         bitDecoded = 0;
         mHigh = mLow + length_0 - 1;
    } else {
         bitDecoded = 1;
         mLow = mLow + length_0;
    }

    while(((mLow & MSB_MASK) == (mHigh & MSB_MASK)) || ((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK)))) {
        
       if((mLow & MSB_MASK) == (mHigh & MSB_MASK)) {
            mLow = mLow << 1;
            mHigh = mHigh << 1;
            mLow = mLow & RESET_LSB_MASK;
            mHigh = mHigh | SET_LSB_MASK;
            mTag = mTag << 1;
            int bit = ReadBitFromFile();
            if(bit == 1) {
                mTag = mTag | SET_LSB_MASK;
            } else {
                mTag = mTag & RESET_LSB_MASK;
            }        
            mHigh = mHigh & MAXINT;
            mLow = mLow & MAXINT;
            mTag = mTag  & MAXINT;
        }
        if((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK))) {
            mLow = mLow << 1;
            mHigh = mHigh << 1;
            mLow = mLow & RESET_LSB_MASK;
            mHigh = mHigh | SET_LSB_MASK;
            mTag = mTag << 1;
            int bit = ReadBitFromFile();
            if(bit == 1) {
                mTag = mTag | SET_LSB_MASK;
            } else {
                mTag = mTag & RESET_LSB_MASK;
            }                       
            mLow = mLow ^ MSB_MASK;           
            mHigh = mHigh ^ MSB_MASK;
            mTag = mTag ^ MSB_MASK;
            mHigh = mHigh & MAXINT;
            mLow = mLow & MAXINT;
            mTag = mTag  & MAXINT;
        }
    }

    mHigh = mHigh & MAXINT;
    mLow = mLow & MAXINT;
    mTag = mTag  & MAXINT;

    return (bitDecoded);
}


int ABACDecoder::ReadBitFromFile() {
    int bit; 
    if (number_of_bits_in_byte == 0) {  
        number_of_bits_in_byte = 8;
        if(codestream_code.is_next_valid()) {
            std::byte byte = codestream_code.get_next_byte();
            mBitBuffer=std::to_integer<int>(byte);
        }
    }
    bit = mBitBuffer&01;
    mBitBuffer = mBitBuffer >> 1;
    number_of_bits_in_byte--;
    return(bit);  
}

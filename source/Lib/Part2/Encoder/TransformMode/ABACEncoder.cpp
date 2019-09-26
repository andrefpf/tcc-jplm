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

/** \file     ABACEncoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */


#include "ABACEncoder.h"

void ABACEncoder::start(FILE *ofp) {
/*! initializes arithmetic coding: sets the output file, sets the interval to [0,1) */
    file_ptr = ofp;
    mLow = 0;
    mHigh = MAXINT;
    mNumberOfBitsInBuffer = 0;
    mScalingsCounter = 0;
    mBitBuffer = 0;    
}

/*! encodes a binary symbol using the given probability model */   
void ABACEncoder::encode_bit(bool inputbit, const ProbabilityModel &mPmodel) {
    unsigned long int acumFreq_0 = mPmodel.get_frequency_of_zeros();
    unsigned long int acumFreq_1 = mPmodel.get_frequency_of_ones();
    unsigned long int length_0 = (((mHigh - mLow + 1) * acumFreq_0)/acumFreq_1);

    if(inputbit) {//1
        mLow = mLow + length_0;
    } else { //0
        mHigh = mLow + length_0 - 1;
    }

    while(((mLow & MSB_MASK) == (mHigh & MSB_MASK)) || ((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK)))) {
        if((mLow & MSB_MASK) == (mHigh & MSB_MASK)) {
            int bit = 1;
            if((mLow & MSB_MASK) == 0)
                bit = 0;
            
            output_bit(bit);
                        
            mLow = mLow << 1;
            mHigh = mHigh << 1;
            mLow = mLow & RESET_LSB_MASK;
            mHigh = mHigh | SET_LSB_MASK;
            mHigh = mHigh & MAXINT;
            mLow = mLow & MAXINT;
            while(mScalingsCounter > 0) {
                mScalingsCounter--;
                output_bit(1-bit);
            }                
        }
        if((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK))) {
            mLow = mLow << 1;
            mHigh = mHigh << 1;
            mLow = mLow & RESET_LSB_MASK;
            mHigh = mHigh | SET_LSB_MASK;
            mLow = mLow ^ MSB_MASK;
            mHigh = mHigh ^ MSB_MASK;
            mScalingsCounter++;

            mHigh = mHigh & MAXINT;
            mLow = mLow & MAXINT;
        }
    }
    mHigh = mHigh & MAXINT;
    mLow = mLow & MAXINT;
}

void ABACEncoder::finish() { //flush
/*! write remaining bits in the outputfile */       
    mScalingsCounter++;
    int bit;
    if(mLow >= SECOND_MSB_MASK)
        bit = 1;
    else
        bit = 0;
    
    output_bit(bit);
    while(mScalingsCounter > 0) {
        output_bit(1-bit);
        mScalingsCounter--;
    }
    
    if(mNumberOfBitsInBuffer > 0) {
        for(int n = mNumberOfBitsInBuffer; n < 8; n++) {
            mBitBuffer = mBitBuffer >> 1;
        }
    }
    fprintf(file_ptr, "%c", mBitBuffer);
    mNumberOfBitsInBuffer = 0;    
}

/*! write bits in the output file */
void ABACEncoder::output_bit(bool bit) {
    mBitBuffer = mBitBuffer >> 1;
    if (bit) //==1
        mBitBuffer = mBitBuffer | 0x80;
    
    mNumberOfBitsInBuffer++;
    if(mNumberOfBitsInBuffer == 8) {
        fprintf(file_ptr, "%c", mBitBuffer);
        mNumberOfBitsInBuffer = 0;
    }
}
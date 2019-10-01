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

/** \file     MuleCodec.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

/*
* @Author: Ismael Seidel
* @Date:   2019-03-26 16:16:15
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-14 12:17:31
*/
#include "MuleCodec.h"


void MuleCodec::initialize_extension_lenghts() {
    // extension_length_t = parameter_handler.number_of_vertical_views%parameter_handler.transform_length_t;
    // extension_length_s = parameter_handler.number_of_horizontal_views%parameter_handler.transform_length_s;
    // extension_length_v = decoded_lightfield.mNumberOfViewLines%parameter_handler.transform_length_v;
    // extension_length_u = decoded_lightfield.mNumberOfViewColumns%parameter_handler.transform_length_u;

    // if(parameter_handler.extension_method != SHRINK_TO_FIT && extension_length_t+extension_length_s+extension_length_v+extension_length_u > 0)
    //     needs_block_extension=true;
}

MuleCodec::~MuleCodec() {
    if (encoded_file_pointer != nullptr)
        fclose(encoded_file_pointer);
}

void MuleCodec::open_encoded_lightfield(const char* modes) {
    if((encoded_file_pointer = fopen(parameter_handler.encoded_lightfield.string().c_str(), modes)) == NULL) {
        std::cerr << "Error: input file " << parameter_handler.encoded_lightfield << "was not found" << std::endl;
        exit(1);
    }
}



void MuleCodec::setup_transform_coefficients(bool forward) {
    DCT4DCoefficientsManager::get_instance(forward).set_transform_max_sizes(
        parameter_handler.transform_length_u, 
        parameter_handler.transform_length_v, 
        parameter_handler.transform_length_s, 
        parameter_handler.transform_length_t);

    DCT4DCoefficientsManager::get_instance(forward).set_transform_gains(
        parameter_handler.transform_scale_u, 
        parameter_handler.transform_scale_v, 
        parameter_handler.transform_scale_s, 
        parameter_handler.transform_scale_t);
}
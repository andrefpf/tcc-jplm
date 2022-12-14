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

/** \file     EncoderRunTimeStatistics.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-02-10
 */
#include "Lib/Utils/Stats/EncoderRunTimeStatistics.h"
#include "ho-hashlibpp.h"

EncoderRunTimeStatistics::EncoderRunTimeStatistics(std::fstream &stream)
    : RunTimeStatistics(), ref_to_stream(stream),
      initial_of_stream_position(stream.tellp()) {
  if (!ref_to_stream.is_open()) {
    std::cerr << "Error opening output file" << std::endl;
    exit(EXIT_FAILURE);
  }
}


void EncoderRunTimeStatistics::mark_end() {
  if (!finished_counting_bytes) {
    final_of_stream_position = ref_to_stream.tellp();
    finished_counting_bytes = true;
  }
  RunTimeStatistics::mark_end();
}


void EncoderRunTimeStatistics::show_statistics() {
  RunTimeStatistics::show_statistics();
  std::cout << "Bytes written to file: "
            << final_of_stream_position - initial_of_stream_position
            << " bytes " << std::endl;

  std::cout << "MD5: " << HO_Hashlibpp::MD5(ref_to_stream) << std::endl;
  std::cout << "SHA1: " << HO_Hashlibpp::SHA1(ref_to_stream) << std::endl;
}
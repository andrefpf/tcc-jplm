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

/** \file     CompressionTypeLightField.h
 *  \brief    This files keeps the definition of CompressionTypeLightField enumeration
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-22
 */

#ifndef JPLM_LIB_PART2_COMMON_COMPRESSIONTYPELIGHTFIELD_H__
#define JPLM_LIB_PART2_COMMON_COMPRESSIONTYPELIGHTFIELD_H__

#include "Lib/Common/Boxes/Generic/CompressionType.h"

/**
 * \brief      Enum class for representing the compression type of light field.
 * \note       The values that are not defined in the present enum are reserved for ISO use.
 */
enum class CompressionTypeLightField : compression_type_data {
	transform_mode = 0, //!< Indicates that the 4D transform mode is used to encode the light field. It is coded as compression_type_data value 0
	prediction_mode = 1, //!< Indicates that the 4D prediction mode is used to encode the light field. It is coded as compression_type_data value 1
	// all other values are reserved for ISO use
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_COMPRESSIONTYPELIGHTFIELD_H__ */
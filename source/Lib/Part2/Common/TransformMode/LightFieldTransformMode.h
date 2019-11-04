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

/** \file     LightFieldTransformMode.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-27
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_LIGHTFIELDTRANSFORMMODE_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_LIGHTFIELDTRANSFORMMODE_H__

#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
#include "Lib/Part2/Common/TransformMode/Block4D.h"

template<typename T = uint16_t>
class LightFieldTransformMode : public LightfieldFromPPMFile<T> {
 public:
  LightFieldTransformMode(const LightfieldIOConfiguration& configuration,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : LightfieldFromPPMFile<T>(configuration, std::move(view_io_policy)) {
        std::cout << "LightFieldTransformMode A" << std::endl;
        {const auto& [t, s, v, u] = configuration.get_size();
                std::cout << "t: " << t << std::endl;
                std::cout << "s: " << s << std::endl;
                std::cout << "v: " << v << std::endl;
                std::cout << "u: " << u << std::endl;}

        std::cout << "This size: " << std::endl;
        const auto& [t, s, v, u] = this->template get_dimensions<uint32_t>();
        std::cout << "t: " << t << std::endl;
        std::cout << "s: " << s << std::endl;
        std::cout << "v: " << v << std::endl;
        std::cout << "u: " << u << std::endl;
  }

  LightFieldTransformMode(const LightfieldIOConfiguration& configuration,
      std::size_t max_value, const PixelMapType type,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : LightfieldFromPPMFile<T>(
            configuration, max_value, type, std::move(view_io_policy)) {
        std::cout << "LightFieldTransformMode B" << std::endl;
        {const auto& [t, s, v, u] = configuration.get_size();
                std::cout << "t: " << t << std::endl;
                std::cout << "s: " << s << std::endl;
                std::cout << "v: " << v << std::endl;
                std::cout << "u: " << u << std::endl;}
                std::cout << "This size: " << std::endl;
        const auto& [t, s, v, u] = this->template get_dimensions<uint32_t>();
        std::cout << "t: " << t << std::endl;
        std::cout << "s: " << s << std::endl;
        std::cout << "v: " << v << std::endl;
        std::cout << "u: " << u << std::endl;
  }


  virtual ~LightFieldTransformMode() = default;


  Block4D get_block_4D_from(const int channel,
      const LightfieldCoordinate<uint32_t>& coordinate_4d,
      const LightfieldDimension<uint32_t>& size) {
    auto block = Block4D(size);
    const auto& [t_initial, s_initial, v_initial, u_initial] = coordinate_4d;
    const auto [t_max, s_max, v_max, u_max] = coordinate_4d + size;
    auto c = 0;
    for (auto t = t_initial; t < t_max; ++t) {
      for (auto s = s_initial; s < s_max; ++s) {
        const auto& image_channel =
            this->template get_image_at<BT601Image>({t, s}).get_channel(
                channel);
              
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            // std::cout << t << ", " << s << ", " << v << ", " << u << std::endl;
            if (this->is_coordinate_valid({t, s, v, u})) {
              block.mPixelData[c++] = image_channel[v][u];
            } else {
              c++;
              // std::cout << "invalid" << std::endl;
            }
          }
        }
      }
    }
    return block;
  }


  void set_block_4D_at(const Block4D& block_4d, const int channel,
      const LightfieldCoordinate<uint32_t>& coordinate_4d) {
    const auto& [t_initial, s_initial, v_initial, u_initial] = coordinate_4d;
    const auto [t_max, s_max, v_max, u_max] =
        coordinate_4d + block_4d.get_dimension();
    auto c = 0;
    for (auto t = t_initial; t < t_max; ++t) {
      for (auto s = s_initial; s < s_max; ++s) {
        auto& image_channel =
            this->template get_image_at<BT601Image>({t, s}).get_channel(
                channel);
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            if (this->is_coordinate_valid({t, s, v, u})) {
              image_channel[v][u] = block_4d.mPixelData[c++];
            } else {
              c++;
            }
          }
        }
      }
    }
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_LIGHTFIELDTRANSFORMMODE_H__ */
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

#include "Lib/Part2/Common/LightfieldFromFile.h"
#include "Lib/Part2/Common/TransformMode/Block4D.h"

template<typename T = uint16_t>
class LightFieldTransformMode : public LightfieldFromFile<T> {
 public:
  LightFieldTransformMode(const LightfieldIOConfiguration& configuration,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : LightfieldFromFile<T>(configuration, std::move(view_io_policy)) {
  }


  /**
   * @brief      Constructs a new instance (decoder side).
   *
   * @param[in]  configuration       The configuration
   * @param[in]  number_of_channels  The number of channels
   * @param[in]  bits_per_sample     The bits per sample
   * @param      view_io_policy      The view i/o policy
   * 
   * \todo number_of_channels and bits_per_sample could be on LightfieldIOConfiguration. However, this means
   * only one constructor. How to know if encoder or decoder?
   */
  LightFieldTransformMode(const LightfieldIOConfiguration& configuration,
      std::size_t number_of_channels, std::size_t bits_per_sample,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : LightfieldFromFile<T>(configuration, number_of_channels,
            bits_per_sample, std::move(view_io_policy)) {
  }


  virtual ~LightFieldTransformMode() = default;


  Block4D get_block_4D_from(const int channel,
      const LightfieldCoordinate<uint32_t>& coordinate_4d,
      const LightfieldDimension<uint32_t>& size);


  void set_block_4D_at(const Block4D& block_4d, const int channel,
      const LightfieldCoordinate<uint32_t>& coordinate_4d);
};


template<typename T>
Block4D LightFieldTransformMode<T>::get_block_4D_from(const int channel,
    const LightfieldCoordinate<uint32_t>& coordinate_4d,
    const LightfieldDimension<uint32_t>& size) {
  auto block = Block4D(size);
  const auto& [t_initial, s_initial, v_initial, u_initial] = coordinate_4d;
  const auto [t_max, s_max, v_max, u_max] = coordinate_4d + size;
  auto c = 0;
  bool has_invalid_coordinates = false;
  for (auto t = t_initial; t < t_max; ++t) {
    for (auto s = s_initial; s < s_max; ++s) {
      // std::cout << "getting block from " << t << ", " << s << ", channel "
      //           << channel << std::endl;
      //! \todo Check this for errors
      if (this->is_coordinate_valid({t, s, v_initial, u_initial})) {
        const auto& image_channel =
            this->get_image_at({t, s}).get_channel(channel);
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            // std::cout << t << ", " << s << ", " << v << ", " << u << std::endl;
            if (this->is_coordinate_valid({t, s, v, u})) {
              block.mPixelData[c++] = image_channel[v][u];
            } else {
              has_invalid_coordinates = true;
              block.mPixelData[c++] = 0;
            }
          }
        }
      } else {
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            has_invalid_coordinates = true;
            // ++c;
            block.mPixelData[c++] = 0;
          }
        }
      }
    }
  }


  if (has_invalid_coordinates) {
    uint32_t last_valid_t = 0;
    auto t = t_initial;
    while (this->is_coordinate_valid({t, s_initial, v_initial, u_initial}) &&
           t < t_max) {
      ++t;
      ++last_valid_t;
    }

    uint32_t last_valid_s = 0;
    auto s = s_initial;
    while (this->is_coordinate_valid({t_initial, s, v_initial, u_initial}) &&
           s < s_max) {
      ++s;
      ++last_valid_s;
    }

    uint32_t last_valid_v = 0;
    auto v = v_initial;
    while (this->is_coordinate_valid({t_initial, s_initial, v, u_initial}) &&
           v < v_max) {
      ++v;
      ++last_valid_v;
    }

    uint32_t last_valid_u = 0;
    auto u = u_initial;
    while (this->is_coordinate_valid({t_initial, s_initial, v_initial, u}) &&
           u < u_max) {
      ++u;
      ++last_valid_u;
    }
#ifdef _WIN32
    LightfieldCoordinate<int> c(
        last_valid_t, last_valid_s, last_valid_v, last_valid_u);
    block.extend(c);
#else
    block.extend({last_valid_t, last_valid_s, last_valid_v, last_valid_u});
#endif
  }

  return block;
}

template<typename T>
void LightFieldTransformMode<T>::set_block_4D_at(const Block4D& block_4d,
    const int channel, const LightfieldCoordinate<uint32_t>& coordinate_4d) {
  const auto& [t_initial, s_initial, v_initial, u_initial] = coordinate_4d;
  const auto [t_max, s_max, v_max, u_max] =
      coordinate_4d + block_4d.get_dimension();
  auto c = 0;
  for (auto t = t_initial; t < t_max; ++t) {
    for (auto s = s_initial; s < s_max; ++s) {
      if (this->is_coordinate_valid({t, s, v_initial, u_initial})) {
        auto& image_channel = this->get_image_at({t, s}).get_channel(channel);
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            if (this->is_coordinate_valid({t, s, v, u})) {
              image_channel[v][u] = block_4d.mPixelData[c++];
            } else {
              ++c;
            }
          }
        }
      } else {
        for (auto v = v_initial; v < v_max; ++v) {
          for (auto u = u_initial; u < u_max; ++u) {
            ++c;
          }
        }
      }
    }
  }
}

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_LIGHTFIELDTRANSFORMMODE_H__ */
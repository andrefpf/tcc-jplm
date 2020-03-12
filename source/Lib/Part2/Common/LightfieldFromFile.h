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

/** \file     LightfieldFromFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-07
 */

#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDFROMFILE_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDFROMFILE_H__

#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/ViewFromPGXFile.h"


/**
 * \brief A class that holds a complete lightfield, where the views are obtained from PPM Files.
 * \details [long description]
 * 
 * \tparam T Its the type of each pixel in the Lightfield.
 */
template<typename T>
class LightfieldFromFile : public Lightfield<T> {
 public:
  /**
   * \brief      Constructs the object.
   *
   * \param[in]  configuration   The configuration
   * \param[in]      view_io_policy  The view i/o policy
   * 
   * \details This kind of lightfield can be build using only the configuration and a default view io policy. 
   * 
   * For instance:
   * \snippet App/Utils/LightfieldVisualization.cpp Instantiating a LightfieldFromFile using a LightfieldIOConfiguration
   */
  LightfieldFromFile(const LightfieldIOConfiguration& configuration,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : Lightfield<T>(configuration.get_size().get_t_and_s(),
            std::move(view_io_policy), true) {
    //number of channels
    std::size_t number_of_channels =
        configuration
            .get_number_of_colour_channels();  //<! \todo check this out...
    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      auto channel_path = std::filesystem::path(
          configuration.get_path() + "/" + std::to_string(i));
      if (!std::filesystem::exists(channel_path)) {
        //<! \todo should throw error, as in the encoder the inputs should be present
        //throw
        std::cout << "should throw error..." << std::endl;
      }
    }


    for (const auto& coordinate : configuration.get_raster_view_coordinates()) {
      this->set_view_at(
          std::move(std::make_unique<ViewFromPGXFile<T>>(
              configuration.get_path(), coordinate, number_of_channels)),
          coordinate);
    }
  }


  LightfieldFromFile(const LightfieldIOConfiguration& configuration,
      std::size_t number_of_channels, std::size_t bits_per_sample,
      ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>())
      : Lightfield<T>(configuration.get_size().get_t_and_s(),
            std::move(view_io_policy), true) {
    // std::size_t bits_per_sample = 10;

    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      auto channel_path = std::filesystem::path(
          configuration.get_path() + "/" + std::to_string(i));
      if (!std::filesystem::exists(channel_path)) {
        std::filesystem::create_directory(channel_path);
      }
    }

    for (const auto& coordinate : configuration.get_raster_view_coordinates()) {
      this->set_view_at(std::move(std::make_unique<ViewFromPGXFile<T>>(
                            configuration.get_path(), coordinate,
                            configuration.get_size().get_v_and_u(),
                            bits_per_sample, number_of_channels)),
          coordinate);
    }


    this->lightfield_dimension =
        std::make_unique<LightfieldDimension<std::size_t>>(
            configuration.get_size());
  }


  /**
   * \brief Destructor of the LightfieldFromFile (default)
   */
  virtual ~LightfieldFromFile() = default;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDFROMFILE_H__ */
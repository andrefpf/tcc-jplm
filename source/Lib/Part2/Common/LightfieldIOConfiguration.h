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

/** \file     LightfieldIOConfiguration.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */

#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__

#include <filesystem>
#include <tuple>
#include <vector>
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Utils/Image/ImageFile.h"


class LightfieldIOConfiguration {
 protected:
  const std::string lightfield_path;
  const LightfieldDimension<std::size_t> lightfield_size;
  const LightfieldCoordinate<std::size_t> lightfield_initial_coordinate;
  const uint16_t number_of_colour_channels = 3;

 public:
  LightfieldIOConfiguration(const std::string& path_to_lightfield,
      const LightfieldDimension<std::size_t>& lightfield_size,
      const LightfieldCoordinate<std::size_t>& initial_coordinate = {0, 0, 0,
          0},
      uint16_t number_of_colour_channels = 3);


  LightfieldIOConfiguration(const std::string& path_to_lightfield,
      const LightfieldCoordinate<std::size_t>& initial_coordinate,
      const LightfieldCoordinate<std::size_t>& final_coordinate,
      uint16_t number_of_colour_channels = 3);


  /**
   * @brief      Checks if configuration is valid.
   * 
   * Basically checks if it contains a valid lightfield_path. Otherwise throws
   * LightfieldIOConfigurationExceptions::InvalidLightfieldPath.
   * 
   * \exception LightfieldIOConfigurationExceptions::InvalidLightfieldPath if the light field path is invalid
   */
  void check_configurations();

  // LightfieldIOConfiguration(const std::string& path_to_lightfield,
  //   const std::pair<std::size_t, std::size_t>& initial_st_coordinate)

  virtual ~LightfieldIOConfiguration() = default;


  /**
   * @brief      Gets the initial coordinate.
   *
   * @return     The initial coordinate.
   */
  LightfieldCoordinate<std::size_t> get_initial_coordinate() const noexcept;


  /**
   * @brief      Gets the size.
   *
   * @return     The size.
   */
  LightfieldDimension<std::size_t> get_size() const noexcept;


  /**
   * @brief      Gets the path.
   *
   * @return     The path.
   */
  std::string get_path() const noexcept;


  /**
   * @brief      Gets the number of pixels per view.
   *
   * @return     The number of pixels per view.
   */
  std::size_t get_number_of_pixels_per_view() const noexcept;


  /**
   * @brief      Gets the number of views per lightfield.
   *
   * @return     The number of views per lightfield.
   */
  std::size_t get_number_of_views_per_lightfield() const noexcept;


  /**
   * @brief      Gets the number of pixels per lightfield.
   *
   * @return     The number of pixels per lightfield.
   */
  std::size_t get_number_of_pixels_per_lightfield() const noexcept;


  /**
   * @brief      Gets the number of colour channels.
   *
   * @return     The number of colour channels.
   */
  uint16_t get_number_of_colour_channels() const noexcept;


  /**
   * @brief      Gets the raster view coordinates.
   *
   * @return     The raster view coordinates.
   */
  std::vector<std::pair<std::size_t, std::size_t>> get_raster_view_coordinates()
      const noexcept;
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDIOCONFIGURATION_H__ */

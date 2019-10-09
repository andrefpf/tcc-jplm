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

/** \file     JPLM4DTransformModeLightFieldEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_ENCODER_JPLM4DTRANSFORMMODELIGHTFIELDENCODER_H__
#define JPLM_LIB_PART2_ENCODER_JPLM4DTRANSFORMMODELIGHTFIELDENCODER_H__

#include "Lib/Common/JPLMEncoderConfigurationLightField4DTransformMode.h"
#include "Lib/Part2/Common/TransformMode/JPLM4DTransformModeLightFieldCodec.h"
#include "Lib/Part2/Encoder/JPLMLightFieldEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/Hierarchical4DEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/TransformPartition.h"

template<typename PelType = uint16_t>
class JPLM4DTransformModeLightFieldEncoder
    : public JPLM4DTransformModeLightFieldCodec<PelType>,
      public JPLMLightFieldEncoder<PelType> {
 protected:
  std::unique_ptr<JPLMEncoderConfigurationLightField4DTransformMode>
      transform_mode_configuration;
  Hierarchical4DEncoder hierarchical_4d_encoder;
  TransformPartition tp;

 public:
  JPLM4DTransformModeLightFieldEncoder(
      std::unique_ptr<JPLMEncoderConfigurationLightField4DTransformMode>&&
          configuration)
      : JPLMLightFieldCodec<PelType>(
            std::make_unique<LightfieldFromPPMFile<PelType>>(
                configuration->get_lightfield_io_configurations())),
      JPLM4DTransformModeLightFieldCodec<PelType>({configuration->get_lightfield_io_configurations().get_size()}, {
      	configuration->get_maximal_transform_sizes()
      }), 
        JPLMLightFieldEncoder<PelType>(*configuration),
        transform_mode_configuration(std::move(configuration)),
        tp(transform_mode_configuration
                ->get_minimal_transform_size_inter_view_vertical(),
            transform_mode_configuration
                ->get_minimal_transform_size_inter_view_horizontal(),
            transform_mode_configuration
                ->get_minimal_transform_size_intra_view_vertical(),
            transform_mode_configuration
                ->get_minimal_transform_size_intra_view_horizontal()) {
    tp.mPartitionData.set_dimension(
        transform_mode_configuration
            ->get_maximal_transform_size_inter_view_vertical(),  //T
        transform_mode_configuration
            ->get_maximal_transform_size_inter_view_horizontal(),  //S
        transform_mode_configuration
            ->get_maximal_transform_size_intra_view_vertical(),  //V
        transform_mode_configuration
            ->get_maximal_transform_size_intra_view_horizontal());  //U
    setup_hierarchical_4d_encoder();

    this->setup_transform_coefficients(true,
        transform_mode_configuration->get_maximal_transform_sizes(),
        transform_mode_configuration->get_transform_scalings());

    write_initial_data_to_encoded_file();

    this->initialize_extension_lenghts();
  }


  void setup_hierarchical_4d_encoder() {
    hierarchical_4d_encoder.mTransformLength_t =
        transform_mode_configuration
            ->get_maximal_transform_size_inter_view_vertical();
    hierarchical_4d_encoder.mTransformLength_s =
        transform_mode_configuration
            ->get_maximal_transform_size_inter_view_horizontal();
    hierarchical_4d_encoder.mTransformLength_v =
        transform_mode_configuration
            ->get_maximal_transform_size_intra_view_vertical();
    hierarchical_4d_encoder.mTransformLength_u =
        transform_mode_configuration
            ->get_maximal_transform_size_intra_view_horizontal();
    hierarchical_4d_encoder.create_temporary_buffer(
        hierarchical_4d_encoder.mTransformLength_u);
    hierarchical_4d_encoder.mMinimumTransformLength_t =
        transform_mode_configuration
            ->get_minimal_transform_size_inter_view_vertical();
    hierarchical_4d_encoder.mMinimumTransformLength_s =
        transform_mode_configuration
            ->get_minimal_transform_size_inter_view_horizontal();
    hierarchical_4d_encoder.mMinimumTransformLength_v =
        transform_mode_configuration
            ->get_minimal_transform_size_intra_view_vertical();
    hierarchical_4d_encoder.mMinimumTransformLength_u =
        transform_mode_configuration
            ->get_minimal_transform_size_intra_view_horizontal();
    // std::tie(
    //     hierarchical_4d_encoder.mNumberOfVerticalViews,
    //     hierarchical_4d_encoder.mNumberOfHorizontalViews,
    //     hierarchical_4d_encoder.mNumberOfViewLines,
    //     hierarchical_4d_encoder.mNumberOfViewColumns) = lightfield->get_dimensions<int>();
    const auto& [T, S, V, U] =
        this->light_field->template get_dimensions<uint32_t>();
    hierarchical_4d_encoder.mNumberOfVerticalViews = T;
    hierarchical_4d_encoder.mNumberOfHorizontalViews = S;
    hierarchical_4d_encoder.mNumberOfViewLines = V;
    hierarchical_4d_encoder.mNumberOfViewColumns = U;

    hierarchical_4d_encoder.mPGMScale =
        std::pow(2, this->light_field->get_views_bpp()) - 1;
  }


  void
  write_initial_data_to_encoded_file() {  //read_initial_data_from_encoded_file
    hierarchical_4d_encoder.write_initial_data();
  }


  virtual ~JPLM4DTransformModeLightFieldEncoder() = default;


  virtual void run_for_block_4d() override {
    std::cout << "running 4d block in encoder..." << std::endl;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPLM4DTRANSFORMMODELIGHTFIELDENCODER_H__ */

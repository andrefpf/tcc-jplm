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
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Part2/Common/TransformMode/LightFieldConfigurationMarkerSegment.h"
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
  LightFieldTransformMode<PelType>& ref_to_lightfield;
  LightFieldConfigurationMarkerSegment lightfield_configuration_marker_segment;

public:
  JPLM4DTransformModeLightFieldEncoder(
      std::unique_ptr<JPLMEncoderConfigurationLightField4DTransformMode>&&
      configuration)
      : JPLMLightFieldCodec<PelType>(
      std::make_unique<LightFieldTransformMode<PelType>>(
          configuration->get_lightfield_io_configurations())),
        JPLM4DTransformModeLightFieldCodec<PelType>(
            {configuration->get_lightfield_io_configurations().get_size()},
            {configuration->get_maximal_transform_sizes()}),
        JPLMLightFieldEncoder<PelType>(*configuration),
        transform_mode_configuration(std::move(configuration)),
        tp(transform_mode_configuration->get_minimal_transform_dimension()),
      //        hierarchical_4d_encoder(*transform_mode_configuration),
        ref_to_lightfield(static_cast<LightFieldTransformMode<PelType>&>(
                              *(this->light_field))),
        lightfield_configuration_marker_segment(
            {transform_mode_configuration->get_lightfield_io_configurations()
                 .get_size()},  //lightfield_dimension,
            {ComponentSsizParameter(10), ComponentSsizParameter(10),
             ComponentSsizParameter(10)},  //Ssiz
            {transform_mode_configuration
                 ->get_maximal_transform_sizes()},  //block_dimension,
            {30, 30, 30},  //max_bitplane
            (transform_mode_configuration->get_border_blocks_policy() ==
             BorderBlocksPolicy::truncate)
            ? true
            : false  //truncate
        ) {
    tp.mPartitionData.set_dimension(
        transform_mode_configuration->get_maximal_transform_dimension());
    setup_hierarchical_4d_encoder();

    this->setup_transform_coefficients(true,
                                       transform_mode_configuration->get_maximal_transform_sizes(),
                                       transform_mode_configuration->get_transform_scalings());

    // write_initial_data_to_encoded_file();
    hierarchical_4d_encoder.write_marker(
        Marker::SOC);  //writes the start of codestream
    hierarchical_4d_encoder.write_lightfield_configuration_marker_segment(
        lightfield_configuration_marker_segment);

    this->initialize_extension_lengths();
  }


  void setup_hierarchical_4d_encoder() {
    hierarchical_4d_encoder.set_transform_dimension(
        transform_mode_configuration->get_maximal_transform_dimension());

    hierarchical_4d_encoder.create_temporary_buffer();

    hierarchical_4d_encoder.set_minimum_transform_dimension(
        transform_mode_configuration->get_minimal_transform_dimension());

    hierarchical_4d_encoder.set_lightfield_dimension(
        this->light_field->template get_dimensions<uint32_t>());

    hierarchical_4d_encoder.set_level_shift(
        std::pow(2, this->light_field->get_views_bpp()) - 1);
  }


  virtual BorderBlocksPolicy get_border_blocks_policy() const override {
    return transform_mode_configuration->get_border_blocks_policy();
  }


  virtual void finalization() override {
    auto& codestreams = this->jpl_file->get_reference_to_codestreams();
    auto& first_codestream = *(codestreams.at(0));
    auto& first_codestream_as_part2 =
        static_cast<JpegPlenoLightFieldBox&>(first_codestream);
    auto& lightfield_box_contents =
        first_codestream_as_part2.get_ref_to_contents();

    lightfield_box_contents.add_contiguous_codestream_box(
        std::make_unique<ContiguousCodestreamBox>(
            std::move(hierarchical_4d_encoder.move_codestream_code_out())));
  }


  virtual ~JPLM4DTransformModeLightFieldEncoder() = default;


  virtual void run_for_block_4d(const uint32_t channel,
                                const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
                                const LightfieldDimension<uint32_t>& size) override {
    hierarchical_4d_encoder.write_marker(Marker::SOB);

    auto block_4d =
        ref_to_lightfield.get_block_4D_from(channel, position, size);
    block_4d += 0 - level_shift;


    const auto lambda = transform_mode_configuration->get_lambda();
    tp.rd_optimize_transform(block_4d, hierarchical_4d_encoder, lambda);
    tp.encode_partition(hierarchical_4d_encoder, lambda);


    //reset prob models here at the end to ensure no flush is called.
    //assumes that the prob models are initialized in reset state...
    hierarchical_4d_encoder.reset_probability_models();
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPLM4DTRANSFORMMODELIGHTFIELDENCODER_H__ */
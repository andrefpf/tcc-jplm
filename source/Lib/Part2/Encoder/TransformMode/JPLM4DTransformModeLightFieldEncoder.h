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

#include "CppConsoleTable/CppConsoleTable.hpp"
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "Lib/Common/JPLMEncoderConfigurationLightField4DTransformMode.h"
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Part2/Common/TransformMode/JPLM4DTransformModeLightFieldCodec.h"
#include "Lib/Part2/Common/TransformMode/LightFieldConfigurationMarkerSegment.h"
#include "Lib/Part2/Encoder/JPLMLightFieldEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/Hierarchical4DEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/TransformPartition.h"
#include "Lib/Utils/Image/ImageChannelUtils.h"

template<typename PelType = uint16_t>
class JPLM4DTransformModeLightFieldEncoder
    : public JPLM4DTransformModeLightFieldCodec<PelType>,
      public JPLMLightFieldEncoder<PelType> {
 protected:
  std::shared_ptr<JPLMEncoderConfigurationLightField4DTransformMode>
      transform_mode_encoder_configuration;
  Hierarchical4DEncoder hierarchical_4d_encoder;
  TransformPartition transform_partition;
  LightFieldTransformMode<PelType>& ref_to_lightfield;
  LightFieldConfigurationMarkerSegment lightfield_configuration_marker_segment;

  // std::vector<uint64_t>

  std::vector<double> sse_per_channel;
  std::vector<std::size_t>
      bytes_per_channel;  //<! Accumulates the total number of encoded bytes of each channel. Does not include header information.

  virtual uint16_t get_number_of_colour_components() const override {
    return transform_mode_encoder_configuration
        ->get_number_of_colour_channels();
  }

 public:
  JPLM4DTransformModeLightFieldEncoder(
      std::shared_ptr<JPLMEncoderConfigurationLightField4DTransformMode>
          configuration)
      : JPLMLightFieldCodec<PelType>(
            std::make_unique<LightFieldTransformMode<PelType>>(
                configuration->get_lightfield_io_configurations()),
            *configuration),
        JPLM4DTransformModeLightFieldCodec<PelType>(
            {configuration->get_lightfield_io_configurations().get_size()},
            {configuration->get_maximal_transform_sizes()}, *configuration),
        JPLMLightFieldEncoder<PelType>(*configuration),
        transform_mode_encoder_configuration(configuration),
        transform_partition(transform_mode_encoder_configuration
                                ->get_minimal_transform_dimension()),
        ref_to_lightfield(static_cast<LightFieldTransformMode<PelType>&>(
            *(this->light_field))),
        lightfield_configuration_marker_segment(
            {transform_mode_encoder_configuration
                    ->get_lightfield_io_configurations()
                    .get_size()},  //lightfield_dimension,
            transform_mode_encoder_configuration->get_bitdepths(),  //Ssiz
            {transform_mode_encoder_configuration
                    ->get_maximal_transform_sizes()},  //block_dimension,
            transform_mode_encoder_configuration
                ->get_max_bitplanes(),  //max_bitplane
            is_truncated()) {
    transform_partition.mPartitionData.set_dimension(
        transform_mode_encoder_configuration
            ->get_maximal_transform_dimension());
    setup_hierarchical_4d_encoder();

    this->setup_transform_coefficients(true,
        transform_mode_encoder_configuration->get_maximal_transform_sizes(),
        transform_mode_encoder_configuration->get_transform_scalings());

    // write_initial_data_to_encoded_file();
    hierarchical_4d_encoder.write_marker(
        Marker::SOC);  //writes the start of codestream
    hierarchical_4d_encoder.write_lightfield_configuration_marker_segment(
        lightfield_configuration_marker_segment);

    this->initialize_extension_lengths();


    auto number_of_channels =
        ref_to_lightfield.get_number_of_channels_in_view();

    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      sse_per_channel.push_back(0.0);
      bytes_per_channel.push_back(0);
    }
  }


  bool is_truncated() {
    return transform_mode_encoder_configuration->get_border_blocks_policy() ==
           BorderBlocksPolicy::truncate;
  }


  void setup_hierarchical_4d_encoder() {
    hierarchical_4d_encoder.set_transform_dimension(
        transform_mode_encoder_configuration
            ->get_maximal_transform_dimension());

    hierarchical_4d_encoder.create_temporary_buffer();

    hierarchical_4d_encoder.set_minimum_transform_dimension(
        transform_mode_encoder_configuration
            ->get_minimal_transform_dimension());

    hierarchical_4d_encoder.set_lightfield_dimension(
        this->light_field->template get_dimensions<uint32_t>());

    hierarchical_4d_encoder.set_level_shift(
        std::pow(2, this->light_field->get_views_bpp()) - 1);
  }


  virtual BorderBlocksPolicy get_border_blocks_policy() const override {
    return transform_mode_encoder_configuration->get_border_blocks_policy();
  }


  void finalization() override;


  virtual ~JPLM4DTransformModeLightFieldEncoder() = default;


  void run_for_block_4d(const uint32_t channel,
      const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) override;


  samilton::ConsoleTable get_console_table() {
    samilton::ConsoleTable table(1, 1, samilton::Alignment::centre);
    samilton::ConsoleTable::TableChars chars;
    chars.topDownSimple = '\0';
    chars.leftSeparation = '\0';
    chars.centreSeparation = '\0';
    chars.downLeft = '\0';
    chars.downRight = '\0';
    chars.leftRightSimple = '\0';
    chars.rightSeparation = '\0';
    chars.topLeft = '\0';
    chars.topRight = '\0';
    chars.topSeparation = '\0';
    chars.downSeparation = '\0';
    table.setTableChars(chars);
    return table;
  }

  void show_error_estimate();
};

template<typename PelType>
void JPLM4DTransformModeLightFieldEncoder<PelType>::show_error_estimate() {
  auto number_of_channels = ref_to_lightfield.get_number_of_channels_in_view();
  auto number_of_pels =
      ref_to_lightfield.get_total_number_of_pixels_per_channel();
  auto bpp = ref_to_lightfield.get_views_bpp();


  std::cout << (this->jpl_file->size() * 8.0) /
                   static_cast<double>(number_of_pels)
            << " bpp" << std::endl;

  if (transform_mode_encoder_configuration->show_error_estimate()) {
    std::cout << "\n############### Estimated error ###############\n";

    auto table = get_console_table();
    auto line = 0;
    table[line++][0](samilton::Alignment::right) = "Channel: ";
    table[line++][0](samilton::Alignment::right) = "Est. MSE: ";
    table[line++][0](samilton::Alignment::right) = "Est. PSNR (dB): ";
    table[line++][0](samilton::Alignment::right) = "bpp: ";
    for (auto i = decltype(number_of_channels){0}; i < number_of_channels;
         ++i) {
      double mse = sse_per_channel.at(i) / static_cast<double>(number_of_pels);
      double psnr = ImageChannelUtils::get_peak_signal_to_noise_ratio(bpp, mse);
      double bpp_per_channel_after_encoding =
          (bytes_per_channel.at(i) * 8.0) / static_cast<double>(number_of_pels);

      line = 0;
      table[line++][i + 1] = i;
      table[line++][i + 1] = mse;
      table[line++][i + 1] = psnr;
      table[line++][i + 1] = bpp_per_channel_after_encoding;
    }

    std::cout << table;
    std::cout << "\n###############################################\n\n";
    if (!is_truncated()) {
      std::cerr
          << "WARNING: the error estimates are considering errors outside the "
             "lightfield as the block dimensions are not truncated. "
          << '\n';
      std::cerr << "\n###############################################\n\n";
    }
  }
}

template<typename PelType>
void JPLM4DTransformModeLightFieldEncoder<PelType>::finalization() {
  auto& codestreams = this->jpl_file->get_reference_to_codestreams();
  auto& first_codestream = *(codestreams.at(0));
  auto& first_codestream_as_part2 =
      static_cast<JpegPlenoLightFieldBox&>(first_codestream);
  auto& lightfield_box_contents =
      first_codestream_as_part2.get_ref_to_contents();

  auto codestream_code =
      std::move(hierarchical_4d_encoder.move_codestream_code_out());
  auto contents = std::make_unique<ContiguousCodestreamContents>(
      std::move(codestream_code));
  auto codestream_box =
      std::make_unique<ContiguousCodestreamBox>(std::move(contents));
  lightfield_box_contents.add_contiguous_codestream_box(
      std::move(codestream_box));

  this->show_error_estimate();
}

template<typename PelType>
void JPLM4DTransformModeLightFieldEncoder<PelType>::run_for_block_4d(
    const uint32_t channel, const LightfieldCoordinate<uint32_t>& position,
    const LightfieldDimension<uint32_t>& size) {
  const auto number_of_bytes_in_codestream_before_encoding_block =
      hierarchical_4d_encoder.get_ref_to_codestream_code().size();

  std::cout << "position in the codestream: "
            << hierarchical_4d_encoder.get_ref_to_codestream_code().size()
            << std::endl;

  hierarchical_4d_encoder.write_marker(Marker::SOB);

  auto block_4d = ref_to_lightfield.get_block_4D_from(channel, position, size);

  int level_shift = -std::pow(2.0, ref_to_lightfield.get_views_bpp() - 1);

  block_4d += level_shift;


  const auto lambda = transform_mode_encoder_configuration->get_lambda();
  auto rd_cost = transform_partition.rd_optimize_transform(
      block_4d, hierarchical_4d_encoder, lambda);
  //<! \todo check what happens to the metrics for shrink = 0;

  sse_per_channel.at(channel) += rd_cost.get_error();

  if (transform_mode_encoder_configuration->is_verbose()) {
    transform_partition.show_partition_codes_and_inferior_bit_plane();
    hierarchical_4d_encoder.show_inferior_bit_plane();
  }


  transform_partition.encode_partition(hierarchical_4d_encoder, lambda);


  const auto increase_in_bytes =
      hierarchical_4d_encoder.get_ref_to_codestream_code().size() -
      number_of_bytes_in_codestream_before_encoding_block;

  bytes_per_channel.at(channel) += increase_in_bytes;

  //reset prob models here at the end to ensure no flush is called.
  //assumes that the prob models are initialized in reset state...
  hierarchical_4d_encoder.reset_probability_models();
}

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_JPLM4DTRANSFORMMODELIGHTFIELDENCODER_H__ */
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

/** \file     JPLM4DTransformModeLightFieldDecoder.h
 *  \brief
 *  \details
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-09
 */

#ifndef JPLM_LIB_PART2_DECODER_JPLM4DTRANSFORMMODELIGHTFIELDDECODER_H__
#define JPLM_LIB_PART2_DECODER_JPLM4DTRANSFORMMODELIGHTFIELDDECODER_H__

#include <iostream>
#include <tuple>
#include "Lib/Common/JPLMDecoderConfiguration.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldBox.h"
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "Lib/Part2/Common/TransformMode/JPLM4DTransformModeLightFieldCodec.h"
#include "Lib/Part2/Common/TransformMode/LightFieldTransformMode.h"
#include "Lib/Part2/Decoder/JPLMLightFieldDecoder.h"
#include "Lib/Part2/Decoder/TransformMode/CodestreamPointerSetMarkerSegmentParser.h"
#include "Lib/Part2/Decoder/TransformMode/ColourComponentScalingMarkerSegmentParser.h"
#include "Lib/Part2/Decoder/TransformMode/CommonExceptions.h"
#include "Lib/Part2/Decoder/TransformMode/Hierarchical4DDecoder.h"
#include "Lib/Part2/Decoder/TransformMode/LightFieldContigurationMarkerSegmentParser.h"
#include "Lib/Part2/Decoder/TransformMode/PartitionDecoder.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include "magic_enum.hpp"


template<typename PelType = uint16_t>
class JPLM4DTransformModeLightFieldDecoder
    : public JPLM4DTransformModeLightFieldCodec<PelType>,
      public JPLMLightFieldDecoder<PelType> {
 protected:
  std::shared_ptr<JPLMDecoderConfiguration>
      transform_mode_decoder_configuration;
  std::unique_ptr<LightFieldConfigurationMarkerSegment>
      lightfield_configuration_marker_segment = nullptr;

 private:
  PartitionDecoder partition_decoder;
  const ContiguousCodestreamCode& codestream_code;
  Hierarchical4DDecoder hierarchical_4d_decoder;
  LightFieldTransformMode<PelType>&
      ref_to_lightfield;  //! \todo move this to transform mode codec...

  BorderBlocksPolicy border_blocks_policy = BorderBlocksPolicy::truncate;

  // const JPLFile&
  //     transform_mode_jpl_file;  //temporary, need to refactor to use base class jpl file...
 public:
  JPLM4DTransformModeLightFieldDecoder(
      std::shared_ptr<JPLFile>
          jpl_file,  // ! \todo use this as the JPLCodec file
      const JpegPlenoLightFieldBox& light_field_box,
      const std::string& lightfield_path,
      std::shared_ptr<JPLMDecoderConfiguration> configuration)
      : JPLMLightFieldCodec<PelType>(jpl_file,
            std::make_unique<LightFieldTransformMode<PelType>>(
                LightfieldIOConfiguration(lightfield_path,
                    light_field_box.get_ref_to_contents()
                        .get_ref_to_light_field_header_box()
                        .get_ref_to_contents()
                        .get_ref_to_light_field_header_box()
                        .get_ref_to_contents()
                        .get_light_field_dimension<std::size_t>()),
                light_field_box.get_ref_to_contents()
                    .get_ref_to_light_field_header_box()
                    .get_ref_to_contents()
                    .get_ref_to_light_field_header_box()
                    .get_ref_to_contents()
                    .get_number_of_components(),
                light_field_box.get_ref_to_contents()
                    .get_ref_to_light_field_header_box()
                    .get_ref_to_contents()
                    .get_ref_to_light_field_header_box()
                    .get_ref_to_contents()
                    .get_bits_per_component()),
            *configuration),
        JPLM4DTransformModeLightFieldCodec<PelType>(
            light_field_box.get_ref_to_contents()
                .get_ref_to_light_field_header_box()
                .get_ref_to_contents()
                .get_ref_to_light_field_header_box()
                .get_ref_to_contents()
                .get_light_field_dimension(),
            {9, 9, 64, 64},
            *configuration),  //temporary
        JPLMLightFieldDecoder<PelType>(*configuration),
        // light_field_box_(light_field_box),
        transform_mode_decoder_configuration(configuration),
        codestream_code(light_field_box.get_ref_to_contents()
                            .get_ref_to_contiguous_codestream_box()
                            .get_ref_to_contents()
                            .get_ref_to_code()),
        hierarchical_4d_decoder(codestream_code),
        ref_to_lightfield(static_cast<LightFieldTransformMode<PelType>&>(
            *(this->light_field))) {
    read_initial_data_from_codestream_code();
    this->setup_transform_coefficients(false,
        hierarchical_4d_decoder.get_transform_dimensions(),
        {1.0, 1.0, 1.0, 1.0});

    // std::cout << "dec LF dimension: " << light_field_box.get_ref_to_contents()
    //                     .get_ref_to_light_field_header_box()
    //                     .get_ref_to_contents()
    //                     .get_ref_to_light_field_header_box()
    //                     .get_ref_to_contents()
    //                     .get_light_field_dimension<std::size_t>() << std::endl;

    //initializes possible extension lengths
    this->initialize_extension_lengths();

    auto& view_io_policy = ref_to_lightfield.get_ref_to_view_io_policy();
    view_io_policy.set_save_image_when_release(true)
        .set_overwrite_image_when_save_if_file_already_exists(true);
  }


  void read_marker(Marker marker) {
    auto bytes = Markers::get_bytes(marker);
    if (auto byte = codestream_code.get_next_byte(); byte != bytes[0]) {
      throw JPLM4DTransformModeLightFieldDecoderExceptions::
          ExpectingAMarkerException(marker);
    }
    if (auto byte = codestream_code.get_next_byte(); byte != bytes[1]) {
      throw JPLM4DTransformModeLightFieldDecoderExceptions::
          ExpectingAMarkerException(marker);
    }
  }


  std::array<std::byte, 2> get_next_marker_bytes() {
    auto marker = std::array<std::byte, 2>();
    marker[0] = codestream_code.get_next_byte();
    if (marker[0] != std::byte{0xFF}) {
      //throw
    }
    marker[1] = codestream_code.get_next_byte();
    return marker;
  }


  void read_optional_markers_and_marker_segments() {
    auto marker = get_next_marker_bytes();
    auto bytes_of_scc_marker = Markers::get_bytes(Marker::SCC);
    while (marker[1] == bytes_of_scc_marker[1]) {
      //detected a SCC optional marker
      auto colour_component_scaling_marker_segment =
          ColourComponentScalingMarkerSegmentParser::
              get_colour_component_scaling_marker_segment(codestream_code);
      marker = get_next_marker_bytes();

      auto index_variant =
          colour_component_scaling_marker_segment.get_colour_component_index();
      auto scaling_factor =
          colour_component_scaling_marker_segment.get_scaling_factor();

      std::visit(
          [this, scaling_factor](auto& index) {
            this->partition_decoder.set_colour_component_scaling_factor(
                index, scaling_factor);
            if (transform_mode_decoder_configuration->is_verbose()) {
            std::cout << "Detected scaling factor " << scaling_factor
                      << " for colour channel " << index << '\n';
                    }
          },
          index_variant);
    }
    auto bytes_of_pnt_marker = Markers::get_bytes(Marker::PNT);
    if (marker[1] == bytes_of_pnt_marker[1]) {
      auto codestream_pointer_set_marker_segment =
          CodestreamPointerSetMarkerSegmentParser::
              get_codestream_pointer_set_marker_segment(codestream_code);
      this->check_consistency_of_pnt(
          codestream_code, codestream_pointer_set_marker_segment);
      marker = get_next_marker_bytes();
      if (transform_mode_decoder_configuration->is_verbose()) {
        std::cout << "Found a consistent PNT\n";
      }
    } else {
      if (transform_mode_decoder_configuration->is_verbose()) {
        std::cout
            << "There is no PNT marker segment in the contiguous codestream\n";
      }
    }
    if (transform_mode_decoder_configuration->is_verbose()) {
      std::cout << "No more optional markers" << std::endl;
    }
  }



  void read_mandatory_markers() {
    read_marker(Marker::SOC);
    read_marker(Marker::LFC);
  }


  virtual uint16_t get_number_of_colour_components() const override {
    return this->lightfield_configuration_marker_segment
        ->get_number_of_colour_components();
  }


  void read_light_field_configuration_marker_segment() {
    auto lightfield_configuration_marker_segment =
        LightFieldContigurationMarkerSegmentParser::
            get_light_field_configuration_marker_segment(codestream_code);

    //setup the number of channels (NC) in the hierarchical 4d encoder
    partition_decoder.set_number_of_colour_components(
        lightfield_configuration_marker_segment
            .get_number_of_colour_components());


    //gets the superior bit plane value from the LFC marker segment
    auto superior_bit_plane =
        lightfield_configuration_marker_segment.get_max_bitplane_at_channel(0);
    hierarchical_4d_decoder.set_superior_bit_plane(superior_bit_plane);

    //gets the maximum transform sizes from the LFC marker segment
    const auto& [transform_length_t, transform_length_s, transform_length_v,
                    transform_length_u] =
        lightfield_configuration_marker_segment.get_ref_to_block_dimension()
            .as_tuple();
    hierarchical_4d_decoder.set_transform_dimension(
        lightfield_configuration_marker_segment.get_ref_to_block_dimension());


    this->block_4d_dimension =
        lightfield_configuration_marker_segment.get_ref_to_block_dimension();

    const auto& [number_of_vertical_views, number_of_horizontal_views,
                    mNumberOfViewLines, mNumberOfViewColumns] =
        lightfield_configuration_marker_segment
            .get_ref_to_lightfield_dimension();

    hierarchical_4d_decoder.set_lightfield_dimension(
        lightfield_configuration_marker_segment
            .get_ref_to_lightfield_dimension());

    auto bpp =
        lightfield_configuration_marker_segment.get_max_bitdepth_at_channel(0);

    auto level_shift = static_cast<int>(std::pow(2.0, bpp) - 1.0);
    hierarchical_4d_decoder.set_level_shift(level_shift);


    if (!lightfield_configuration_marker_segment.get_truncate_flag()) {
      border_blocks_policy = BorderBlocksPolicy::padding;
    }

    if (transform_mode_decoder_configuration->is_verbose()) {
      std::cout << "superior_bit_plane: "
                << static_cast<uint32_t>(superior_bit_plane) << "\n";
      std::cout << "transform_length_t: " << transform_length_t << "\n";
      std::cout << "transform_length_s: " << transform_length_s << "\n";
      std::cout << "transform_length_v: " << transform_length_v << "\n";
      std::cout << "transform_length_u: " << transform_length_u << "\n";
      std::cout << "number_of_vertical_views: " << number_of_vertical_views
                << "\n";
      std::cout << "number_of_horizontal_views: " << number_of_horizontal_views
                << "\n";
      std::cout << "mNumberOfViewLines (v): " << mNumberOfViewLines << "\n";
      std::cout << "mNumberOfViewColumns (u): " << mNumberOfViewColumns << "\n";
      std::cout << "level_shift: " << level_shift << "\n";
      std::cout << "number of colour components: "
                << lightfield_configuration_marker_segment
                       .get_number_of_colour_components()
                << '\n';
      std::cout << "Border policy: "
                << magic_enum::enum_name(this->get_border_blocks_policy())
                << std::endl;
    }

    this->lightfield_configuration_marker_segment =
        std::make_unique<LightFieldConfigurationMarkerSegment>(
            std::move(lightfield_configuration_marker_segment));
  }


  void read_initial_data_from_codestream_code() {
    read_mandatory_markers();
    // LightFieldConfigurationMarkerSegment
    read_light_field_configuration_marker_segment();
    read_optional_markers_and_marker_segments();
  }


  virtual BorderBlocksPolicy get_border_blocks_policy() const override {
    return border_blocks_policy;
  }


  virtual ~JPLM4DTransformModeLightFieldDecoder() = default;


  virtual void finalization() override {
    ref_to_lightfield.save_views_according_to_view_io_policies();
  }


  virtual void run_for_block_4d(const uint32_t channel,
      const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) override {
    hierarchical_4d_decoder.reset_probability_models();

    auto decoded_block = partition_decoder.decode_partition(
        channel, hierarchical_4d_decoder, size);


    int level_shift = (hierarchical_4d_decoder.get_level_shift() + 1) / 2;
    decoded_block += level_shift;


    decoded_block.clip(0, hierarchical_4d_decoder.get_level_shift());

    ref_to_lightfield.set_block_4D_at(decoded_block, channel, position);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_DECODER_JPLM4DTRANSFORMMODELIGHTFIELDDECODER_H__ */

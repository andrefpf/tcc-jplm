/*
* @Author: Ismael Seidel
* @Date:   2019-03-26 16:32:19
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-14 12:16:47
*/

#include "MuleDecoder.h"

void MuleDecoder::open_decoded_lightfield() {
  auto dimension = LightfieldDimension<std::size_t>(
      parameter_handler.number_of_vertical_views,
      parameter_handler.number_of_horizontal_views,
      hierarchical_4d_decoder.mNumberOfViewLines,
      hierarchical_4d_decoder.mNumberOfViewColumns);
  auto config = LightfieldIOConfiguration(
      parameter_handler.decoded_lightfield.string(), dimension);


  lightfield = std::make_unique<LightFieldTransformMode<>>(
      config, 1023, PixelMapType::P6);
  auto& view_io_policy = lightfield->get_ref_to_view_io_policy();
  view_io_policy.set_save_image_when_release(true)
      .set_overwrite_image_when_save_if_file_already_exists(true);
}


MuleDecoder::MuleDecoder(
    ParameterHandler handler, const ContiguousCodestreamCode& codestream_code)
    : MuleCodec(handler), codestream_code(codestream_code),
      hierarchical_4d_decoder(codestream_code) {
  read_initial_data_from_compressed_file();

  setup_header_data_into_decoded_lightfield();
  open_decoded_lightfield();
  setup_transform_coefficients(false);

  //initializes possible extension lenghs
  initialize_extension_lenghts();

  hierarchical_4d_decoder.start();
}


MuleDecoder::~MuleDecoder() {
  lightfield->save_views_according_to_view_io_policies();
  hierarchical_4d_decoder.finish();
}


void MuleDecoder::run_for_block_4d(const uint32_t channel,
    [[maybe_unused]] const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
    const LightfieldDimension<uint32_t>& size) {

  hierarchical_4d_decoder.reset_probability_models();

  auto decoded_block =
      partition_decoder.decode_partition(hierarchical_4d_decoder, size);
  decoded_block += (hierarchical_4d_decoder.mPGMScale+1)/2;


  // if (needs_block_extension) {
  //     if(used_size_u != BLOCK_SIZE_u)
  //         current_block->extend(parameter_handler.extension_method, extension_length_u, LightFieldDimension::U);
  //     if(used_size_v != BLOCK_SIZE_v)
  //         current_block->extend(parameter_handler.extension_method, extension_length_v, LightFieldDimension::V);
  //     if(used_size_s != BLOCK_SIZE_s)
  //         current_block->extend(parameter_handler.extension_method, extension_length_s, LightFieldDimension::S);
  //     if(used_size_t != BLOCK_SIZE_t)
  //         current_block->extend(parameter_handler.extension_method, extension_length_t, LightFieldDimension::T);
  // }

  lightfield->set_block_4D_at(decoded_block, channel, position);
}


void MuleDecoder::setup_header_data_into_decoded_lightfield() {
  hierarchical_4d_decoder.mTransformLength_t =
      parameter_handler.transform_length_t;
  hierarchical_4d_decoder.mTransformLength_s =
      parameter_handler.transform_length_s;
  hierarchical_4d_decoder.mTransformLength_v =
      parameter_handler.transform_length_v;
  hierarchical_4d_decoder.mTransformLength_u =
      parameter_handler.transform_length_u;

  hierarchical_4d_decoder.mMinimumTransformLength_t =
      parameter_handler.min_transform_length_t;
  hierarchical_4d_decoder.mMinimumTransformLength_s =
      parameter_handler.min_transform_length_s;
  hierarchical_4d_decoder.mMinimumTransformLength_v =
      parameter_handler.min_transform_length_v;
  hierarchical_4d_decoder.mMinimumTransformLength_u =
      parameter_handler.min_transform_length_u;

  hierarchical_4d_decoder.mNumberOfVerticalViews =
      parameter_handler.number_of_vertical_views;
  hierarchical_4d_decoder.mNumberOfHorizontalViews =
      parameter_handler.number_of_horizontal_views;
}


uint16_t read_int_from_codestream_code(
    const ContiguousCodestreamCode& codestream_code) {
  auto bytes = std::vector<std::byte>();
  auto byte_0 = codestream_code.get_next_byte();
  auto byte_1 = codestream_code.get_next_byte();
  bytes.push_back(byte_0);
  bytes.push_back(byte_1);
  return BinaryTools::get_value_from_big_endian_byte_vector<uint16_t>(bytes);
}


void MuleDecoder::read_initial_data_from_compressed_file() {
  // //reads the superior bit plane value
  hierarchical_4d_decoder.mSuperiorBitPlane =
      read_int_from_codestream_code(codestream_code);
  // //reads the maximum transform sizes
  parameter_handler.transform_length_t =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.transform_length_s =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.transform_length_v =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.transform_length_u =
      read_int_from_codestream_code(codestream_code);
  // //reads the minimum transform sizes
  parameter_handler.min_transform_length_t =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.min_transform_length_s =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.min_transform_length_v =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.min_transform_length_u =
      read_int_from_codestream_code(codestream_code);
  // //reads the number of views of the lightfield
  parameter_handler.number_of_vertical_views =
      read_int_from_codestream_code(codestream_code);
  parameter_handler.number_of_horizontal_views =
      read_int_from_codestream_code(codestream_code);
  // //reads the number of lines and columns of each view
  hierarchical_4d_decoder.mNumberOfViewLines =
      read_int_from_codestream_code(codestream_code);
  hierarchical_4d_decoder.mNumberOfViewColumns =
      read_int_from_codestream_code(codestream_code);
  hierarchical_4d_decoder.mPGMScale =
      read_int_from_codestream_code(codestream_code);

  //reads the extension_method from file...
  // int extension_method_code;
  // read_int_from_file(&extension_method_code, encoded_file_pointer);
  // switch(extension_method_code) {
  //     case 0: parameter_handler.extension_method = ExtensionMethod::SHRINK_TO_FIT;
  //         break;
  //     default: parameter_handler.extension_method = ExtensionMethod::REPEAT_LAST;
  // }
}
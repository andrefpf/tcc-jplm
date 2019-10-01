/*
* @Author: Ismael Seidel
* @Date:   2019-03-26 16:32:19
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-14 12:16:47
*/

#include "MuleDecoder.h"

MuleDecoder::MuleDecoder(ParameterHandler handler) : MuleCodec(handler) {
    open_encoded_lightfield("rb"); //oppening in read binary mode
    read_initial_data_from_compressed_file();

    setup_header_data_into_decoded_lightfield();
    open_decoded_lightfield('w'); //oppening in write mode
    setup_transform_coefficients(false);

    //initializes possible extension lenghs
    initialize_extension_lenghts();
}

MuleDecoder::~MuleDecoder() {
    hierarchical_4d_decoder.finish();
}

template <typename encodedColorHolder>
void MuleDecoder::decode() {
    auto T = decoded_lightfield.mNumberOfVerticalViews;
    auto S = decoded_lightfield.mNumberOfHorizontalViews;
    auto V = decoded_lightfield.mNumberOfViewLines;
    auto U = decoded_lightfield.mNumberOfViewColumns;

    auto BLOCK_SIZE_t = parameter_handler.transform_length_t;
    auto BLOCK_SIZE_s = parameter_handler.transform_length_s;
    auto BLOCK_SIZE_v = parameter_handler.transform_length_v;
    auto BLOCK_SIZE_u = parameter_handler.transform_length_u;

    PartitionDecoder partition_decoder;
    auto rgb_4d_block = RGBBlock4DHolder(r_block,  g_block,  b_block, decoded_lightfield.mPGMScale);
    encodedColorHolder spectral_4d_block(y_block, cb_block, cr_block, decoded_lightfield.mPGMScale);

    if (parameter_handler.extension_method != SHRINK_TO_FIT) {
        rgb_4d_block.set_block_dimensions(              BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u);
        spectral_4d_block.set_block_dimensions(         BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u);
        partition_decoder.mPartitionData.set_dimension( BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u);

    }

   
    hierarchical_4d_decoder.start(encoded_file_pointer);

    for(auto t = 0; t < T; t += BLOCK_SIZE_t) {
        auto used_size_t = (t + BLOCK_SIZE_t > T)? T%BLOCK_SIZE_t : BLOCK_SIZE_t;
        for(auto s = 0; s < S; s += BLOCK_SIZE_s) {
            auto used_size_s = (s + BLOCK_SIZE_s > S)? S%BLOCK_SIZE_s : BLOCK_SIZE_s;
            for(auto v = 0; v < V; v += BLOCK_SIZE_v) {
                auto used_size_v = (v + BLOCK_SIZE_v > V)? V%BLOCK_SIZE_v : BLOCK_SIZE_v;
                for(auto u = 0; u < U; u += BLOCK_SIZE_u) {
                    auto used_size_u = (u + BLOCK_SIZE_u > U)? U%BLOCK_SIZE_u : BLOCK_SIZE_u;
                    
                    if (parameter_handler.extension_method == SHRINK_TO_FIT) {
                        partition_decoder.mPartitionData.resize_avoiding_free(used_size_t, used_size_s, used_size_v, used_size_u);
                        rgb_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
                        spectral_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
                        // std::cout << "Shrink" << std::endl;
                    }

                    //hierarchical_4d_decoder.reset_probability_models();

                    for(auto current_block: spectral_4d_block.as_ptr_array()){
                        if(parameter_handler.verbose) {
                            printf("transforming the 4D block at position (%d %d %d %d)\n", t, s, v, u);
                        }
                        hierarchical_4d_decoder.reset_probability_models();

                        current_block->fill_with_zeros();
                        // std::cout << "current_block->mlength_t " << current_block->mlength_t << std::endl;
                        partition_decoder.mPartitionData.fill_with_zeros();
                        // std::cout << "partition_decoder " << partition_decoder.mPartitionData.mlength_t << std::endl;
                        partition_decoder.decode_partition(hierarchical_4d_decoder);
                        // std::cout << "partition_decoder2 " << partition_decoder.mPartitionData.mlength_t << std::endl;
                        current_block->swap_data_with(partition_decoder.mPartitionData);
                        // std::cout << "partition_decoder3 " << partition_decoder.mPartitionData.mlength_t << std::endl;
                        // std::cout << "hierarchical_4d_decoder " << hierarchical_4d_decoder.mSubbandLF.mlength_t << std::endl;
                        // current_block->copy_sub_block_from(partition_decoder.mPartitionData, 0, 0, 0, 0);

                        if (needs_block_extension) {
                            if(used_size_u != BLOCK_SIZE_u)
                                current_block->extend(parameter_handler.extension_method, extension_length_u, LightFieldDimension::U);
                            if(used_size_v != BLOCK_SIZE_v) 
                                current_block->extend(parameter_handler.extension_method, extension_length_v, LightFieldDimension::V);
                            if(used_size_s != BLOCK_SIZE_s)
                                current_block->extend(parameter_handler.extension_method, extension_length_s, LightFieldDimension::S);
                            if(used_size_t != BLOCK_SIZE_t)
                                current_block->extend(parameter_handler.extension_method, extension_length_t, LightFieldDimension::T);
                        }
                                                                        
                    }
                    
                    spectral_4d_block.add_constant_to_pels((decoded_lightfield.mPGMScale+1)/2);
                    spectral_4d_block.convert_to(rgb_4d_block);

                    //this should be a post-processing of the LF
                    if(parameter_handler.is_lenslet_13x13) {
                        if(t == 0) {
                            if(s == 0) {
                                rgb_4d_block.shift_uv_planes(-2, 0, 0);
                            }
                            if((s + BLOCK_SIZE_s >= S)&&(s <= S)) { 
                                rgb_4d_block.shift_uv_planes(-2, 0, S-s-1);
                            }
                        }
                        if((t + BLOCK_SIZE_t >= T)&&(t <= T)) {
                            if(s == 0) {
                                rgb_4d_block.shift_uv_planes(-2, T-t-1, 0);
                            }
                            if((s + BLOCK_SIZE_s >= S)&&(s <= S)) {
                                rgb_4d_block.shift_uv_planes(-2, T-t-1, S-s-1);
                            }
                        }
                    }
                    // ------------    
                    rgb_4d_block.clip(0, decoded_lightfield.mPGMScale);
                    // std::cerr << "before" << std::endl;
                    rgb_4d_block.set_data_into_lightfield(decoded_lightfield, t, s, v, u);
                    // std::cerr << "after" << std::endl;
                }
            }
        }
    }

}

void MuleDecoder::setup_header_data_into_decoded_lightfield() {
    hierarchical_4d_decoder.mTransformLength_t = parameter_handler.transform_length_t;
    hierarchical_4d_decoder.mTransformLength_s = parameter_handler.transform_length_s;
    hierarchical_4d_decoder.mTransformLength_v = parameter_handler.transform_length_v;
    hierarchical_4d_decoder.mTransformLength_u = parameter_handler.transform_length_u;

    hierarchical_4d_decoder.mMinimumTransformLength_t = parameter_handler.min_transform_length_t;
    hierarchical_4d_decoder.mMinimumTransformLength_s = parameter_handler.min_transform_length_s;
    hierarchical_4d_decoder.mMinimumTransformLength_v = parameter_handler.min_transform_length_v;
    hierarchical_4d_decoder.mMinimumTransformLength_u = parameter_handler.min_transform_length_u;

    hierarchical_4d_decoder.mNumberOfVerticalViews = parameter_handler.number_of_vertical_views;
    hierarchical_4d_decoder.mNumberOfHorizontalViews = parameter_handler.number_of_horizontal_views;

    decoded_lightfield.set_configurations(parameter_handler.number_of_vertical_views,
                                        parameter_handler.number_of_horizontal_views, 
                                        parameter_handler.transform_length_v);

    std::cout << "The configurations were: " << std::endl;

    std::cout << parameter_handler.number_of_vertical_views << std::endl;
    std::cout << parameter_handler.number_of_horizontal_views << std::endl; 
    std::cout << parameter_handler.transform_length_v << std::endl;

    decoded_lightfield.mPGMScale = hierarchical_4d_decoder.mPGMScale;
    decoded_lightfield.mNumberOfViewColumns = hierarchical_4d_decoder.mNumberOfViewColumns;
    decoded_lightfield.mNumberOfViewLines = hierarchical_4d_decoder.mNumberOfViewLines;
    decoded_lightfield.mVerticalViewNumberOffset = parameter_handler.first_vertical_view_number;
    decoded_lightfield.mHorizontalViewNumberOffset = parameter_handler.first_horizontal_view_number;

    std::cout << "Other configurations were: " << std::endl;
    std::cout << decoded_lightfield.mPGMScale << std::endl;
    std::cout << decoded_lightfield.mNumberOfViewColumns << std::endl;
    std::cout << decoded_lightfield.mNumberOfViewLines << std::endl;
    std::cout << decoded_lightfield.mVerticalViewNumberOffset << std::endl;
    std::cout << decoded_lightfield.mHorizontalViewNumberOffset << std::endl;
}

void read_int_from_file(int* dest, FILE* fp) {
    *dest=0;   //why is this initialization necessary?
    if (fread(dest, 2, 1, fp) != 1) {
        std::cerr << "Error while reading file." << std::endl;
        exit(1);
    }
}

void MuleDecoder::read_initial_data_from_compressed_file() {
    //reads the superior bit plane value
    read_int_from_file(&(hierarchical_4d_decoder.mSuperiorBitPlane), encoded_file_pointer);

    //reads the maximum transform sizes
    read_int_from_file(&(parameter_handler.transform_length_t), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.transform_length_s), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.transform_length_v), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.transform_length_u), encoded_file_pointer);

    //reads the minimum transform sizes
    read_int_from_file(&(parameter_handler.min_transform_length_t), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.min_transform_length_s), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.min_transform_length_v), encoded_file_pointer);
    read_int_from_file(&(parameter_handler.min_transform_length_u), encoded_file_pointer);

    //reads the number of views of the lightfield
    read_int_from_file(&(parameter_handler.number_of_vertical_views), encoded_file_pointer);    
    read_int_from_file(&(parameter_handler.number_of_horizontal_views), encoded_file_pointer);

    //reads the number of lines and columns of each view
    read_int_from_file(&(hierarchical_4d_decoder.mNumberOfViewLines), encoded_file_pointer);
    read_int_from_file(&(hierarchical_4d_decoder.mNumberOfViewColumns), encoded_file_pointer);
    
    //reads the bit precision of each component of the pixels of the views
    read_int_from_file(&(hierarchical_4d_decoder.mPGMScale), encoded_file_pointer);

    std::cerr << "Checking: " << std::endl;
    std::cerr << hierarchical_4d_decoder.mPGMScale << std::endl;
    std::cerr << hierarchical_4d_decoder.mNumberOfViewLines << std::endl;
    std::cerr << hierarchical_4d_decoder.mNumberOfViewColumns << std::endl;

    //reads the extension_method from file...
    // int extension_method_code;
    // read_int_from_file(&extension_method_code, encoded_file_pointer);
    // switch(extension_method_code) {
    //     case 0: parameter_handler.extension_method = ExtensionMethod::SHRINK_TO_FIT;
    //         break;
    //     default: parameter_handler.extension_method = ExtensionMethod::REPEAT_LAST;
    // }
}

//choses the required decoder according to the parameters
void MuleDecoder::decode() {
    switch(parameter_handler.color_transform) {
        case ColorSpaces::ColorSpace::BT601: return decode<YCbCrBlock4DHolder>();
        case ColorSpaces::ColorSpace::YCoCg: return decode<YCoCgBlock4DHolder>();
        default: std::cerr << "Unknown color space. Exiting now"; exit(2);
    }
}
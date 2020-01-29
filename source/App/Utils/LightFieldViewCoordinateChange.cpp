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

/** \file     LightFieldViewCoordinateChange.cpp
 *  \brief    This util is intended to change the filenames of light-field views given an offset
 *  \details
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-29
 */

#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <sstream>
#include <vector>
#include <getopt.h>
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageExceptions.h"
#include "Lib/Utils/Image/ImageIO.h"
#include "Lib/Utils/Image/PixelMapFileIO.h"
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"


std::string get_view_name(
        const std::pair<std::size_t, std::size_t>& coordinate) {
    std::ostringstream string_stream;
    string_stream << std::setw(3) << std::setfill('0') << std::get<0>(coordinate)
                  << '_' << std::setw(3) << std::setfill('0')
                  << std::get<1>(coordinate) << ".ppm";
    return string_stream.str();
}


void copy_view(
        const std::string& filename_input, const std::string& filename_output) {
    namespace fs = std::filesystem;
    if (!fs::exists(filename_input)) {
        throw ImageIOExceptions::InexistentFileException();
    }
    if (fs::exists(filename_output)) {
        throw ImageIOExceptions::FileAlreadyExistsException();
    }
    fs::copy(filename_input, filename_output);
}

//how is the file named? T_S or S_T?
//coordinate is t_s and filename is s_t

std::pair<std::size_t, std::size_t> find_final_coordinates(const std::string& input_path, const std::pair<std::size_t, std::size_t>& initial_coordinate, const std::pair<std::size_t, std::size_t>& step) {
    const auto& [initial_t, initial_s] = initial_coordinate;
    const auto& [step_t, step_s] = step;
    auto name_translator = PPM3CharViewToFilename();

    //find max_t
    auto t=initial_t;
    while(std::filesystem::is_regular_file(input_path+name_translator.view_position_to_filename({t, initial_s}))) {
//        std::cout << name_translator.view_position_to_filename({t, initial_s}) << std::endl;
        t+=step_t;
    }
    t-=step_t;

    //find max_s
    auto s=initial_s;
    while(std::filesystem::is_regular_file(input_path+name_translator.view_position_to_filename({initial_t, s}))) {
//        std::cout << name_translator.view_position_to_filename({initial_t, s}) << std::endl;
        s+=step_s;
    }
    s-=step_s;


    return {t,s};
}


bool are_input_and_output_paths_valid(
        const std::string& input_path, const std::string& output_path) {
    auto valid = true;
    if (input_path == output_path) {
        std::cerr << "Input and output paths must be different" << std::endl;
        valid = false;
    }

    if (!std::filesystem::is_directory(input_path)) {
        std::cerr << "Input path must be a directory" << std::endl;
        valid = false;
    }

    if (!std::filesystem::is_directory(output_path)) {
        std::cerr << "Output_path path must be a directory" << std::endl;
        valid = false;
    }
    return valid;
}


void shift_coordinate_in_filenames(const std::string& input_path,
        const std::string& output_path,
        const std::pair<std::size_t, std::size_t>& initial_input_coordinate,
        const std::pair<std::size_t, std::size_t>& final_input_coordinate,
        const std::pair<std::size_t, std::size_t>& initial_output_coordinate,
        const std::pair<std::size_t, std::size_t>& step_input = {1,1},
        const std::pair<std::size_t, std::size_t>& step_output = {1,1}) {

    const auto& [initial_input_t, initial_input_s] = initial_input_coordinate;
    const auto& [initial_output_t, initial_output_s] = initial_output_coordinate;
    const auto& [final_t, final_s] = final_input_coordinate;
    const auto& [step_input_t, step_input_s] = step_input;
    const auto& [step_output_t, step_output_s] = step_output;


    auto output_t = initial_output_t;
    auto output_s = initial_output_s;

    for (auto t = initial_input_t; t < final_t; t+=step_input_t) {
        for (auto s = initial_input_s; s < final_s; s+=step_input_s) {
            auto input_view_name = get_view_name({t, s});
            auto output_view_name = get_view_name({output_t, output_s});
            std::cout << "Copying view " << input_view_name << '\n';
            std::cout << "To view " << output_view_name << '\n';
            //copy_view({input_path + input_view_name}, {output_path + output_view_name});
            output_s+=step_output_s;
        }
        output_t+=step_output_t;
    }
}


class HelpDescription {
private:
    std::string long_option;
    std::string description;
    std::string short_option;
    std::string default_argument;
    std::size_t size_of_short_option = 0;
    std::size_t size_of_long_option = 0;
public:
    HelpDescription(const std::string& long_option, const std::string& description, const std::string& short_option={""}, const std::string& default_argument={""}) :
    long_option(long_option), description(description), short_option(short_option), default_argument(default_argument) {
        if(short_option != std::string("")) {
            size_of_short_option=short_option.size()+3;
        }
        size_of_long_option=long_option.size()+2;
    }


    void print_help(int spacing_short, int spacing_long) const {
        std::cout << "  ";
        if(size_of_short_option > 0) {
            std::cout << "-" << short_option << ", ";
        }
        std::cout << std::string(spacing_short-size_of_short_option, ' ');
        std::cout << "--" << long_option ;

        std::cout << std::string(spacing_long-size_of_long_option+2, ' ');
        std::cout << description;
        if(default_argument != std::string("")) {
            std::cout << " [default: " << default_argument << "]";
        }
        std::cout << std::endl;
    }

    auto get_size_short() const {
        return size_of_short_option;
    }
    auto get_size_long() const {
        return size_of_long_option;
    }

};



void print_help() {
    auto help_items = std::vector<HelpDescription>({
    HelpDescription("input=PATH", "specifies the input path where view files are to be found", "i PATH"),
    HelpDescription("output=PATH", "specifies the output path where renamed view files are to be saved", "o PATH"),
    HelpDescription("initial_input_t=UINT", "specifies the initial T coordinate of the input lightfield"),
    HelpDescription("initial_input_s=UINT", "specifies the initial S coordinate of the input lightfield"),
    HelpDescription("initial_output_t=UINT", "specifies the initial T coordinate of the output lightfield"),
    HelpDescription("initial_output_s=UINT", "specifies the initial S coordinate of the output lightfield"),
    HelpDescription("step_input_t=UINT", "specifies the step to obtain the next input view in the T direction", "", "1"),
    HelpDescription("step_input_s=UINT", "specifies the step to obtain the next input view in the S direction", "", "1"),
    HelpDescription("step_output_t=UINT", "specifies the step to obtain the next output view in the T direction", "", "1"),
    HelpDescription("step_output_s=UINT", "specifies the step to obtain the next output view in the S direction", "", "1"),
    HelpDescription("final_input_t=UINT", "specifies the final T coordinate of input views. If no final_input_t is specified, this util will check the files within the input path"),
    HelpDescription("final_input_s=UINT", "specifies the final S coordinate of input views. If no final_input_t is specified, this util will check the files within the input path")
    });



    auto max_number_of_spaces_short_needed = 0;
    auto max_number_of_spaces_long_needed = 0;

    for(const auto& help_item: help_items) {
        if(help_item.get_size_short() > max_number_of_spaces_short_needed) {
            max_number_of_spaces_short_needed = help_item.get_size_short();
        }
        if(help_item.get_size_long() > max_number_of_spaces_long_needed) {
            max_number_of_spaces_long_needed = help_item.get_size_long();
        }
    }

    std::cout << "Usage: lightfield_coordinate_shift [options]" << std::endl;
    std::cout << "Options:\n";

    for(const auto& help_item: help_items) {
        help_item.print_help(max_number_of_spaces_short_needed, max_number_of_spaces_long_needed);
    }
}


int main(int argc, char** argv) {

    int opt ;

    std::string input_path;
    std::string output_path;
    auto initial_input_coordinate = std::pair<std::size_t, std::size_t>(0,0);
    auto final_input_coordinate = std::pair<std::size_t, std::size_t>(0,0);
    auto initial_output_coordinate = std::pair<std::size_t, std::size_t>(0,0);
    auto step_input = std::pair<std::size_t, std::size_t>(1,1);
    auto step_output = std::pair<std::size_t, std::size_t>(1,1);


//    initial_input_t
//    initial_input_s
//    initial_output_t
//    initial_output_s
//    step_input_t
//    step_input_s
//    step_output_t
//    step_output_s
//    final_input_t
//    final_input_s

    const struct option stopcoes[] = {
            {"input"            , required_argument , 0               , 'i'},
            {"output"           , required_argument , 0               , 'o'},
            {"help"             , no_argument       , 0               , 'h'},
            {"initial_input_t"  , required_argument , 0               , 1},
            {"initial_input_s"  , required_argument , 0               , 2},
            {"initial_output_t" , required_argument , 0               , 3},
            {"initial_output_s" , required_argument , 0               , 4},
            {"step_input_t"     , required_argument , 0               , 5},
            {"step_input_s"     , required_argument , 0               , 6},
            {"step_output_t"    , required_argument , 0               , 7},
            {"step_output_s"    , required_argument , 0               , 8},
            {"final_input_t"    , required_argument , 0               , 9},
            {"final_input_s"    , required_argument , 0               , 10},
            {0                  , 0         , 0               , 0  },
    } ;


    while( (opt = getopt_long(argc, argv, "i:o:h", stopcoes, NULL)) > 0 ) {
        switch ( opt ) {
            case 'i': input_path  = std::string(optarg); break;
            case 'o': output_path = std::string(optarg); break;
            case 'h': print_help(); exit(0);
            case 1: initial_input_coordinate = std::pair<std::size_t, std::size_t>(atoi(optarg), initial_input_coordinate.second); break;
            case 2: initial_input_coordinate = std::pair<std::size_t, std::size_t>(initial_input_coordinate.first, atoi(optarg)); break;
            case 3: initial_output_coordinate = std::pair<std::size_t, std::size_t>(atoi(optarg), initial_output_coordinate.second); break;
            case 4: initial_output_coordinate = std::pair<std::size_t, std::size_t>(initial_output_coordinate.first, atoi(optarg)); break;
            case 5: step_input = std::pair<std::size_t, std::size_t>(atoi(optarg), step_input.second); break;
            case 6: step_input = std::pair<std::size_t, std::size_t>(step_input.first, atoi(optarg)); break;
            case 7: step_output = std::pair<std::size_t, std::size_t>(atoi(optarg), step_output.second); break;
            case 8: step_output = std::pair<std::size_t, std::size_t>(step_output.first, atoi(optarg)); break;
            case 9: final_input_coordinate = std::pair<std::size_t, std::size_t>(atoi(optarg), final_input_coordinate.second); break;
            case 10: final_input_coordinate = std::pair<std::size_t, std::size_t>(final_input_coordinate.first, atoi(optarg)); break;
            default: std::cout << "Invalid option or argument missing" << std::endl;
        }
    }


    if (!are_input_and_output_paths_valid(input_path, output_path)) {
        exit(1);
    }

    auto final_coordinate = find_final_coordinates(input_path, {0,2}, step_input);

//    shift_coordinate_in_filenames(input_path, output_path, {0,0})

    return 0;
}
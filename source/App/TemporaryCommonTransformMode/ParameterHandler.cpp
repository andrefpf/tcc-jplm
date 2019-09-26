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

/** \file     ParameterHandler.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

/*
* @Author: Ismael Seidel
* @Date:   2019-03-22 14:24:45
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-09 11:43:39
*/

#include "ParameterHandler.h"

int Option::option_counter = 256; //the option counter initializes after the char range (even if someone changes to unsigned char...)

std::string get_lower_case_argument(const char* arg) {
	std::locale loc;
	std::string unknown_case(arg);
	std::string lower_case(unknown_case);
	std::transform(unknown_case.begin(), unknown_case.end(), lower_case.begin(), [loc](auto val){return std::tolower(val, loc);});
	return lower_case;
}

void ParameterHandler::parse_extension_method(const char* arg) {
	// ZERO_PADDING, REPEAT_LAST, CYCLIC, EXTENDED_DCT, TRANSFORM_DOMAIN_ZERO_PADDING, MEAN_VALUE, NONE
	auto extension_name = get_lower_case_argument(arg);
	if (extension_name == "repeat_last") {
		extension_method=ExtensionMethod::REPEAT_LAST;
		return;
	}
	if (extension_name == "cyclic") {
		extension_method=ExtensionMethod::CYCLIC;
		return;
	}
	if (extension_name == "shrink") {
		extension_method=ExtensionMethod::SHRINK_TO_FIT;
		return;
	}
	std::cerr << "Unknown extension method: " << extension_name << std::endl;
	exit(1);
}

void ParameterHandler::parse_color_transform(const char* arg) {
	auto colorspace_name = get_lower_case_argument(arg);
	if (colorspace_name == "ycbcr") {
		color_transform=ColorSpaces::ColorSpace::BT601;
		return;
	}
	if (colorspace_name == "ycocg") {
		color_transform=ColorSpaces::ColorSpace::YCoCg;
		return;
	}
	std::cerr << "Unknown color space: " << colorspace_name << std::endl;
	exit(1);
}

void ParameterHandler::print_usage() const {
	std::cout << "Usage:" << std::endl;
	if(is_encoder) {
		std::cout << "\t./mule-encoder-bin --input <path to the raw lightfield> --output <file name of the encoded lightfield> [--cfg <configuration file> | options]" << std::endl;
	} else {
		std::cout << "\t./mule-encoder-bin --input <file name of the encoded lightfield> --output <path to the raw lightfield> [--cfg <configuration file> | options]" << std::endl;
	}
}

void ParameterHandler::print_help(const char* program_name) const {
	std::cout << "This is the help for " << program_name << std::endl;
	print_usage();
	std::cout << "Options: " << std::endl;
	holder.print_all_options();
	exit(0);
}

void ParameterHandler::print_configurations() {
	std::cout << "encoded_lightfield: " << encoded_lightfield << std::endl;
	std::cout << "decoded_lightfield: " << decoded_lightfield << std::endl;
	std::cout << "is_lenslet_13x13: " << is_lenslet_13x13 << std::endl;
	std::cout << "number_of_vertical_views: " << number_of_vertical_views << std::endl;
	std::cout << "number_of_horizontal_views: " << number_of_horizontal_views << std::endl;
	std::cout << "first_horizontal_view_number: " << first_horizontal_view_number << std::endl;
	std::cout << "first_vertical_view_number: "   << first_vertical_view_number << std::endl;
	std::cout << "color_transform: "        << color_transform << std::endl;
	std::cout << "extension_method: "       << extension_method << std::endl;
	std::cout << "lambda: "                 << lambda << std::endl;
	std::cout << "transform_length_t: "     << transform_length_t << std::endl;
	std::cout << "transform_length_s: "     << transform_length_s << std::endl;
	std::cout << "transform_length_v: "     << transform_length_v << std::endl;
	std::cout << "transform_length_u: "     << transform_length_u << std::endl;
	std::cout << "min_transform_length_t: " << min_transform_length_t << std::endl;
	std::cout << "min_transform_length_s: " << min_transform_length_s << std::endl;
	std::cout << "min_transform_length_v: " << min_transform_length_v << std::endl;
	std::cout << "min_transform_length_u: " << min_transform_length_u << std::endl;
	std::cout << "transform_scale_t: " << transform_scale_t << std::endl;
	std::cout << "transform_scale_s: " << transform_scale_s << std::endl;
	std::cout << "transform_scale_v: " << transform_scale_v << std::endl;
	std::cout << "transform_scale_u: " << transform_scale_u << std::endl;
	std::cout << "verbose: " << verbose << std::endl;
}

void ParameterHandler::parse_configuration_file(const char* filename) {
	std::ifstream file(filename, std::ios::in);
	std::string line;
	std::string comment;
	auto argvector = std::vector<std::string>({std::string(filename)});

	while(std::getline(file,line)) {
		if (line.size() == 0)
			continue;

		std::istringstream string_stream(line); //creates a new stream from the line string, to use getline
		for(std::string token; std::getline(string_stream, token, ' ');) {//breaks the line into substrings separated by whitespace 
			if (token.size() == 0) //check if token is empty
				continue;

			if(token.rfind("#", 0) == 0){ // treating comments in the configuration file
				comment=token;
				std::getline(string_stream, token); //once a comment is found, reads all the remaining characteres from the line
				if (verbose)
					std::cout << "Readed comment from configuration file: " << comment << " " << token << std::endl;
				continue;
			}
			token.erase(std::remove(token.begin(), token.end(), '\n'),token.end()); //not sure if still necessary
			argvector.push_back(token);			
		}
	} 
	char* argv_from_file[argvector.size()];
	for(auto i=std::size_t{0}; i< argvector.size();++i) {
		argv_from_file[i] = new char[strlen(argvector[i].c_str())+1];
		strcpy(argv_from_file[i], argvector[i].c_str());
	}
	auto current_optind = optind;
	optind=1;
	parse(argvector.size(), argv_from_file);
	optind=current_optind; 
	file.close();
}

void ParameterHandler::parse(int argc, char* const argv[])  {
	auto short_options = holder.generate_short_options(is_encoder);
    const struct option* optionas = holder.generate_options_struct(is_encoder);    

    int opt;
    while((opt = getopt_long_only(argc, argv, short_options.c_str(), optionas, NULL)) > 0 ) {
    	if (opt == ':') {
    		std::cerr << "option " << holder.get_option_by_id(optopt).get_names()[0] << " expected argument" << std::endl;
    		
    		continue;
    	}
    	if (opt == '?') {
    		std::cerr << "Unknown option: " << std::string(argv[optind-1]) << std::endl;
    		continue;
    	}

    	auto obtained_option = holder.get_option_by_id(opt);
    	if (obtained_option.has_argument) {
    		obtained_option.handle(optarg);
    	} else {
    		obtained_option.handle();
    	}
    }
}

void ParameterHandler::parse_decoded_lightfield(const char* arg) {
	decoded_lightfield=fs::path(arg);
}

void ParameterHandler::parse_encoded_lightfield(const char* arg) {
	encoded_lightfield=fs::path(arg);
}

void ParameterHandler::parse_input(const char* arg) {
	if (is_encoder) {
		parse_decoded_lightfield(arg);
	} else {
		parse_encoded_lightfield(arg);
	}
}
void ParameterHandler::parse_output(const char* arg) {
	if (is_encoder) {
		parse_encoded_lightfield(arg);
	} else {
		parse_decoded_lightfield(arg);
	}
}

void ParameterHandler::instantiate_options() {
	//option_names, option_handler, short_option, argument, available_in_encoder, available_in_decoder
	holder.add_option({{"help"}, {[this](){this->print_help("MuLE");}}, 'h'}).set_synopsys(
		"Shows this help");

	holder.add_option({{"lambda"}, {[this](auto v){this->lambda=atof(v);}}, 'a'}).set_synopsys(
		"Sets the lambda value used for Lagrangian RD optimization. Requires a double as argument.");

	holder.add_option({{"color_transform"}, {[this](auto v){this->parse_color_transform(v);}}, 'c'});

	holder.add_option({{"decoded_lightfield"}, {[this](auto v){this->parse_decoded_lightfield(v);}}, 'd'});
	holder.add_option({{"extension_method"}, {[this](auto v){this->parse_extension_method(v);}}, 'e'});
	holder.add_option({{"input"}, {[this](auto v){this->parse_input(v);}}, 'i'});
	holder.add_option({{"encoded_lightfield", "lf"}, {[this](auto v){this->parse_encoded_lightfield(v);}}, 'l'});
	holder.add_option({{"output"}, {[this](auto v){this->parse_output(v);}}, 'o'});
	
	holder.add_option({{"transform_length_t"}, {[this](auto v){this->transform_length_t=atoi(v);}}, 't'});
	holder.add_option({{"transform_length_s"}, {[this](auto v){this->transform_length_s=atoi(v);}}, 's'});
	holder.add_option({{"transform_length_v"}, {[this](auto v){this->transform_length_v=atoi(v);}}, 'v'});
	holder.add_option({{"transform_length_u"}, {[this](auto v){this->transform_length_u=atoi(v);}}, 'u'});

	holder.add_option({{"min_transform_length_t", "min_t"}, {[this](auto v){this->min_transform_length_t=atoi(v);}}});
	holder.add_option({{"min_transform_length_s", "min_s"}, {[this](auto v){this->min_transform_length_s=atoi(v);}}});
	holder.add_option({{"min_transform_length_v", "min_v"}, {[this](auto v){this->min_transform_length_v=atoi(v);}}});
	holder.add_option({{"min_transform_length_u", "min_u"}, {[this](auto v){this->min_transform_length_u=atoi(v);}}});
	
	holder.add_option({{"transform_scale_t", "t_scale"}, {[this](auto v){this->transform_scale_t=atof(v);}}});
	holder.add_option({{"transform_scale_s", "s_scale"}, {[this](auto v){this->transform_scale_s=atof(v);}}});
	holder.add_option({{"transform_scale_v", "v_scale"}, {[this](auto v){this->transform_scale_v=atof(v);}}});
	holder.add_option({{"transform_scale_u", "u_scale"}, {[this](auto v){this->transform_scale_u=atof(v);}}});

	holder.add_option({{"is_lenslet_13x13", "lenslet_13x13", "lenslet13x13"}, {[this](){this->is_lenslet_13x13=true;}}});
	
	holder.add_option({{"number_of_vertical_views", "nv"}, {[this](auto v){this->number_of_vertical_views=atoi(v);}}});
	holder.add_option({{"number_of_horizontal_views", "nh"}, {[this](auto v){this->number_of_horizontal_views=atoi(v);}}});

	holder.add_option({{"first_horizontal_view_number", "off_h"}, {[this](auto v){this->first_horizontal_view_number=atoi(v);}}});
	holder.add_option({{"first_vertical_view_number", "off_v"}, {[this](auto v){this->first_vertical_view_number=atoi(v);}}});

	holder.add_option({{"configuration_file", "cf", "cfg"}, {[this](auto v){this->parse_configuration_file(v);}}});

	holder.add_option({{"verbose"}, {[this](){this->verbose=true;}}});
}
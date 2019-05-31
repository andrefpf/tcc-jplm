/*
* @Author: Ismael Seidel
* @Date:   2019-05-30 16:10:57
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-31 11:05:40
*/
#include <filesystem>
#include <iomanip>  //std::setw and std::setfill
#include <iostream>
#include <sstream>
#include "Image.h"
#include "ImageExceptions.h"
#include "ImageIO.h"
#include "PixelMapFileIO.h"


void shift_view(
    const std::string& filename_input, const std::string& filename_output) {
  auto view_file = PixelMapFileIO::open(filename_input);
  auto view_variant = view_file->read_full_image();
  std::visit(
      [filename_output](auto& view) {
        view->shift_pixels_by(2);
        ImageIO::write(*(view.get()), filename_output);
      },
      view_variant);
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

std::string get_view_name(
    const std::pair<std::size_t, std::size_t>& coordinate) {
  std::ostringstream string_stream;
  string_stream << std::setw(3) << std::setfill('0') << std::get<0>(coordinate)
                << '_' << std::setw(3) << std::setfill('0')
                << std::get<1>(coordinate) << ".ppm";
  return string_stream.str();
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

int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cout << "Expecting 2 params\n";
    std::cout << "\tUsage: " << argv[0]
              << " /path/to/input/directory/ /path/to/output/directory"
              << std::endl;
    exit(1);
  }

  std::string input_path(argv[1]);
  std::string output_path(argv[2]);

  if (!are_input_and_output_paths_valid(input_path, output_path)) {
    exit(1);
  }

  auto initial_t = 1;
  auto initial_s = 1;
  auto final_t = initial_t + 13;
  auto final_s = initial_s + 13;

  for (auto t = initial_t; t < final_t; ++t) {
    for (auto s = initial_s; s < final_s; ++s) {
      auto view_name = get_view_name({t, s});
      if ((t == initial_t) || (t == final_t - 1)) {
        if ((s == initial_s) || (s == final_s - 1)) {
          std::cout << "Shifting view " << view_name << '\n';
          shift_view({input_path + view_name}, {output_path + view_name});
          continue;
        }
      }
      std::cout << "Copying view " << view_name << '\n';
      copy_view({input_path + view_name}, {output_path + view_name});
    }
  }

  return 0;
}
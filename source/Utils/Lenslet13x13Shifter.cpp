/*
* @Author: Ismael Seidel
* @Date:   2019-05-30 16:10:57
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-30 18:04:35
*/
#include <iostream>
#include "Image.h"
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

  if (input_path == output_path) {
    std::cerr << "Input and output paths must be different" << std::endl;
  }

  // 0 0 -> 1 1
  // 0 12 -> 1 13
  // 12 0 -> 13 1
  // 12 12 -> 13 13
  std::vector<std::string> view_names(
      {"001_001.ppm", "001_013.ppm", "013_001.ppm", "013_013.ppm"});

  for (const auto& view_name : view_names) {
    shift_view({input_path + view_name}, {output_path + view_name});
  }

  return 0;
}
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

/** \file     PixelMapFileIO.cpp
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-07
 */


#include "PixelMapFileIO.h"

/**
 * \brief      Determines if a char is comment delimiter.
 *
 * \param[in]  c     The char to be checked
 *
 * \return     True if comment delimiter, False otherwise.
 */
bool is_comment_delimiter(char c) {
  const char sharp = 35;
  return (c == sharp);  // c#?
}


/**
 * \brief      Reads in a comment.
 *
 * \param      istream  The input stream
 */
void read_comment(std::ifstream& istream) {
  std::string comment;
  getline(istream, comment);
  std::cout << "Comment: " << comment << std::endl;
}


/**
 * \brief      Reads the pixel map stream until finding the next valid field.
 * \details    Reads, char-by-char the istream while getting spaces or comments.
 *
 * \param      istream  The input stream
 */
void read_pixel_map_stream_until_next_field(std::ifstream& istream) {
  char c;
  do {
    istream.get(c);
  } while (isspace(c));
  if (is_comment_delimiter(c)) {
    read_comment(istream);
    read_pixel_map_stream_until_next_field(istream);  //recursive call
  } else {
    istream.unget();  //returns the last character to the stream;
  }
}


/**
 * \brief      Fills in the string that holds a size.
 * \details Because of the recursive nature of this function we are using an in/out param.
 * The need for reading char-by-char is because there can be a comment between the chars of a field...
 *
 * \param[in]      istream  The input stream
 * \param[in/out]  size     The size
 */
void fill_size_string(std::ifstream& istream, std::string& size) {
  read_pixel_map_stream_until_next_field(istream);
  char c;
  istream.get(c);
  while (isdigit(c)) {
    size.push_back(c);
    istream.get(c);
  }
  if (is_comment_delimiter(c)) {
    read_comment(istream);
    fill_size_string(istream, size);  //recursive call;
  }
}


/**
 * \brief      Gets the file type.
 * \details    It is expected that the next char in the istream be a P, followed by a number in the range [0..6]
 *
 * \param      istream  The input stream
 *
 * \return     The file type.
 */
PixelMapType get_file_type(std::ifstream& istream) {
  std::string type;
  istream >> type;
  if (type[0] != 'P') {
    std::cerr << "Invalid file. Expecting P identifier. Got " << type[0]
              << std::endl;
    throw ImageIOExceptions::UnknownImageTypeException();
  }

  auto int_type = std::stoi(type.substr(1, 1));
  // std::cout << "readed a file type " << type << std::endl;

  if ((int_type > 0) && (int_type < 7)) {
    return static_cast<PixelMapType>(int_type);
  }

  std::cerr << "Invalid PPM Type. Expecting P1 to P6. Got " << type
            << std::endl;
  throw PixelMapFileIOExceptions::InvalidPixelMapIndexException();
}


/**
 * \brief      Gets the next size.
 * \details    a char-to-char reading is necessary because the container allows a comment in between the digits of a token...
 *
 * \param      istream  The input stream
 *
 * \return     The next size.
 */
std::size_t get_next_size(std::ifstream& istream) {
  std::string size("");
  fill_size_string(istream, size);
  return std::stoull(size);
}


// namespace PixelMapFileIO {

/**
 * \brief      opens a pixel map file from a filename.
 *
 * \param[in]  filename  The filename
 *
 * \return     A unique_ptr to the PixelMapFile
 */
std::unique_ptr<PixelMapFile> PixelMapFileIO::open(
    const std::string& filename) {

  ImageFile::check(filename);

  std::ifstream file(filename, std::ios::in);

  if (!file.is_open()) {
    std::cerr << "Unable to open file " << filename << "." << std::endl;
    throw ImageIOExceptions::UnableToOpenFileException();
  }

  auto type = get_file_type(file);
  auto width = get_next_size(file);
  if (width == 0) {
    throw PixelMapFileIOExceptions::WidthMustBeLargerThanZeroException();
  }

  auto height = get_next_size(file);
  if (height == 0) {
    throw PixelMapFileIOExceptions::HeightMustBeLargerThanZeroException();
  }
  auto max_value = get_next_size(file);
  if (max_value == 0) {
    throw PixelMapFileIOExceptions::BppMustBeLargerThanZeroException();
  }
  if (max_value > 65535) {  //(2^16)-1
    throw PixelMapFileIOExceptions::BppMustBeSmallerOrEqualToSixteenException();
  }


  read_pixel_map_stream_until_next_field(file);  //next field is the 'raster'

  auto raster_begin = file.tellg();

  file.close();

  switch (type) {
    case PixelMapType::P1:
      throw PixelMapFileIOExceptions::
          NoImplementedYetPixelMapFileException();  //return std::make_unique<PBMASCCIFile>(filename, raster_begin, width, height, max_value);
    case PixelMapType::P2:
      throw PixelMapFileIOExceptions::
          NoImplementedYetPixelMapFileException();  //return std::make_unique<PGMASCCIFile>(filename, raster_begin, width, height, max_value);
    case PixelMapType::P3:
      throw PixelMapFileIOExceptions::
          NoImplementedYetPixelMapFileException();  //return std::make_unique<PPMASCCIFile>(filename, raster_begin, width, height, max_value);
    case PixelMapType::P4:
      throw PixelMapFileIOExceptions::
          NoImplementedYetPixelMapFileException();  //return std::make_unique<PBMBinaryFile>(filename, raster_begin, width, height, max_value);
    case PixelMapType::P5:
      throw PixelMapFileIOExceptions::
          NoImplementedYetPixelMapFileException();  //return std::make_unique<PGMBinaryFile>(filename, raster_begin, width, height, max_value);
    case PixelMapType::P6:
      return std::make_unique<PPMBinaryFile>(
          filename, raster_begin, width, height, max_value);
    default: {
      throw PixelMapFileIOExceptions::InvalidPixelMapIndexException();
      std::cerr << "Not supported yet..." << std::endl;
    }
  }
}


std::unique_ptr<PixelMapFile> PixelMapFileIO::open(const std::string& filename,
    PixelMapType type, std::size_t width, std::size_t height,
    std::size_t max_value) {
  if (!std::filesystem::exists(filename)) {
    std::fstream file(filename, std::ios::out);
    if (file.is_open()) {
      file << 'P' << type << std::endl;
      file << width << " " << height << std::endl;
      file << max_value << std::endl;
      auto raster_begin = file.tellg();
      file.flush();
      file.close();
      switch (type) {
        case PixelMapType::P1:
          throw PixelMapFileIOExceptions::
              NoImplementedYetPixelMapFileException();  //return std::make_unique<PBMASCCIFile>(filename, raster_begin, width, height, max_value);
        case PixelMapType::P2:
          throw PixelMapFileIOExceptions::
              NoImplementedYetPixelMapFileException();  //return std::make_unique<PGMASCCIFile>(filename, raster_begin, width, height, max_value);
        case PixelMapType::P3:
          throw PixelMapFileIOExceptions::
              NoImplementedYetPixelMapFileException();  //return std::make_unique<PPMASCCIFile>(filename, raster_begin, width, height, max_value);
        case PixelMapType::P4:
          throw PixelMapFileIOExceptions::
              NoImplementedYetPixelMapFileException();  //return std::make_unique<PBMBinaryFile>(filename, raster_begin, width, height, max_value);
        case PixelMapType::P5:
          throw PixelMapFileIOExceptions::
              NoImplementedYetPixelMapFileException();  //return std::make_unique<PGMBinaryFile>(filename, raster_begin, width, height, max_value);
        case PixelMapType::P6:
          return std::make_unique<PPMBinaryFile>(
              filename, raster_begin, width, height, max_value);
        default: { std::cerr << "Not supported yet..." << std::endl; }
      }
    } else {
      std::cerr << "Unable to open file " << filename << "." << std::endl;
    }
  } else {
    auto existing_image = open(filename);
    if (existing_image->has_properties(type, width, height, max_value))
      return existing_image;
    std::cerr
        << "This image file already exists and has differente properties.";
  }

  exit(2);
}
/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     BoxParserHelperBase.h
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */
#ifndef JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPERBASE_H__
#define JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPERBASE_H__


#include <cstdint>
#include "Lib/Utils//Stream/BinaryTools.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserHelperBase {
 protected:
  ManagedStream managed_stream;
  uint64_t length;
  uint32_t t_box_value_in_stream;
  bool has_xl_field = false;


  /**
   * @brief      Gets the l box value from stream.
   *
   * @return     The l box value from stream.
   */
  uint32_t get_l_box_value_from_stream();


  /**
   * @brief      Gets the xl box value from stream.
   *
   * @return     The xl box value from stream.
   */
  uint64_t get_xl_box_value_from_stream();


  /**
   * @brief      Gets the t box value from stream.
   *
   * @return     The t box value from stream.
   */
  uint32_t get_t_box_value_from_stream();


  /**
   * @brief      Constructs a new instance.
   *
   * @param      stream                 The stream
   * @param[in]  length                 The length
   * @param[in]  t_box_value_in_stream  The t box value in stream
   * @param[in]  has_xl_field           Indicates if xl field
   */
  BoxParserHelperBase(ManagedStream& stream, uint64_t length,
      uint32_t t_box_value_in_stream, bool has_xl_field);


 public:
  /**
   * @brief      Constructs a new instance.
   *
   * @param      stream  The stream
   */
  BoxParserHelperBase(ManagedStream& stream);


  /**
   * @brief      Gets the helper with protected range.
   *
   * @return     The helper with protected range.
   */
  BoxParserHelperBase get_helper_with_protected_range();


  /**
   * @brief      Gets the length.
   *
   * @return     The length.
   */
  uint64_t get_length() const noexcept;


  /**
   * @brief      Gets the t box value.
   *
   * @return     The t box value.
   */
  uint32_t get_t_box_value() const noexcept;


  /**
   * @brief      Gets the non data length.
   *
   * @return     The non data length.
   */
  uint64_t get_non_data_length() const noexcept;


  /**
   * @brief      Gets the data length.
   *
   * @return     The data length.
   */
  uint64_t get_data_length() const noexcept;


  /**
   * @brief      Gets the data stream.
   *
   * @return     The data stream.
   */
  ManagedStream get_data_stream();


  /**
   * @brief      Gets the remaining stream.
   *
   * @return     The remaining stream.
   */
  ManagedStream get_remaining_stream();


  /**
   * @brief      Determines if data available.
   *
   * @return     True if data available, False otherwise.
   */
  bool has_data_available() const noexcept;


  /**
   * @brief      Determines the current position on the stream
   *
   * @return     Current position on the stream
   */
  uint64_t tell() const noexcept;


  /**
   * \brief      Gets the next value with type T from the stream.
   *
   * \tparam     T     The type that must be read from the stream
   *
   * \return     The value readed from the stream.
   * \sideeffect{The stream position is sizeof(T) bytes ahead its position before calling this method}
   */
  template<typename T>
  T get_next();


  /** 
   * \brief      Gets the next n values with type T from the stream..
   *
   * \tparam     T     The type that must be read from the stream
   * \tparam     n     { description }
   *
   * \return     Vector with n values with type T from the stream.
   * \sideeffect{The stream position is n*sizeof(T) bytes ahead its position before calling this method}
   */
  template<typename T, std::size_t n>
  std::vector<T> get_next();


  template<typename T>
  std::vector<T> get_next(const std::size_t n);


  /**
   * \brief      Gets the next value with type T from the stream.
   *
   * \tparam     T     The type that must be read from the stream
   *
   * \return     The value readed from the stream.
   * \sideeffect{The stream position is sizeof(T) bytes ahead its position before calling this method}
   */
  template<typename... Args>
  std::enable_if_t<(sizeof...(Args) > 1), std::tuple<Args...>> get_next();


  // template<class BoxToParse>
  // std::unique_ptr<BoxToParse> get_next_box() {
  //     auto managed_substream = managed_stream.get_remaining_sub_managed_stream();
  //     auto box_parser_helper = BoxParserHelperBase(managed_substream);
  //     if(!box_parser_helper.is_a_box_with_id(BoxToParse::id)) {
  //       managed_substream.rewind();
  //       return nullptr;
  //     }
  //     return BoxParserRegistry::get_instance().parse<BoxToParse>(managed_substream);
  // }

  // template<class NextParsingBox>
  // bool is_next_a_box() {
  //   auto current_pos = managed_stream.get_current_pos();
  //   if(!managed_stream.is_valid(current_pos+8)) {
  //     return false;
  //   }

  // }


  /**
    * @brief      Determines whether the specified identifier is a box with identifier.
    *
    * @param[in]  id    The identifier
    *
    * @return     True if the specified identifier is a box with identifier, False otherwise.
    */
  bool is_a_box_with_id(const uint32_t id);


  /**
   * @brief      Destroys the object.
   */
  virtual ~BoxParserHelperBase() = default;
};


template<typename T>
std::vector<T> BoxParserHelperBase::get_next(const std::size_t n) {
  using namespace BinaryTools;
  auto ret_vect = std::vector<T>();
  ret_vect.reserve(n);
  auto readed_vector_from_stream = managed_stream.get_n_bytes(n * sizeof(T));
  std::size_t pos = 0;
  for (auto i = decltype(n){0}; i < n; ++i) {
    ret_vect.push_back(get_value_from_big_endian_byte_vector<T>(
        readed_vector_from_stream, pos));
    pos += sizeof(T);
  }
  return ret_vect;
}


template<typename T, std::size_t n>
std::vector<T> BoxParserHelperBase::get_next() {
  using namespace BinaryTools;
  auto ret_vect = std::vector<T>();
  ret_vect.reserve(n);
  constexpr auto bytes_to_read = sizeof(T) * n;
  auto readed_vector_from_stream = managed_stream.get_bytes<bytes_to_read>();
  std::size_t pos = 0;
  for (auto i = decltype(n){0}; i < n; ++i) {
    ret_vect.push_back(get_value_from_big_endian_byte_vector<T>(
        readed_vector_from_stream, pos));
    pos += sizeof(T);
  }
  return ret_vect;
}


template<typename... Args>
std::enable_if_t<(sizeof...(Args) > 1), std::tuple<Args...>>
BoxParserHelperBase::get_next() {
  using namespace BinaryTools;
  return get_tuple_from_big_endian_byte_vector<Args...>(
      managed_stream.get_bytes<(sizeof(Args) + ... + 0)>());
}


template<typename T>
T BoxParserHelperBase::get_next() {
  using namespace BinaryTools;
  return get_value_from_big_endian_byte_vector<T>(
      managed_stream.get_bytes<sizeof(T)>());
}


#endif  //JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSERHELPERBASE_H__

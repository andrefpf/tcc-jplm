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

/** \file     XMLContents.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-20
 */

#include "Lib/Common/Boxes/Generic/XMLContents.h"


XMLContents *XMLContents::clone() const {
  return new XMLContents(*this);
}


uint64_t XMLContents::size() const noexcept {
  return contents.size();
  //4 for ver and location + the size of the string + the null termination char
}


bool XMLContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other = dynamic_cast<const XMLContents &>(other);
  return *this == cast_other;
}


bool XMLContents::operator==(const XMLContents &other) const {
  return this->contents == other.contents;
}


bool XMLContents::operator!=(const XMLContents &other) const {
  return !this->operator==(other);
}


const std::string &XMLContents::get_string_with_contents() const noexcept {
  return contents;
}


std::vector<std::byte> XMLContents::get_bytes() const noexcept {
  auto bytes = std::vector<std::byte>(this->size());
  // bytes.reserve(this->size());

  std::cout << "contents.size() = " << contents.size() << std::endl;
  std::cout << "contents = " << contents << std::endl;
  std::cout << "bytes size = " << bytes.size() << std::endl;

  std::transform(contents.begin(), contents.end(), bytes.begin(),
      [](const auto &character) { return std::byte(character); });

  std::cout << "bytes size = " << bytes.size() << std::endl;

  return bytes;
}


void XMLContents::set_contents(const std::string &new_content) {
  contents = new_content;
}


void XMLContents::set_contents(std::string &&new_content) {
  contents = std::move(new_content);
}
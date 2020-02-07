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

/** \file     ContiguousCodestreamContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCONTENTS_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCONTENTS_H__

#include <memory>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"
#include "Lib/Common/Boxes/SuperBoxDBox.h"


class ContiguousCodestreamContents : public SuperBoxDBox {
  //not implemented yet...
 protected:
  std::unique_ptr<ContiguousCodestreamCode> code;

 public:
  ContiguousCodestreamContents(const ContiguousCodestreamCode& code)
      : code(std::unique_ptr<ContiguousCodestreamCode>(code.clone())) {
  }


  ContiguousCodestreamContents(std::unique_ptr<ContiguousCodestreamCode>&& code)
      : code(std::move(code)) {
  }


  ContiguousCodestreamContents(const ContiguousCodestreamContents& other)
      : code(std::unique_ptr<ContiguousCodestreamCode>(other.code->clone())){}


            //! \todo remove this method (it is not necessary to be defined as const in its name)
            [[nodiscard]] const ContiguousCodestreamCode
        & get_const_ref_to_code() const;


  [[nodiscard]] const ContiguousCodestreamCode& get_ref_to_code() const;


  ContiguousCodestreamCode& get_ref_to_code();


  uint64_t size() const noexcept override;


  ContiguousCodestreamContents* clone() const override;


  bool is_equal(const DBox& other) const override;


  bool operator==(const ContiguousCodestreamContents& other) const;


  bool operator!=(const ContiguousCodestreamContents& other) const;


  std::ostream& write_to(std::ostream& stream) const final;
};


#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCONTENTS_H__ */
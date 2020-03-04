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

/** \file     RDCostResult.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-02
 */

#ifndef JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_RD_COST_RESULT_H
#define JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_RD_COST_RESULT_H


class RDCostResult {
 protected:
  double j_cost;
  double error;
  double rate;
  double energy;

 public:
  RDCostResult(double j, double d, double r, double e)
      : j_cost(j), error(d), rate(r), energy(e) {
  }


  virtual ~RDCostResult() = default;


  /**
   * @brief      Gets the rate.
   *
   * @return     The rate.
   */
  double get_rate() const {
    return rate;
  }


  void add_to_rate(double r) {
    rate += r;
  }


  /**
   * @brief      Gets the j cost.
   *
   * @return     The j cost.
   */
  double get_j_cost() const {
    return j_cost;
  }


  void set_j_cost(double j) {
    j_cost = j;
  }


  void add_to_j_cost(double j) {
    j_cost += j;
  }


  /**
   * @brief      Gets the error.
   *
   * @return     The error.
   */
  double get_error() const {
    return error;
  }


  void add_to_error(double d) {
    error += d;
  }


  /**
   * @brief      Gets the energy.
   *
   * @return     The energy.
   */
  double get_energy() const {
    return energy;
  }


  void add_to_energy(double d) {
    energy += d;
  }


  /**
   * @brief      Addition operator.
   *
   * @param[in]  other  The other
   *
   * @return     The result of the addition
   */
  RDCostResult operator+(const RDCostResult& other) const {
    return RDCostResult(this->j_cost + other.j_cost, this->error + other.error,
        this->rate + other.rate, this->energy + other.energy);
  }
};


#endif  // JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_RD_COST_RESULT_H
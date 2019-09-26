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

/** \file     LightField.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef LIGHTFIELD_H__
#define LIGHTFIELD_H__

#include "View.h"
#include "Block4D.h"

class LightField {
public:    
    char *mViewFileNamePrefix = nullptr;  /*!< lightfield view name: <mViewFileNamePrefix>_<horizontal index>_<vertical index>_<mViewFilenameSuffix>.pgm */
    char *mViewFileNameSuffix = nullptr;  /*!< lightfield view name: <mViewFileNamePrefix>_<horizontal index>_<vertical index>_<mViewFilenameSuffix>.pgm */
    int mNumberOfHorizontalDigits = 3;    /*!< number of digits used to represent the horizontal index in the view file names */
    int mNumberOfVerticalDigits = 3;      /*!< number of digits used to represent the vertical index in the view file names */
    int mNumberOfHorizontalViews = 0;     /*!< total number of horizontal views of the lightfield */
    int mNumberOfVerticalViews = 0;       /*!< total number of vertical views of the lightfield */
    View **mViewCache=nullptr;            /*!< pointer to the two dimensional circular separable cache of views*/
    int mNumberOfCacheHorizontalViews=0;  /*!< number of horizontal views of the lightfield in the cache */
    int mNumberOfCacheVerticalViews=0;    /*!< number of vertical views of the lightfield in the cache */
    int mFirstCacheHorizontalView=0;      /*!< horizontal index of the first cached view */
    int mFirstCacheVerticalView=0;        /*!< vertical index of the first cached view */
    int mHorizontalIndexOffset=0;         /*!< position of the first horizontal view in the circular cache */
    int mVerticalIndexOffset=0;           /*!< position of the first vertical view in the circular cache */
    char mReadOrWriteLightField=0;        /*!< read or write flag */
    int mNumberOfViewLines=0;             /*!< vertical resolution of each view */
    int mNumberOfViewColumns=0;           /*!< horizontal resolution of each view */
    int mPGMScale=0;                      /*!< scale of the pgm files*/
    int mVerticalViewNumberOffset=0;      /*!< number of vertical views to skip */
    int mHorizontalViewNumberOffset=0;    /*!< number of horizontal views to skip */
    int mViewType=1;                      /*!< mViewType = 0 -> PGM, mViewType = 1 -> PPM */    
    LightField() = default;
    LightField(int numberOfCacheVerticalViews, int numberOfCacheHorizontalViews, int numberOfViewCacheLines);
    ~LightField();
    void set_configurations(int numberOfCacheVerticalViews, int numberOfCacheHorizontalViews, int numberOfViewCacheLines);
    void OpenLightFieldPGM(const char *viewFileNamePrefix, char *viewFileNameSuffix, int numberOfVerticalViews, int numberOfHorizontalViews, int numberOfVerticalDigits, int numberOfHorizontalDigits, char readOrWriteLightField);
    void OpenLightFieldPPM(const char *viewFileNamePrefix, char *viewFileNameSuffix, int numberOfVerticalViews, int numberOfHorizontalViews, int numberOfVerticalDigits, int numberOfHorizontalDigits, char readOrWriteLightField);
    void CloseLightField();
    void ReadBlock4DfromLightField(Block4D *targetBlock, int position_t, int position_s, int position_v, int position_u, int component=0);
    void WriteBlock4DtoLightField(Block4D *targetBlock, int position_t, int position_s, int position_v, int position_u, int component=0);
    int FindViewFileName(char *viewFileName, int index_t, int index_s);
    void SetViewVerbosity(char verbosity);
};

#endif /* end of include guard: LIGHTFIELD_H__ */
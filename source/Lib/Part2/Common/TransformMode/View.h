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

/** \file     View.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 *  \note Temporary file... It will be removed during the refactorings
 */



#ifndef VIEW_H__
#define VIEW_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "Block4D.h"

#define PixelType unsigned short int 

#define DefaultPixelValue 0

class View
{
public:
    FILE *mViewFilePointer;                     /*!< pointer to view file */
    PixelType *mPixelData = nullptr;            /*!< pointer to the circular array of pixels for the line cache */
    int mNumberCacheLines;                      /*!< maximum number of view lines in the cache */
    int mFirstPixelPosition;                    /*!< position of the first pixel of the first loaded line in the circular cache */
    int mLines;                                 /*!< view number of lines */
    int mColumns;                               /*!< view number of columns */
    int mLastLine;                              /*!< last view line index currently loaded in the cache */
    int mFirstLine;                             /*!< first view line index currently loaded in the cache */
    int mNumberOfPixelsInCache;                 /*!< total number of valid pixels in the cache */
    char mReadOrWriteView;                      /*!< write or read view type flag */
    int mNumberOfFileBytesPerPixelComponent;    /*!< pixel precision in bytes */
    int mNumberOfBands;                         /*!< number of bands of a multispectral image */
    int mPGMScale;                              /*!< scale of the PGM view file */
    long int mPGMDataOffset;                    /*!< PGM header size in bytes */
    char mUnavailable;                          /*!< if set the view contains no data */
    char mVerbosity;                            /*!< indicates the verbosity level for console messages */
    View(void);
    ~View(void);
    void SetCacheSize(int maxBlockLines);
    void OpenViewFilePGM(char *name, char readOrWriteView);
    int OpenViewFilePPM(char *name, char readOrWriteView);
    void CloseViewFile(void);
    PixelType GetPixel(int position_v, int position_u, int component=0);
    void PutPixel(PixelType pixelValue, int position_v, int position_u, int component=0);
    void GetBlock2D(block4DElementType **pixel, int position_v, int position_u, int length_v, int length_u, int component=0);
    void PutBlock2D(block4DElementType **pixelValue, int position_v, int position_u, int length_v, int length_u, int component=0);
    void ReadPixelFromFile(int pixelPositionInCache);
    void WritePixelToFile(int pixelPositionInCache);    
 };

#endif /* end of include guard: VIEW_H__ */
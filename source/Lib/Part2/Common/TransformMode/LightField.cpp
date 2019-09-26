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

/** \file     LightField.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 *  \note Temporary file... It will be removed during the refactorings
 */

#include "LightField.h"

/*******************************************************************************/
/*                        LightField class methods                             */
/*******************************************************************************/

LightField :: LightField(int numberOfCacheVerticalViews, int numberOfCacheHorizontalViews, int numberOfViewCacheLines) {
    set_configurations(numberOfCacheVerticalViews, numberOfCacheHorizontalViews, numberOfViewCacheLines);
}

void LightField::set_configurations(
    int numberOfCacheVerticalViews, 
    int numberOfCacheHorizontalViews, 
    int numberOfViewCacheLines) {
    mNumberOfCacheHorizontalViews = numberOfCacheHorizontalViews;
    mNumberOfCacheVerticalViews = numberOfCacheVerticalViews;
    
    mViewCache = new View * [mNumberOfCacheVerticalViews];
    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
        mViewCache[index_t] = new View [mNumberOfCacheHorizontalViews];
        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
            mViewCache[index_t][index_s].SetCacheSize(numberOfViewCacheLines);
        }
    }
    mReadOrWriteLightField = 'r';
}

LightField::~LightField() {    
    if (mViewCache != nullptr) {
        for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
           delete [] mViewCache[index_t];
        }
        delete [] mViewCache;
    }
}

void LightField :: OpenLightFieldPGM(
    const char *viewFileNamePrefix, 
    char *viewFileNameSuffix, 
    int numberOfVerticalViews, 
    int numberOfHorizontalViews, 
    int numberOfVerticalDigits, 
    int numberOfHorizontalDigits, 
    char readOrWriteLightField) {



/*! opens lightfield pgm views for reading or writing */
    
    mViewFileNamePrefix = new char [strlen(viewFileNamePrefix)+1];
    mViewFileNameSuffix = new char [strlen(viewFileNameSuffix)+1];
    
    strcpy(mViewFileNamePrefix, viewFileNamePrefix);
    strcpy(mViewFileNameSuffix, viewFileNameSuffix);
    
    mNumberOfVerticalViews = numberOfVerticalViews;
    mNumberOfHorizontalViews = numberOfHorizontalViews;
    
    mNumberOfVerticalDigits = numberOfVerticalDigits;
    mNumberOfHorizontalDigits =  numberOfHorizontalDigits; 
    
    mReadOrWriteLightField = readOrWriteLightField;
    
    int endOfStrByte = 1;
    char *ViewFileName = new char[strlen(viewFileNamePrefix)+strlen(viewFileNameSuffix)+mNumberOfVerticalDigits+mNumberOfHorizontalDigits+1 + endOfStrByte];
    
    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews && index_t < mNumberOfVerticalViews; index_t++) {
        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews && index_s < mNumberOfHorizontalViews; index_s++) {
            
            FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
            //printf("Opening %s view file\n", ViewFileName);
            if(mReadOrWriteLightField == 'w') {
                mViewCache[index_t][index_s].mLines = mNumberOfViewLines;
                mViewCache[index_t][index_s].mColumns = mNumberOfViewColumns;
                mViewCache[index_t][index_s].mPGMScale = mPGMScale;
            }
            mViewCache[index_t][index_s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
        }
    }
    delete [] ViewFileName;
    if(mReadOrWriteLightField == 'r') {
        mNumberOfViewLines = mViewCache[0][0].mLines;
        mNumberOfViewColumns = mViewCache[0][0].mColumns;
        mPGMScale = mViewCache[0][0].mPGMScale;
    }
}

void LightField :: OpenLightFieldPPM(
    const char *viewFileNamePrefix, 
    char *viewFileNameSuffix, 
    int numberOfVerticalViews, 
    int numberOfHorizontalViews, 
    int numberOfVerticalDigits, 
    int numberOfHorizontalDigits, 
    char readOrWriteLightField) {

    mViewType = 1;
    
    mViewFileNamePrefix = new char [strlen(viewFileNamePrefix)+1];
    mViewFileNameSuffix = new char [strlen(viewFileNameSuffix)+1];
    
    strcpy(mViewFileNamePrefix, viewFileNamePrefix);
    strcpy(mViewFileNameSuffix, viewFileNameSuffix);
    
    mNumberOfVerticalViews = numberOfVerticalViews;
    mNumberOfHorizontalViews = numberOfHorizontalViews;
    
    mNumberOfVerticalDigits = numberOfVerticalDigits;
    mNumberOfHorizontalDigits =  numberOfHorizontalDigits; 
    
    mReadOrWriteLightField = readOrWriteLightField;
    
    int endOfStrByte = 1;
    char *ViewFileName = new char[strlen(viewFileNamePrefix)+strlen(viewFileNameSuffix)+mNumberOfVerticalDigits+mNumberOfHorizontalDigits+1 + endOfStrByte];
    
    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews && index_t < mNumberOfVerticalViews; index_t++) {
        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews && index_s < mNumberOfHorizontalViews; index_s++) {
            
            FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
            //printf("Opening %s view file\n", ViewFileName);
            if(mReadOrWriteLightField == 'w') {
                mViewCache[index_t][index_s].mLines = mNumberOfViewLines;
                mViewCache[index_t][index_s].mColumns = mNumberOfViewColumns;
                mViewCache[index_t][index_s].mPGMScale = mPGMScale;
            }
            mViewCache[index_t][index_s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
        }
    }
    delete [] ViewFileName;
    if(mReadOrWriteLightField == 'r') {
        mNumberOfViewLines = mViewCache[0][0].mLines;
        mNumberOfViewColumns = mViewCache[0][0].mColumns;
        mPGMScale = mViewCache[0][0].mPGMScale;
    }
}

void LightField :: CloseLightField() {
/*! closes all opened view files*/
    
    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
            mViewCache[index_t][index_s].CloseViewFile();
        }
    }
    
    if(mViewFileNamePrefix != NULL) {
        delete [] mViewFileNamePrefix;
        mViewFileNamePrefix = NULL;
    }
    if(mViewFileNameSuffix != NULL) {
        delete [] mViewFileNameSuffix;
        mViewFileNameSuffix = NULL;
    }
        
    mNumberOfVerticalViews = 0;
    mNumberOfHorizontalViews = 0;
    
    mNumberOfVerticalDigits = 0;
    mNumberOfHorizontalDigits =  0;  
    
}
void LightField :: ReadBlock4DfromLightField(Block4D *targetBlock, int position_t, int position_s, int position_v, int position_u, int component) {
/*! reads a 4 dimensional block from the lightfield at position (position_t,position_s,position_v,position_u). */
/*! position_t is the vertical view index; */
/*! position_s is the horizontal view index; */
/*! position_v is the line index; */
/*! position_u is the column index. */    

    //if((position_t >= mFirstCacheVerticalView)&&(position_t + targetBlock->mlength_t <= mFirstCacheVerticalView + mNumberOfCacheVerticalViews)&&(position_s >= mFirstCacheHorizontalView)&&(position_s +targetBlock->mlength_s <= mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews)) {
    if((position_t >= mFirstCacheVerticalView)&&((position_t + targetBlock->mlength_t < mNumberOfVerticalViews ? position_t + targetBlock->mlength_t : mNumberOfVerticalViews) <= mFirstCacheVerticalView + mNumberOfCacheVerticalViews)&&(position_s >= mFirstCacheHorizontalView)&&((position_s +targetBlock->mlength_s < mNumberOfHorizontalViews ? position_s +targetBlock->mlength_s : mNumberOfHorizontalViews ) <= mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews)) {
        //The requested block is inside the current cache of views. No need to open new ones
        for(int index_t = position_t-mFirstCacheVerticalView; index_t < position_t-mFirstCacheVerticalView+targetBlock->mlength_t; index_t++) {
            for(int index_s = position_s-mFirstCacheHorizontalView; index_s < position_s-mFirstCacheHorizontalView+targetBlock->mlength_s; index_s++) {
 		if((index_s + mFirstCacheHorizontalView < mNumberOfHorizontalViews)&&(index_t + mFirstCacheVerticalView < mNumberOfVerticalViews)) {
                    int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                    int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                    //mViewCache[v][u].GetBlock2D(targetBlock->mPixel[index_t][index_s],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u);
                    mViewCache[t][s].GetBlock2D(targetBlock->mPixel[index_t-position_t+mFirstCacheVerticalView][index_s-position_s+mFirstCacheHorizontalView],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u, component);
                }
            }
        }        
    }
    else {
	const int endOfStrByte = 1;
        char *ViewFileName = new char[strlen(mViewFileNamePrefix)+strlen(mViewFileNameSuffix)+mNumberOfVerticalDigits+mNumberOfHorizontalDigits+1 + endOfStrByte];
        int numberOfVerticalViewsToRead;
        int numberOfHorizontalViewsToRead;
        int numberOfVerticalViewsToKeep;
        int numberOfHorizontalViewsToKeep;
        if(position_t + targetBlock->mlength_t > mFirstCacheVerticalView + mNumberOfCacheVerticalViews) {
            if(position_s + targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                //and mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
					numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
					numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if((index_t < numberOfVerticalViewsToRead)||(index_s < numberOfHorizontalViewsToRead))
							mViewCache[t][s].CloseViewFile();
                    }                   
                }
                
                mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews) mHorizontalIndexOffset = 0;
                mVerticalIndexOffset += numberOfVerticalViewsToRead;
                if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews) mVerticalIndexOffset = 0;
                
                
                for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                            if(mViewType == 0)
                                mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                            if(mViewType == 1)
                                mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                        }
                    }                   
                }
                for(int index_t = 0; index_t < numberOfVerticalViewsToKeep; index_t++) {
                    for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                            if(mViewType == 0)
                                mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                            if(mViewType == 1)
                                mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                        }
                    }                   
                }
            }
            else {
                if(position_s >= mFirstCacheHorizontalView) {
                    //new status after reading: mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                    numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                    for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            mViewCache[t][s].CloseViewFile();
                        }                   
                    }

                    mVerticalIndexOffset += numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews)
						mVerticalIndexOffset = 0;
                    
                    for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }
                    }
                  
                }
                else {
                    //new status after reading: mFirstCacheHorizontalView = position_s
                    //and mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                   
                    numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                    numberOfHorizontalViewsToRead = mFirstCacheHorizontalView - position_s;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                    mFirstCacheHorizontalView = position_s;
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if((index_t < numberOfVerticalViewsToRead)||(index_s >= numberOfHorizontalViewsToKeep))
				mViewCache[t][s].CloseViewFile();
                        }                   
                    }
                    
                    mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset < 0)
						mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;
                    mVerticalIndexOffset += numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews)
						mVerticalIndexOffset = 0;
                    
                    for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                    for(int index_t = 0; index_t < numberOfVerticalViewsToKeep; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
            }
        }
        else {
            if(position_t >= mFirstCacheVerticalView) {
                if(position_s +targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                    //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                    
                    numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                     for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            mViewCache[t][s].CloseViewFile();
                        }
                     }
                    
                    mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews)
						mHorizontalIndexOffset = 0;
                    
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
                else { //position_s < mFirstCacheHorizontalView
                    //new status after reading: mFirstCacheHorizontalView = position_s
                    
                    numberOfHorizontalViewsToRead = mFirstCacheHorizontalView - position_s;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheHorizontalView = position_s;
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset+numberOfHorizontalViewsToKeep)%mNumberOfHorizontalViews;
                            mViewCache[t][s].CloseViewFile();
                        }
                    }
                    
                    mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset < 0)
						mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;
                    
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
            }
            else {
                if(position_s +targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                    //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                    //and mFirstCacheVerticalView = position_t
                    numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                    numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                    mFirstCacheVerticalView = position_t;
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if((index_s < numberOfHorizontalViewsToRead)||(index_t >= numberOfVerticalViewsToKeep)) 
				mViewCache[t][s].CloseViewFile();
                        }                   
                    }
                    
                    mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset < 0)
						mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                    mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews)
						mHorizontalIndexOffset = mHorizontalIndexOffset%mNumberOfCacheHorizontalViews;
                    
                    for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                    for(int index_s = 0; index_s < numberOfHorizontalViewsToKeep; index_s++) {
                        for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }                     
                }
                else { 
                    if(position_s >= mFirstCacheHorizontalView) {
                        //and mFirstCacheVerticalView = position_t
                        numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                        if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
							numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                        numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                        mFirstCacheVerticalView = position_t;
                        for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                mViewCache[t][s].CloseViewFile();
                             }
                        }
                        
                        mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                        if(mVerticalIndexOffset < 0)
							mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                        
                        for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                            for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }
                       
                    }
                    else { //position_s < mFirstCacheHorizontalView
                        //new status after reading: mFirstCacheVerticalView = position_t
                        //new status after reading: mFirstCacheHorizontalView = position_s
                      
                        numberOfHorizontalViewsToRead =  mFirstCacheHorizontalView - position_s;
                        numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                        if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
							numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                        if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
							numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                        numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                        numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                        mFirstCacheHorizontalView = position_s;
                        mFirstCacheVerticalView = position_t;
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if((index_s >= numberOfHorizontalViewsToKeep)||(index_t >= numberOfVerticalViewsToKeep)) 
                                    mViewCache[t][s].CloseViewFile();
                            }                   
                        }

                        mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                        if(mVerticalIndexOffset < 0)
							mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                        mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                        if(mHorizontalIndexOffset < 0)
							mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;

                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            for(int index_t = numberOfVerticalViewsToRead; index_t < mNumberOfCacheVerticalViews; index_t++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }                     
                    }
                    
                }
            
            }
        }
        delete [] ViewFileName;
        
        for(int index_t = position_t-mFirstCacheVerticalView; index_t < position_t-mFirstCacheVerticalView+targetBlock->mlength_t; index_t++) {
            for(int index_s = position_s-mFirstCacheHorizontalView; index_s < position_s-mFirstCacheHorizontalView+targetBlock->mlength_s; index_s++) {
 		if((index_s + mFirstCacheHorizontalView < mNumberOfHorizontalViews)&&(index_t + mFirstCacheVerticalView < mNumberOfVerticalViews)) {
                    int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                    int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                    //mViewCache[v][u].GetBlock2D(targetBlock->mPixel[index_t][index_s],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u);
                    mViewCache[t][s].GetBlock2D(targetBlock->mPixel[index_t-position_t+mFirstCacheVerticalView][index_s-position_s+mFirstCacheHorizontalView],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u, component);
                }
            }
        }
        
    }
}
void LightField :: WriteBlock4DtoLightField(Block4D *targetBlock, int position_t, int position_s, int position_v, int position_u, int component) {
/*! writes a 4 dimensional block to the lightfield at position (position_t,position_s,position_v,position_u). */
/*! position_t is the vertical view index; */
/*! position_s is the horizontal view index; */
/*! position_v is the line index; */
/*! position_u is the column index. */    
    
    //if((position_t >= mFirstCacheVerticalView)&&(position_t + targetBlock->mlength_t <= mFirstCacheVerticalView + mNumberOfCacheVerticalViews)&&(position_s >= mFirstCacheHorizontalView)&&(position_s +targetBlock->mlength_s <= mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews)) {
    if((position_t >= mFirstCacheVerticalView)&&((position_t + targetBlock->mlength_t < mNumberOfVerticalViews ? position_t + targetBlock->mlength_t : mNumberOfVerticalViews) <= mFirstCacheVerticalView + mNumberOfCacheVerticalViews)&&(position_s >= mFirstCacheHorizontalView)&&((position_s +targetBlock->mlength_s < mNumberOfHorizontalViews ? position_s +targetBlock->mlength_s : mNumberOfHorizontalViews ) <= mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews)) {
        //The requested block is inside the current cache of views. No need to open new ones
        for(int index_t = position_t-mFirstCacheVerticalView; index_t < position_t-mFirstCacheVerticalView+targetBlock->mlength_t; index_t++) {
            for(int index_s = position_s-mFirstCacheHorizontalView; index_s < position_s-mFirstCacheHorizontalView+targetBlock->mlength_s; index_s++) {
 		if((index_s + mFirstCacheHorizontalView < mNumberOfHorizontalViews)&&(index_t + mFirstCacheVerticalView < mNumberOfVerticalViews)) {
                    int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                    int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                    //mViewCache[v][u].PutBlock2D(targetBlock->mPixel[index_t][index_s],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u);
                    mViewCache[t][s].PutBlock2D(targetBlock->mPixel[index_t-position_t+mFirstCacheVerticalView][index_s-position_s+mFirstCacheHorizontalView],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u, component);
                }
            }
        }        
    }
    else {
	int endOfStrByte = 1;
        char *ViewFileName = new char[strlen(mViewFileNamePrefix)+strlen(mViewFileNameSuffix)+mNumberOfVerticalDigits+mNumberOfHorizontalDigits+1 + endOfStrByte];
        int numberOfVerticalViewsToRead;
        int numberOfHorizontalViewsToRead;
        int numberOfVerticalViewsToKeep;
        int numberOfHorizontalViewsToKeep;
        if(position_t + targetBlock->mlength_t > mFirstCacheVerticalView + mNumberOfCacheVerticalViews) {
            if(position_s + targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                //and mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
					numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
					numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if((index_t < numberOfVerticalViewsToRead)||(index_s < numberOfHorizontalViewsToRead))
                            mViewCache[t][s].CloseViewFile();
                    }                   
                }
                
                mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews)
					mHorizontalIndexOffset = 0;
                mVerticalIndexOffset += numberOfVerticalViewsToRead;
                if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews)
					mVerticalIndexOffset = 0;
                
                
                for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                            if(mViewType == 0)
                                mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                            if(mViewType == 1)
                                mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                        }
                   }                   
                }
                for(int index_t = 0; index_t < numberOfVerticalViewsToKeep; index_t++) {
                    for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                        int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                        int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                        if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                            if(mViewType == 0)
                                mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                            if(mViewType == 1)
                                mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                        }
                    }                   
                }
            }
            else {
                if(position_s >= mFirstCacheHorizontalView) {
                    //new status after reading: mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                    numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                    for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            mViewCache[t][s].CloseViewFile();
                        }                   
                    }

                    mVerticalIndexOffset += numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews)
						mVerticalIndexOffset = 0;
                    
                    for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                  
                }
                else {
                    //new status after reading: mFirstCacheHorizontalView = position_s
                    //and mFirstCacheVerticalView = position_t + targetBlock->mlength_t - mNumberOfCacheVerticalViews
                   
                    numberOfVerticalViewsToRead = position_t + targetBlock->mlength_t - mFirstCacheVerticalView - mNumberOfCacheVerticalViews;
                    numberOfHorizontalViewsToRead = mFirstCacheHorizontalView - position_s;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheVerticalView += numberOfVerticalViewsToRead;
                    mFirstCacheHorizontalView = position_s;
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if((index_t < numberOfVerticalViewsToRead)||(index_s >= numberOfHorizontalViewsToKeep))
				mViewCache[t][s].CloseViewFile();
                        }                   
                    }
                    
                    mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset < 0)
						mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;
                    mVerticalIndexOffset += numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset >= mNumberOfCacheVerticalViews)
						mVerticalIndexOffset = 0;
                    
                    for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                    for(int index_t = 0; index_t < numberOfVerticalViewsToKeep; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
            }
        }
        else {
            if(position_t >= mFirstCacheVerticalView) {
                if(position_s +targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                    //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                    
                    numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                     for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            mViewCache[t][s].CloseViewFile();
                        }
                     }
                    
                    mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews)
						mHorizontalIndexOffset = 0;
                    
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
                else { //position_s < mFirstCacheHorizontalView
                    //new status after reading: mFirstCacheHorizontalView = position_s
                    
                    numberOfHorizontalViewsToRead = mFirstCacheHorizontalView - position_s;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    mFirstCacheHorizontalView = position_s;
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset+numberOfHorizontalViewsToKeep)%mNumberOfHorizontalViews;
                            mViewCache[t][s].CloseViewFile();
                        }
                    }
                    
                    mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset < 0)
						mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;
                    
                    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                }
            }
            else {
                if(position_s +targetBlock->mlength_s > mFirstCacheHorizontalView + mNumberOfCacheHorizontalViews) {
                    //new status after reading: mFirstCacheHorizontalView = position_s +targetBlock->mlength_s - mNumberOfCacheHorizontalViews
                    //and mFirstCacheVerticalView = position_t
                    numberOfHorizontalViewsToRead = position_s + targetBlock->mlength_s - mFirstCacheHorizontalView - mNumberOfCacheHorizontalViews;
                    numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                    if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
						numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                    if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
						numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                    numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                    numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                    mFirstCacheHorizontalView += numberOfHorizontalViewsToRead;
                    mFirstCacheVerticalView = position_t;
                    for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if((index_s < numberOfHorizontalViewsToRead)||(index_t >= numberOfVerticalViewsToKeep))
				mViewCache[t][s].CloseViewFile();
                        }                   
                    }
                    
                    mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                    if(mVerticalIndexOffset < 0)
						mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                    mHorizontalIndexOffset += numberOfHorizontalViewsToRead;
                    if(mHorizontalIndexOffset >= mNumberOfCacheHorizontalViews)
						mHorizontalIndexOffset = mHorizontalIndexOffset%mNumberOfCacheHorizontalViews;
                    
                    for(int index_s = numberOfHorizontalViewsToKeep; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                        for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }
                    for(int index_s = 0; index_s < numberOfHorizontalViewsToKeep; index_s++) {
                        for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                            //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                            int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                            int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                            if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                if(mViewType == 0)
                                    mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                if(mViewType == 1)
                                    mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                            }
                        }                   
                    }                     
                }
                else { 
                    if(position_s >= mFirstCacheHorizontalView) {
                        //and mFirstCacheVerticalView = position_t
                        numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                        if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
							numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                        numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                        mFirstCacheVerticalView = position_t;
                        for(int index_t = numberOfVerticalViewsToKeep; index_t < mNumberOfCacheVerticalViews; index_t++) {
                            for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                mViewCache[t][s].CloseViewFile();
                             }
                        }
                        
                        mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                        if(mVerticalIndexOffset < 0)
							mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                        
                        for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                            for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }
                       
                    }
                    else { //position_s < mFirstCacheHorizontalView
                        //new status after reading: mFirstCacheVerticalView = position_t
                        //new status after reading: mFirstCacheHorizontalView = position_s
                      
                        numberOfHorizontalViewsToRead =  mFirstCacheHorizontalView - position_s;
                        numberOfVerticalViewsToRead = mFirstCacheVerticalView - position_t;
                        if(numberOfHorizontalViewsToRead > mNumberOfCacheHorizontalViews)
							numberOfHorizontalViewsToRead = mNumberOfCacheHorizontalViews;
                        if(numberOfVerticalViewsToRead > mNumberOfCacheVerticalViews)
							numberOfVerticalViewsToRead = mNumberOfCacheVerticalViews;
                        numberOfHorizontalViewsToKeep = mNumberOfCacheHorizontalViews - numberOfHorizontalViewsToRead;
                        numberOfVerticalViewsToKeep = mNumberOfCacheVerticalViews - numberOfVerticalViewsToRead;
                        mFirstCacheHorizontalView = position_s;
                        mFirstCacheVerticalView = position_t;
                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if((index_s >= numberOfHorizontalViewsToKeep)||(index_t >= numberOfVerticalViewsToKeep))
									mViewCache[t][s].CloseViewFile();
                            }                   
                        }

                        mVerticalIndexOffset -= numberOfVerticalViewsToRead;
                        if(mVerticalIndexOffset < 0)
							mVerticalIndexOffset += mNumberOfCacheVerticalViews;
                        mHorizontalIndexOffset -= numberOfHorizontalViewsToRead;
                        if(mHorizontalIndexOffset < 0)
							mHorizontalIndexOffset += mNumberOfCacheHorizontalViews;

                        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
                            for(int index_t = 0; index_t < numberOfVerticalViewsToRead; index_t++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }
                        for(int index_s = 0; index_s < numberOfHorizontalViewsToRead; index_s++) {
                            for(int index_t = numberOfVerticalViewsToRead; index_t < mNumberOfCacheVerticalViews; index_t++) {
                                //FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView);
                                int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                                int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                                if(FindViewFileName(ViewFileName, index_t+mFirstCacheVerticalView, index_s+mFirstCacheHorizontalView)) {
                                    if(mViewType == 0)
                                        mViewCache[t][s].OpenViewFilePGM(ViewFileName, mReadOrWriteLightField);
                                    if(mViewType == 1)
                                        mViewCache[t][s].OpenViewFilePPM(ViewFileName, mReadOrWriteLightField);
                                }
                            }                   
                        }                     
                    }
                    
                }
            
            }
        }
        delete [] ViewFileName;
        
        for(int index_t = position_t-mFirstCacheVerticalView; index_t < position_t-mFirstCacheVerticalView+targetBlock->mlength_t; index_t++) {
            for(int index_s = position_s-mFirstCacheHorizontalView; index_s < position_s-mFirstCacheHorizontalView+targetBlock->mlength_s; index_s++) {
 		if((index_s + mFirstCacheHorizontalView < mNumberOfHorizontalViews)&&(index_t + mFirstCacheVerticalView < mNumberOfVerticalViews)) {
                    int s = (index_s+mHorizontalIndexOffset)%mNumberOfCacheHorizontalViews;
                    int t = (index_t+mVerticalIndexOffset)%mNumberOfCacheVerticalViews;
                    //mViewCache[v][u].PutBlock2D(targetBlock->mPixel[index_t][index_s],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u);
                    mViewCache[t][s].PutBlock2D(targetBlock->mPixel[index_t-position_t+mFirstCacheVerticalView][index_s-position_s+mFirstCacheHorizontalView],position_v,position_u,targetBlock->mlength_v,targetBlock->mlength_u, component);
                }
            }
        }
        
    }
}

int LightField :: FindViewFileName(char *viewFileName, int index_t, int index_s) {
/*! light field name conversion*/
    
    if((index_t >= mNumberOfVerticalViews) || (index_s >= mNumberOfHorizontalViews)) {
        return(0);
    }
    
    char *fileIndex = new char[mNumberOfVerticalDigits+mNumberOfHorizontalDigits+3];
    char *fileNameDigits = new char[mNumberOfVerticalDigits+mNumberOfHorizontalDigits+2];
        
    sprintf(fileIndex, "%d", index_s+mHorizontalViewNumberOffset);
    while(strlen(fileIndex) < static_cast<size_t>(mNumberOfVerticalDigits)) {
        strcpy(fileNameDigits, "0");
        strcat(fileNameDigits, fileIndex);
        strcpy(fileIndex, fileNameDigits);
    }
    strcpy(viewFileName, mViewFileNamePrefix);
    strcat(viewFileName, fileIndex);
    strcat(viewFileName, "_");
    sprintf(fileIndex, "%d", index_t+mVerticalViewNumberOffset);
    while(strlen(fileIndex) < static_cast<size_t>(mNumberOfHorizontalDigits)) {
        strcpy(fileNameDigits, "0");
        strcat(fileNameDigits, fileIndex);
        strcpy(fileIndex, fileNameDigits);
    }
    strcat(viewFileName, fileIndex);
    strcat(viewFileName, mViewFileNameSuffix);
        
    //delete [] viewFileNameindex_t;
    delete [] fileIndex;
    delete [] fileNameDigits;
    
    return(1);
   
}
void LightField :: SetViewVerbosity(char verbosity) {
/*! sets the verbosity of console messages */
    
    for(int index_t = 0; index_t < mNumberOfCacheVerticalViews; index_t++) {
        for(int index_s = 0; index_s < mNumberOfCacheHorizontalViews; index_s++) {
            mViewCache[index_t][index_s].mVerbosity = verbosity;
        }
    }
}

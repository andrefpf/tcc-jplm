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

/** \file     View.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 *  \note Temporary file... It will be removed during the refactorings
 */


#include "View.h"

/*******************************************************************************/
/*                            View class methods                               */
/*******************************************************************************/

inline unsigned short change_endianness_16b(unsigned short val)
{
    return (val << 8) | ((val >> 8) & 0x00ff); 
}


ViewOLD :: ViewOLD(void) {
    mPixelData = NULL;
    mLines = 0;
    mColumns = 0;
    mViewFilePointer = NULL;
    mFirstPixelPosition = 0;
    mLastLine = 0;
    mFirstLine = 0;
    mNumberOfPixelsInCache = 0;
    mNumberOfFileBytesPerPixelComponent = 0;
    mNumberOfBands = 1;
    mReadOrWriteView = 'r';
    mUnavailable = 1;
    mVerbosity = 1;
}

ViewOLD :: ~ViewOLD(void) {
    if(mPixelData != NULL) {
        delete [] mPixelData;
	mPixelData = NULL;
    }
    if(mViewFilePointer != NULL) fclose(mViewFilePointer);
}

void ViewOLD :: SetCacheSize(int maxBlockLines) {
/*! sets the maximum number of view lines contained in the cache */   
    
    mNumberCacheLines = maxBlockLines;

}
PixelType ViewOLD :: GetPixel(int position_v, int position_u, int component) {
/*! returns the pixel value at position_v line and position_u column of the view */   
    
    if(mUnavailable == 1) 
        return(DefaultPixelValue);
    
    if((position_v >= mFirstLine)&&(position_v <= mLastLine)) {
        //The requested pixel is in the cache. No need to read from file. 
        //return(mPixelData[((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)]);
        return(mPixelData[mNumberOfBands*(((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component]);
      
    }
    else {
        //The requested pixel is not in the cache. Read a new line from file and make mFirstPixelPosition += mColumns. 
        //Make mFirstpixelPositionixelposition = FirstPixelPosition%(mNumberCacheLines*mColumns)
        if((position_v > mLastLine)) {
            while(position_v > mLastLine) {
                for(int pixelCount = 0; pixelCount < mColumns; pixelCount++) {
                    
                    ReadPixelFromFile(pixelCount);
/*
                    PixelType pixelValue=0; 
                    fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
                    if(mNumberOfFileBytesPerPixelComponent == 2) 
                        mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)]= change_endianness_16b(pixelValue);
                    else 
                        mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)]= pixelValue;
*/
               }
                mFirstLine++;
                mLastLine++;
                mFirstPixelPosition += mColumns;
                mFirstPixelPosition = mFirstPixelPosition%(mNumberCacheLines*mColumns);
            }
        }
        else {
            fseek(mViewFilePointer, position_v*mColumns*mNumberOfFileBytesPerPixelComponent*mNumberOfBands+mPGMDataOffset, SEEK_SET);
            mFirstPixelPosition = 0;
            for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
                
                ReadPixelFromFile(pixelCount);
/*
                PixelType pixelValue=0; 
                fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
                if(mNumberOfFileBytesPerPixelComponent == 2) 
                    mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = change_endianness_16b(pixelValue);
                else
                    mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = pixelValue;
*/
            }
            mFirstLine = position_v;
            mLastLine = position_v+mNumberCacheLines-1;
        }
        //return(mPixelData[((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)]);
        return(mPixelData[mNumberOfBands*(((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component]);
    }
}

void ViewOLD :: PutPixel(PixelType pixelValue, int position_v, int position_u, int component) {
/*! writes pixelValue at position_v line and position_u column of the view */   
/*! each component of a musltispectral pixel is independently stored and the operation is finished after the component 0 is written */   
   
    if(mUnavailable == 1) 
        return;
    
    if((position_v >= mFirstLine)&&(position_v <= mLastLine)) {
        //The requested pixel position is in the cache. No need to write to file. 
        mPixelData[mNumberOfBands*(((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component] = pixelValue;
        if(component == 0) 
            mNumberOfPixelsInCache++;
    }
    else {
        //Writes buffer to the file
        //for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
        for(int pixelCount = 0; pixelCount < mNumberOfPixelsInCache; pixelCount++) {
            
            WritePixelToFile(pixelCount);
/*            
            int ClippedPixelValue = mPixelData[(pixelCount)];
            if(ClippedPixelValue > mPGMScale) 
                ClippedPixelValue = mPGMScale;
            if(ClippedPixelValue < 0) 
                ClippedPixelValue = 0;
            unsigned short bigEndianPixelValue = (mNumberOfFileBytesPerPixelComponent == 2) ? change_endianness_16b(ClippedPixelValue) : ClippedPixelValue;
            //unsigned short bigEndianPixelValue = change_endianness_16b(mPixelData[(pixelCount)]);
            fwrite(&bigEndianPixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
*/
        }
        mFirstLine += mNumberCacheLines;
        mLastLine += mNumberCacheLines;
        //mPixelData[((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)] = pixelValue;
        mPixelData[mNumberOfBands*(((position_v-mFirstLine)*mColumns + position_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component] = pixelValue;
        if(component == 0)
            mNumberOfPixelsInCache = 1;
      
     }
    
}

void ViewOLD :: GetBlock2D(block4DElementType **pixel, int position_v, int position_u, int length_v, int length_u, int component) {
/*! returns the pixel values of a 2D length_v x length_u region starting at position_v line and position_u column of the view */   
         
    if(mUnavailable == 1) {
        for(int index_v = 0; index_v < length_v; index_v++) {
            for(int index_u = 0; index_u < length_u; index_u++) {
                pixel[index_v][index_u] = DefaultPixelValue;
       
            }
        }
        return;       
    }
    
    //if((position_v >= mFirstLine)&&(position_v + length_v -1 <= mLastLine)) {
    if((position_v >= mFirstLine)&&((position_v + length_v - 1 <= mLines - 1 ? position_v + length_v - 1 : mLines - 1) <= mLastLine)) {
        //The requested pixels are in the cache. No need to read from file. 
        for(int index_v = 0; index_v < length_v; index_v++) {
            for(int index_u = 0; index_u < length_u; index_u++) {
                if((position_u + index_u < mColumns)&&(position_v + index_v < mLines)) {
                    //pixel[index_v][index_u] = mPixelData[((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)];
                    pixel[index_v][index_u] = mPixelData[mNumberOfBands*(((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component];
                }
            }
        }
    }
    else {
        //The requested pixels are not in the cache. Read a new line from file and make mFirstPixelPosition += mColumns. 
        //Make mFirstpixelPositionixelposition = FirstPixelPosition%(mNumberCacheLines*mColumns)
        if((position_v + length_v -1 > mLastLine)) {
            while(position_v  + length_v -1 > mLastLine) {
                for(int pixelCount = 0; pixelCount < mColumns; pixelCount++) {
                    
                    ReadPixelFromFile(pixelCount);
/*
                    PixelType pixelValue=0; 
                    fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
                    if(mNumberOfFileBytesPerPixelComponent == 2) 
                        mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = change_endianness_16b(pixelValue);
                    else
                        mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = pixelValue;
*/                    
                }
                mFirstLine++;
                mLastLine++;
                mFirstPixelPosition += mColumns;
                mFirstPixelPosition = mFirstPixelPosition%(mNumberCacheLines*mColumns);
            }
        }
        else {
            //fseek(mViewFilePointer, position_v*mColumns*mNumberOfFileBytesPerPixelComponent+mPGMDataOffset, SEEK_SET);
            fseek(mViewFilePointer, position_v*mColumns*mNumberOfFileBytesPerPixelComponent*mNumberOfBands+mPGMDataOffset, SEEK_SET);
            mFirstPixelPosition = 0;
            for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
                
                ReadPixelFromFile(pixelCount);
/*
                PixelType pixelValue=0;
                fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
                if(mNumberOfFileBytesPerPixelComponent == 2) 
                    mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = change_endianness_16b(pixelValue);
                else
                    mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = pixelValue;
*/
            }
            mFirstLine = position_v;
            mLastLine = position_v+mNumberCacheLines-1;
        }
        for(int index_v = 0; index_v < length_v; index_v++) {
            for(int index_u = 0; index_u < length_u; index_u++) {
                if((position_u + index_u < mColumns)&&(position_v + index_v < mLines)) {
                    //pixel[index_v][index_u] = mPixelData[((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)];
                    pixel[index_v][index_u] = mPixelData[mNumberOfBands*(((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component];
                }
            }
        }
    }    
}

void ViewOLD :: PutBlock2D(block4DElementType **pixelValue, int position_v, int position_u, int length_v, int length_u, int component) {
/*! writes the pixels contained in the array pixelValue of a 2D length_v x length_u region starting at position_v line and position_u column of the view */   
/*! each component of a musltispectral image is independently stored and the operation is finished after the component 0 is written */   
   
    if(mUnavailable == 1)
        return;
    
    //if((position_v >= mFirstLine)&&(position_v + length_v - 1 <= mLastLine)) {
    if((position_v >= mFirstLine)&&((position_v + length_v - 1 <= mLines - 1 ? position_v + length_v - 1 : mLines - 1) <= mLastLine)) {
        //The requested pixels positions are in the cache. No need to write to file. 
        for(int index_v = 0; index_v < length_v; index_v++) {
            for(int index_u = 0; index_u < length_u; index_u++) {
                if((position_u + index_u < mColumns)&&(position_v + index_v < mLines)) {
                    //mPixelData[((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)] = pixelValue[index_v][index_u];
                    //mNumberOfPixelsInCache++;
                    mPixelData[mNumberOfBands*(((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component] = pixelValue[index_v][index_u];
                    if(component == 0) 
                        mNumberOfPixelsInCache++;
                }
            }
        }
    }
    else {
        //Writes buffer to the file
        //for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
        for(int pixelCount = 0; pixelCount < mNumberOfPixelsInCache; pixelCount++) {
            
            WritePixelToFile(pixelCount);
/*            
            int ClippedPixelValue = mPixelData[(pixelCount)];
            if(ClippedPixelValue > mPGMScale) 
                ClippedPixelValue = mPGMScale;
            if(ClippedPixelValue < 0) 
                ClippedPixelValue = 0;
            //unsigned short bigEndianPixelValue = change_endianness_16b(mPixelData[(pixelCount)]);
            unsigned short bigEndianPixelValue = (mNumberOfFileBytesPerPixelComponent == 2) ? change_endianness_16b(ClippedPixelValue) : ClippedPixelValue;
            fwrite(&bigEndianPixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
*/            
        }
        //mFirstLine += mNumberCacheLines;
        //mLastLine += mNumberCacheLines;
        mFirstLine += mNumberOfPixelsInCache/mColumns;
        mLastLine += mNumberOfPixelsInCache/mColumns;
        mNumberOfPixelsInCache = 0;
        for(int index_v = 0; index_v < length_v; index_v++) {
            for(int index_u = 0; index_u < length_u; index_u++) {
                if((position_u + index_u < mColumns)&&(position_v + index_v < mLines)) {
                    //mPixelData[((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns)] = pixelValue[index_v][index_u];
                    //mNumberOfPixelsInCache++;
                    //For a multiband image, the writing is concluded
                    //only after band 0 is written
                    mPixelData[mNumberOfBands*(((position_v+index_v-mFirstLine)*mColumns + position_u + index_u + mFirstPixelPosition)%(mNumberCacheLines*mColumns))+component] = pixelValue[index_v][index_u];
                    if(component == 0) 
                        mNumberOfPixelsInCache++;
                }
            }
        }      
     }
    
}

void ViewOLD :: OpenViewFilePGM(char *name, char readOrWriteView) {
/*! opens the view file for reading or writing */  
    
    char tag[256];
    if(readOrWriteView == 'r') {
        mReadOrWriteView = 'r';
	if((mViewFilePointer = fopen(name, "rb")) == NULL) {
	    printf("unable to open %s view file for reading\n", name);
            return;
	}
        if(mVerbosity > 0) printf("Opening %s view file\n", name);
 	//Reads header of pgm file, sets mLines and mColums accordingly.
	//allocates mNumberCaheLines*mColums positions for *mPixelData,
	//reads mNumberCaheLines*mColums pixels, sets mFirstPixelPosition = 0
        if(fscanf(mViewFilePointer, "%s", tag) != 1) std::cerr << "Error reading file" << std::endl;
        if(strcmp(tag, "P5")==0) {
           if(fscanf(mViewFilePointer, "%s", tag)) std::cerr << "Error reading file" << std::endl;
            while(tag[0] == '#') {
	        while(tag[0] != '\n') tag[0] = fgetc(mViewFilePointer);
		if(fscanf(mViewFilePointer, "%s", tag) != 1) std::cerr << "Error reading file" << std::endl;
	    }
            mColumns = atoi(tag);
            if(fscanf(mViewFilePointer, "%d", &mLines) != 1) std::cerr << "Error reading file" << std::endl;
            if(fscanf(mViewFilePointer, "%d", &mPGMScale) != 1) std::cerr << "Error reading file" << std::endl;
            fgetc(mViewFilePointer);    //reads newline character
            mPGMDataOffset = ftell(mViewFilePointer);
        }
        else {
            printf("Error: input file is not PGM\n");
            return;
        }
        mUnavailable = 0;
        mPixelData = new PixelType [mNumberCacheLines*mColumns];
        mFirstPixelPosition = 0;
        mNumberOfFileBytesPerPixelComponent = (mPGMScale <= 255 ? 1 : 2);
        for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
            PixelType pixelValue=DefaultPixelValue; 
            if(fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer) != 1) std::cerr << "Error reading file" << std::endl;
            if(mNumberOfFileBytesPerPixelComponent == 2) 
                mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = change_endianness_16b(pixelValue);
            else
                mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = pixelValue;
        }
        mFirstLine = 0;
        mLastLine = mNumberCacheLines-1;
        
    }
    if(readOrWriteView == 'w') {
        mReadOrWriteView = 'w';
	if((mViewFilePointer = fopen(name, "wb")) == NULL) {
	    printf("unable to open %s view file for writing\n", name);
            return;
	}
        mUnavailable = 0;
        mNumberOfFileBytesPerPixelComponent = (mPGMScale <= 255 ? 1 : 2);
        fprintf(mViewFilePointer, "P5\n%d %d\n%d\n", mColumns, mLines, mPGMScale);
        mPixelData = new PixelType [mNumberCacheLines*mColumns];
        mFirstPixelPosition = 0;
        mFirstLine = 0;
        mLastLine = mNumberCacheLines-1;
        mNumberOfPixelsInCache = 0;
   }
    
}

int ViewOLD :: OpenViewFilePPM(char *name, char readOrWriteView) {
  
    char tag[256];
    if(readOrWriteView == 'r') {
        mReadOrWriteView = 'r';
	if((mViewFilePointer = fopen(name, "rb")) == NULL) {
	    printf("unable to open %s view file for reading\n", name);
            return(-1);
	}
        if(mVerbosity > 0) printf("Opening %s view file\n", name);
 	//Reads header of ppm file, sets mLines and mColums accordingly.
	//allocates mNumberCaheLines*mColums positions for *mPixelData,
	//reads mNumberCaheLines*mColums pixels, sets mFirstPixelPosition = 0
        if(fscanf(mViewFilePointer, "%s", tag) != 1) std::cerr << "Error reading file" << std::endl;
        if(strcmp(tag, "P6")==0) {
           if(fscanf(mViewFilePointer, "%s", tag) != 1) std::cerr << "Error reading file" << std::endl;
            while(tag[0] == '#') {
	        while(tag[0] != '\n') tag[0] = fgetc(mViewFilePointer);
		if(fscanf(mViewFilePointer, "%s", tag) != 1) std::cerr << "Error reading file" << std::endl;
	    }
            mColumns = atoi(tag);
            if(fscanf(mViewFilePointer, "%d", &mLines) != 1) std::cerr << "Error reading file" << std::endl;;
            if(fscanf(mViewFilePointer, "%d", &mPGMScale) != 1) std::cerr << "Error reading file" << std::endl;;
            fgetc(mViewFilePointer);    //reads newline character
            mPGMDataOffset = ftell(mViewFilePointer);
        }
        else {
            printf("Error: input file is not PPM\n");
            return(-1);
        }
        mUnavailable = 0;
        mNumberOfBands = 3;
        mPixelData = new PixelType [mNumberOfBands*mNumberCacheLines*mColumns];
        mFirstPixelPosition = 0;
        mNumberOfFileBytesPerPixelComponent = (mPGMScale <= 255 ? 1 : 2);
        for(int pixelCount = 0; pixelCount < mNumberCacheLines*mColumns; pixelCount++) {
            
            ReadPixelFromFile(pixelCount);
/*            
            PixelType pixelValue=DefaultPixelValue; 
            fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
            if(mNumberOfFileBytesPerPixelComponent == 2) 
                mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = change_endianness_16b(pixelValue);
            else
                mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)] = pixelValue;
*/            
        }
        mFirstLine = 0;
        mLastLine = mNumberCacheLines-1;
        
    }
    if(readOrWriteView == 'w') {
        mReadOrWriteView = 'w';
	if((mViewFilePointer = fopen(name, "wb")) == NULL) {
	    printf("unable to open %s view file for writing\n", name);
            return(-1);
	}
        if(mVerbosity > 0) printf("Opening %s view file\n", name);        
        mUnavailable = 0;
        mNumberOfBands = 3;
        mNumberOfFileBytesPerPixelComponent = (mPGMScale <= 255 ? 1 : 2);
        fprintf(mViewFilePointer, "P6\n%d %d\n%d\n", mColumns, mLines, mPGMScale);
        mPixelData = new PixelType [mNumberOfBands*mNumberCacheLines*mColumns];
        mFirstPixelPosition = 0;
        mFirstLine = 0;
        mLastLine = mNumberCacheLines-1;
        mNumberOfPixelsInCache = 0;
    }
    return(0); 
}


void ViewOLD :: ReadPixelFromFile(int pixelPositionInCache) {
    
    for(int component_index = 0; component_index < mNumberOfBands; component_index++) {
        PixelType pixelValue=0; 
        if(fread(&pixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer) != 1) {
            std::cerr << "Error reading pixel from file" << std::endl;;
            pixelValue = 0;
        }
        if(mNumberOfFileBytesPerPixelComponent == 2) 
            mPixelData[mNumberOfBands*((mFirstPixelPosition+mNumberCacheLines*mColumns+pixelPositionInCache)%(mNumberCacheLines*mColumns))+component_index]= change_endianness_16b(pixelValue);
        else 
            mPixelData[mNumberOfBands*((mFirstPixelPosition+mNumberCacheLines*mColumns+pixelPositionInCache)%(mNumberCacheLines*mColumns))+component_index]= pixelValue;
    }
    
}

void ViewOLD :: WritePixelToFile(int pixelPositionInCache) {
    
    for(int component_index = 0; component_index < mNumberOfBands; component_index++) {
        int ClippedPixelValue = mPixelData[mNumberOfBands*(pixelPositionInCache)+component_index];
        if(ClippedPixelValue > mPGMScale) 
            ClippedPixelValue = mPGMScale;
        if(ClippedPixelValue < 0) 
            ClippedPixelValue = 0;
        unsigned short bigEndianPixelValue = (mNumberOfFileBytesPerPixelComponent == 2) ? change_endianness_16b(ClippedPixelValue) : ClippedPixelValue;
        fwrite(&bigEndianPixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
    }
    
}

void ViewOLD :: CloseViewFile(void) {
/*! flushes the cache and closes the view file */
    
    if(mUnavailable == 1)
        return;
    
    if(mReadOrWriteView == 'w') {
        for(int pixelCount = 0; pixelCount < mNumberOfPixelsInCache ; pixelCount++) {
            
            WritePixelToFile(pixelCount);
/*            
            //int ClippedPixelValue = mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)];
            int ClippedPixelValue = mPixelData[pixelCount];
            if(ClippedPixelValue > mPGMScale) 
                ClippedPixelValue = mPGMScale;
            if(ClippedPixelValue < 0) 
                ClippedPixelValue = 0;
            unsigned short bigEndianPixelValue = (mNumberOfFileBytesPerPixelComponent == 2) ? change_endianness_16b(ClippedPixelValue) : ClippedPixelValue;
            //unsigned short bigEndianPixelValue = change_endianness_16b(mPixelData[(mFirstPixelPosition+mNumberCacheLines*mColumns+pixelCount)%(mNumberCacheLines*mColumns)]);
            fwrite(&bigEndianPixelValue, mNumberOfFileBytesPerPixelComponent, 1, mViewFilePointer);
 */ 
        }
        mNumberOfPixelsInCache = 0;
    }
    if(mPixelData != NULL) {
        delete [] mPixelData;
        mPixelData = NULL;
    }
    fclose(mViewFilePointer);
    if(mViewFilePointer != NULL)
		mViewFilePointer = NULL;
    
    mUnavailable = 1;
}

//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Class definition of ossimU11ImageData.  Specialized image data object for
// unsigned short data with an 11 bit depth.
//
// NOTE:  This object is optimized for unsigned short data and assumes the
//        following:  null pixel value  = 0.0
//                    min  pixel value  = 1.0
//                    max  pixel value  = 2047.0  (2^11 - 1)
//
//        If you want anything else use the less efficient ossimImageData.
//
//*************************************************************************
// $Id: ossimU11ImageData.cpp,v 1.18 2006/01/10 13:10:29 gpotts Exp $

#include <cstring>  // for memset function
using namespace std;

#include <imaging/ossimU11ImageData.h>
#include <base/common/ossimSource.h>
#include <base/context/ossimErrorContext.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimMultiBandHistogram.h>
#include <base/data_types/ossimHistogram.h>

RTTI_DEF1(ossimU11ImageData, "ossimU11ImageData", ossimImageData)

const ossimNormalizedU11RemapTable ossimU11ImageData::theRemapTable;
   
ossimU11ImageData::ossimU11ImageData()
   :
      ossimImageData()
{
}

ossimU11ImageData::ossimU11ImageData(ossimSource* source,
                                     ossim_uint32 bands)
   :
      ossimImageData(source,
                     OSSIM_USHORT11,
                     bands)
{
}

ossimU11ImageData::ossimU11ImageData(ossimSource* source,
                                     ossim_uint32 bands,
                                     ossim_uint32 width,
                                     ossim_uint32 height)
   :
      ossimImageData(source,
                     OSSIM_USHORT11,
                     bands,
                     width,
                     height)
{
}

ossimU11ImageData::ossimU11ImageData(const ossimU11ImageData &rhs)
   :
      ossimImageData(rhs)
{}

ossimU11ImageData::~ossimU11ImageData()
{
}

ossimObject* ossimU11ImageData::dup()const
{
   return new ossimU11ImageData(*this);
}

ossimDataObjectStatus ossimU11ImageData::validate() const
{
   if (theDataBuffer == NULL)
   {
      setDataObjectStatus(OSSIM_NULL);
      return OSSIM_NULL;
   }
   
   ossim_uint32 count = 0;
   const ossim_uint32 SIZE = getSize();
   const ossim_uint32 BOUNDS = getSizePerBand();
   const ossim_uint32 NUMBER_OF_BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < NUMBER_OF_BANDS; ++band)
   {
      const ossim_uint16* p  = getUshortBuf(band);
      for (ossim_uint32 i=0; i<BOUNDS; i++)
      {
         // check if the band is null
         if (p[i] != 0) count++;         
      }
   }
   
   if (!count)
      setDataObjectStatus(OSSIM_EMPTY);
   else if (count == SIZE)
      setDataObjectStatus(OSSIM_FULL);
   else
      setDataObjectStatus(OSSIM_PARTIAL);

   return getDataObjectStatus();
}

void ossimU11ImageData::getNormalizedFloat(ossim_uint32 offset,
                                           ossim_uint32 bandNumber,
                                           float& result)const
{
   if(!result)
   {
      return;
   }
   
   // make sure that the types and width and height are good.
   if( (getDataObjectStatus() == OSSIM_NULL) && isValidBand(bandNumber) )
   {
      return;
   }
   
   const ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
   result = static_cast<float>(theRemapTable.normFromPix(sourceBuf[offset]));
}

void ossimU11ImageData::setNormalizedFloat(ossim_uint32 offset,
                                           ossim_uint32 bandNumber,
                                           float inputValue)
{
   // make sure that the types and width and height are
   // good.
   if((getDataObjectStatus() == OSSIM_NULL)&&
      (bandNumber < getNumberOfBands()))
   {
      return;
   }

   ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
   sourceBuf[offset]
      = static_cast<ossim_uint16>(theRemapTable.pixFromNorm(inputValue));
}

void
ossimU11ImageData::convertToNormalizedFloat(ossimImageData* result)const
{
   if(!result)
   {
      return;
   }
   // make sure that the types and width and height are
   // good.
   if( (result->getScalarType() != OSSIM_NORMALIZED_FLOAT) ||
       (result->getNumberOfBands() != this->getNumberOfBands())||
       (result->getWidth() != this->getWidth()) ||
       (result->getHeight() != this->getHeight())||
       (result->getDataObjectStatus() == OSSIM_NULL) ||
       (getDataObjectStatus() == OSSIM_NULL))
   {
      return;
   }

   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 bandCount = 0; bandCount < theNumberOfDataComponents; ++bandCount)
      {
         const ossim_uint16* sourceBuf = getUshortBuf(bandCount);
         float* resultBuf = static_cast<float*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter]
               = static_cast<float>(theRemapTable.
                                    normFromPix(sourceBuf[counter]));
         }
      }
   }
}

void ossimU11ImageData::convertToNormalizedDouble(ossimImageData* result)const
{
   if(!result)
   {
      return;
   }
   
   // make sure that the types and width and height are
   // good.
   if( (result->getScalarType() != OSSIM_NORMALIZED_FLOAT) ||
       (result->getNumberOfBands() != this->getNumberOfBands())||
       (result->getWidth() != this->getWidth()) ||
       (result->getHeight() != this->getHeight())||
       (result->getDataObjectStatus() == OSSIM_NULL) ||
       (getDataObjectStatus() == OSSIM_NULL))
   {
      return;
   }

   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 bandCount = 0; bandCount < theNumberOfDataComponents; ++bandCount)
      {
         const ossim_uint16* sourceBuf = getUshortBuf(bandCount);
         double* resultBuf = static_cast<double*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter] = theRemapTable.normFromPix(sourceBuf[counter]);
         }
      }
   }

}

void ossimU11ImageData::unnormalizeInput(ossimImageData* normalizedInput)
{
   if((normalizedInput->getScalarType() != OSSIM_NORMALIZED_FLOAT) &&
      (normalizedInput->getScalarType() != OSSIM_NORMALIZED_DOUBLE) )
   {
      //ERROR
      return;
   }
   
   ossim_uint32 counter = 0;
   ossim_uint32 bandCount = 0;
   ossim_uint32 size = getSizePerBand();
   ossimScalarType scalarType = normalizedInput->getScalarType();

   if(size > 0)
   {
      if(scalarType == OSSIM_NORMALIZED_FLOAT)
      {
         for(bandCount = 0; bandCount < theNumberOfDataComponents; ++bandCount)
         {
            float* sourceBuf =
               static_cast<float*>(normalizedInput->getBuf(bandCount));
            ossim_uint16* resultBuf = getUshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint16>(theRemapTable.
                                              pixFromNorm(sourceBuf[counter]));
            }
         }
      }
      else // Double
      {
         for(bandCount = 0; bandCount < theNumberOfDataComponents; ++bandCount)
         {
            double* sourceBuf =
               static_cast<double*>(normalizedInput->getBuf(bandCount));
            ossim_uint16* resultBuf = getUshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint16>(theRemapTable.
                                              pixFromNorm(sourceBuf[counter]));
            }
         }
      }
   }   
}

double ossimU11ImageData::computeMeanSquaredError(double meanValue,
                                                  ossim_uint32 bandNumber)
{
   double result = -1; // invalid MSE
   ossim_uint32 index = 0;
   double delta = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_uint16* buffer = getUshortBuf(bandNumber);
   if(buffer)
   {
      ossim_uint32 bounds = getWidth()*getHeight();
      for(index = 0; index < bounds; ++index)
      {
         if(!isNull(index))
         {
            delta = *buffer - meanValue;
            result += (delta*delta);
            ++validPixelCount;
         }
         ++buffer;
      }
      if(validPixelCount > 0)
      {
         result /= validPixelCount;
      }
   }

   return result;
}

//******************************************************************
//
// NOTE: I was checking for null and not adding it to the histogram.
//       this was messing up the equalization algorithms since the
//       accumulation histogram did not represent the area of the
//       image.  For now I will leave out the check for "is null" and
//       add this to the count so that the total accumulation is the
//       area of the image.
//
//******************************************************************
void ossimU11ImageData::populateHistogram(ossimMultiBandHistogram* histo)
{
   ossim_uint32 numberOfBands = getNumberOfBands();

   if( (getDataObjectStatus() == OSSIM_NULL) ||
       (getDataObjectStatus() == OSSIM_EMPTY))
   {
      return;
   }

   for(ossim_uint32 band = 0; band < numberOfBands; ++band)
   {
      ossimHistogram* currentHisto = histo->getHistogram(band);
      ossim_uint16* buffer = getUshortBuf(band);
      
      if(currentHisto)
      {
	//float *counts = currentHisto->GetCounts();
         ossim_uint32 upperBound = getWidth()*getHeight();
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
	   currentHisto->UpCount((float)buffer[offset]);
         }
      }
   }
}

double ossimU11ImageData::computeAverageBandValue(ossim_uint32 bandNumber)
{
   double result = 0.0;
   ossim_uint32 index = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_uint16* buffer = getUshortBuf(bandNumber);
   if(buffer)
   {
      ossim_uint32 bounds = getSizePerBand();
      for(index = 0; index < bounds; ++index)
      {
         if(!isNull(index))
         {
            result += *buffer;
            ++validPixelCount;
         }
         ++buffer;
      }
      if(validPixelCount > 0)
      {
         result /= validPixelCount;
      }
   }

   return result;
}

void ossimU11ImageData::makeBlank()
{
   void* s = getBuf();

   if (s == NULL) return; // nothing to do...
   
   ossim_uint32 size_in_pixels = getSizePerBand();
   
   for(ossim_uint32 band =0; band < getNumberOfBands(); ++band)
   {
      ossim_uint16* p = getUshortBuf(band);
      for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = 0;
   }

   setDataObjectStatus(OSSIM_EMPTY);
}

void ossimU11ImageData::setValue(long x, long y, double color)
{
   if(theDataBuffer&&isWithin(x, y))
   {
      //***
      // Compute the offset into the buffer for (x,y).  This should always
      // come out positive.
      //***
      ossim_uint32 ux = static_cast<ossim_uint32>(x - theOrigin.x);
      ossim_uint32 uy = static_cast<ossim_uint32>(y - theOrigin.y);
      
      ossim_uint32 offset = uy * theSpatialExtents[0] + ux;
      
      for(ossim_uint32 band = 0; band < theNumberOfDataComponents; band++)
      {
         ossim_uint16* buf = getUshortBuf(band)+offset;
         *buf = (ossim_uint16)color;
      }
   }
}

void ossimU11ImageData::fill(ossim_uint32 band, double value)
{
   void* s         = getBuf(band);

   if (s == NULL) return; // nothing to do...

   ossim_uint32 size_in_pixels = getSizePerBand();
   ossim_uint16* p = getUshortBuf(band);
   ossim_uint16 np = static_cast<ossim_uint16>(value);

   for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;

   // Set the status to unknown since we don't know about the other bands.
   setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
}


bool ossimU11ImageData::isNull(ossim_uint32 offset)const
{
   for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)  
   {
      const ossim_uint16* buf = getUshortBuf(band)+offset;
      
      if((*buf) != theNullPixelValue[band])
      {
         return false;
      }
   }

   return true;
}

void ossimU11ImageData::setNull(ossim_uint32 offset)
{
   ossim_uint32 bands = getNumberOfBands();
   for(ossim_uint32 band = 0; band < bands; ++band)  
   {
      ossim_uint16* buf = getUshortBuf(band)+offset;
      *buf          = (ossim_uint16)theNullPixelValue[band];
   }
}

void ossimU11ImageData::loadBand(const void* src,
                                 const ossimIrect& src_rect,
                                 ossim_uint32 band)
{
   ossimImageData::loadBand(ossim_uint16(0), src, src_rect, band);
}

void ossimU11ImageData::loadTile(void* src,
                                const ossimIrect& src_rect,
                                ossimInterleaveType il_type)
{
   // Load the tile from the buffer.
   switch (il_type)
   {
   case OSSIM_BIP:
      ossimImageData::loadTileFromBip(ossim_uint16(0), src, src_rect);
      break;
   case OSSIM_BIL:
      ossimImageData::loadTileFromBil(ossim_uint16(0), src, src_rect);
      break;
   case OSSIM_BSQ:
      ossimImageData::loadTileFromBsq(ossim_uint16(0), src, src_rect);
      break;
   default:
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimU11ImageData::loadTile"
         << "\nUnknown interleave type passed to method.  Returning..."
         << endl;
      break;
   } 
}

void ossimU11ImageData::unloadBand(void* dest,
                                   const ossimIrect& dest_rect,
                                   ossim_uint32 band) const
{
   ossimImageData::unloadBand(ossim_uint16(0), dest, dest_rect,
                              getImageRectangle(), band);
}

void ossimU11ImageData::copyTileToNormalizedBuffer(double* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         const ossim_uint16* s = getUshortBuf(band);  // source
         double* d = buf + (band*size);   // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            d[index] = theRemapTable.
                       normFromPix(static_cast<ossim_int32>(s[index]));
         }
      }
   }
}

void ossimU11ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
                                                   double* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf(band)) return;
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      const ossim_uint16* s = getUshortBuf(band);  // source
      double* d = buf;   // destination
      
      for(ossim_uint32 index = 0; index < size; index++)
      {
         *d = theRemapTable.
              normFromPix(static_cast<ossim_int32>(*s));
      }
   }
}


void ossimU11ImageData::copyNormalizedBufferToTile(double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         double* s = buf + (band*size); // source
         ossim_uint16* d = getUshortBuf(band); // destination

         for(ossim_uint32 index = 0; index <  size; index++)
         {
            d[index] = theRemapTable.pixFromNorm(s[index]);
         }
      }
   }
}

void ossimU11ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                   double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if((size > 0)&&getBuf(band))
   {
      double* s = buf; // source
      ossim_uint16* d = getUshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         *d = theRemapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}


void ossimU11ImageData::copyTileToNormalizedBuffer(float* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         const ossim_uint16* s = getUshortBuf(band);  // source
         float* d = buf + (band*size);   // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            d[index] = theRemapTable.
                       normFromPix(static_cast<ossim_int32>(s[index]));
         }
      }
   }
}

void ossimU11ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
                                                   float* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf(band)) return;
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      const ossim_uint16* s = getUshortBuf(band);  // source
      float* d = buf;   // destination
      
      for(ossim_uint32 index = 0; index < size; index++)
      {
         *d = theRemapTable.
              normFromPix(static_cast<ossim_int32>(*s));
      }
   }
}

void ossimU11ImageData::copyNormalizedBufferToTile(float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         float* s = buf + (band*size); // source
         ossim_uint16* d = getUshortBuf(band); // destination

         for(ossim_uint32 index = 0; index <  size; index++)
         {
            d[index] = theRemapTable.pixFromNorm(s[index]);
         }
      }
   }
}


void ossimU11ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                   float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if((size > 0)&&getBuf(band))
   {
      float* s = buf; // source
      ossim_uint16* d = getUshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         *d = theRemapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}


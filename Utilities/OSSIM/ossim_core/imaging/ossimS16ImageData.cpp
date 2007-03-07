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
// Class definition of ossimU16ImageData.  Specialized image data object for
// signed short data.
//
//*************************************************************************
// $Id: ossimS16ImageData.cpp,v 1.19 2004/09/02 13:59:03 dburken Exp $

#include <imaging/ossimS16ImageData.h>
#include <base/common/ossimErrorCodes.h>
#include <base/context/ossimErrorContext.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimMultiBandHistogram.h>
#include <base/data_types/ossimHistogram.h>

RTTI_DEF1(ossimS16ImageData, "ossimS16ImageData", ossimImageData)

ossimS16ImageData::ossimS16ImageData()
   :
      ossimImageData()
{
}

ossimS16ImageData::ossimS16ImageData(ossimSource* source,
                                     ossim_uint32 bands)
   :
      ossimImageData(source,
                     OSSIM_SSHORT16,
                     bands)
{
}

ossimS16ImageData::ossimS16ImageData(ossimSource* source,
                                     ossim_uint32 bands,
                                     ossim_uint32 width,
                                     ossim_uint32 height)
   :
      ossimImageData(source,
                     OSSIM_SSHORT16,
                     bands,
                     width,
                     height)
{
}

ossimS16ImageData::ossimS16ImageData(const ossimS16ImageData &rhs)
   :
      ossimImageData(rhs)
{
}

ossimS16ImageData::~ossimS16ImageData()
{
}

ossimObject* ossimS16ImageData::dup()const
{
   return new ossimS16ImageData(*this);
}

ossimDataObjectStatus ossimS16ImageData::validate() const
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
      ossim_sint16 np = (ossim_sint16)theNullPixelValue[band];
      const ossim_sint16* p  = getSshortBuf(band);
      for (ossim_uint32 i=0; i<BOUNDS; i++)
      {
         // check if the band is null
         if (p[i] != np) count++;         
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

void ossimS16ImageData::getNormalizedFloat(ossim_uint32 offset,
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
   
   ossim_int32 p = getSshortBuf(bandNumber)[offset];

   getNormFromPix(p, result, bandNumber);
}

void ossimS16ImageData::setNormalizedFloat(ossim_uint32 offset,
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

   ossim_int32 p;

   getPixFromNorm(p, inputValue, bandNumber);
   
   getSshortBuf(bandNumber)[offset] = static_cast<ossim_sint16>(p);
}

void
ossimS16ImageData::convertToNormalizedFloat(ossimImageData* result)const
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
      for(ossim_uint32 bandCount = 0; bandCount < theNumberOfDataComponents;
          ++bandCount)
      {
         const ossim_sint16* sourceBuf = getSshortBuf(bandCount);
         ossim_float32* resultBuf = static_cast<ossim_float32*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            getNormFromPix(sourceBuf[counter], resultBuf[counter], bandCount);
         }
      }
   }
}

void ossimS16ImageData::convertToNormalizedDouble(ossimImageData* result)const
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
      for(ossim_uint32 bandCount = 0; bandCount < theNumberOfDataComponents;
          ++bandCount)
      {
         const ossim_sint16* sourceBuf = getSshortBuf(bandCount);
         ossim_float64* resultBuf = static_cast<ossim_float64*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            getNormFromPix(sourceBuf[counter],
                           resultBuf[counter],
                           bandCount);
         }
      }
   }

}

void ossimS16ImageData::unnormalizeInput(ossimImageData* normalizedInput)
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
            ossim_float32* sourceBuf =
               static_cast<ossim_float32*>(normalizedInput->getBuf(bandCount));
            ossim_sint16* resultBuf = getSshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               ossim_float32 f = sourceBuf[counter];
               ossim_int32 p;
               getPixFromNorm(p, f, bandCount);
               resultBuf[counter] = static_cast<ossim_sint16>(p);
            }
         }
      }
      else // Double
      {
         for(bandCount = 0; bandCount < theNumberOfDataComponents; ++bandCount)
         {
            ossim_float64* sourceBuf =
               static_cast<ossim_float64*>(normalizedInput->getBuf(bandCount));
            ossim_sint16* resultBuf = getSshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               ossim_int32 p;
               getPixFromNorm(p, sourceBuf[counter], bandCount);
               resultBuf[counter] = static_cast<ossim_sint16>(p);
            }
         }
      }
   }   
}

double ossimS16ImageData::computeMeanSquaredError(double meanValue,
                                                  ossim_uint32 bandNumber)
{
   double result = -1; // invalid MSE
   ossim_uint32 index = 0;
   double delta = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_sint16* buffer = getSshortBuf(bandNumber);
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
void ossimS16ImageData::populateHistogram(ossimMultiBandHistogram* histo)
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
      ossim_sint16* buffer = getSshortBuf(band);
      
      if(currentHisto)
      {
         float *counts = currentHisto->GetCounts();
         ossim_uint32 upperBound = getWidth()*getHeight();
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            counts[buffer[offset]]++;
         }
      }
   }
}

double ossimS16ImageData::computeAverageBandValue(ossim_uint32 bandNumber)
{
   double result = 0.0;
   ossim_uint32 index = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_sint16* buffer = getSshortBuf(bandNumber);
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

void ossimS16ImageData::makeBlank()
{
   void* s = getBuf();

   if (s == NULL) return; // nothing to do...
   
   ossim_uint32 size_in_pixels = getSizePerBand();
   
   for(ossim_uint32 band =0; band < getNumberOfBands(); ++band)
   {
      ossim_sint16 np = (ossim_sint16)theNullPixelValue[band];
      ossim_sint16* p = getSshortBuf(band);
      for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
   }

   setDataObjectStatus(OSSIM_EMPTY);
}

void ossimS16ImageData::setValue(ossim_int32 x, ossim_int32 y, double color)
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
         ossim_sint16* buf = getSshortBuf(band)+offset;
         *buf = (ossim_sint16)color;
      }
   }
}

void ossimS16ImageData::fill(ossim_uint32 band, double value)
{
   void* s         = getBuf(band);

   if (s == NULL) return; // nothing to do...

   ossim_uint32 size_in_pixels = getSizePerBand();
   ossim_sint16* p = getSshortBuf(band);

   for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = (ossim_sint16)value;

   // Set the status to unknown since we don't know about the other bands.
   setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
}


bool ossimS16ImageData::isNull(ossim_uint32 offset)const
{
   for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)  
   {
      const ossim_sint16* buf = getSshortBuf(band)+offset;
      
      if((*buf) != theNullPixelValue[band])
      {
         return false;
      }
   }

   return true;
}

void ossimS16ImageData::setNull(ossim_uint32 offset)
{
   for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)  
   {
      ossim_sint16* buf = getSshortBuf(band)+offset;
      *buf       = static_cast<ossim_sint16>(theNullPixelValue[band]);
   }
}

void ossimS16ImageData::loadBand(const void* src,
                                 const ossimIrect& src_rect,
                                 ossim_uint32 band)
{
   ossimImageData::loadBand(ossim_sint16(0), src, src_rect, band);
}

void ossimS16ImageData::loadTile(void* src,
                                const ossimIrect& src_rect,
                                ossimInterleaveType il_type)
{
   // Load the tile from the buffer.
   switch (il_type)
   {
      case OSSIM_BIP:
         ossimImageData::loadTileFromBip(ossim_sint16(0), src, src_rect);
         break;
      case OSSIM_BIL:
         ossimImageData::loadTileFromBil(ossim_sint16(0), src, src_rect);
         break;
      case OSSIM_BSQ:
         ossimImageData::loadTileFromBsq(ossim_sint16(0), src, src_rect);
         break;
      default:
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimS16ImageData::loadTile"
            << "\nUnknown interleave type passed to method.  Returning..."
            << endl;
         break;
   } 
}

void ossimS16ImageData::unloadBand(void* dest,
                                   const ossimIrect& dest_rect,
                                   ossim_uint32 band) const
{
   ossimImageData::unloadBand(ossim_sint16(0), dest, dest_rect,
                              getImageRectangle(), band);
}

void ossimS16ImageData::copyTileToNormalizedBuffer(double* buf) const
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimS16ImageData::copyTileToNormalizedBuffer ERROR:"
           << "\nBuffer passed to method is NULL!"
           << endl;
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         const ossim_sint16* s = getSshortBuf(band);  // source
         ossim_float64*      d = buf + (band*size);   // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            getNormFromPix(s[index], d[index], band);
         }
      }
   }
}

void ossimS16ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
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
      const ossim_sint16* s = getSshortBuf(band);  // source
      double* d = buf;   // destination
      
      for(ossim_uint32 index = 0; index < size; index++)
      {
         getNormFromPix(*s, *d, band);
      }
   }
}

void ossimS16ImageData::copyNormalizedBufferToTile(double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimS16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         ossim_float64* s = buf + (band*size);  // source
         ossim_sint16*  d = getSshortBuf(band); // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            ossim_int32 p;
            getPixFromNorm(p, s[index], band);
            d[index] = static_cast<ossim_sint16>(p);
         }
      }
   }
}

void ossimS16ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
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
      ossim_sint16* d = getSshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; index++)
      {
         ossim_int32 p;
         getPixFromNorm(p, *s, band);
         *d = static_cast<ossim_sint16>(p);
         ++d;
         ++s;
      }
   }
}

void ossimS16ImageData::copyTileToNormalizedBuffer(float* buf) const
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimS16ImageData::copyTileToNormalizedBuffer ERROR:"
         << "\nBuffer passed to method is NULL!" << endl;
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         const ossim_sint16* s = getSshortBuf(band);  // source
         float*    d = buf + (band*size);   // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            getNormFromPix(s[index], d[index], band);
         }
      }
   }
}

void ossimS16ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
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
      const ossim_sint16* s = getSshortBuf(band);  // source
      float* d = buf;   // destination
      
      for(ossim_uint32 index = 0; index < size; index++)
      {
         getNormFromPix(*s, *d, band);
      }
   }
}

void ossimS16ImageData::copyNormalizedBufferToTile(float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimS16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); band++)
      {
         float*    s = buf + (band*size);  // source
         ossim_sint16* d = getSshortBuf(band); // destination

         for(ossim_uint32 index = 0; index < size; index++)
         {
            ossim_int32 p;
            getPixFromNorm(p, s[index], band);
            d[index] = static_cast<ossim_sint16>(p);
         }
      }
   }
}

void ossimS16ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
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
      ossim_sint16* d = getSshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; index++)
      {
         ossim_int32 p;
         getPixFromNorm(p, *s, band);
         *d = static_cast<ossim_sint16>(p);
         ++d;
         ++s;
      }
   }
}


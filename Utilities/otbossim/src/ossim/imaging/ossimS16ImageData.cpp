//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Class definition of ossimS16ImageData.  Specialized image data object for
// signed short data.
//
//*************************************************************************
// $Id: ossimS16ImageData.cpp 16052 2009-12-08 22:20:40Z dburken $

#include <ossim/imaging/ossimS16ImageData.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>

RTTI_DEF1(ossimS16ImageData, "ossimS16ImageData", ossimImageData)
   
const ossimNormalizedS16RemapTable ossimS16ImageData::m_remapTable;

ossimS16ImageData::ossimS16ImageData()
   :
      ossimImageData()
{
   m_scalarType = OSSIM_SINT16;
}

ossimS16ImageData::ossimS16ImageData(ossimSource* source,
                                     ossim_uint32 bands)
   :
      ossimImageData(source,
                     OSSIM_SINT16,
                     bands)
{
}

ossimS16ImageData::ossimS16ImageData(ossimSource* source,
                                     ossim_uint32 bands,
                                     ossim_uint32 width,
                                     ossim_uint32 height)
   :
      ossimImageData(source,
                     OSSIM_SINT16,
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
   if (getBuf() == NULL)
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
      ossim_sint16 np = (ossim_sint16)m_nullPixelValue[band];
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
   if( (getDataObjectStatus() != OSSIM_NULL) && isValidBand(bandNumber) )
   {
      const ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
      result =
         static_cast<float>(m_remapTable.normFromPix(sourceBuf[offset]));
   }
}

void ossimS16ImageData::setNormalizedFloat(ossim_uint32 offset,
                                           ossim_uint32 bandNumber,
                                           float inputValue)
{
   if( (getDataObjectStatus() != OSSIM_NULL) &&  isValidBand(bandNumber) )
   {
      ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
      sourceBuf[offset]
         = static_cast<ossim_sint16>(m_remapTable.pixFromNorm(inputValue));
   }
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
      for(ossim_uint32 bandCount = 0;
          bandCount < m_numberOfDataComponents;
          ++bandCount)
      {
         const ossim_sint16* sourceBuf = getSshortBuf(bandCount);
         float* resultBuf = static_cast<float*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter]
               = static_cast<float>(m_remapTable.
                                    normFromPix(sourceBuf[counter]));
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
      for(ossim_uint32 bandCount = 0;
          bandCount < m_numberOfDataComponents;
          ++bandCount)
      {
         const ossim_sint16* sourceBuf = getSshortBuf(bandCount);
         double* resultBuf = static_cast<double*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter] = m_remapTable.normFromPix(sourceBuf[counter]);
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
         for(bandCount = 0; bandCount < m_numberOfDataComponents; ++bandCount)
         {
            float* sourceBuf =
               static_cast<float*>(normalizedInput->getBuf(bandCount));
            ossim_sint16* resultBuf = getSshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_sint16>(m_remapTable.
                                              pixFromNorm(sourceBuf[counter]));
            }
         }
      }
      else // Double
      {
         for(bandCount = 0; bandCount < m_numberOfDataComponents; ++bandCount)
         {
            double* sourceBuf =
               static_cast<double*>(normalizedInput->getBuf(bandCount));
            ossim_sint16* resultBuf = getSshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_sint16>(m_remapTable.
                                              pixFromNorm(sourceBuf[counter]));
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

void ossimS16ImageData::setValue(ossim_int32 x, ossim_int32 y, double color)
{
   if(getBuf() != NULL && isWithin(x, y))
   {
      //***
      // Compute the offset into the buffer for (x,y).  This should always
      // come out positive.
      //***
      ossim_uint32 ux = static_cast<ossim_uint32>(x - m_origin.x);
      ossim_uint32 uy = static_cast<ossim_uint32>(y - m_origin.y);

      ossim_uint32 offset = uy * m_spatialExtents[0] + ux;
      
      for(ossim_uint32 band = 0; band < m_numberOfDataComponents; band++)
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
      
      if((*buf) != m_nullPixelValue[band])
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
      *buf       = static_cast<ossim_sint16>(m_nullPixelValue[band]);
   }
}

void ossimS16ImageData::copyTileToNormalizedBuffer(double* buf) const
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

   const ossim_uint32 SIZE = getSizePerBand();
   
   if(SIZE > 0)
   {   
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         const ossim_sint16* s = getSshortBuf(band);  // source
         double* d = buf + (band*SIZE);   // destination
         
         for(ossim_uint32 index = 0; index < SIZE; ++index)
         {
            d[index] = m_remapTable.
               normFromPix(static_cast<ossim_int32>(s[index]));
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
                    "ossimS16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   if(!getBuf(band)) return;
   
   const ossim_uint32 SIZE = getSizePerBand();
   
   if(SIZE)
   {
      const ossim_float64 RANGE = (getMaxPix(band)-getMinPix(band)+1);
      
      const ossim_sint16* s = getSshortBuf(band);  // source
      double* d = buf;   // destination
      
      for(ossim_uint32 index = 0; index < SIZE; ++index)
      {
         ossim_float64 p = s[index];
         
         if (p == m_nullPixelValue[band])
         {
            d[index] = 0.0;
         }
         else if (p >= m_maxPixelValue[band])
         {
            d[index] = 1.0;
         }
         else
         {
            //---
            // Normalize...
            // Note that this will shift any negatives to positive prior
            // to dividing.
            //---
            d[index] =
               ((p-m_minPixelValue[band]+1) / RANGE);
         }
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
         double* s = buf + (band*size); // source
         ossim_sint16* d = getSshortBuf(band); // destination
         
         for(ossim_uint32 index = 0; index <  size; index++)
         {
            d[index] = m_remapTable.pixFromNorm(s[index]);
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
                    "ossimS11ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if((size > 0)&&getBuf(band))
   {
      double* s = buf; // source
      ossim_sint16* d = getSshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         *d = m_remapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}

void ossimS16ImageData::copyTileToNormalizedBuffer(float* buf) const
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
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         const ossim_sint16* s = getSshortBuf(band);  // source
         float* d = buf + (band*size);   // destination
         
         for(ossim_uint32 index = 0; index < size; ++index)
         {
            d[index] = m_remapTable.
               normFromPix(static_cast<ossim_int32>(s[index]));
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
                    "ossimS16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
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
      
      for(ossim_uint32 index = 0; index < size; ++index)
      {
         *d = m_remapTable.normFromPix(static_cast<ossim_int32>(*s));
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
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         float* s = buf + (band*size); // source
         ossim_sint16* d = getSshortBuf(band); // destination
         
         for(ossim_uint32 index = 0; index <  size; ++index)
         {
            d[index] = m_remapTable.pixFromNorm(s[index]);
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
                    "ossimS16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   
   ossim_uint32 size = getSizePerBand();
   
   if((size > 0)&&getBuf(band))
   {
      float* s = buf; // source
      ossim_sint16* d = getSshortBuf(band); // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         *d = m_remapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}


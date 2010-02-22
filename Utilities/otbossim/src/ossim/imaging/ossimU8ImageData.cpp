//*******************************************************************
// 
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Class definition of ossimU8ImageData.  Specialized image data object for
// unsigned char data.
//
// NOTE:  This object is optimized for unsigned char data and assumes the
//        following:  null pixel value  = 0.0
//                    min  pixel value  = 1.0
//                    max  pixel value  = 255.0
//
//        If you want anything else use the less efficient ossimImageData.
//
//*************************************************************************
// $Id: ossimU8ImageData.cpp 16052 2009-12-08 22:20:40Z dburken $

#include <cstring>  // for memset function
using namespace std;

#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>

RTTI_DEF1(ossimU8ImageData, "ossimU8ImageData", ossimImageData)

const ossimNormalizedU8RemapTable ossimU8ImageData::m_remapTable;
   
ossimU8ImageData::ossimU8ImageData(ossimSource* source,
                                   ossim_uint32 bands)
   :
      ossimImageData(source,
                     OSSIM_UCHAR,
                     bands)
{
}

ossimU8ImageData::ossimU8ImageData(ossimSource* source,
                                   ossim_uint32 bands,
                                   ossim_uint32 width,
                                   ossim_uint32 height)
   :
      ossimImageData(source,
                     OSSIM_UCHAR,
                     bands,
                     width,
                     height)
{
}

ossimU8ImageData::~ossimU8ImageData()
{
}

ossimObject* ossimU8ImageData::dup()const
{
   return new ossimU8ImageData(*this);
}

void ossimU8ImageData::fill(double value)
{
   ossimImageData::fill(value);
}

ossimDataObjectStatus ossimU8ImageData::validate() const
{
   if (m_dataBuffer.size() == 0)
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
      const ossim_uint8* p  = getUcharBuf(band);
      ossim_uint8 np = (ossim_uint8)getNullPix(band);
      for (ossim_uint32 i=0; i<BOUNDS; ++i)
      {
         // check if the band is null
         if (p[i] != np) ++count;         
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

void ossimU8ImageData::getNormalizedFloat(ossim_uint32 offset,
                                          ossim_uint32 bandNumber,
                                          float& result)const
{
   // make sure that the types and width and height are good.
   if( (getDataObjectStatus() == OSSIM_NULL) && isValidBand(bandNumber) )
   {
      return;
   }
   
   const ossim_uint8* sourceBuf = getUcharBuf(bandNumber);
   result = static_cast<float>(m_remapTable.normFromPix(sourceBuf[offset]));
}

void ossimU8ImageData::setNormalizedFloat(ossim_uint32 offset,
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

   ossim_uint8* sourceBuf = getUcharBuf(bandNumber);
   sourceBuf[offset]
      = static_cast<ossim_uint8>(m_remapTable.pixFromNorm(inputValue));
}

void ossimU8ImageData::convertToNormalizedFloat(ossimImageData* result)const
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
      for(ossim_uint32 bandCount = 0; bandCount < m_numberOfDataComponents; ++bandCount)
      {
         const ossim_uint8* sourceBuf = getUcharBuf(bandCount);
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

void ossimU8ImageData::convertToNormalizedDouble(ossimImageData* result)const
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
      for(ossim_uint32 bandCount = 0; bandCount < m_numberOfDataComponents; ++bandCount)
      {
         const ossim_uint8* sourceBuf = getUcharBuf(bandCount);
         double* resultBuf = static_cast<double*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter] = m_remapTable.normFromPix(sourceBuf[counter]);
         }
      }
   }
}

void ossimU8ImageData::unnormalizeInput(ossimImageData* normalizedInput)
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
            ossim_uint8* resultBuf = getUcharBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint8>(m_remapTable.
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
            ossim_uint8* resultBuf = getUcharBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint8>(m_remapTable.
                                       pixFromNorm(sourceBuf[counter]));
            }
         }
      }
   }   
}

double ossimU8ImageData::computeMeanSquaredError(double meanValue,
                                                 ossim_uint32 bandNumber)
{
   double result = -1; // invalid MSE
   ossim_uint32 index = 0;
   double delta = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_uint8* buffer = getUcharBuf(bandNumber);
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

double ossimU8ImageData::computeAverageBandValue(ossim_uint32 bandNumber)
{
   double result = 0.0;
   ossim_uint32 index = 0;
   ossim_uint32 validPixelCount=0;
   
   ossim_uint8* buffer = getUcharBuf(bandNumber);
   if(buffer)
   {
      ossim_uint32 bounds = getWidth()*getHeight();
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

void ossimU8ImageData::setValue(ossim_int32 x, ossim_int32 y, double color)
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
      
      for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
      {
         ossim_uint8* buf = getUcharBuf(band)+offset;
         *buf = (ossim_uint8)color;
      }
   }
}

void ossimU8ImageData::fill(ossim_uint32 band, double value)
{
   void* s         = getBuf(band);

   if (s == NULL) return; // nothing to do...

   ossim_uint32 size_in_pixels = getSizePerBand();
   ossim_uint8* p = getUcharBuf(band);
   ossim_uint8 np = static_cast<ossim_uint8>(value);

   for (ossim_uint32 i=0; i<size_in_pixels; ++i) p[i] = np;

   // Set the status to unknown since we don't know about the other bands.
   setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
}


bool ossimU8ImageData::isNull(ossim_uint32 offset)const
{
   for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)  
   {
      const ossim_uint8* buf = getUcharBuf(band)+offset;
      
      if((*buf) != m_nullPixelValue[band])
      {
         return false;
      }
   }

   return true;
}

void ossimU8ImageData::setNull(ossim_uint32 offset)
{
   for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)  
   {
      ossim_uint8* buf = (getUcharBuf(band))+offset;
      *buf       = (ossim_uint8)m_nullPixelValue[band];
   }
}

void ossimU8ImageData::copyTileToNormalizedBuffer(double* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   ossim_uint32 size = getSizePerBand();
   if(!getBuf())
   {
      memset(buf, '\0', size);
      return;
   }
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         const ossim_uint8*  s = getUcharBuf(band);   // source
         double* d = buf + (band*size);   // destination
         if(s&&d)
         {
            for(ossim_uint32 index = 0; index < size; ++index)
            {
               d[index] = m_remapTable.
                          normFromPix(static_cast<ossim_int32>(s[index]));
            }
         }
      }
   }
}

void ossimU8ImageData::copyTileToNormalizedBuffer(float* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   ossim_uint32 size = getSizePerBand();
   if(!getBuf())
   {
      memset(buf, '\0', size);
      return;
   }
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         const ossim_uint8*  s = getUcharBuf(band);   // source
         float* d = buf + (band*size);   // destination
         if(s&&d)
         {
            for(ossim_uint32 index = 0; index < size; ++index)
            {
               d[index] = m_remapTable.
                          normFromPix(static_cast<ossim_int32>(s[index]));
            }
         }
      }
   }
}

void ossimU8ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
                                                  double* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   ossim_uint32 size = getSizePerBand();
   if(!getBuf(band))
   {
      memset(buf, '\0', size);
      return;
   }
   
   if(size > 0)
   {
      const ossim_uint8*  s = getUcharBuf(band);   // source
      double* d = buf;   // destination
      for(ossim_uint32 index = 0; index < size; ++index)
      {
         *d = m_remapTable.
              normFromPix(static_cast<ossim_int32>(*s));
         ++d;
         ++s;
      }
   }
}

void ossimU8ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
                                                  float* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   ossim_uint32 size = getSizePerBand();
   if(!getBuf(band))
   {
      memset(buf, '\0', size);
      return;
   }
   
   if(size > 0)
   {
      const ossim_uint8*  s = getUcharBuf(band);   // source
      float* d = buf;   // destination
      for(ossim_uint32 index = 0; index < size; ++index)
      {
         *d = m_remapTable.
              normFromPix(static_cast<ossim_int32>(*s));
         ++d;
         ++s;
      }
   }
}

void ossimU8ImageData::copyNormalizedBufferToTile(double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf()) return;
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         double* s = buf + (band*size);  // source
         ossim_uint8*  d = getUcharBuf(band);  // destination

         for(ossim_uint32 index = 0; index <  size; ++index)
         {
            d[index] = m_remapTable.pixFromNorm(s[index]);
         }
      }
   }
}

void ossimU8ImageData::copyNormalizedBufferToTile(float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf()) return;
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      for(ossim_uint32 band = 0; band < getNumberOfBands(); ++band)
      {
         float* s = buf + (band*size);  // source
         ossim_uint8*  d = getUcharBuf(band);  // destination

         for(ossim_uint32 index = 0; index < size; ++index)
         {
            d[index] = m_remapTable.pixFromNorm(s[index]);
         }
      }
   }
}

void ossimU8ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                  double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf(band)) return;
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      double* s = buf;  // source
      ossim_uint8*  d = getUcharBuf(band);  // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         d[index] = m_remapTable.pixFromNorm(s[index]);
      }
   }
}

void ossimU8ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                  float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU8ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
                    __FILE__,
                    __LINE__);
      return;
   }
   if(!getBuf(band)) return;
   ossim_uint32 size = getSizePerBand();
   
   if(size > 0)
   {
      float* s = buf;  // source
      ossim_uint8*  d = getUcharBuf(band);  // destination
      
      for(ossim_uint32 index = 0; index <  size; ++index)
      {
         d[index] = m_remapTable.pixFromNorm(s[index]);
      }
   }
}


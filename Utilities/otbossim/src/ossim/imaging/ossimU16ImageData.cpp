//*******************************************************************
//
// License:   See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Class definition of ossimU16ImageData.  Specialized image data object for
// unsigned short data.
//
// NOTE:  This object is optimized for unsigned short data and assumes the
//        following:  null pixel value  = 0.0
//                    min  pixel value  = 1.0
//                    max  pixel value  = 65535.0  (2^16 - 1)
//
//        If you want anything else use the less efficient ossimImageData.
//
//*************************************************************************
// $Id: ossimU16ImageData.cpp 16052 2009-12-08 22:20:40Z dburken $

#include <ossim/imaging/ossimU16ImageData.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>

RTTI_DEF1(ossimU16ImageData, "ossimU16ImageData", ossimImageData)

const ossimNormalizedU16RemapTable ossimU16ImageData::m_remapTable;
   
ossimU16ImageData::ossimU16ImageData()
   :
      ossimImageData()
{
   m_scalarType = OSSIM_UINT16;
}

ossimU16ImageData::ossimU16ImageData(ossimSource* source,
                                     ossim_uint32 bands)
   :
      ossimImageData(source, OSSIM_UINT16, bands)
{
}

ossimU16ImageData::ossimU16ImageData(ossimSource* source,
                                     ossim_uint32 bands,
                                     ossim_uint32 width,
                                     ossim_uint32 height)
   :
      ossimImageData(source, OSSIM_UINT16, bands, width, height)
{
}

ossimU16ImageData::ossimU16ImageData(const ossimU16ImageData &rhs)
   :
      ossimImageData(rhs)
{
}

ossimU16ImageData::~ossimU16ImageData()
{
}

ossimObject* ossimU16ImageData::dup()const
{
   return new ossimU16ImageData(*this);
}

ossimDataObjectStatus ossimU16ImageData::validate() const
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

void ossimU16ImageData::getNormalizedFloat(ossim_uint32 offset,
                                           ossim_uint32 bandNumber,
                                           float& result)const
{
   if( (getDataObjectStatus() != OSSIM_NULL) && isValidBand(bandNumber) )
   {
      const ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
      result =
         static_cast<float>(m_remapTable.normFromPix(sourceBuf[offset]));
   }
}

void ossimU16ImageData::setNormalizedFloat(ossim_uint32 offset,
                                           ossim_uint32 bandNumber,
                                           float inputValue)
{
   if( (getDataObjectStatus() != OSSIM_NULL) &&  isValidBand(bandNumber) )
   {
      ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
      sourceBuf[offset]
         = static_cast<ossim_uint16>(m_remapTable.pixFromNorm(inputValue));
   }
}

void
ossimU16ImageData::convertToNormalizedFloat(ossimImageData* result)const
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
         const ossim_uint16* sourceBuf = getUshortBuf(bandCount);
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

void ossimU16ImageData::convertToNormalizedDouble(ossimImageData* result)const
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
         const ossim_uint16* sourceBuf = getUshortBuf(bandCount);
         double* resultBuf = static_cast<double*>(result->getBuf(bandCount));
         for(ossim_uint32 counter = 0; counter <  size; ++counter)
         {
            resultBuf[counter] = m_remapTable.normFromPix(sourceBuf[counter]);
         }
      }
   }

}

void ossimU16ImageData::unnormalizeInput(ossimImageData* normalizedInput)
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
            ossim_uint16* resultBuf = getUshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint16>(m_remapTable.
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
            ossim_uint16* resultBuf = getUshortBuf(bandCount);
            for(counter = 0; counter <  size; ++counter)
            {
               resultBuf[counter]
                  = static_cast<ossim_uint16>(m_remapTable.
                                          pixFromNorm(sourceBuf[counter]));
            }
         }
      }
   }   
}

double ossimU16ImageData::computeMeanSquaredError(double meanValue,
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

double ossimU16ImageData::computeAverageBandValue(ossim_uint32 bandNumber)
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

void ossimU16ImageData::setValue(long x, long y, double color)
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
         ossim_uint16* buf = getUshortBuf(band)+offset;
         *buf = (ossim_uint16)color;
      }
   }
}

void ossimU16ImageData::fill(ossim_uint32 band, double value)
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

void ossimU16ImageData::copyTileToNormalizedBuffer(double* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
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
            d[index] = m_remapTable.
                       normFromPix(static_cast<ossim_int32>(s[index]));
         }
      }
   }
}

void ossimU16ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
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
         *d = m_remapTable.
              normFromPix(static_cast<ossim_int32>(*s));
      }
   }
}

void ossimU16ImageData::copyNormalizedBufferToTile(double* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
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
            d[index] = m_remapTable.pixFromNorm(s[index]);
         }
      }
   }
}

void ossimU16ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
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
      
      for(ossim_uint32 index = 0; index <  size; index++)
      {
         *d = m_remapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}

void ossimU16ImageData::copyTileToNormalizedBuffer(float* buf) const
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
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
            d[index] = m_remapTable.
                       normFromPix(static_cast<ossim_int32>(s[index]));
         }
      }
   }
}

void ossimU16ImageData::copyTileToNormalizedBuffer(ossim_uint32 band,
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
         *d = m_remapTable.
              normFromPix(static_cast<ossim_int32>(*s));
      }
   }
}

void ossimU16ImageData::copyNormalizedBufferToTile(float* buf)
{
   if (!buf)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimU16ImageData::copyTileToNormalizedBuffer File %s line %d\nNull pointer passed to method!",
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
            d[index] = m_remapTable.pixFromNorm(s[index]);
         }
      }
   }
}

void ossimU16ImageData::copyNormalizedBufferToTile(ossim_uint32 band,
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
      
      for(ossim_uint32 index = 0; index <  size; index++)
      {
         *d = m_remapTable.pixFromNorm(*s);
         ++d;
         ++s;
      }
   }
}


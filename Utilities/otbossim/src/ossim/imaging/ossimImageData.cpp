//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageData.cpp 22161 2013-02-25 12:10:04Z gpotts $

#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimScalarTypeLut.h>

#include <iterator>
#include <ostream>


RTTI_DEF1(ossimImageData, "ossimImageData", ossimRectilinearDataObject)

ossimImageData::ossimImageData()
   : ossimRectilinearDataObject(2,            // 2d
                                0,         // owner
                                1,            // bands
                                OSSIM_UINT8), // scalar
     m_nullPixelValue(0),
     m_minPixelValue(0),
     m_maxPixelValue(0),
     m_alpha(0),
     m_origin(0, 0),
     m_indexedFlag(false)
{
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   m_spatialExtents[0] = tileSize.x;
   m_spatialExtents[1] = tileSize.y;
   initializeDefaults();
}

ossimImageData::ossimImageData(ossimSource*    owner,
                               ossimScalarType scalar,
                               ossim_uint32    bands)
   : ossimRectilinearDataObject(2,
                                owner,
                                bands,
                                scalar),
     m_nullPixelValue(0),
     m_minPixelValue(0),
     m_maxPixelValue(0),
     m_alpha(0),
     m_origin(0, 0),
     m_indexedFlag(false)
{
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   m_spatialExtents[0] = tileSize.x;
   m_spatialExtents[1] = tileSize.y;
   initializeDefaults();
}

ossimImageData::ossimImageData(ossimSource* owner,
                               ossimScalarType scalar,
                               ossim_uint32 bands,
                               ossim_uint32 width,
                               ossim_uint32 height)
   : ossimRectilinearDataObject(owner,
                               bands,
                               width, // 2-D array
                               height,
                               scalar),
     m_nullPixelValue(0),
     m_minPixelValue(0),
     m_maxPixelValue(0),
     m_alpha(0),
     m_origin(0, 0),
     m_indexedFlag(false)
{   
   m_spatialExtents[0] = width;
   m_spatialExtents[1] = height;
   initializeDefaults();
}

ossimImageData::ossimImageData(const ossimImageData &rhs)
   : ossimRectilinearDataObject(rhs),
     m_nullPixelValue(rhs.m_nullPixelValue),
     m_minPixelValue(rhs.m_minPixelValue),
     m_maxPixelValue(rhs.m_maxPixelValue),
     m_alpha(rhs.m_alpha),
     m_origin(rhs.m_origin),
     m_indexedFlag(rhs.m_indexedFlag)
{
}

const ossimImageData& ossimImageData::operator=(const ossimImageData& rhs)
{
   if (this != &rhs)
   {
      // ossimRectilinearDataObject initialization:
      ossimRectilinearDataObject::operator=(rhs);
      
      // ossimImageData (this) members:
      m_nullPixelValue = rhs.m_nullPixelValue;
      m_minPixelValue  = rhs.m_minPixelValue;
      m_maxPixelValue  = rhs.m_maxPixelValue;
      m_alpha          = rhs.m_alpha;
      m_origin         = rhs.m_origin;
      m_indexedFlag    = rhs.m_indexedFlag;
   }
   return *this;
}

ossimImageData::~ossimImageData()
{
}

bool ossimImageData::isValidBand(ossim_uint32 band) const
{
   return (band<getNumberOfDataComponents());
}

ossim_uint32 ossimImageData::getSize() const
{
   return (getSizePerBand() * getNumberOfDataComponents());
}

ossim_uint32 ossimImageData::getSizePerBand() const
{
   return (getHeight() * getWidth());
}

ossim_uint32 ossimImageData::getSizeInBytes() const
{
   return (getSizePerBandInBytes() * getNumberOfDataComponents());
}

ossim_uint32 ossimImageData::getSizePerBandInBytes() const
{
   return (getHeight() * getWidth() * getScalarSizeInBytes());
}

const ossim_uint8* ossimImageData::getAlphaBuf() const
{
   if (m_alpha.size() > 0)
   {
      return &m_alpha.front();
   }
   return 0;
}

ossim_uint8* ossimImageData::getAlphaBuf()
{
   if (m_alpha.size() > 0)
   {
      return &m_alpha.front();
   }
   return 0;
}

const void* ossimImageData::getBuf() const
{
   if (m_dataBuffer.size() > 0)
   {
      return static_cast<const void*>(&m_dataBuffer.front());
   }
   return 0;
}

void* ossimImageData::getBuf()
{
   if (m_dataBuffer.size() > 0)
   {
      return static_cast<void*>(&m_dataBuffer.front());
   }
   return 0;
}

const void* ossimImageData::getBuf(ossim_uint32 band) const
{
   const ossim_uint8* b = static_cast<const ossim_uint8*>(getBuf());
   
   if (isValidBand(band) && b != 0)
   {
      b += (band * getSizePerBandInBytes());
      return static_cast<const void*>(b);
   }
   return 0;
}

void* ossimImageData::getBuf(ossim_uint32 band)
{
   ossim_uint8* b = static_cast<ossim_uint8*>(getBuf());
   
   if (isValidBand(band) && b != 0)
   {
      b += (band * getSizePerBandInBytes());
      return static_cast<void*>(b);
   }
   return 0;
}

const ossim_uint8* ossimImageData::getUcharBuf() const
{
   if (m_scalarType == OSSIM_UINT8)
   {
      return static_cast<const ossim_uint8*>(getBuf());
   }
   return 0;
}

const ossim_uint16* ossimImageData::getUshortBuf() const
{
   if (m_scalarType == OSSIM_UINT16 ||
       m_scalarType == OSSIM_USHORT11)
   {
      return static_cast<const ossim_uint16*>(getBuf());
   }
   return 0;
}

const ossim_sint16* ossimImageData::getSshortBuf() const
{
   if (m_scalarType == OSSIM_SINT16)
   {
      return static_cast<const ossim_sint16*>(getBuf());
   }
   return 0;
}

const ossim_float32* ossimImageData::getFloatBuf() const
{
   if (m_scalarType == OSSIM_FLOAT32 ||
       m_scalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<const ossim_float32*>(getBuf());
   }
   return 0;
}

const ossim_float64* ossimImageData::getDoubleBuf() const
{
   if (m_scalarType == OSSIM_FLOAT64 ||
       m_scalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<const ossim_float64*>(getBuf());
   }
   return 0;
}

ossim_uint8* ossimImageData::getUcharBuf() 
{
   if (m_scalarType == OSSIM_UINT8)
   {
      return static_cast<ossim_uint8*>(getBuf());
   }
   return 0;
}

ossim_uint16* ossimImageData::getUshortBuf() 
{
   if (m_scalarType == OSSIM_UINT16 ||
       m_scalarType == OSSIM_USHORT11)
   {
      return static_cast<ossim_uint16*>(getBuf());
   }
   return 0;
}

ossim_sint16* ossimImageData::getSshortBuf() 
{
   if (m_scalarType == OSSIM_SINT16)
   {
      return static_cast<ossim_sint16*>(getBuf());
   }
   return 0;
}

ossim_float32* ossimImageData::getFloatBuf() 
{
   if (m_scalarType == OSSIM_FLOAT32 ||
       m_scalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<ossim_float32*>(getBuf());
   }
   return 0;
}

ossim_float64* ossimImageData::getDoubleBuf() 
{
   if (m_scalarType == OSSIM_FLOAT64 ||
       m_scalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<ossim_float64*>(getBuf());
   }
   return 0;
}

const ossim_uint8* ossimImageData::getUcharBuf(ossim_uint32 band) const
{
   if (m_scalarType == OSSIM_UINT8)
   {
      return static_cast<const ossim_uint8*>(getBuf(band));
   }
   return 0;
}

const ossim_uint16* ossimImageData::getUshortBuf(ossim_uint32 band) const
{
   if (m_scalarType == OSSIM_UINT16 ||
       m_scalarType == OSSIM_USHORT11)
   {
      return static_cast<const ossim_uint16*>(getBuf(band));
   }
   return 0;
}

const ossim_sint16* ossimImageData::getSshortBuf(ossim_uint32 band) const
{
   if (m_scalarType == OSSIM_SINT16)
   {
      return static_cast<const ossim_sint16*>(getBuf(band));
   }
   return 0;
}

const ossim_float32* ossimImageData::getFloatBuf(ossim_uint32 band) const
{
   if (m_scalarType == OSSIM_FLOAT32 ||
       m_scalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<const ossim_float32*>(getBuf(band));
   }
   return 0;
}

const ossim_float64* ossimImageData::getDoubleBuf(ossim_uint32 band) const
{
   if (m_scalarType == OSSIM_FLOAT64 ||
       m_scalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<const ossim_float64*>(getBuf(band));
   }
   return 0;
}

ossim_uint8* ossimImageData::getUcharBuf(ossim_uint32 band) 
{
   if (m_scalarType == OSSIM_UINT8)
   {
      return static_cast<ossim_uint8*>(getBuf(band));
   }
   return 0;
}

ossim_uint16* ossimImageData::getUshortBuf(ossim_uint32 band) 
{
   if (m_scalarType == OSSIM_UINT16 ||
       m_scalarType == OSSIM_USHORT11)
   {
      return static_cast<ossim_uint16*>(getBuf(band));
   }
   return 0;
}

ossim_sint16* ossimImageData::getSshortBuf(ossim_uint32 band) 
{
   if (m_scalarType == OSSIM_SINT16)
   {
      return static_cast<ossim_sint16*>(getBuf(band));
   }
   return 0;
}

ossim_float32* ossimImageData::getFloatBuf(ossim_uint32 band) 
{
   if (m_scalarType == OSSIM_FLOAT32 ||
       m_scalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<ossim_float32*>(getBuf(band));
   }
   return 0;
}

ossim_float64* ossimImageData::getDoubleBuf(ossim_uint32 band) 
{
   if (m_scalarType == OSSIM_FLOAT64 ||
       m_scalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<ossim_float64*>(getBuf(band));
   }
   return 0;
}

void ossimImageData::getNormalizedFloat(ossim_uint32 offset,
                                        ossim_uint32 bandNumber,
                                        ossim_float32& result)const
{
   // Make sure that the types and width and height are good.
   if( (getDataObjectStatus() != OSSIM_NULL) && (bandNumber < getNumberOfDataComponents()) )
   {
      ossim_float32 p = 0.0;

      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            const unsigned char* sourceBuf = getUcharBuf(bandNumber);
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_SINT8:
         {
            const ossim_sint8* sourceBuf = static_cast<const ossim_sint8*>(getBuf(bandNumber));
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_USHORT11:
         case OSSIM_UINT16:
         {
            const ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_SINT16:
         {
            const ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_UINT32:
         {
            const ossim_uint32* sourceBuf =
               static_cast<const ossim_uint32*>(getBuf(bandNumber));
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_SINT32:
         {
            const ossim_sint32* sourceBuf = static_cast<const ossim_sint32*>(getBuf(bandNumber));
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_FLOAT32:
         {
            const ossim_float32* sourceBuf = getFloatBuf(bandNumber);
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            const ossim_float64* sourceBuf = getDoubleBuf(bandNumber);
            p = sourceBuf[offset];
            break;
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            // Shouldn't hit this.
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimImageData::setNormalizedFloat Unsupported scalar type!"
               << std::endl;
         }
         
      } // Matches: switch (getScalarType())

      if ( p != m_nullPixelValue[bandNumber] )
      {
         const ossim_float32 DELTA = m_maxPixelValue[bandNumber] - m_minPixelValue[bandNumber] - 1;
         const ossim_float32 OFFSET_TO_ONE = 1 - m_minPixelValue[bandNumber];
         
         result = ( p <= m_maxPixelValue[bandNumber] ) ?
            ( ( p >= m_minPixelValue[bandNumber] ) ? ( p + OFFSET_TO_ONE ) / DELTA : 0.0 ) : 1.0;
      }
      else
      {
         result = 0.0;
      }
      
   } // Matches: if( (getDataObjectStatus() ...
   
} // End: ossimImageData::getNormalizedFloat

void ossimImageData::setNormalizedFloat(ossim_uint32 offset,
                                        ossim_uint32 bandNumber,
                                        ossim_float32 inputValue)
{
   // Make sure that the types and width and height are good.
   if( (getDataObjectStatus() != OSSIM_NULL) && (bandNumber < getNumberOfDataComponents()) )
   {

      ossim_float32 p = 0.0;

      if ( inputValue )
      {
         const ossim_float32 DELTA = m_maxPixelValue[bandNumber] - m_minPixelValue[bandNumber] - 1;
         const ossim_float32 OFFSET_TO_MIN = m_minPixelValue[bandNumber] - 1;
         p = inputValue * DELTA + OFFSET_TO_MIN + 0.5;
         if ( p > m_maxPixelValue[bandNumber] )
         {
            p = m_maxPixelValue[bandNumber];
         }
      }
      else
      {
         p = m_nullPixelValue[bandNumber];
      }
   
      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            unsigned char* sourceBuf = getUcharBuf(bandNumber);
            sourceBuf[offset] = static_cast<ossim_uint8>( p );
            break;
         }
         case OSSIM_SINT8:
         {
            ossim_sint8* sourceBuf = static_cast<ossim_sint8*>(getBuf(bandNumber));
            sourceBuf[offset] = static_cast<ossim_sint8>( p );
            break;
         }
         case OSSIM_USHORT11:
         case OSSIM_UINT16:
         {
            ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
            sourceBuf[offset] = static_cast<ossim_uint16>( p );
            break;
         }
         case OSSIM_SINT16:
         {
            ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
            sourceBuf[offset] = static_cast<ossim_sint16>( p );
            break;
         }
         case OSSIM_UINT32:
         {
            ossim_uint32* sourceBuf = static_cast<ossim_uint32*>(getBuf(bandNumber));
            sourceBuf[offset] = static_cast<ossim_uint32>( p );
            break;
         }
         case OSSIM_SINT32:
         {
            ossim_sint32* sourceBuf = static_cast<ossim_sint32*>(getBuf(bandNumber));
            sourceBuf[offset] = static_cast<ossim_sint32>( p );
            break;
         }
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_FLOAT32:
         {
            ossim_float32* sourceBuf = getFloatBuf(bandNumber);
            sourceBuf[offset] = p;
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            ossim_float64* sourceBuf = getDoubleBuf(bandNumber);
            sourceBuf[offset] = p;
            break;
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
            // Shouldn't hit this.
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimImageData::setNormalizedFloat Unsupported scalar type!"
               << std::endl;
            
      } // Matches: switch (getScalarType())

   } // Matches: if( (getDataObjectStatus() ...
   
} // End: ossimImageData::setNormalizedFloat

void ossimImageData::convertToNormalizedFloat(ossimImageData* result)const
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

   copyTileToNormalizedBuffer((ossim_float32*)result->getBuf());
   result->setDataObjectStatus(getDataObjectStatus());
}

ossimRefPtr<ossimImageData> ossimImageData::newNormalizedFloat()const
{
   ossimRefPtr<ossimImageData> result =
      new ossimImageData(0,
                         OSSIM_NORMALIZED_FLOAT,
                         this->getNumberOfBands(),
                         this->getWidth(),
                         this->getHeight());
   
   result->initialize();
   
   convertToNormalizedFloat(result.get());
   
   return result;
}

void ossimImageData::convertToNormalizedDouble(ossimImageData* result)const
{
   if(!result)
   {
      return;
   }
   // make sure that the types and width and height are
   // good.
   if( (result->getScalarType() != OSSIM_NORMALIZED_DOUBLE) ||
       (result->getNumberOfBands() != this->getNumberOfBands())||
       (result->getWidth() != this->getWidth()) ||
       (result->getHeight() != this->getHeight())||
       (result->getDataObjectStatus() == OSSIM_NULL) ||
       (getDataObjectStatus() == OSSIM_NULL))
   {
      return;
   }

   copyTileToNormalizedBuffer((ossim_float64*)result->getBuf());
   result->setDataObjectStatus(getDataObjectStatus());
}

ossimImageData* ossimImageData::newNormalizedDouble()const
{
   ossimImageData* result = new ossimImageData(0,
                                               OSSIM_NORMALIZED_DOUBLE,
                                               this->getNumberOfBands(),
                                               this->getWidth(),
                                               this->getHeight());
   result->initialize();

   convertToNormalizedDouble(result);
   
   return result;   
}


void ossimImageData::unnormalizeInput(ossimImageData* normalizedInput)
{
   if((normalizedInput->getScalarType() != OSSIM_NORMALIZED_FLOAT) &&
      (normalizedInput->getScalarType() != OSSIM_NORMALIZED_DOUBLE) )
   {
      //ERROR
      return;
   }

   if(normalizedInput->getScalarType() == OSSIM_NORMALIZED_DOUBLE)
   {
      copyNormalizedBufferToTile((ossim_float64*)normalizedInput->getBuf());
   }
   else
   {
      copyNormalizedBufferToTile((ossim_float32*)normalizedInput->getBuf());
   }
}

ossim_float64 ossimImageData::computeMeanSquaredError(
   ossim_float64 meanValue,
   ossim_uint32 bandNumber) const
{
   ossim_float64 result = -1; // invalid MSE
   
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         result = computeMeanSquaredError(ossim_uint8(0),
                                          meanValue,
                                          bandNumber);
         break;
      }
      case OSSIM_SINT8:
      {
         result = computeMeanSquaredError(ossim_sint8(0),
                                          meanValue,
                                          bandNumber);
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         result = computeMeanSquaredError(ossim_uint16(0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_SINT16:
      {
         result = computeMeanSquaredError(ossim_sint16(0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_UINT32:
      {
         result = computeMeanSquaredError(ossim_uint32(0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_SINT32:
      {
         result = computeMeanSquaredError(ossim_sint32(0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         result = computeMeanSquaredError(ossim_float32(0.0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         result = computeMeanSquaredError(ossim_float64(0.0),
                                          meanValue,
                                          bandNumber);
         break;
      }  
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::computeMeanSquaredError File %s line %d\n\
Invalid scalar type:  %d",
                       __FILE__,
                       __LINE__,
                       getScalarType());
         break;
      }
   }

   return result;
}
   
template <class T> ossim_float64 ossimImageData::computeMeanSquaredError(
   T, /* dummyTemplate */
   ossim_float64 meanValue,
   ossim_uint32 bandNumber) const
{
   ossim_float64  result               = -1.0; // invalid MSE

   if ( (getDataObjectStatus() == OSSIM_NULL) ||
        (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return result;
   }
   
   ossim_uint32  index           = 0;
   ossim_float64 delta           = 0.0;
   ossim_uint32  validPixelCount = 0;

   const T* BUFFER = static_cast<const T*>(getBuf(bandNumber));
   if(BUFFER)
   {
      const ossim_uint32 BOUNDS = getSizePerBand();
      for(index = 0; index < BOUNDS; ++index)
      {
         if(!isNull(index))
         {
            delta = BUFFER[index] - meanValue;
            result += (delta*delta);
            ++validPixelCount;
         }
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
void ossimImageData::populateHistogram(ossimRefPtr<ossimMultiBandHistogram> histo)
{
   ossim_uint32 numberOfBands = getNumberOfBands();

   if( (getDataObjectStatus() == OSSIM_NULL) ||
       (getDataObjectStatus() == OSSIM_EMPTY)||
       (!histo))
   {
      return;
   }
   switch(getScalarType())
   {
      case OSSIM_UINT8:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            if(currentHisto.valid())
            {
               float* histoBins = currentHisto->GetCounts();
               int binCount = currentHisto->GetRes();
               ossim_uint8* buffer = (ossim_uint8*)getBuf(band);
               ossim_uint32 upperBound = getWidth()*getHeight();
               if ( binCount == 256 )
               {
                  for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
                  {
                     ++histoBins[ buffer[offset] ];
                  }
               }
               else
               {
                 for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
                  {
                     currentHisto->UpCount((float)buffer[offset]);
                  }
               }
            }
         }
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
	 
             ossim_uint16* buffer = (ossim_uint16*)getBuf(band);
             if(currentHisto.valid())
             {
                ossim_uint32 upperBound = getWidth()*getHeight();
                for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
                {
                  currentHisto->UpCount((float)buffer[offset]);
                }
             }
         }
         break;
      }
      case OSSIM_SINT16:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            ossim_sint16* buffer = (ossim_sint16*)getBuf(band);
            
            if(currentHisto.valid())
            {
               ossim_uint32 upperBound = getWidth()*getHeight();
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  currentHisto->UpCount((float)buffer[offset]);
               }
            }
         }
         break;
      }
      case OSSIM_SINT32:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            ossim_sint32* buffer = (ossim_sint32*)getBuf(band);
            
            if(currentHisto.valid())
            {
               ossim_uint32 upperBound = getWidth()*getHeight();
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  currentHisto->UpCount((float)buffer[offset]);
               }
            }
         }
         break;
      }
      case OSSIM_UINT32:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            ossim_uint32* buffer = (ossim_uint32*)getBuf(band);
            
            if(currentHisto.valid())
            {
               ossim_uint32 upperBound = getWidth()*getHeight();
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  currentHisto->UpCount((float)buffer[offset]);
               }
            }
         }
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            ossim_float64* buffer = (ossim_float64*)getBuf(band);
            
            if(currentHisto.valid())
            {
               ossim_uint32 upperBound = getWidth()*getHeight();
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  currentHisto->UpCount((float)buffer[offset]);
               }
            }
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            ossimRefPtr<ossimHistogram> currentHisto = histo->getHistogram(band);
            ossim_float32* buffer = (ossim_float32*)getBuf(band);
            
            if(currentHisto.valid())
            {
               ossim_uint32 upperBound = getWidth()*getHeight();
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  currentHisto->UpCount((float)buffer[offset]);
               }
            }
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         //ERROR
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::populateHistogram\n"
            << "Unknown scalar type." << std::endl;
      }
   }  // end of switch
}

ossim_float64 ossimImageData::computeAverageBandValue(ossim_uint32 bandNumber) const
{
   ossim_float64 result = 0.0;
   
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         result = computeAverageBandValue(ossim_uint8(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_SINT8:
      {
         result = computeAverageBandValue(ossim_sint8(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         result = computeAverageBandValue(ossim_uint16(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_SINT16:
      {
         result = computeAverageBandValue(ossim_sint16(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_UINT32:
      {
         result = computeAverageBandValue(ossim_uint32(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_SINT32:
      {
         result = computeAverageBandValue(ossim_sint32(0),
                                          bandNumber);
         break;
      }  
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         result = computeAverageBandValue(ossim_float32(0.0),
                                          bandNumber);
         break;
      }  
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         result = computeAverageBandValue(ossim_float64(0.0),
                                          bandNumber);
         break;
      }  
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::computeAverageBandValue File %s line %d\n\
Invalid scalar type:  %d",
                       __FILE__,
                       __LINE__,
                       getScalarType());
         break;
      }
   }

   return result;
}

template <class T> ossim_float64 ossimImageData::computeAverageBandValue(
   T, /* dummy */
   ossim_uint32 bandNumber) const
{
   ossim_float64  result = 0.0;

   if ( (getDataObjectStatus() == OSSIM_NULL) ||
        (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return result;
   }
   
   ossim_uint32 index = 0;
   ossim_uint32 validPixelCount = 0;

   const T* BUFFER = static_cast<const T*>(getBuf(bandNumber));
   if(BUFFER)
   {
      const ossim_uint32 BOUNDS = getSizePerBand();
      for(index = 0; index < BOUNDS; ++index)
      {
         if(!isNull(index))
         {
            result += BUFFER[index];
            ++validPixelCount;
         }
      }
      if(validPixelCount > 0)
      {
         result /= validPixelCount;
      }
   }

   return result;
}

ossimDataObjectStatus ossimImageData::validate() const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         return validate(ossim_uint8(0));
      }
      case OSSIM_SINT8:
      {
         return validate(ossim_sint8(0));
      }
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         return validate(ossim_uint16(0));
      }  
      case OSSIM_SINT16:
      {
         return validate(ossim_sint16(0));
      }
         
      case OSSIM_UINT32:
      {
         return validate(ossim_uint32(0));
      }  
      case OSSIM_SINT32:
      {
         return validate(ossim_sint32(0));
      }  
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         return validate(ossim_float32(0.0));
      }
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         return validate(ossim_float64(0.0));
      }  
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
         break;
      }
   }

   return OSSIM_STATUS_UNKNOWN;
}

template <class T>
ossimDataObjectStatus ossimImageData::validate(T /* dummyTemplate */ ) const
{
   if (m_dataBuffer.size() == 0)
   {
      setDataObjectStatus(OSSIM_NULL);
      return OSSIM_NULL;
   }

   ossim_uint32       count           = 0;
   const ossim_uint32 SIZE            = getSize();
   const ossim_uint32 BOUNDS          = getSizePerBand();
   const ossim_uint32 NUMBER_OF_BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < NUMBER_OF_BANDS; ++band)
   {
      const T NP = static_cast<T>(m_nullPixelValue[band]);
      const T* p = static_cast<const T*>(getBuf(band));
      
      for (ossim_uint32 i = 0; i < BOUNDS; ++i)
      {
         // check if the band is null
         if (p[i] != NP) ++count;         
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

void ossimImageData::makeBlank()
{
   if ( (m_dataBuffer.size() == 0) || (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return; // nothing to do...
   }

   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         makeBlank(ossim_uint8(0));
         return;
      }  
      case OSSIM_SINT8:
      {
         makeBlank(ossim_sint8(0));
         return;
      }  
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         makeBlank(ossim_uint16(0));
         return;
      }  
      case OSSIM_SINT16:
      {
         makeBlank(ossim_sint16(0));
         return;
      }  
      case OSSIM_UINT32:
      {
         makeBlank(ossim_uint32(0));
         return;
      }
      case OSSIM_SINT32:
      {
         makeBlank(ossim_sint32(0));
         return;
      }  
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         makeBlank(ossim_float32(0.0));
         return;
      }  
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         makeBlank(ossim_float64(0.0));
         return;
      }  
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::makeBlank File %s line %d\n\
Invalid scalar type:  %d",
                       __FILE__,
                       __LINE__,
                       getScalarType());
         break;
      }
   }
}

template <class T> void ossimImageData::makeBlank(T /* dummyTemplate */ )
{
   // Note: Empty buffer or already OSSIM_EMPTY checked in public method.
   
   const ossim_uint32 BANDS = getNumberOfBands();
   const ossim_uint32 SPB   = getSizePerBand();
   ossim_uint32 sizePerBandInBytes = getSizePerBandInBytes();
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const T NP = static_cast<T>(m_nullPixelValue[band]);
      if(NP == 0.0)
      {
         // do an optimize byte fill
         memset(getBuf(band), '\0', sizePerBandInBytes);
      }
      else
      {
         // slower assign call
         T* p = static_cast<T*>(getBuf(band));
         for (ossim_uint32 i = 0; i < SPB; ++i)
         {
            p[i] = NP;
         }
      }
   }
   
   setDataObjectStatus(OSSIM_EMPTY);
}

void ossimImageData::initialize()
{
   // let the base class allocate a buffer
   ossimRectilinearDataObject::initialize();
   
   if (m_dataBuffer.size() > 0)
   {
      makeBlank();  // Make blank will set the status.
   }
}

// Write the tile out to disk with a general raster header file.
bool ossimImageData::write(const ossimFilename& f) const
{
   bool result = false;

   std::ofstream os;
   os.open(f.c_str(), ios::out | ios::binary);
   if (os.good())
   {
      // Write the tile out.
      os.write(static_cast<const char*>(getBuf()),
               static_cast<std::streamsize>(getSizeInBytes()));
      
      result = os.good();
      if (result)
      {
         // Write a header file that we can use to read the tile.
         os.close();
         ossimFilename hdrFile = f;
         hdrFile.setExtension("hdr");
         os.open(hdrFile.c_str(), ios::out);
         result = os.good();
         if (result)
         {
            os << "filename: " << f.c_str()
               << "\nimage_type:  general_raster_bsq"
               << "\nindexed: " << m_indexedFlag               
               << "\ninterleave_type:  bsq"
               << "\norigin: " << m_origin
               << "\nnumber_bands: " << ossimString::toString(getNumberOfBands())
               << "\nnumber_lines: " << ossimString::toString(getHeight())
               << "\nnumber_samples: " << ossimString::toString(getWidth())
               << "\nscalar_type: "
               << ossimScalarTypeLut::instance()->getEntryString(getScalarType())
               << "\n";
            for(ossim_uint32 band=0; band < getNumberOfBands(); ++band)
            {
               ossimString bs = "band";
               bs += ossimString::toString(band+1); 
               os << bs.c_str() << ".min_value: " << m_minPixelValue[band] << "\n"
                  << bs.c_str() << ".max_value: " << m_maxPixelValue[band] << "\n"
                  << bs.c_str() << ".null_value: " << m_nullPixelValue[band]
                  << std::endl;
            }
         }
      }
   }
   os.close();
   
   return result;
}

bool ossimImageData::isWithin(ossim_int32 x, ossim_int32 y)
{
   return ((x >= m_origin.x) &&
           (x <  m_origin.x + static_cast<ossim_int32>(m_spatialExtents[0])) &&
           (y >= m_origin.y) &&
           (y <  m_origin.y + static_cast<ossim_int32>(m_spatialExtents[1])));
           
}

void ossimImageData::setValue(ossim_int32 x, ossim_int32 y, ossim_float64 color)
{
   if(m_dataBuffer.size() > 0 && isWithin(x, y))
   {
      ossim_uint32 band=0;

      //***
      // Compute the offset into the buffer for (x,y).  This should always
      // come out positive.
      //***
      ossim_uint32 ux = static_cast<ossim_uint32>(x - m_origin.x);
      ossim_uint32 uy = static_cast<ossim_uint32>(y - m_origin.y);
      
      ossim_uint32 offset = uy * m_spatialExtents[0] + ux;
      
      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               unsigned char* buf = static_cast<unsigned char*>(getBuf(band))+
                  offset;
               *buf = (unsigned char)color;
            }
            break;
         }
         case OSSIM_SINT8:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_sint8* buf = static_cast<ossim_sint8*>(getBuf(band))+
                  offset;
               *buf = (ossim_sint8)color;
            }
            break;
         }
         case OSSIM_UINT16:
         case OSSIM_USHORT11:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_uint16* buf = static_cast<ossim_uint16*>(getBuf(band))+
                  offset;
               *buf = (ossim_uint16)color;
            }
            break;
         }
         case OSSIM_SINT16:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               signed short* buf = static_cast<signed short*>(getBuf(band))+
                  offset;
               *buf = (signed short)color;
            }
            break;
         }
         case OSSIM_UINT32:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_uint32* buf = static_cast<ossim_uint32*>(getBuf(band))+
                  offset;
               *buf = (ossim_uint32)color;
            }
            break;
         }
         case OSSIM_SINT32:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_sint32* buf = static_cast<ossim_sint32*>(getBuf(band))+
                  offset;
               *buf = (ossim_sint32)color;
            }
            break;
         }
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_FLOAT32:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_float32* buf = static_cast<ossim_float32*>(getBuf(band))+offset;
               *buf = (ossim_float32)color;
            }
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            for(band = 0; band < m_numberOfDataComponents; band++)
            {
               ossim_float64* buf = static_cast<ossim_float64*>(getBuf(band))+offset;
               *buf = color;
            }
            break;
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            //ERROR
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimImageData::setValue Unsupported scalar type!"
               << std::endl;
            
         }
         
      } // End of:  switch (getScalarType())
   }
}

void ossimImageData::initializeDefaults()
{
   initializeMinDefault();
   initializeMaxDefault();
   initializeNullDefault();
}

void ossimImageData::initializeMinDefault()
{
   if(!m_numberOfDataComponents)
   {
      return;
   }

   m_minPixelValue.resize(m_numberOfDataComponents);

   ossim_float64 value = ossim::defaultMin( getScalarType() );
   
   for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
   {
      m_minPixelValue[band]  = value;
   }
}

void ossimImageData::initializeMaxDefault()
{
   if(!m_numberOfDataComponents)
   {
      return;
   }

   m_maxPixelValue.resize(m_numberOfDataComponents);

   ossim_float64 value = ossim::defaultMax( getScalarType() );
      
   for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
   {
      m_maxPixelValue[band]  = value;
   }
}

void ossimImageData::initializeNullDefault()
{
   if(!m_numberOfDataComponents)
   {
      return;
   }

   m_nullPixelValue.resize(m_numberOfDataComponents);

   ossim_float64 value = ossim::defaultNull( getScalarType() );
   
   for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
   {
      m_nullPixelValue[band]  = value;
   }
}

bool ossimImageData::isEqualTo(const ossimDataObject& rhs,
                               bool deepTest)const
{
   ossimImageData* rhsPtr = PTR_CAST(ossimImageData, &rhs);
   if(!(&rhs)||(!rhsPtr)) return false;
   bool result = ( (m_scalarType         == rhsPtr->m_scalarType)&&
                   (m_numberOfDataComponents == rhsPtr->m_numberOfDataComponents)&&
                   (m_origin             == rhsPtr->m_origin)&&
                   (getWidth()            == rhsPtr->getWidth())&&
                   (getHeight()           == rhsPtr->getHeight()));

   if(result)
   {
      bool test=true;
      for(ossim_uint32 index = 0; index < m_numberOfDataComponents; ++index)
      {
         if(m_minPixelValue[index] != rhsPtr->m_minPixelValue[index])
         {
            test = false;
            break;
         }
         if(m_maxPixelValue[index] != rhsPtr->m_maxPixelValue[index])
         {
            test = false;
            break;
         }
         if(m_nullPixelValue[index] != rhsPtr->m_nullPixelValue[index])
         {
            test = false;
            break;
         }
      }
      result = test;
   }
   if(deepTest&&result)
   {
      if(getBuf() != 0 && rhsPtr->getBuf() != 0)
      {
         if(memcmp(getBuf(), rhsPtr->getBuf(), getSizeInBytes()) != 0)
         {
            result = false;
         }
      }
      else if(getBuf() == 0 && rhsPtr->getBuf() == 0)
      {
         // nothing both are null so don't change the result.
      }
      else // one is null so not equal.
      {
         result = false;
      }
   }

   return result;
}

ossimString ossimImageData::getScalarTypeAsString() const
{
   return ossimScalarTypeLut::instance()->getEntryString(getScalarType());
}

ossim_uint32 ossimImageData::getNumberOfBands() const
{
   return getNumberOfDataComponents();
}

ossim_float64 ossimImageData::getPix(const ossimIpt& position,
                              ossim_uint32 band) const
{
   ossimIpt relative( position.x - m_origin.x,
                      position.y - m_origin.y);
   return getPix((m_spatialExtents[0])*relative.y + relative.x, band);
}

ossim_float64 ossimImageData::getPix(ossim_uint32 offset, ossim_uint32 band) const
{
   switch(m_scalarType)
   {
      case OSSIM_UINT8:
      {
         const ossim_uint8* buf = getUcharBuf(band);
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
      }
      case OSSIM_SINT8:
      {
         const ossim_sint8* buf = static_cast<const ossim_sint8*>(getBuf(band));
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         const ossim_uint16* buf = getUshortBuf(band);
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
         break;
      }
      case OSSIM_SINT16:
      {
         const ossim_sint16* buf = getSshortBuf(band);
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
         break;
      }
      case OSSIM_SINT32:
      {
         const ossim_sint32* buf = static_cast<const ossim_sint32*>(getBuf(band));
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
         break;
      }
      case OSSIM_UINT32:
      {
         const ossim_uint32* buf = static_cast<const ossim_uint32*>(getBuf(band));
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         const ossim_float64* buf = getDoubleBuf(band);
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }      
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         const ossim_float32* buf = getFloatBuf(band);
         if(buf)
         {
            return (ossim_float64)buf[offset];
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::fill Unsupported scalar type!"
            << std::endl;   
   }

   return 0.0;
}

void ossimImageData::fill(ossim_uint32 band, ossim_float64 value)
{
   void* s         = getBuf(band);

   if (s == 0) return; // nothing to do...

   ossim_uint32 size_in_pixels = getWidth()*getHeight();
   
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         ossim_uint8* p = getUcharBuf(band);
         ossim_uint8 np = static_cast<ossim_uint8>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_SINT8:
      {
         ossim_sint8* p = static_cast<ossim_sint8*>(getBuf(band));
         ossim_sint8 np = static_cast<ossim_sint8>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         ossim_uint16* p = getUshortBuf(band);
         ossim_uint16 np = static_cast<ossim_uint16>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_SINT16:
      {
         ossim_sint16* p = getSshortBuf(band);
         ossim_sint16 np = static_cast<ossim_sint16>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_UINT32:
      {
         ossim_uint32* p = static_cast<ossim_uint32*>(getBuf(band));
         ossim_uint32 np = static_cast<ossim_uint32>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_SINT32:
      {
         ossim_sint32* p = static_cast<ossim_sint32*>(getBuf(band));
         ossim_sint32 np = static_cast<ossim_sint32>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         ossim_float32* p = getFloatBuf(band);
         ossim_float32 np = static_cast<ossim_float32>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         ossim_float64* p = getDoubleBuf(band);
         ossim_float64 np = static_cast<ossim_float64>(value);
         for (ossim_uint32 i=0; i<size_in_pixels; i++) p[i] = np;
         
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::makeBlank File %s line %d\n\
Invalid scalar type:  %d",
                       __FILE__,
                       __LINE__,
                       getScalarType());
         return;
   }
   
   setDataObjectStatus(OSSIM_EMPTY);
   
}

void ossimImageData::fill(ossim_float64 value)
{
   ossim_uint32 valueNullCount= 0;
   for(ossim_uint32 band=0; band < getNumberOfBands(); ++band)
   {
      if (value == m_nullPixelValue[band])
      {
         ++valueNullCount;
      }
      
      fill(band, value);
   }

   if (valueNullCount==0)
   {
      setDataObjectStatus(OSSIM_FULL);
   }
   else if(valueNullCount==getNumberOfBands())
   {
      setDataObjectStatus(OSSIM_EMPTY);
   }
   else
   {
      setDataObjectStatus(OSSIM_PARTIAL);
   }
}

bool ossimImageData::isNull(ossim_uint32 offset)const
{
   ossim_uint32 numberOfBands = getNumberOfBands();
   ossim_uint32 band=0;
   if(!getBuf())
   {
      return true;
   }

   switch(getScalarType())
   {
      case OSSIM_UINT8:
      {
         for(band = 0; band < numberOfBands; ++band)  
         {
            const ossim_uint8* buf = static_cast<const ossim_uint8*>(getBuf(band))+offset;
            if((*buf) != (ossim_uint8)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_SINT8:
      {
         for(band = 0; band < numberOfBands; ++band)  
         {
            const ossim_sint8* buf = static_cast<const ossim_sint8*>(getBuf(band))+offset;
            if((*buf) != (ossim_uint8)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_uint16* buf = static_cast<const ossim_uint16*>(getBuf(band))+offset;
            if((*buf) != (ossim_uint16)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_SINT16:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_sint16* buf = static_cast<const ossim_sint16*>(getBuf(band))+offset;
            if((*buf) != (ossim_sint16)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_UINT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_uint32* buf = static_cast<const ossim_uint32*>(getBuf(band))+offset;
            if((*buf) != (ossim_uint32)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_SINT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_sint32* buf = static_cast<const ossim_sint32*>(getBuf(band))+offset;
            if((*buf) != (ossim_sint32)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_float32* buf = static_cast<const ossim_float32*>(getBuf(band))+offset;
            if((*buf) != 0.0)
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_FLOAT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_float32* buf = static_cast<const ossim_float32*>(getBuf(band))+offset;
            if((*buf) != (ossim_float32)getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_float64* buf = static_cast<const ossim_float64*>(getBuf(band))+offset;
            if((*buf) != 0.0)
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_FLOAT64:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            const ossim_float64* buf = static_cast<const ossim_float64*>(getBuf(band))+offset;
            if((*buf) != getNullPix(band))
            {
               return false;
            }
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         //ERROR
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::isNull Unsupported scalar type!"
            << std::endl;
      }
   }
   return true;
}

bool ossimImageData::isNull(ossim_uint32 offset, ossim_uint32 band)const
{
   switch(getScalarType())
   {
      case OSSIM_UINT8:
      {
         const ossim_uint8* buf =
            static_cast<const ossim_uint8*>(getBuf(band))+offset;
         
         if((*buf) != (ossim_uint8)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_SINT8:
      {
         const ossim_sint8* buf =
            static_cast<const ossim_sint8*>(getBuf(band))+offset;
         
         if((*buf) != (ossim_sint8)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         const ossim_uint16* buf =
            static_cast<const ossim_uint16*>(getBuf(band))+offset;
         if((*buf) != (ossim_uint16)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_SINT16:
      {
         const ossim_sint16* buf =
            static_cast<const ossim_sint16*>(getBuf(band))+offset;
         if((*buf) != (ossim_sint16)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_UINT32:
      {
         const ossim_uint32* buf =
            static_cast<const ossim_uint32*>(getBuf(band))+offset;
         if((*buf) != (ossim_uint32)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_SINT32:
      {
         const ossim_sint32* buf =
            static_cast<const ossim_sint32*>(getBuf(band))+offset;
         if((*buf) != (ossim_sint32)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         const ossim_float32* buf = static_cast<const ossim_float32*>(getBuf(band))+offset;
         if((*buf) != (ossim_float32)getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         const ossim_float64* buf = static_cast<const ossim_float64*>(getBuf(band))+offset;
         if((*buf) != getNullPix(band))
         {
            return false;
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         //ERROR
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::isNull Unsupported scalar type!"
            << std::endl;
      }
   }
   return true;
}

bool ossimImageData::isNull(const ossimIpt& pt)const
{
   ossim_int32 xNew = (pt.x - m_origin.x);
   ossim_int32 yNew = (pt.y - m_origin.y);
   if(xNew < 0 || xNew >= static_cast<ossim_int32>(m_spatialExtents[0]) ||
      yNew < 0 || yNew >= static_cast<ossim_int32>(m_spatialExtents[1]) )
   {
      return true;
   }
   ossim_uint32 offset = getWidth()*yNew + xNew;
   
   return isNull(offset);
}

bool ossimImageData::isNull(const ossimIpt& pt, ossim_uint32 band)const
{
   ossim_int32 xNew = (pt.x - m_origin.x);
   ossim_int32 yNew = (pt.y - m_origin.y);
   if(xNew < 0 || xNew >= static_cast<ossim_int32>(m_spatialExtents[0]) ||
      yNew < 0 || yNew >= static_cast<ossim_int32>(m_spatialExtents[1]) )
   {
      return true;
   }
   ossim_uint32 offset = getWidth()*yNew + xNew;

   return isNull(offset, band);
}

void ossimImageData::setNull(ossim_uint32 offset)
{
   ossim_uint32 numberOfBands = getNumberOfBands();
   ossim_uint32 band=0;
   switch(getScalarType())
   {
      case OSSIM_UINT8:
      {
         for(band = 0; band < numberOfBands; ++band)  
         {
            ossim_uint8* buf = static_cast<ossim_uint8*>(getBuf(band))+offset;
            *buf       = (ossim_uint8)getNullPix(band);
         }
         break;
      }
      case OSSIM_SINT8:
      {
         for(band = 0; band < numberOfBands; ++band)  
         {
            ossim_sint8* buf = static_cast<ossim_sint8*>(getBuf(band))+offset;
            *buf       = (ossim_sint8)getNullPix(band);
         }
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_uint16* buf = static_cast<ossim_uint16*>(getBuf(band))+
               offset;
            *buf = (ossim_uint16)getNullPix(band);
         }
         break;
      }
      case OSSIM_SINT16:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_sint16* buf = static_cast<ossim_sint16*>(getBuf(band))+
               offset;
            *buf = (ossim_sint16)getNullPix(band);
         }
         break;
      }
      case OSSIM_UINT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_uint32* buf = static_cast<ossim_uint32*>(getBuf(band))+
               offset;
            *buf = (ossim_uint32)getNullPix(band);
         }
         break;
      }
      case OSSIM_SINT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_sint32* buf = static_cast<ossim_sint32*>(getBuf(band))+
               offset;
            *buf = (ossim_sint32)getNullPix(band);
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_float32* buf = static_cast<ossim_float32*>(getBuf(band))+offset;
            *buf = (ossim_float32)getNullPix(band);
         }
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         for(band = 0; band < numberOfBands; band++)
         {
            ossim_float64* buf = static_cast<ossim_float64*>(getBuf(band))+offset;
            *buf = getNullPix(band);
         }
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         //ERROR
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::isNull Unsupported scalar type!"
            << std::endl;
      }
   }
}

void ossimImageData::setNull(ossim_uint32 offset, ossim_uint32 band)
{
   switch(getScalarType())
   {
      case OSSIM_UINT8:
      {
         ossim_uint8* buf = static_cast<ossim_uint8*>(getBuf(band))+offset;
         *buf       = (ossim_uint8)getNullPix(band);
         break;
      }
      case OSSIM_SINT8:
      {
         ossim_sint8* buf = static_cast<ossim_sint8*>(getBuf(band))+offset;
         *buf       = (ossim_sint8)getNullPix(band);
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         ossim_uint16* buf = static_cast<ossim_uint16*>(getBuf(band))+offset;
         *buf = (ossim_uint16)getNullPix(band);
         break;
      }
      case OSSIM_SINT16:
      {
         ossim_sint16* buf = static_cast<ossim_sint16*>(getBuf(band))+offset;
         *buf = (ossim_sint16)getNullPix(band);
         break;
      }
      case OSSIM_UINT32:
      {
         ossim_uint32* buf = static_cast<ossim_uint32*>(getBuf(band))+offset;
         *buf       = (ossim_uint32)getNullPix(band);
         break;
      }
      case OSSIM_SINT32:
      {
         ossim_sint32* buf = static_cast<ossim_sint32*>(getBuf(band))+offset;
         *buf       = (ossim_sint32)getNullPix(band);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         ossim_float32* buf = static_cast<ossim_float32*>(getBuf(band))+offset;
         *buf = (ossim_float32)getNullPix(band);
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_float64* buf = static_cast<ossim_float64*>(getBuf(band))+offset;
         *buf = getNullPix(band);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         //ERROR
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::isNull Unsupported scalar type!"
            << std::endl;
      }
   }
}

void ossimImageData::setNull(const ossimIpt& pt)
{
   ossim_int32 xNew = (pt.x - m_origin.x);
   ossim_int32 yNew = (pt.y - m_origin.y);
   
   if(xNew < 0 || xNew >= (int)m_spatialExtents[0] ||
      yNew < 0 || yNew >= (int)m_spatialExtents[1])
   {
      return;
   }
   ossim_uint32 offset = ((int)getWidth())*yNew + xNew;
   
   setNull(offset);
}

void ossimImageData::setNull(const ossimIpt& pt, ossim_uint32 band)
{
   ossim_int32 xNew = (pt.x - m_origin.x);
   ossim_int32 yNew = (pt.y - m_origin.y);
   
   if(xNew < 0 || xNew >= (int)m_spatialExtents[0] ||
      yNew < 0 || yNew >= (int)m_spatialExtents[1])
   {
      return;
   }
   ossim_uint32 offset = ((int)getWidth())*yNew + xNew;
   
   setNull(offset, band);
}

void ossimImageData::setNullPix(ossim_float64 null_pix)
{
   if(!m_numberOfDataComponents)
   {
      return;
   }
   m_nullPixelValue.resize(m_numberOfDataComponents);
   for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
   {
      m_nullPixelValue[band] = null_pix;
   }
}

void ossimImageData::setNullPix(ossim_float64 null_pix, ossim_uint32 band)
{
   if( !m_numberOfDataComponents || (band >= m_numberOfDataComponents) )
   {
      return;
   }
   if (m_nullPixelValue.size() != m_numberOfDataComponents)
   {
      initializeNullDefault();
   }
   m_nullPixelValue[band] = null_pix;
}

void ossimImageData::setNullPix(const ossim_float64* nullPixArray,
                                ossim_uint32 numberOfValues)
{
   if(!nullPixArray || !m_numberOfDataComponents)
   {
      return;
   }

   if (m_nullPixelValue.size() != m_numberOfDataComponents)
   {
      initializeNullDefault();
   }
   
   for(ossim_uint32 band = 0;
       (band < numberOfValues) && (band < m_numberOfDataComponents);
       ++band)
   {
      m_nullPixelValue[band] = nullPixArray[band];
   }
}

void ossimImageData::setMinPix(ossim_float64 min_pix)
{
   if(!m_numberOfDataComponents)
   {
      return;
   }
   m_minPixelValue.resize(m_numberOfDataComponents);
   for(ossim_uint32 band = 0; band < m_minPixelValue.size(); ++band)
   {
      m_minPixelValue[band] = min_pix;
   }
}

void ossimImageData::setMinPix(ossim_float64 min_pix, ossim_uint32 band)
{
   if( !m_numberOfDataComponents || (band >= m_numberOfDataComponents) )
   {
      return;
   }
   if (m_minPixelValue.size() != m_numberOfDataComponents)
   {
      initializeMinDefault();
   }
   m_minPixelValue[band] = min_pix;
}

void ossimImageData::setMinPix(const ossim_float64* minPixArray,
                               ossim_uint32 numberOfValues)
{
   if( !minPixArray || !m_numberOfDataComponents )
   {
      return;
   }

   if (m_minPixelValue.size() != m_numberOfDataComponents)
   {
      initializeMinDefault();
   }
   
   for(ossim_uint32 band = 0;
       (band < numberOfValues) && (band < m_numberOfDataComponents);
       ++band)
   {
      m_minPixelValue[band] = minPixArray[band];
   }
}

void ossimImageData::setMaxPix(ossim_float64 max_pix)
{
   if(!m_numberOfDataComponents)
   {
      return;
   }
   m_maxPixelValue.resize(m_numberOfDataComponents);
   for(ossim_uint32 band = 0; band < m_numberOfDataComponents; ++band)
   {
      m_maxPixelValue[band] = max_pix;
   }
}

void ossimImageData::setMaxPix(ossim_float64 max_pix, ossim_uint32 band)
{
   if( !m_numberOfDataComponents || (band >= m_numberOfDataComponents) )
   {
      return;
   }
   if (m_maxPixelValue.size() != m_numberOfDataComponents)
   {
      initializeMaxDefault();
   }
   m_maxPixelValue[band] = max_pix;
}

void ossimImageData::setMaxPix(const ossim_float64* maxPixArray,
                               ossim_uint32 /* numberOfValues */)
{
   if(!maxPixArray || !m_numberOfDataComponents )
   {
      return;
   }

   if (m_maxPixelValue.size() != m_numberOfDataComponents)
   {
      initializeMaxDefault();
   }
   
   for(ossim_uint32 band = 0;
       (band < m_numberOfDataComponents) &&
          (band < m_numberOfDataComponents);
       ++band)
   {
      m_maxPixelValue[band] = maxPixArray[band];
   }
}

void ossimImageData::setNumberOfBands(ossim_uint32 bands,
                                      bool reallocate)
{
   ossim_uint32 b  = getNumberOfBands();
   if(bands && (b != bands))
   {
      setNumberOfDataComponents(bands);
      if(reallocate)
      {
         ossimRectilinearDataObject::initialize();
      }
      
      ossim_uint32 minBands = ossim::min(b, bands);


      vector<ossim_float64> newNull(bands);
      vector<ossim_float64> newMin(bands);
      vector<ossim_float64> newMax(bands);

      ossim_uint32 i = 0;
      while (i < minBands)
      {
         newNull[i] = m_nullPixelValue[i];
         newMin[i]  = m_minPixelValue[i];
         newMax[i]  = m_maxPixelValue[i];
         ++i;
      }

      if(b)
      {
        while (i < bands)
        {
           newNull[i] = m_nullPixelValue[b-1];
           newMin[i]  = m_minPixelValue[b-1];
           newMax[i]  = m_maxPixelValue[b-1];
           ++i;
        }
      }
      
      m_nullPixelValue = newNull;
      m_minPixelValue  = newMin;
      m_maxPixelValue  = newMax;
   }
}

void ossimImageData::setImageRectangleAndBands(const ossimIrect& rect,
                                               ossim_uint32 numberOfBands)
{
   if(rect.hasNans())
   {
      return;
   }
   
   ossim_uint32 bands   = getNumberOfBands();
   ossim_uint32 w       = getWidth();
   ossim_uint32 h       = getHeight();
   ossim_uint32 nw      = rect.width();
   ossim_uint32 nh      = rect.height();

   setOrigin(rect.ul());
   setWidthHeight(nw, nh);
   setNumberOfBands(numberOfBands, false);
   
   // we will try to be non destructive on the resize of the number of bands
   if( ( (w*h != nw*nh)  || ( bands != numberOfBands) ) &&
       (theDataObjectStatus != OSSIM_NULL) )
   {
      initialize();
   }
}

void ossimImageData::setImageRectangle(const ossimIrect& rect)
{
   if(rect.hasNans())
   {
      return;
   }
   
   ossim_uint32 w  = getWidth();
   ossim_uint32 h  = getHeight();
   ossim_uint32 nw = rect.width();
   ossim_uint32 nh = rect.height();

   setOrigin(rect.ul());
   setWidthHeight(nw, nh);

   // we will try to be non destructive on the resize of the number of bands
   if ( (w*h != nw*nh) && (theDataObjectStatus != OSSIM_NULL) )
   {
      initialize();
   }
}

void ossimImageData::assign(const ossimDataObject* data)
{
   if(!data) return;
   ossimImageData* d = PTR_CAST(ossimImageData, data);
   if(d)
   {
      assign(d);
   }
}

void ossimImageData::assign(const ossimImageData* data)
{
   ossimSource* tmp_owner = getOwner();
   
   ossimRectilinearDataObject::assign(data);

   //***
   // The data member "theSource" will be overwritten so capture it and then
   // set it back.
   //***
   setOwner(tmp_owner);

   if(this != data)
   {
      ossim_uint32 numberOfBands = getNumberOfBands();
      if(!numberOfBands)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::assign\n"
            << "Number of components is 0, can't assign" << std::endl;
         return;
      }
      m_origin = data->m_origin;

      m_minPixelValue  = data->m_minPixelValue;
      m_maxPixelValue  = data->m_maxPixelValue;
      m_nullPixelValue = data->m_nullPixelValue;

      if (m_minPixelValue.size() == 0 ||
          m_maxPixelValue.size() == 0 ||
          m_nullPixelValue.size() == 0)
      {
         initializeDefaults();
      }
   }
}

void ossimImageData::assignBand(const ossimImageData* data,
                                ossim_uint32 source_band,
                                ossim_uint32 output_band)
{
   //***
   // This method requires this image data to be initialized to the same
   // size as the source data being copied.
   //***

   // Some basic error checking...
   if ( !data->isValidBand(source_band) )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::assignBand ERROR:"
         << "\nInvalid source band!" << std::endl;
      return;
   }
   if ( !isValidBand(output_band) )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::assignBand ERROR:"
         << "\nInvalid output band!" << std::endl;
      return;
   }
   if (data->getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::assignBand ERROR:"
         << "\nSource data status is null!" << std::endl;
      return;
   }
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::assignBand ERROR:"
         << "\nThis status is null!" << std::endl;
      return;
   }
   
   //***
   // Make sure this buffer is the same size; if not, return.
   //***
   ossim_uint32 source_size = data->getSizePerBandInBytes();
      
   if ( source_size != getSizePerBandInBytes() )
   {
      return;
   }

   // Get the pointers to the bands.
   const void*  s = data->getBuf(source_band);
   void*        d = getBuf(output_band);

   // One last check.
   if (s == 0 || d == 0)
   {
      return;
   }

   // Ok copy the image data...
   memcpy(d, s, source_size);
}

void ossimImageData::loadBand(const void* src,
                              const ossimIrect& src_rect,
                              ossim_uint32 band)
{
   // Call the appropriate load method.
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadBandTemplate(ossim_uint8(0), src, src_rect, band);
         return;

      case OSSIM_SINT8:
         loadBandTemplate(ossim_sint8(0), src, src_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadBandTemplate(ossim_uint16(0), src, src_rect, band);
         return;
         
      case OSSIM_SINT16:
         loadBandTemplate(ossim_sint16(0), src, src_rect, band);
         return;
         
      case OSSIM_UINT32:
         loadBandTemplate(ossim_uint32(0), src, src_rect, band);
         return;

      case OSSIM_SINT32:
         loadBandTemplate(ossim_sint32(0), src, src_rect, band);
         return;
         
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
         loadBandTemplate(ossim_float32(0), src, src_rect, band);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadBandTemplate(ossim_float64(0), src, src_rect, band);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadBand\n\
File %s line %d\nUnknown scalar type!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadBand(const void* src,
                              const ossimIrect& src_rect,
                              const ossimIrect& clip_rect,
                              ossim_uint32 band)
{
   // Call the appropriate load method.
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadBandTemplate(ossim_uint8(0), src, src_rect, clip_rect, band);
         return;

      case OSSIM_SINT8:
         loadBandTemplate(ossim_sint8(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadBandTemplate(ossim_uint16(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_SINT16:
         loadBandTemplate(ossim_sint16(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT32:
         loadBandTemplate(ossim_uint32(0), src, src_rect, clip_rect, band);
         return;

      case OSSIM_SINT32:
         loadBandTemplate(ossim_sint32(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
         loadBandTemplate(ossim_float32(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadBandTemplate(ossim_float64(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadBand\n\
File %s line %d\nUnknown scalar type!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

template <class T> void ossimImageData::loadBandTemplate(T, // dummy template variable
                                                         const void* src,
                                                         const ossimIrect& src_rect,
                                                         ossim_uint32 band)
{
   const ossimIrect img_rect = getImageRectangle();

   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   
   loadBandTemplate(T(0), src, src_rect, clip_rect, band);
}

template <class T> void ossimImageData::loadBandTemplate(T, // dummy template variable
                                                         const void* src,
                                                         const ossimIrect& src_rect,
                                                         const ossimIrect& clip_rect,
                                                         ossim_uint32 band)
{
   static const char  MODULE[] = "ossimImageData::loadBand";
   
   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   
   // Check the band.
   if (!isValidBand(band))
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nInvalid band:  %d",
                    MODULE,
                    __FILE__,
                    __LINE__,
                    band);
      return;
   }
   
   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }

   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
      return;
   }
   
   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();

   // Get the width of the buffers.
   ossim_uint32 s_width = src_rect.width();
   ossim_uint32 d_width = getWidth();
   
   const T* s = static_cast<const T*>(src);
   T* d = static_cast<T*>(getBuf(band));

   // Move the pointers to the first valid pixel.
   s += (clip_rect.ul().y - src_rect.ul().y) * s_width +
        clip_rect.ul().x - src_rect.ul().x;
   
   d += (clip_rect.ul().y - img_rect.ul().y) * d_width +
        clip_rect.ul().x - img_rect.ul().x;
   
   // Copy the data.
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth  = clip_rect.width();
  
   for (ossim_uint32 line = 0; line < clipHeight; ++line)
   {
      for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
      {
         d[sample] = s[sample];
      }

      s += s_width;
      d += d_width;
   }
}

void ossimImageData::loadTile(const ossimImageData* src)
{
   if (!src)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:"
         << "Null tile passed to method!" << std::endl;
      return;
   }

   if (!src->getBuf())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:"
         << "Source tile buff is null!" << std::endl;
      return;
   }

   if (!this->getBuf())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:"
         << "This tile not initialized!" << std::endl;
      return;
   }

   if (src->getNumberOfBands() != this->getNumberOfBands())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:"
         << "Tiles do not have same number of bands!" << std::endl;
      return;
   }

   //***
   // Set the status of this tile to the status of the source tile.
   // Do this in place of validate.
   //***
   setDataObjectStatus(src->getDataObjectStatus());
   
   if(getScalarType() == src->getScalarType())
   {      
      loadTile((void*)(src->getBuf()),
               src->getImageRectangle(),
               OSSIM_BSQ);
      setNullPix(src->getNullPix(), src->getNumberOfBands());
   }
   else // do a slow generic normalize to unnormalize copy
   {
      // Check the pointer.
      ossimIrect src_rect = src->getImageRectangle();
      const ossimIrect img_rect = getImageRectangle();
      
      // Check for intersect.
      if ( !img_rect.intersects(src_rect) )
      {
         return; // Nothing to do here.
      }

      // Get the clip rect.
      const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

      // Check the status and allocate memory if needed.
      if (getDataObjectStatus() == OSSIM_NULL) initialize();
      
      // Get the width of the buffers.
      ossim_uint32 num_bands     = getNumberOfBands();
      ossim_uint32 s_width       = src_rect.width();
      ossim_uint32 d_width       = getWidth();
            
      ossim_uint32 band;
      
      ossim_uint32 sourceOffset = (clip_rect.ul().y - src_rect.ul().y) *
         s_width + (clip_rect.ul().x - src_rect.ul().x);      
      
      ossim_uint32 destinationOffset = (clip_rect.ul().y - img_rect.ul().y) *
         d_width + (clip_rect.ul().x - img_rect.ul().x);
      
      ossim_uint32 clipHeight = clip_rect.height();
      ossim_uint32 clipWidth  = clip_rect.width();

      // Copy the data.
      for (band=0; band<num_bands; ++band)
      {
         ossim_uint32 sourceIndex      = sourceOffset;
         ossim_uint32 destinationIndex = destinationOffset;
         ossim_float32 tempResult      = 0.0;

         for (ossim_uint32 line = 0; line < clipHeight; ++line)
         {
            for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
            {
               src->getNormalizedFloat(sourceIndex + sample,
                                       band,
                                       tempResult);
               
               this->setNormalizedFloat(destinationIndex + sample,
                                        band,
                                        tempResult);
            }
            sourceIndex      += s_width;
            destinationIndex += d_width;
         }
      }
   }
}

void ossimImageData::loadTile(const void* src,
                              const ossimIrect& src_rect,
                              ossimInterleaveType il_type)
{
   switch (il_type)
   {
   case OSSIM_BIP:
      loadTileFromBip(src, src_rect);
      return;
   case OSSIM_BIL:
      loadTileFromBil(src, src_rect);
      return;
   case OSSIM_BSQ:
      loadTileFromBsq(src, src_rect);
      return;
   default:
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:  unsupported interleave type!"
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimImageData::loadTile\n\
File %s line %d\nUnknown interleave type!",
                    __FILE__,
                    __LINE__);      
      return;
   } // End of "switch (type)"
}

void ossimImageData::loadTile(const void* src,
                              const ossimIrect& src_rect,
                              const ossimIrect& clip_rect,
                              ossimInterleaveType il_type)
{
   switch (il_type)
   {
   case OSSIM_BIP:
      loadTileFromBip(src, src_rect, clip_rect);
      return;
   case OSSIM_BIL:
      loadTileFromBil(src, src_rect, clip_rect);
      return;
   case OSSIM_BSQ:
      loadTileFromBsq(src, src_rect, clip_rect);
      return;
   default:
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::loadTile ERROR:  unsupported interleave type!"
         << std::endl;
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "ossimImageData::loadTile\n\
File %s line %d\nUnknown interleave type!",
                    __FILE__,
                    __LINE__);      
      return;
   } // End of "switch (type)"
}


void ossimImageData::nullTileAlpha(const ossim_uint8* src,
                                   const ossimIrect& src_rect,
                                   bool multiplyAlphaFlag)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   nullTileAlpha(src, src_rect, clip_rect, multiplyAlphaFlag);
}

void ossimImageData::nullTileAlpha(const ossim_uint8* src,
                                   const ossimIrect& src_rect,
                                   const ossimIrect& clip_rect,
                                   bool multiplyAlphaFlag)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         nullTileAlphaTemplate(ossim_uint8(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;

      case OSSIM_SINT8:
         nullTileAlphaTemplate(ossim_sint8(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         nullTileAlphaTemplate(ossim_uint16(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_SINT16:
         nullTileAlphaTemplate(ossim_sint16(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_UINT32:
         nullTileAlphaTemplate(ossim_uint32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;

      case OSSIM_SINT32:
         nullTileAlphaTemplate(ossim_sint32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         nullTileAlphaTemplate(ossim_float32(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         nullTileAlphaTemplate(ossim_float64(0), src, src_rect, clip_rect, multiplyAlphaFlag);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::nullTileAlpha\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
   
}

void ossimImageData::loadTileFromBip(const void* src,
                                     const ossimIrect& src_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBipTemplate(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBipTemplate(ossim_sint8(0), src, src_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBipTemplate(ossim_uint16(0), src, src_rect);
         return;
         
      case OSSIM_SINT16:
         loadTileFromBipTemplate(ossim_sint16(0), src, src_rect);
         return;
         
      case OSSIM_UINT32:
         loadTileFromBipTemplate(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBipTemplate(ossim_sint32(0), src, src_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBipTemplate(ossim_float32(0), src, src_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBipTemplate(ossim_float64(0), src, src_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBip\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadTileFromBip(const void* src,
                                     const ossimIrect& src_rect,
                                     const ossimIrect& clip_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBipTemplate(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBipTemplate(ossim_sint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBipTemplate(ossim_uint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBipTemplate(ossim_sint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBipTemplate(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBipTemplate(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBipTemplate(ossim_float32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBipTemplate(ossim_float64(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBip\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadTileFromBil(const void* src,
                                     const ossimIrect& src_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBilTemplate(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBilTemplate(ossim_sint8(0), src, src_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBilTemplate(ossim_uint16(0), src, src_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBilTemplate(ossim_sint16(0), src, src_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBilTemplate(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBilTemplate(ossim_sint32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBilTemplate(ossim_float32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBilTemplate(ossim_float64(0), src, src_rect);
         return;

      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBil\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadTileFromBil(const void* src,
                                     const ossimIrect& src_rect,
                                     const ossimIrect& clip_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBilTemplate(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBilTemplate(ossim_sint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBilTemplate(ossim_uint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBilTemplate(ossim_sint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBilTemplate(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBilTemplate(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBilTemplate(ossim_float32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBilTemplate(ossim_float64(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBil\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadTileFromBsq(const void* src,
                                     const ossimIrect& src_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBsqTemplate(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBsqTemplate(ossim_sint8(0), src, src_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBsqTemplate(ossim_uint16(0), src, src_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBsqTemplate(ossim_sint16(0), src, src_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBsqTemplate(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBsqTemplate(ossim_sint32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBsqTemplate(ossim_float32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBsqTemplate(ossim_float64(0), src, src_rect);
         return;
      
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBsq\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::loadTileFromBsq(const void* src,
                                     const ossimIrect& src_rect,
                                     const ossimIrect& clip_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBsqTemplate(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBsqTemplate(ossim_sint8(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBsqTemplate(ossim_uint16(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         loadTileFromBsqTemplate(ossim_sint16(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         loadTileFromBsqTemplate(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBsqTemplate(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBsqTemplate(ossim_float32(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBsqTemplate(ossim_float64(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::loadTileFromBsq\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::computeMinMaxPix(vector<ossim_float64>& minBands,
                                      vector<ossim_float64>& maxBands) const
{
   if ( (getDataObjectStatus() == OSSIM_NULL) || 
        (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return;
   }

   switch(m_scalarType)
   {
      case OSSIM_UINT8:
      {
         computeMinMaxPix((ossim_uint8)0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_SINT8:
      {
         computeMinMaxPix((ossim_sint8)0,
                          minBands,
                          maxBands);
         break;
         
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         computeMinMaxPix((ossim_uint16)0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_SINT16:
      {
         computeMinMaxPix((ossim_sint16)0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_UINT32:
      {
         computeMinMaxPix((ossim_uint32)0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_SINT32:
      {
         computeMinMaxPix((ossim_sint32)0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         computeMinMaxPix((ossim_float32)0.0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         computeMinMaxPix((ossim_float64)0.0,
                          minBands,
                          maxBands);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::computeMinPix Unsupported scalar type!"
            << std::endl;
         break;
      }
   }
}

template <class T>
void ossimImageData::computeMinMaxPix(T /* dummyValue */,
                                      vector<ossim_float64>& minBands,
                                      vector<ossim_float64>& maxBands) const
{
   const ossim_uint32 BANDS = getNumberOfBands();
   const ossim_uint32 SPB   = getSizePerBand(); 
   
   if( minBands.size() != BANDS ||
       maxBands.size() != BANDS )
   {
      minBands.resize(BANDS);
      maxBands.resize(BANDS);
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         // Set min to max and max to min for starters.
         minBands[band] = getMaxPix(band);
         maxBands[band] = getMinPix(band);
      }
   }
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const T* bandBuffer = (const T*)getBuf(band);
      if(bandBuffer)
      {
         const T NP   = static_cast<T>(getNullPix(band));
         ossim_float64 currentMin = minBands[band];
         ossim_float64 currentMax = maxBands[band];
         for(ossim_uint32 offset = 0; offset < SPB; ++offset)
         {
            T p = bandBuffer[offset];
            if(p != NP)
            {
               if(p < currentMin)
               {
                  currentMin = p;
               }
               else if(p > currentMax)
               {
                  currentMax = p;
               }
            }
         }
         minBands[band] = currentMin;
         maxBands[band] = currentMax;
      }
   }
}

void ossimImageData::computeMinMaxNulPix(vector<ossim_float64>& minBands,
                                         vector<ossim_float64>& maxBands,
                                         vector<ossim_float64>& nulBands) const
{
   if ( getDataObjectStatus() == OSSIM_NULL )
   {
      return;
   }
   
   switch(m_scalarType)
   {
      case OSSIM_UINT8:
      {
         computeMinMaxNulPix((ossim_uint8)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_SINT8:
      {
         computeMinMaxNulPix((ossim_sint8)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;  
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         computeMinMaxNulPix((ossim_uint16)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_SINT16:
      {
         computeMinMaxNulPix((ossim_sint16)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_UINT32:
      {
         computeMinMaxNulPix((ossim_uint32)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_SINT32:
      {
         computeMinMaxNulPix((ossim_sint32)0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         computeMinMaxNulPix((ossim_float32)0.0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         computeMinMaxNulPix((ossim_float64)0.0,
                             minBands,
                             maxBands,
                             nulBands);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::computeMinNulPix Unsupported scalar type!"
            << std::endl;
         break;
      }
   }
}

template <class T>
void ossimImageData::computeMinMaxNulPix(T /* dummyValue */,
                                         vector<ossim_float64>& minBands,
                                         vector<ossim_float64>& maxBands,
                                         vector<ossim_float64>& nulBands) const
{
   const ossim_uint32 BANDS = getNumberOfBands();
   const ossim_uint32 SPB   = getSizePerBand(); 
   T DEFAULT_NULL = static_cast<T>(ossim::defaultNull(getScalarType()));   

   if( minBands.size() != BANDS ||
       maxBands.size() != BANDS ||
       nulBands.size() != BANDS )
   {
      // First time through.
      minBands.resize(BANDS);
      maxBands.resize(BANDS);
      nulBands.resize(BANDS);
      for (ossim_uint32 band = 0; band < BANDS; ++band)
      {
         // Set min to max and max to min for starters.
         minBands[band] = getMaxPix(band);
         maxBands[band] = getMinPix(band);
         nulBands[band] = getMaxPix(band);
      }
   }
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const T* bandBuffer = (const T*)getBuf(band);
      if(bandBuffer)
      {
         ossim_float64 currentMin = minBands[band];
         ossim_float64 currentMax = maxBands[band];
         ossim_float64 currentNul = nulBands[band];
         for(ossim_uint32 offset = 0; offset < SPB; ++offset)
         {
            T p = bandBuffer[offset];

            //---
            // Since we are scanning for nulls this is making an assumption that the default
            // null is incorrect and should be ignored in this scan as it could have been
            // introduced by a make blank on a partial tile so ignore it.
            //---
            if ( p != DEFAULT_NULL )
            {
               // Must do null first as min depends on null.
               if ( p < currentNul )
               {
                  currentNul = p;
               }
               if( ( p < currentMin ) && ( p > currentNul ) )
               {
                  currentMin = p;
               }
               else if( p > currentMax )
               {
                  currentMax = p;
               }
            }
         }
         minBands[band] = currentMin;
         maxBands[band] = currentMax;
         nulBands[band] = currentNul;
      }
   }
}

template <class T>
void ossimImageData::loadTileFromBipTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBipTemplate(T(0), src, src_rect, clip_rect);
}

template <class T>
void ossimImageData::loadTileFromBipTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect,
                                             const ossimIrect& clip_rect)
{
   static const char  MODULE[] = "ossimImageData::loadTileFromBip";
   
   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                     "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   
   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }
   
   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
      return;
   }
   
   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();

   // Get the width of the buffers.
   ossim_uint32 num_bands  = getNumberOfBands();
   ossim_uint32 s_width    = (src_rect.width()) * num_bands;
   ossim_uint32 d_width    = getWidth();
   ossim_uint32 band       = 0;
   const T* s        = static_cast<const T*>(src);
   T** d             = new T*[num_bands];
   
   // Make destination pointers to each one.
   for (band=0; band<num_bands; band++)
   {
      d[band] = static_cast<T*>(getBuf(band));
      
      // Move the pointers to the first valid pixel.
      d[band] += (clip_rect.ul().y - img_rect.ul().y) * d_width +
                 clip_rect.ul().x - img_rect.ul().x;
   }
   
   // Move the source pointer to the first valid pixel.
   s += (clip_rect.ul().y - src_rect.ul().y) * s_width +
        (clip_rect.ul().x - src_rect.ul().x) * num_bands;
   
   // Copy the data.
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth  = clip_rect.width();
   
   for (ossim_uint32 line = 0; line < clipHeight; ++line)
   {
      ossim_uint32 j = 0;
      for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
      {
         for (band=0; band<num_bands; band++)
         {
            d[band][sample] = s[j+band];
         }
         j += num_bands;
      }
      
      s += s_width;
      for (band=0; band<num_bands; band++)
      {
         d[band] += d_width;
      }
   }
   
   delete [] d;
}
   
template <class T>
void ossimImageData::loadTileFromBilTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBilTemplate(T(0), src, src_rect, clip_rect);
}
   
template <class T>
void ossimImageData::loadTileFromBilTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect,
                                             const ossimIrect& clip_rect)
{
   static const char  MODULE[] = "ossimImageData::loadTileFromBil";
   
   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   
   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }
   
   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
      return;
   }

   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();
   
   // Get the width of the buffers.
   ossim_uint32 num_bands = getNumberOfBands();
   ossim_uint32 s_width = (src_rect.lr().x - src_rect.ul().x + 1);
   ossim_uint32 d_width = getWidth();
   ossim_uint32 band = 0;
   ossim_uint32 d_offset = (clip_rect.ul().y - img_rect.ul().y) * d_width +
                     clip_rect.ul().x - img_rect.ul().x;
   
   const T* s = static_cast<const T*>(src);

   // Get the number of bands and make destination pointers to each one.
   T** d = new T*[num_bands];
   
   for (band=0; band<num_bands; band++)
   {
      d[band] = static_cast<T*>(getBuf(band));
      
      // Move the pointers to the first valid pixel.
      d[band] += d_offset;
   }
   
   // Move the source pointer to the first valid pixel.
   s += (clip_rect.ul().y - src_rect.ul().y) * s_width * num_bands +
        clip_rect.ul().x - src_rect.ul().x;
   
   // Copy the data.
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth  = clip_rect.width();

   for (ossim_uint32 line = 0; line < clipHeight; ++line)
   {
      for (band = 0; band < num_bands; ++band)
      {
         for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
         {
            d[band][sample] = s[sample];
         }
         s       += s_width;
         d[band] += d_width;
      }
   }

   delete [] d;
}
   
template <class T>
void ossimImageData::loadTileFromBsqTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBsqTemplate(T(0), src, src_rect, clip_rect);
}

template <class T>
void ossimImageData::loadTileFromBsqTemplate(T, // dummy template variable
                                             const void* src,
                                             const ossimIrect& src_rect,
                                             const ossimIrect& clip_rect)
{
   static const char  MODULE[] = "ossimImageData::loadTileFromBsq";
   
   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   
   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }

   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
      return;
   }
   
   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();
   
   // Get the width of the buffers.
   ossim_uint32 num_bands = getNumberOfBands();
   ossim_uint32 s_width = src_rect.width();
   ossim_uint32 d_width = getWidth();
   ossim_uint32 s_band_offset = s_width * src_rect.height();
   
   const T* s = static_cast<const T*>(src);
   
   ossim_uint32 band;
   ossim_uint32 destinationOffset = (clip_rect.ul().y - img_rect.ul().y) * d_width +
                              (clip_rect.ul().x - img_rect.ul().x);
   ossim_uint32 destinationIndex = destinationOffset;
   ossim_uint32 sourceOffset = (clip_rect.ul().y - src_rect.ul().y) * s_width +
                         (clip_rect.ul().x - src_rect.ul().x);
   ossim_uint32 sourceIndex = sourceOffset;
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth = clip_rect.width();

   // Copy the data.
   for (band=0; band<num_bands; band++)
   {
      T* destinationBand = static_cast<T*>(getBuf(band));
      destinationIndex = destinationOffset;
      sourceIndex = sourceOffset + s_band_offset*band;
      
      for (ossim_uint32 line = 0; line < clipHeight; ++line)
      {
         for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
         {
            destinationBand[destinationIndex + sample]
               = s[sourceIndex+sample];
         }
         sourceIndex += s_width;
         destinationIndex += d_width;
      }
   }
}

void ossimImageData::loadTileFrom1Band(const void* src,
				       const ossimIrect& src_rect)
{
   ossim_uint32 bands = getNumberOfBands();
   for(ossim_uint32 band = 0; band < bands; ++band)
   {
      loadBand(src, src_rect, band);
   }
}

void ossimImageData::loadTileFrom1Band(const ossimImageData* src)
{
  if(!src) return;
  loadTileFrom1Band((const void*)src->getBuf(),
		    src->getImageRectangle());
}

void ossimImageData::loadShortBand(const void* src,
                                   const ossimIrect& src_rect,
                                   ossim_uint32 band,
                                   bool swap_bytes)
{
   static const char  MODULE[] = "ossimImageData::loadShortBand";

   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }

   // Check the band.
   if (!isValidBand(band))
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nInvalid band:  %d",
                    MODULE,
                    __FILE__,
                    __LINE__,
                    band);
      return;
   }

   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }

   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();

   // Get the width of the buffers.
   ossim_uint32 s_width = src_rect.width();
   ossim_uint32 d_width = getWidth();

   const ossim_uint16* s = static_cast<const ossim_uint16*>(src);
   ossim_uint16* d = getUshortBuf(band);

   // Move the pointers to the first valid pixel.
   s += (clip_rect.ul().y - src_rect.ul().y) * s_width +
        clip_rect.ul().x - src_rect.ul().x;
   
   d += (clip_rect.ul().y - img_rect.ul().y) * d_width +
        clip_rect.ul().x - img_rect.ul().x;
   
   // Copy the data.
   for (ossim_int32 line=clip_rect.ul().y; line<=clip_rect.lr().y; line++)
   {
      ossim_uint32 i = 0;
      for (ossim_int32 sample=clip_rect.ul().x; sample<=clip_rect.lr().x; sample++)
      {
         d[i] = (swap_bytes ? ( (s[i] << 8) | (s[i] >> 8) ) :
                 s[i]);
         ++i;
      }

      s += s_width;
      d += d_width;
   }

   validate();
}

bool ossimImageData::isPointWithin(const ossimIpt& point)const
{
   return ((point.x >= m_origin.x)&&
           (point.y >= m_origin.y)&&
           ((point.x - m_origin.x)<static_cast<ossim_int32>(m_spatialExtents[0]))&&
           ((point.y - m_origin.y)<static_cast<ossim_int32>(m_spatialExtents[1])));
}

bool ossimImageData::isPointWithin(ossim_int32 x, ossim_int32 y)const
{
   return ((x >= m_origin.x)&&
           (y >= m_origin.y)&&
           ((x - m_origin.x) < static_cast<ossim_int32>(m_spatialExtents[0]))&&
           ((y - m_origin.y) < static_cast<ossim_int32>(m_spatialExtents[1])));
}

void ossimImageData::unloadTile(void* dest,
                                const ossimIrect& dest_rect,
                                ossimInterleaveType type) const
{
   unloadTile(dest, dest_rect, getImageRectangle(), type);
}

void ossimImageData::unloadTile(void* dest,
                                const ossimIrect& dest_rect,
                                const ossimIrect& clip_rect,
                                ossimInterleaveType type) const
{
   switch (type)
   {
      case OSSIM_BIP:
         unloadTileToBip(dest, dest_rect, clip_rect);
         return;
      case OSSIM_BIL:
         unloadTileToBil(dest, dest_rect, clip_rect);
         return;
      case OSSIM_BSQ:
         unloadTileToBsq(dest, dest_rect, clip_rect);
         return;
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadTile\n\
File %s line %d\nUnknown scalar type!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadTileToBip(void* dest,
                                     const ossimIrect& dest_rect,
                                     const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBipTemplate(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBipTemplate(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBipTemplate(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBipTemplate(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         unloadTileToBipTemplate(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT32:
         unloadTileToBipTemplate(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBipTemplate(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBipTemplate(ossim_float64(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadTileToBip\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadTileToBipAlpha(void* dest,
                                          const ossimIrect& dest_rect,
                                          const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBipAlphaTemplate(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBipAlphaTemplate(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBipAlphaTemplate(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBipAlphaTemplate(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         unloadTileToBipAlphaTemplate(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT32:
         unloadTileToBipAlphaTemplate(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBipAlphaTemplate(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBipAlphaTemplate(ossim_float64(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadTileToBipAlpha\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadTileToBil(void* dest,
                                     const ossimIrect& dest_rect,
                                     const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBilTemplate(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBilTemplate(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBilTemplate(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBilTemplate(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_UINT32:
         unloadTileToBilTemplate(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         unloadTileToBilTemplate(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBilTemplate(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBilTemplate(ossim_float64(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadTileToBil\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadTileToBsq(void* dest,
                                     const ossimIrect& dest_rect,
                                     const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBsqTemplate(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBsqTemplate(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBsqTemplate(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBsqTemplate(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         unloadTileToBsqTemplate(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         unloadTileToBsqTemplate(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBsqTemplate(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBsqTemplate(ossim_float64(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadTileToBsq\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadBand(void* dest,
                                const ossimIrect& dest_rect,
                                ossim_uint32 band) const
{
   unloadBand(dest, dest_rect, getImageRectangle(), band);
}

void ossimImageData::unloadBand(void* dest,
                                const ossimIrect& dest_rect,
                                const ossimIrect& clip_rect,
                                ossim_uint32 band) const
{
   // Call the appropriate load method.
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadBandTemplate(ossim_uint8(0), dest, dest_rect, clip_rect, band);
         return;

      case OSSIM_SINT8:
         unloadBandTemplate(ossim_sint8(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadBandTemplate(ossim_uint16(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_SINT16:
         unloadBandTemplate(ossim_sint16(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT32:
         unloadBandTemplate(ossim_uint32(0), dest, dest_rect, clip_rect, band);
         return;

      case OSSIM_SINT32:
         unloadBandTemplate(ossim_sint32(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadBandTemplate(ossim_float32(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadBandTemplate(ossim_float64(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
                       ossimErrorCodes::OSSIM_ERROR,
                       "ossimImageData::unloadBand\n\
File %s line %d\nUnsupported scalar type for method!",
                       __FILE__,
                       __LINE__);      
         return;
   }
}

void ossimImageData::unloadBand( void* dest,
                                 ossim_uint32 src_band,
                                 ossim_uint32 dest_band,
                                 const ossimIrect& dest_rect,
                                 ossimInterleaveType il_type,
                                 OverwriteBandRule ow_type ) const
{
   unloadBand( dest, src_band, dest_band, dest_rect, getImageRectangle(), il_type, ow_type );
}

void ossimImageData::unloadBand( void* dest,
                                 ossim_uint32 src_band,
                                 ossim_uint32 dest_band,
                                 const ossimIrect& dest_rect,
                                 const ossimIrect& clip_rect,
                                 ossimInterleaveType il_type,
                                 OverwriteBandRule ow_type ) const
{
   static const char  MODULE[] = "ossimImageData::unloadBand";

   if ( il_type == OSSIM_BSQ )
   {
      unloadBandToBsq( dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " NOTICE:"
         << "\nUnsupported interleave type:  " << il_type << "  Returning..."
         << std::endl;
   }
}

void ossimImageData::unloadBandToBsq( void* dest,
                                      ossim_uint32 src_band,
                                      ossim_uint32 dest_band,
                                      const ossimIrect& dest_rect,
                                      const ossimIrect& clip_rect,
                                      OverwriteBandRule ow_type ) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadBandToBsqTemplate(ossim_uint8(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_SINT8:
         unloadBandToBsqTemplate(ossim_sint8(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadBandToBsqTemplate(ossim_uint16(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_SINT16:
         unloadBandToBsqTemplate(ossim_sint16(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_UINT32:
         unloadBandToBsqTemplate(ossim_uint32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_SINT32:
         unloadBandToBsqTemplate(ossim_sint32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadBandToBsqTemplate(ossim_float32(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadBandToBsqTemplate(ossim_float64(0), dest, src_band, dest_band, dest_rect, clip_rect, ow_type );
         return;

      case OSSIM_SCALAR_UNKNOWN:
      default:
         ossimSetError(getClassName(),
            ossimErrorCodes::OSSIM_ERROR,
            "ossimImageData::unloadBandToBsq\n\
            File %s line %d\nUnsupported scalar type for method!",
            __FILE__,
            __LINE__);      
         return;
   }
}

template <class T>
void ossimImageData::unloadBandTemplate(T, // dummy template variable
                                        void* dest,
                                        const ossimIrect& dest_rect,
                                        const ossimIrect& clip_rect,
                                        ossim_uint32 band) const
{
   static const char  MODULE[] = "ossimImageData::unloadBand";

   // Check the pointers.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }
   
   ossimIrect img_rect = getImageRectangle();
   
   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);
   
   // Check the output clip rect for intersection.
   if (!output_clip_rect.intersects(img_rect))
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }

   // Check for valid band.
   if (!isValidBand(band))
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " NOTICE:"
         << "\nInvalid band:  " << band << "  Returning..."
         << std::endl;
      return;
   }
   
   ossim_uint32 buf_width = dest_rect.lr().x - dest_rect.ul().x + 1;

   // Get a pointer to the source band buffer.
   const T* s = reinterpret_cast<const T*>(getBuf(band));

   // Get the offset for each source band.
   ossim_uint32 src_offset = (output_clip_rect.ul().y - img_rect.ul().y) *
      getWidth() + output_clip_rect.ul().x - img_rect.ul().x;
   
   // Move the pointers to the start.
   T* d = reinterpret_cast<T*>(dest);

   ossim_uint32 dest_offset = (output_clip_rect.ul().y - dest_rect.ul().y) *
      buf_width + output_clip_rect.ul().x - dest_rect.ul().x;

   d += dest_offset;
   s += src_offset;

   for (ossim_int32 line=output_clip_rect.ul().y;
        line<=output_clip_rect.lr().y; ++line)
   {
      ossim_uint32 i=0;
      for (ossim_int32 samp=clip_rect.ul().x;
           samp<=output_clip_rect.lr().x; ++samp)
      {
         d[i] = s[i];
         ++i;
      }
      d += buf_width;
      s += getWidth();
   }
}

template <class T> void
ossimImageData::unloadTileToBipTemplate(T, // dummy template variable
                                        void* dest,
                                        const ossimIrect& dest_rect,
                                        const ossimIrect& clip_rect) const
{
   static const char  MODULE[] = "ossimImageData::unloadTileToBip";
   
   // Check the pointer.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }
   
   bool  dataIsNull = false;
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      dataIsNull = true;
   }
   
   ossimIrect img_rect = getImageRectangle();
   
   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);
   
   // Check the output clip rect for intersection.
   if (output_clip_rect.hasNans())
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }
   
   ossim_int32 num_bands = getNumberOfBands();
   
   ossim_int32 buf_width = dest_rect.width() * num_bands;

   if(!dataIsNull)
   {
      // Get the number of bands and grab a pointers to each one.
      const T** s = new const T*[num_bands];
      
      ossim_int32 band;
      ossim_int32 s_width = getWidth();
      
      for (band=0; band<num_bands; band++)
      {
         s[band] = reinterpret_cast<const T*>(getBuf(band));
      }
      
      // Move the pointers to the start.
      T* d = reinterpret_cast<T*>(dest);      
      
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * buf_width +
           (output_clip_rect.ul().x - dest_rect.ul().x) * num_bands;
      
      // Get the offset for each source band.
      ossim_int32 src_offset = (output_clip_rect.ul().y - img_rect.ul().y) *
         s_width + (output_clip_rect.ul().x - img_rect.ul().x);
      
      ossim_int32 output_clip_width  = output_clip_rect.width();
      ossim_int32 output_clip_height = output_clip_rect.height();
      
      for (band=0; band<(ossim_int32)getNumberOfBands(); band++)
      {
         s[band] += src_offset;
      }
      
      ossim_int32 j;
      for (ossim_int32 line=0; line<output_clip_height; ++line)
      {
         j = 0;
         for (ossim_int32 samp=0; samp<output_clip_width; ++samp, j+=num_bands)
         {
            for (band=0; band<num_bands; ++band)
            {
               d[j+band] = s[band][samp];
            }
         }
         
         // increment to next line...
         d += buf_width;
         for (band=0; band<num_bands; ++band)
         {
            s[band] += s_width;
         }
      
      }
      delete [] s;
   }
   else
   {
      ossim_int32 band = 0;
      T* nulls = new T[num_bands];
      for(band = 0; band < num_bands; ++band)
      {
         nulls[band] = static_cast<T>(m_nullPixelValue[band]);
      }
      // Move the pointers to the start.
      T* d = reinterpret_cast<T*>(dest);
      
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * buf_width +
           (output_clip_rect.ul().x - dest_rect.ul().x) * num_bands;

      for (ossim_int32 line=output_clip_rect.ul().y;
           line<=output_clip_rect.lr().y; ++line)
      {
         ossim_int32 i=0;
         ossim_int32 j=0;
         for (ossim_int32 samp=output_clip_rect.ul().x;
              samp<=output_clip_rect.lr().x; ++samp)
         {
            for (band=0; band<num_bands; ++band)
            {
               d[j+band] = nulls[band];
            }
            ++i;  // increment to next pixel...
            j+= num_bands;
         }
         
         // increment to next line...
         d += buf_width;
      }
      delete [] nulls;
      nulls = 0;
   }
}

template <class T> void
ossimImageData::unloadTileToBipAlphaTemplate(T, // dummy template variable
                                             void* dest,
                                             const ossimIrect& dest_rect,
                                             const ossimIrect& clip_rect) const
{
   static const char  M[] = "ossimImageData::unloadTileToBipAlphaTemplate";

   // Sanity checks:

   // Are we null:
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << M << " ERROR:"
         << "\nAttempting to copy null tile!  Returning..." << std::endl;
      return;
   }
   
   // Check the pointer.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << M << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }

   if ( !hasAlpha() )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << M << " ERROR:"
         << "\nTile has no alpha!  Returning..." << std::endl;
      return; 
   }
   
   ossimIrect img_rect = getImageRectangle();
   
   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);
   
   // Check the output clip rect for intersection.
   if (output_clip_rect.hasNans())
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }
   
   const ossim_int32 NUM_DATA_BANDS = getNumberOfBands();
   const ossim_int32 BANDS = NUM_DATA_BANDS + 1; // Has data plus alpha.
   const ossim_float64 MAX_PIX = static_cast<T>(getMaxPix(0));
   const ossim_float64 ALPHA_MAX_PIX = 255.00;
   const ossim_int32 S_WIDTH = getWidth();
   const ossim_int32 D_WIDTH = dest_rect.width() * BANDS;
   const ossim_int32 OUTPUT_CLIP_WIDTH  = output_clip_rect.width();
   const ossim_int32 OUTPUT_CLIP_HEIGHT = output_clip_rect.height();

   bool uint8Flag = ( getScalarType() == OSSIM_UINT8 );
   
   // Get the offset for each source band relative to start of clip rect.
   const ossim_int32 SRC_OFFSET = (output_clip_rect.ul().y - img_rect.ul().y) *
      S_WIDTH + (output_clip_rect.ul().x - img_rect.ul().x);
   
   // Get pointers to each one band of image data.
   std::vector<const T*> s(NUM_DATA_BANDS);
   ossim_int32 band;
   for (band = 0; band < NUM_DATA_BANDS; ++band)
   {
      s[band] = reinterpret_cast<const T*>(getBuf(band));

      // Move to start of clip rect.
      s[band] += SRC_OFFSET;
   }

   // Get pointer to the alpha channel:
   const ossim_uint8* a = getAlphaBuf();

   // Move to start of clip rect.
   a += SRC_OFFSET;

   // Pointer to the destination buffer.
   T* d = reinterpret_cast<T*>(dest); 

   // Move to start of clip rect.
   d += (output_clip_rect.ul().y - dest_rect.ul().y) * D_WIDTH +
      (output_clip_rect.ul().x - dest_rect.ul().x) * BANDS;
   

   // Loop to copy data:
   for (ossim_int32 line = 0; line < OUTPUT_CLIP_HEIGHT; ++line)
   {
      ossim_int32 j = 0;
      ossim_int32 alphaIdx = 0;
      for (ossim_int32 samp = 0; samp < OUTPUT_CLIP_WIDTH; ++samp, j +=BANDS)
      {
         // Copy the pixels:
         for (band = 0; band < NUM_DATA_BANDS; ++band)
         {
            d[j+band] = s[band][samp];
         }
         
         // Copy alpha channel converting to scalar type.
         if ( uint8Flag )
         {
            d[j+NUM_DATA_BANDS] = a[alphaIdx++];
         }
         else
         {
            d[j+NUM_DATA_BANDS] =
               static_cast<T>( (a[alphaIdx++]/ALPHA_MAX_PIX) * MAX_PIX );
         }
      }
      
      // Increment pointers to next line...
      d += D_WIDTH;
      for (band = 0; band < NUM_DATA_BANDS; ++band)
      {
         s[band] += S_WIDTH;
      }
      a += S_WIDTH;
   }
}

template <class T> void
ossimImageData::unloadTileToBilTemplate(T,  // dummy template arg...
                                        void* dest,
                                        const ossimIrect& dest_rect,
                                        const ossimIrect& clip_rect) const
{
  static const char  MODULE[] = "ossimImageData::unloadTileToBil";

   // Check the pointers.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }
   bool dataIsNull = false;
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      dataIsNull = true;
   }

   ossimIrect img_rect = getImageRectangle();
   
   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);
   
   // Check the output clip rect for intersection.
   if (output_clip_rect.hasNans())
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }

   ossim_uint32 num_bands = getNumberOfBands();

   ossim_uint32 buf_width = dest_rect.width();

   if(!dataIsNull)
   {
      // Get the number of bands and grab a pointers to each one.
      const T** s = new const T*[num_bands];
      
      ossim_uint32 band;
      
      for (band=0; band<num_bands; band++)
      {
         s[band] = reinterpret_cast<const T*>(getBuf(band));
      }
      
      // Get the offset for each source band.
      ossim_uint32 src_offset = (output_clip_rect.ul().y - img_rect.ul().y) *
         getWidth() + (output_clip_rect.ul().x - img_rect.ul().x);
      
      // Move the pointers to the start.
      T* d = reinterpret_cast<T*>(dest);

      d += (output_clip_rect.ul().y - dest_rect.ul().y) *
         buf_width * num_bands +
         (output_clip_rect.ul().x-dest_rect.ul().x);
      
      for (band=0; band<num_bands; ++band)
      {
         s[band] += src_offset;
      }
      
      for (ossim_int32 line=output_clip_rect.ul().y;
           line<=output_clip_rect.lr().y; ++line)
      {
         for (band=0; band<num_bands; ++band)
         {
            ossim_uint32 i=0;
            for (ossim_int32 samp=output_clip_rect.ul().x;
                 samp<=output_clip_rect.lr().x; ++samp)
            {
               d[i] = s[band][i];
               ++i;
            }
            d       += buf_width;
            s[band] += getWidth();
         }
      }
      // Free up memory allocated for pointers.
      delete [] s;
   }
   else
   {
      ossim_uint32 band = 0;
      T* nulls = new T[num_bands];
      for(band = 0; band < num_bands; ++band)
      {
         nulls[band] = static_cast<T>(m_nullPixelValue[band]);
      }
      
      // Move the pointers to the start.
      T* d = reinterpret_cast<T*>(dest);

      d += (output_clip_rect.ul().y - dest_rect.ul().y) *
           buf_width * num_bands + (output_clip_rect.ul().x - dest_rect.ul().x);
      
      for (ossim_int32 line=output_clip_rect.ul().y;
           line<=output_clip_rect.lr().y; ++line)
      {
         for (band=0; band<num_bands; ++band)
         {
            ossim_uint32 i=0;
            for (ossim_int32 samp=output_clip_rect.ul().x;
                 samp<=output_clip_rect.lr().x; ++samp)
            {
               d[i] = nulls[band];
               ++i;
            }
            d += buf_width;
         }
      }
      delete [] nulls;
   }
}

template <class T> void ossimImageData::nullTileAlphaTemplate(
   T,
   const ossim_uint8* src,
   const ossimIrect& src_rect,
   const ossimIrect& clip_rect,
   bool multiplyAlphaFlag)
{
   static const char  MODULE[] = "ossimImageData::nullTileAlphaTemplate";
   
   // Check the pointer.
   if (!src)
   {
      // Set the error...
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "%s File %s line %d\nNULL pointer passed to method!",
                    MODULE,
                    __FILE__,
                    __LINE__);
      return;
   }
   
   const ossimIrect img_rect = getImageRectangle();
   
   // Check for intersect.
   if ( ! img_rect.intersects(src_rect) )
   {
      return; // Nothing to do here.
   }

   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
      return;
   }
   
   // Check the status and allocate memory if needed.
   if (getDataObjectStatus() == OSSIM_NULL) initialize();
   
   // Get the width of the buffers.
   ossim_uint32 num_bands = getNumberOfBands();
   ossim_uint32 s_width = src_rect.width();
   ossim_uint32 d_width = getWidth();
   ossim_uint32 band;
   ossim_uint32 destinationOffset = (clip_rect.ul().y - img_rect.ul().y) * d_width +
                              (clip_rect.ul().x - img_rect.ul().x);
   ossim_uint32 destinationIndex = destinationOffset;
   ossim_uint32 sourceOffset = (clip_rect.ul().y - src_rect.ul().y) * s_width +
                         (clip_rect.ul().x - src_rect.ul().x);
   ossim_uint32 sourceIndex = sourceOffset;
   ossim_uint32 clipHeight = clip_rect.height();
   ossim_uint32 clipWidth = clip_rect.width();

   if(!multiplyAlphaFlag)
   {
      // Copy the data.
      for (band=0; band<num_bands; band++)
      {
         T* destinationBand = static_cast<T*>(getBuf(band));
         destinationIndex = destinationOffset;
         sourceIndex = sourceOffset;
         T nullPix = (T)getNullPix(band);
         T minPix = (T)getMinPix(band);
         for (ossim_uint32 line = 0; line < clipHeight; ++line)
         {
            for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
            {
               if(src[sourceIndex+sample] == 0)
               {
                  destinationBand[destinationIndex + sample] = nullPix;
               }
               else if(destinationBand[destinationIndex + sample] == nullPix)
               {
                  destinationBand[destinationIndex + sample] = minPix;
               }
            }
            sourceIndex += s_width;
            destinationIndex += d_width;
         }
      }
   }
   else
   {
      ossim_float64 normalizer = 1.0/255.0;
      // Copy the data.
      for (band=0; band<num_bands; band++)
      {
         T* destinationBand = static_cast<T*>(getBuf(band));
         destinationIndex = destinationOffset;
         sourceIndex = sourceOffset;
         T nullPix = (T)getNullPix(band);
         T minPix = (T)getMinPix(band);
         for (ossim_uint32 line = 0; line < clipHeight; ++line)
         {
            for (ossim_uint32 sample = 0; sample < clipWidth; ++sample)
            {
               if(src[sourceIndex+sample] != 255)
               {
                  if(src[sourceIndex+sample] != 0)
                  {
                     destinationBand[destinationIndex + sample] = (T)(destinationBand[destinationIndex + sample]*
                                                                      (src[sourceIndex+sample]*normalizer));
                     if(destinationBand[destinationIndex + sample] == nullPix)
                     {
                        destinationBand[destinationIndex + sample] = minPix;
                     }
                  }
                  else
                  {
                     destinationBand[destinationIndex + sample] = nullPix;
                  }
               }
               else if(destinationBand[destinationIndex + sample] == nullPix)
               {
                  destinationBand[destinationIndex + sample] = minPix;
               }
            }
            sourceIndex += s_width;
            destinationIndex += d_width;
         }
      }
      
   }
}

template <class T> void
ossimImageData::unloadTileToBsqTemplate(T,  // dummy template arg...
                                        void* dest,
                                        const ossimIrect& dest_rect,
                                        const ossimIrect& clip_rect) const
{
   static const char  MODULE[] = "ossimImageData::unloadTileToBsq";
   
   // Check the pointers.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }

   bool dataIsNull = false;
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      dataIsNull = true;
   }

   ossimIrect img_rect = getImageRectangle();
   
   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);
   
   // Check the output clip rect for intersection.
   if (output_clip_rect.hasNans())
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }

   ossim_uint32 num_bands = getNumberOfBands();
   ossim_uint32 band      = 0;
   if(!dataIsNull)
   {
      ossim_uint32 d_width       = dest_rect.lr().x - dest_rect.ul().x + 1;
      ossim_uint32 d_band_offset = d_width * (dest_rect.lr().y-dest_rect.ul().y+1);
      ossim_uint32 s_width       = getWidth();
      ossim_uint32 s_offset      = (output_clip_rect.ul().y - img_rect.ul().y) *
                                   s_width + (output_clip_rect.ul().x - img_rect.ul().x);

      T* d        = static_cast<T*>(dest);
      const T** s = new const T*[num_bands];

      // Grab a pointers to each one.
      for (band=0; band<num_bands; ++band)
      {
         s[band] = reinterpret_cast<const T*>(getBuf(band));

         // Move to first valid pixel.
         s[band] += s_offset;
      }

      // Move to first valid pixel.
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * d_width +
           (output_clip_rect.ul().x - dest_rect.ul().x);

      for (band=0; band<num_bands; ++band)
      {
         ossim_uint32 d_buf_offset = 0;

         for (ossim_int32 line=output_clip_rect.ul().y;
              line<=output_clip_rect.lr().y; ++line)
         {
            ossim_int32 i=0;
            for (ossim_int32 samp=output_clip_rect.ul().x;
                 samp<=output_clip_rect.lr().x; ++samp)
            {
               d[d_buf_offset+i] = s[band][i];
               ++i;
            }

            d_buf_offset += d_width;
            s[band]      += s_width;
         }
         d += d_band_offset;
      }
      
      // Free up memory allocated for pointers.
      delete [] s;
   }
   else
   {
      ossim_uint32 d_width       = dest_rect.lr().x - dest_rect.ul().x + 1;
      ossim_uint32 d_band_offset = d_width * (dest_rect.lr().y-dest_rect.ul().y+1);
      
      ossim_uint8* d = static_cast<ossim_uint8*>(dest);
            
      // Move to first valid pixel.
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * d_width +
           (output_clip_rect.ul().x - dest_rect.ul().x);
      
      for (band=0; band<num_bands; ++band)
      {
         ossim_uint8 np = static_cast<ossim_uint8>(m_nullPixelValue[band]);
         ossim_uint32 d_buf_offset = 0;
         
         for (ossim_int32 line=output_clip_rect.ul().y;
              line<=output_clip_rect.lr().y; ++line)
         {
            ossim_int32 i=0;
            for (ossim_int32 samp=output_clip_rect.ul().x;
                 samp<=output_clip_rect.lr().x; ++samp)
            {
               d[d_buf_offset+i] = np;
               ++i;
            }
            
            d_buf_offset += d_width;
         }
         d += d_band_offset;
      }
   }
}

template <class T> void
ossimImageData::unloadBandToBsqTemplate(T,  // dummy template arg...
                                        void* dest,
                                        ossim_uint32 src_band,
                                        ossim_uint32 dest_band,
                                        const ossimIrect& dest_rect,
                                        const ossimIrect& clip_rect,
                                        OverwriteBandRule ow_type) const
{
   static const char  MODULE[] = "ossimImageData::unloadBandToBsq";

   // Check the pointers.
   if (!dest)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " ERROR:"
         << "\nNULL pointer passed to method!  Returning..." << std::endl;
      return;
   }

   bool dataIsNull = false;
   if (getDataObjectStatus() == OSSIM_NULL)
   {
      dataIsNull = true;
   }

   ossimIrect img_rect = getImageRectangle();

   // Clip the clip_rect to the tile rect.
   ossimIrect output_clip_rect = clip_rect.clipToRect(img_rect);

   // Clip it again to the destination rect.
   output_clip_rect = dest_rect.clipToRect(output_clip_rect);

   // Check the output clip rect for intersection.
   if (output_clip_rect.hasNans())
   {
      return;
   }
   if ( !(output_clip_rect.intersects(dest_rect)) )
   {
      return;
   }

   ossim_uint32 num_bands = getNumberOfBands();
   ossim_uint32 band      = 0;
   if(!dataIsNull)
   {
      ossim_uint32 d_width       = dest_rect.lr().x - dest_rect.ul().x + 1;
      ossim_uint32 d_band_offset = d_width * (dest_rect.lr().y-dest_rect.ul().y+1);
      ossim_uint32 s_width  = getWidth();
      ossim_uint32 s_offset = (output_clip_rect.ul().y - img_rect.ul().y) *
         s_width + (output_clip_rect.ul().x - img_rect.ul().x);

      T* d        = static_cast<T*>(dest);
      const T** s = new const T*[num_bands];

      // Grab a pointers to each one.
      for (band=0; band<num_bands; ++band)
      {
         s[band] = reinterpret_cast<const T*>(getBuf(band));

         // Move to first valid pixel.
         s[band] += s_offset;
      }

      // Move to first valid pixel.
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * d_width +
           (output_clip_rect.ul().x - dest_rect.ul().x);

      ossim_uint32 d_dest_band_offset = dest_band * d_band_offset;
      ossim_uint32 d_buf_offset = 0;

      for (ossim_int32 line=output_clip_rect.ul().y;
         line<=output_clip_rect.lr().y; ++line)
      {
         ossim_int32 i=0;
         for (ossim_int32 samp=output_clip_rect.ul().x;
            samp<=output_clip_rect.lr().x; ++samp)
         {
            ossim_uint32 d_pixel_offset = d_buf_offset+i;
            ossim_uint32 d_dest_band_pixel_offset = d_pixel_offset + d_dest_band_offset;

            switch( ow_type )
            {
               case COLOR_DISCREPANCY:
               case COLOR_DISCREPANCY_OF_ANY_FROM_DEST:
               {
                  T d_dest_band = d[d_dest_band_pixel_offset];

                  for ( band=0; band<num_bands && band!=dest_band; ++band )
                  {
                     T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

                     // test for color discrepancy
                     if ( d_other_band != d_dest_band )
                     {
                        d[d_dest_band_pixel_offset] = s[src_band][i];
                        break;
                     }
                  }
               }
               break;

               case COLOR_DISCREPANCY_OF_ALL_FROM_DEST:
               {
                  T d_dest_band = d[d_dest_band_pixel_offset];

                  bool bFoundSameValue = false;
                  for ( band=0; band<num_bands && band!=dest_band; ++band )
                  {
                     T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

                     // test for color sameness
                     if ( d_other_band == d_dest_band )
                     {
                        bFoundSameValue = true;
                        break;
                     }
                  }
                  if ( bFoundSameValue == false )
                  {
                     d[d_dest_band_pixel_offset] = s[src_band][i];
                  }
               }
               break;

               case COLOR_EQUALITY_OF_ANY_TO_SRC:
               {
                  T d_src_band = s[src_band][i];

                  for ( band=0; band<num_bands && band!=dest_band; ++band )
                  {
                     T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

                     // test for color discrepancy
                     if ( d_other_band == d_src_band )
                     {
                        d[d_dest_band_pixel_offset] = s[src_band][i];
                        break;
                     }
                  }
               }
               break;

               case COLOR_EQUALITY_OF_ALL_TO_SRC:
               {
                  T d_src_band = s[src_band][i];

                  bool bFoundDifferentValue = false;
                  for ( band=0; band<num_bands && band!=dest_band; ++band )
                  {
                     T d_other_band = d[d_pixel_offset + (band * d_band_offset)];

                     // test for color discrepancy
                     if ( d_other_band != d_src_band )
                     {
                        bFoundDifferentValue = true;
                        break;
                     }
                  }
                  if ( bFoundDifferentValue == false )
                  {
                     d[d_dest_band_pixel_offset] = s[src_band][i];
                  }
               }
               break;

               case NULL_RULE:
               default:
               {
                  d[d_dest_band_pixel_offset] = s[src_band][i];
               }
               break;
            }

            ++i;
         }

         d_buf_offset += d_width;
         s[src_band]  += s_width;
      }

      // Free up memory allocated for pointers.
      delete [] s;
   }
   else
   {
      ossim_uint32 d_width       = dest_rect.lr().x - dest_rect.ul().x + 1;
      ossim_uint32 d_band_offset = d_width * (dest_rect.lr().y-dest_rect.ul().y+1);

      ossim_uint8* d = static_cast<ossim_uint8*>(dest);

      // Move to first valid pixel.
      d += (output_clip_rect.ul().y - dest_rect.ul().y) * d_width +
         (output_clip_rect.ul().x - dest_rect.ul().x);

      for (band=0; band<num_bands; ++band)
      {
         ossim_uint8 np = static_cast<ossim_uint8>(m_nullPixelValue[band]);
         ossim_uint32 d_buf_offset = 0;

         for (ossim_int32 line=output_clip_rect.ul().y;
            line<=output_clip_rect.lr().y; ++line)
         {
            ossim_int32 i=0;
            for (ossim_int32 samp=output_clip_rect.ul().x;
               samp<=output_clip_rect.lr().x; ++samp)
            {
               d[d_buf_offset+i] = np;
               ++i;
            }

            d_buf_offset += d_width;
         }
         d += d_band_offset;
      }
   }
}

ossimObject* ossimImageData::dup() const
{
   return new ossimImageData(*this);
}

void ossimImageData::copyTileToNormalizedBuffer(ossim_float64* buf)const
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileToNormalizedBuffer ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileToNormalizedBuffer ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }
   
   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         memmove(buf, getBuf(), getDataSizeInBytes());
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         ossim_uint32 upperBound = getWidth()*getHeight()*getNumberOfBands();
         const ossim_float32* inputBuf = static_cast<const ossim_float32*>(getBuf());
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            buf[offset] = inputBuf[offset];
         }
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyTileToNormalizedBuffer((ossim_float32)0, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyTileToNormalizedBuffer((ossim_float64)0, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyTileToNormalizedBuffer((ossim_uint8)0, buf);
         break;
      
      }
      case OSSIM_SINT8:
      {
         copyTileToNormalizedBuffer((ossim_sint8)0, buf);
         break;
      
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyTileToNormalizedBuffer((ossim_uint16)0, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyTileToNormalizedBuffer((ossim_sint16)0, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyTileToNormalizedBuffer((ossim_uint32)0, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyTileToNormalizedBuffer((ossim_sint32)0, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "NOTICE:  copyTileToNormalizedBuffer not implemented yet"
            << std::endl;
      }
      
   }  // End of "switch(getScalarType())"
}

template <class T>
void ossimImageData::copyTileToNormalizedBuffer(T /* dummyTemplate */,
                                                ossim_float64* buf) const
{
   const ossim_uint32 SIZE  = getSizePerBand();
   const ossim_uint32 BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const ossim_float64 MIN_PIX = getMinPix(band);
      const ossim_float64 MAX_PIX = getMaxPix(band);
      const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
      const ossim_float64 NP      = getNullPix(band);

      const T* s = (T*)getBuf(band);  // source
      ossim_float64* d = (ossim_float64*)(buf + (band*SIZE));  // destination
      
      for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
      {
         ossim_float64 p = s[offset];
         if(p != NP)
         {
            if( p == MIN_PIX)
            {
               d[offset] = OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
            }
            else
            {
               d[offset] = (p-MIN_PIX)/RANGE;
            }
         }
         else
         {
            d[offset] = 0.0;
         }
      }
   }   
}

template <class T>
void ossimImageData::copyTileToNormalizedBuffer(T /* dummyTemplate */,
                                                ossim_float32* buf) const
{
   const ossim_uint32 SIZE  = getSizePerBand();
   const ossim_uint32 BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const ossim_float64 MIN_PIX = getMinPix(band);
      const ossim_float64 MAX_PIX = getMaxPix(band);
      const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
      const ossim_float64 NP      = getNullPix(band);
      
      const T* s = (T*)getBuf(band);  // source
      ossim_float32* d = (ossim_float32*)(buf + (band*SIZE));  // destination
      
      for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
      {
         ossim_float64 p = s[offset];
         if(p != NP)
         {
            if( p == MIN_PIX)
            {
               d[offset] = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
            }
            else
            {
               d[offset] = (p-MIN_PIX)/RANGE;
            }
         }
         else
         {
            d[offset] = 0.0;
         }
      }
   }   
}

template <class T>
void ossimImageData::copyTileToNormalizedBuffer(T /* dummyTemplate */,
                                                ossim_uint32 band,
                                                ossim_float64* buf) const
{
   const ossim_uint32  SIZE    = getSizePerBand();
   const ossim_float64 MIN_PIX = getMinPix(band);
   const ossim_float64 MAX_PIX = getMaxPix(band);
   const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
   const ossim_float64 NP      = getNullPix(band);
   
   const T* s = (T*)getBuf(band);  // source
   ossim_float64* d = (ossim_float64*)(buf);  // destination
   
   for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
   {
         ossim_float64 p = s[offset];
         if(p != NP)
         {
            if( p == MIN_PIX)
            {
               d[offset] = OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
            }
            else
            {
               d[offset] = (p-MIN_PIX)/RANGE;
            }
         }
         else
         {
            d[offset] = 0.0;
         }
   }   
}

template <class T>
void ossimImageData::copyTileToNormalizedBuffer(T /* dummyTemplate */,
                                                ossim_uint32 band,
                                                ossim_float32* buf) const
{
   const ossim_uint32  SIZE    = getSizePerBand();
   const ossim_float64 MIN_PIX = getMinPix(band);
   const ossim_float64 MAX_PIX = getMaxPix(band);
   const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
   const ossim_float64 NP      = getNullPix(band);
   
   const T* s = (T*)getBuf(band);  // source
   ossim_float32* d     = (ossim_float32*)(buf);  // destination
   
   for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
   {
      ossim_float64 p = s[offset];
      if(p != NP)
      {
         if( p == MIN_PIX)
         {
            d[offset] = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
         }
         else
         {
            d[offset] = (p-MIN_PIX)/RANGE;
         }
      }
      else
      {
         d[offset] = 0.0;
      }
   }   
}

template <class T>
void ossimImageData::copyNormalizedBufferToTile(T /* dummyTemplate */,
                                                ossim_float64* buf)
{
   const ossim_uint32 SIZE  = getSizePerBand();
   const ossim_uint32 BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const ossim_float64 MIN_PIX = getMinPix(band);
      const ossim_float64 MAX_PIX = getMaxPix(band);
      const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
      const T NP                  = (T)getNullPix(band);
      
      ossim_float64* s = buf + (band*SIZE); // source
      T* d   = (T*)getBuf(band); // destination
      
      for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
      {
         const ossim_float64 P = s[offset];
         if(P != 0.0)
         {
            d[offset] = (T)(MIN_PIX + RANGE*P);
         }
         else
         {
            d[offset] = NP;
         }
      }
   }
}

template <class T>
void ossimImageData::copyNormalizedBufferToTile(T /* dummyTemplate */,
                                                ossim_float32* buf)
{
   const ossim_uint32 SIZE  = getSizePerBand();
   const ossim_uint32 BANDS = getNumberOfBands();
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const ossim_float64 MIN_PIX = getMinPix(band);
      const ossim_float64 MAX_PIX = getMaxPix(band);
      const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
      const T             NP      = (T)getNullPix(band);
      
      ossim_float32* s = buf + (band*SIZE); // source
      T* d   = (T*)getBuf(band); // destination
      
      for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
      {
         const ossim_float64 P = s[offset];
         if(P != 0.0)
         {
	   ossim_float64 test = MIN_PIX + RANGE*P;
	   if(test > MAX_PIX) test = MAX_PIX;
	   d[offset] = (T)test;
         }
         else
         {
            d[offset] = NP;
         }
      }
   }
}

template <class T>
void ossimImageData::copyNormalizedBufferToTile(T /* dummyTemplate */,
                                                ossim_uint32 band,
                                                ossim_float64* buf)
{
   const ossim_uint32  SIZE    = getSizePerBand();
   const ossim_float64 MIN_PIX = getMinPix(band);
   const ossim_float64 MAX_PIX = getMaxPix(band);
   const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
   const T             NP      = (T)getNullPix(band);
   
   ossim_float64* s = buf; // source
   T* d   = (T*)getBuf(band); // destination
   
   for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
   {
      const ossim_float64 P = s[offset];
      if(P != 0.0)
      {
	   ossim_float64 test = MIN_PIX + RANGE*P;
	   if(test > MAX_PIX) test = MAX_PIX;
	   d[offset] = (T)test;
      }
      else
      {
         d[offset] = NP;
      }
   }
}

template <class T>
void ossimImageData::copyNormalizedBufferToTile(T /* dummyTemplate */,
                                                ossim_uint32 band,
                                                ossim_float32* buf)
{
   const ossim_uint32 SIZE     = getSizePerBand();
   const ossim_float64 MIN_PIX = getMinPix(band);
   const ossim_float64 MAX_PIX = getMaxPix(band);
   const ossim_float64 RANGE   = (MAX_PIX-MIN_PIX);
   const T NP                  = (T)getNullPix(band);
   
   ossim_float32* s = buf; // source
   T* d   = (T*)getBuf(band); // destination
   
   for(ossim_uint32 offset = 0; offset < SIZE; ++offset)
   {
      const ossim_float64 P = s[offset];
      if(P != 0.0)
      {
	   ossim_float64 test = MIN_PIX + RANGE*P;
	   if(test > MAX_PIX) test = MAX_PIX;
	   d[offset] = (T)test;
      }
      else
      {
         d[offset] = NP;
      }
   }
}

void ossimImageData::copyTileBandToNormalizedBuffer(ossim_uint32 band,
                                                    ossim_float64* buf) const
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }
   
   if(!getBuf(band)) return;

   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         memmove(buf, getBuf(band), getDataSizeInBytes());
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         ossim_uint32 upperBound = getWidth()*getHeight();
         const ossim_float32* inputBuf = static_cast<const ossim_float32*>(getBuf(band));
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            buf[offset] = inputBuf[offset];
         }
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyTileToNormalizedBuffer((ossim_float32)0, band, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyTileToNormalizedBuffer((ossim_float64)0, band, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyTileToNormalizedBuffer((ossim_uint8)0, band, buf);
         break;
      
      }
      case OSSIM_SINT8:
      {
         copyTileToNormalizedBuffer((ossim_sint8)0, band, buf);
         break;
      
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyTileToNormalizedBuffer((ossim_uint16)0, band, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyTileToNormalizedBuffer((ossim_sint16)0, band, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyTileToNormalizedBuffer((ossim_uint32)0, band, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyTileToNormalizedBuffer((ossim_sint32)0, band, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
            << " Unknown scalar type!"
            << std::endl;
      }
      
   }  // End of "switch(getScalarType())"
   
}

void ossimImageData::copyTileToNormalizedBuffer(ossim_float32* buf)const
{
   
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileToNormalizedBuffer ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileToNormalizedBuffer ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }

   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_uint32 upperBound = getWidth()*getHeight()*getNumberOfBands();
         const ossim_float32* inputBuf = static_cast<const ossim_float32*>(getBuf());
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            buf[offset] = inputBuf[offset];
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         memmove(buf, getBuf(), getDataSizeInBytes());
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyTileToNormalizedBuffer((ossim_float32)0, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyTileToNormalizedBuffer((ossim_float64)0, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyTileToNormalizedBuffer((ossim_uint8)0, buf);
         break;
      
      }
      case OSSIM_SINT8:
      {
         copyTileToNormalizedBuffer((ossim_sint8)0, buf);
         break;
      
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyTileToNormalizedBuffer((ossim_uint16)0, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyTileToNormalizedBuffer((ossim_sint16)0, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyTileToNormalizedBuffer((ossim_uint32)0, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyTileToNormalizedBuffer((ossim_sint32)0, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "NOTICE:  copyTileToNormalizedBuffer not implemented yet"
            << std::endl;
      }
      
   }  // End of "switch(getScalarType())"
}

void ossimImageData::copyTileBandToNormalizedBuffer(ossim_uint32 band,
                                                    ossim_float32* buf)const
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }

   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_uint32 upperBound = getWidth()*getHeight();
         const ossim_float32* inputBuf = static_cast<const ossim_float32*>(getBuf(band));
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            buf[offset] = inputBuf[offset];
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         memmove(buf, getBuf(band), getSizePerBandInBytes());
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyTileToNormalizedBuffer((ossim_float32)0, band, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyTileToNormalizedBuffer((ossim_float64)0, band, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyTileToNormalizedBuffer((ossim_uint8)0, band, buf);
         break;
      
      }
      case OSSIM_SINT8:
      {
         copyTileToNormalizedBuffer((ossim_sint8)0, band, buf);
         break;
      
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyTileToNormalizedBuffer((ossim_uint16)0, band, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyTileToNormalizedBuffer((ossim_sint16)0, band, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyTileToNormalizedBuffer((ossim_uint32)0, band, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyTileToNormalizedBuffer((ossim_sint32)0, band, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::copyTileBandToNormalizedBuffer ERROR:"
            << " Unknown scalar type"
            << std::endl;
      }

   }  // End of "switch(getScalarType())"
}


void ossimImageData::copyNormalizedBufferToTile(ossim_float64* buf)
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }
   
   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         memmove(getBuf(), buf, getDataSizeInBytes());
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         ossim_uint32 upperBound = getWidth()*getHeight()*getNumberOfBands();
         ossim_float32* inputBuf = static_cast<ossim_float32*>(getBuf());
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            inputBuf[offset] = static_cast<ossim_float32>(buf[offset]);
         }
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyNormalizedBufferToTile((ossim_float32)0, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyNormalizedBufferToTile((ossim_float64)0, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyNormalizedBufferToTile((ossim_uint8)0, buf);
         break;
      
      }
      case OSSIM_SINT8:
      {
         copyNormalizedBufferToTile((ossim_sint8)0, buf);
         break;
      
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyNormalizedBufferToTile((ossim_uint16)0, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyNormalizedBufferToTile((ossim_sint16)0, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyNormalizedBufferToTile((ossim_uint32)0, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyNormalizedBufferToTile((ossim_sint32)0, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::copyNormalizedBufferToTile\n"
            << "Unknown scalar type!" << std::endl;
      }
      
   } // End of "switch(getScalarType())".
}

void ossimImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                ossim_float64* buf)
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }

   if(band < getNumberOfBands())
   {
      switch(getScalarType())
      {
         case OSSIM_NORMALIZED_DOUBLE:
         {
            memmove(getBuf(band), buf, getSizePerBandInBytes());
            break;
         }
         case OSSIM_NORMALIZED_FLOAT:
         {
            ossim_uint32 upperBound = getWidth()*getHeight();
            ossim_float32* inputBuf = static_cast<ossim_float32*>(getBuf(band));
            for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
            {
               inputBuf[offset] = static_cast<ossim_float32>(buf[offset]);
            }
            break;
         }
         case OSSIM_FLOAT32:
         {
            copyNormalizedBufferToTile((ossim_float32)0, band, buf);
            break;
         }
         case OSSIM_FLOAT64:
         {
            copyNormalizedBufferToTile((ossim_float64)0, band, buf);
            break;
         }
         case OSSIM_UINT8:
         {
            copyNormalizedBufferToTile((ossim_uint8)0, band, buf);
            break;
         
         }
         case OSSIM_SINT8:
         {
            copyNormalizedBufferToTile((ossim_sint8)0, band, buf);
            break;
         
         }
         case OSSIM_USHORT11:
         case OSSIM_UINT16:
         {
            copyNormalizedBufferToTile((ossim_uint16)0, band, buf);
            break;
         }
         case OSSIM_SINT16:
         {
            copyNormalizedBufferToTile((ossim_sint16)0, band, buf);
            break;
         }
         case OSSIM_UINT32:
         {
            copyNormalizedBufferToTile((ossim_uint32)0, band, buf);
            break;
         }
         case OSSIM_SINT32:
         {
            copyNormalizedBufferToTile((ossim_sint32)0, band, buf);
            break;
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimImageData::copyNormalizedBufferToTile\n"
               << "Unknown scalar type." << std::endl;
         }
         
      } // End of "switch(getScalarType())".
   }
}

void ossimImageData::copyNormalizedBufferToTile(ossim_float32* buf)
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }

   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_uint32 upperBound = getWidth()*getHeight()*getNumberOfBands();
         ossim_float64* inputBuf = static_cast<ossim_float64*>(getBuf());
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            inputBuf[offset] = buf[offset];
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         memmove(getBuf(), buf, getDataSizeInBytes());
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyNormalizedBufferToTile((ossim_float32)0, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyNormalizedBufferToTile((ossim_float64)0, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyNormalizedBufferToTile((ossim_uint8)0, buf);
         break;
         
      }
      case OSSIM_SINT8:
      {
         copyNormalizedBufferToTile((ossim_sint8)0, buf);
         break;
         
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyNormalizedBufferToTile((ossim_uint16)0, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyNormalizedBufferToTile((ossim_sint16)0, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyNormalizedBufferToTile((ossim_uint32)0, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyNormalizedBufferToTile((ossim_sint32)0, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageDatacopyNormalizedBufferToTile\n"
            << "Unknown scalar type!" << std::endl;
      }
      
   }  // End of "switch(getScalarType())".
}

void ossimImageData::copyNormalizedBufferToTile(ossim_uint32 band,
                                                ossim_float32* buf)
{
   if (!buf)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nNull buffer passed to method!  Returning..." << std::endl;
      return;
   }

   if (getDataObjectStatus() == OSSIM_NULL)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::copyNormalizedBufferToTile ERROR:"
         << "\nThis object is null! Returning..." << std::endl;
      return;
   }

   switch(getScalarType())
   {
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_uint32 upperBound = getWidth()*getHeight();
         ossim_float64* inputBuf = static_cast<ossim_float64*>(getBuf(band));
         for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            inputBuf[offset] = buf[offset];
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         memmove(getBuf(band), buf, getSizePerBandInBytes());
         break;
      }
      case OSSIM_FLOAT32:
      {
         copyNormalizedBufferToTile((ossim_float32)0, band, buf);
         break;
      }
      case OSSIM_FLOAT64:
      {
         copyNormalizedBufferToTile((ossim_float64)0, band, buf);
         break;
      }
      case OSSIM_UINT8:
      {
         copyNormalizedBufferToTile((ossim_uint8)0, band, buf);
         break;
         
      }
      case OSSIM_SINT8:
      {
         copyNormalizedBufferToTile((ossim_sint8)0, band, buf);
         break;
         
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyNormalizedBufferToTile((ossim_uint16)0, band, buf);
         break;
      }
      case OSSIM_SINT16:
      {
         copyNormalizedBufferToTile((ossim_sint16)0, band, buf);
         break;
      }
      case OSSIM_UINT32:
      {
         copyNormalizedBufferToTile((ossim_uint32)0, band, buf);
         break;
      }
      case OSSIM_SINT32:
      {
         copyNormalizedBufferToTile((ossim_sint32)0, band, buf);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::copyNormalizedBufferToTile\n"
            << "Unknown scalar type!" << std::endl;
      }

   }  // End of "switch(getScalarType())".
}

ossim_float64 ossimImageData::getMinNormalizedPix() const
{
   if (m_scalarType == OSSIM_NORMALIZED_DOUBLE ||
       m_scalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return m_minPixelValue[0];  // Already normalized.
   }
   return (1.0 / (m_maxPixelValue[0] - m_minPixelValue[0] + 1.0));
}

std::ostream& ossimImageData::print(std::ostream& out) const
{
   out << getClassName() << "::print:"
       << "\nm_origin:  " << m_origin << "\n";
   if(m_nullPixelValue.size())
   {
      out << "Null values: ";
      
      copy(m_nullPixelValue.begin(),
           m_nullPixelValue.begin() + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   if(m_minPixelValue.size())
   {
      out << "Min values:  ";
      
      copy(m_minPixelValue.begin(),
           m_minPixelValue.begin() + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   if(m_maxPixelValue.size())
   {
      out << "Max values:  ";
      
      copy(m_maxPixelValue.begin(),
           m_maxPixelValue.begin() + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   out << "width:            " << getWidth()
       << "\nheight:           " << getHeight()
       << "\nimage rectangle:  " << getImageRectangle()
       << "\nindexed:          " << m_indexedFlag
       << std::endl;

   return ossimRectilinearDataObject::print(out);
}

void ossimImageData::stretchMinMax()
{
   if ( (getDataObjectStatus() != OSSIM_NULL) &&
        (getDataObjectStatus() != OSSIM_EMPTY) )
   {
      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            stretchMinMax(ossim_uint8(0));
            return;
         }  
         case OSSIM_SINT8:
         {
            stretchMinMax(ossim_sint8(0));
            return;
         }  
         case OSSIM_UINT16:
         case OSSIM_USHORT11:
         {
            stretchMinMax(ossim_uint16(0));
            return;
         }  
         case OSSIM_SINT16:
         {
            stretchMinMax(ossim_sint16(0));
            return;
         }  
         case OSSIM_UINT32:
         {
            stretchMinMax(ossim_uint32(0));
            return;
         }
         case OSSIM_SINT32:
         {
            stretchMinMax(ossim_sint32(0));
            return;
         }  
         case OSSIM_FLOAT32:
         case OSSIM_NORMALIZED_FLOAT:
         {
            stretchMinMax(ossim_float32(0.0));
            return;
         }  
         case OSSIM_NORMALIZED_DOUBLE:
         case OSSIM_FLOAT64:
         {
            stretchMinMax(ossim_float64(0.0));
            return;
         }  
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_ERROR,
                          "ossimImageData::stretchMinMax File %s line %d\n\
Invalid scalar type:  %d",
                          __FILE__,
                          __LINE__,
                          getScalarType());
            break;
         }
      }
   }
}

template <class T> void ossimImageData::stretchMinMax(T /* dummyTemplate */)
{
   const ossim_uint32 BANDS  = getNumberOfBands();
   const ossim_uint32 SPB    = getSizePerBand();

   // scalar min
   const ossim_float64 S_MIN = ossim::defaultMin(getScalarType());

   // scalar max
   const ossim_float64 S_MAX = ossim::defaultMax(getScalarType());

   // scalar null
   const ossim_float64 S_NUL = ossim::defaultNull(getScalarType());

   // scalar range
   const ossim_float64 S_RNG = S_MAX-S_MIN+1.0;

   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      T* s = static_cast<T*>(getBuf(band));

      if (s)
      {
         const ossim_float64 T_NUL = m_nullPixelValue[band]; // tile null
         const ossim_float64 T_MIN = m_minPixelValue[band];  // tile min
         const ossim_float64 T_MAX = m_maxPixelValue[band];  // tile max
         const ossim_float64 T_RNG = (T_MAX-T_MIN)+1;           // tile range
         const ossim_float64 SPP = S_RNG / T_RNG; // stretch per pixel
         
         for(ossim_uint32 i = 0; i < SPB; ++i)
         {
            ossim_float64 p = s[i];
            if ( p == T_NUL )
            {
               p = S_NUL;
            }
            else if (p <= T_MIN)
            {
               p = S_MIN;
            }
            else if (p >= T_MAX)
            {
               p = S_MAX;
            }
            else
            {
               // Stretch...
               p = (p - T_MIN + 1.0) * SPP + S_MIN - 1.0;
            }
            s[i] = ossim::round<T>(p);
         }

         // Set the min, max, null:
         m_minPixelValue[band]  = S_MIN;
         m_maxPixelValue[band]  = S_MAX;
         m_nullPixelValue[band] = S_NUL;
         
      }
   } 
}

void ossimImageData::computeAlphaChannel()
{
   if ( getDataObjectStatus() != OSSIM_NULL )
   {
      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            computeAlphaChannel(ossim_uint8(0));
            return;
         }  
         case OSSIM_SINT8:
         {
            computeAlphaChannel(ossim_sint8(0));
            return;
         }  
         case OSSIM_UINT16:
         case OSSIM_USHORT11:
         {
            computeAlphaChannel(ossim_uint16(0));
            return;
         }  
         case OSSIM_SINT16:
         {
            computeAlphaChannel(ossim_sint16(0));
            return;
         }  
         case OSSIM_UINT32:
         {
            computeAlphaChannel(ossim_uint32(0));
            return;
         }
         case OSSIM_SINT32:
         {
            computeAlphaChannel(ossim_sint32(0));
            return;
         }  
         case OSSIM_FLOAT32:
         case OSSIM_NORMALIZED_FLOAT:
         {
            computeAlphaChannel(ossim_float32(0.0));
            return;
         }  
         case OSSIM_NORMALIZED_DOUBLE:
         case OSSIM_FLOAT64:
         {
            computeAlphaChannel(ossim_float64(0.0));
            return;
         }  
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            setDataObjectStatus(OSSIM_STATUS_UNKNOWN);
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_ERROR,
                          "ossimImageData::computeAlphaChannel File %s line %d\n\
Invalid scalar type:  %d",
                          __FILE__,
                          __LINE__,
                          getScalarType());
            break;
         }
      }
   }
}

template <class T> void ossimImageData::computeAlphaChannel(T /* dummyTemplate */)
{
   const ossim_uint32 SPB = getSizePerBand();
   const ossim_uint8  ANP = 0;   // Alpha Null Pixel
   const ossim_uint8  AVP = 255; // Alpha Valid Pixel

   // Allocate the alpha channel if not already.
   if (m_alpha.size() != SPB)
   {
      m_alpha.resize(SPB);
   }

   if (getDataObjectStatus() == OSSIM_FULL)
   {
      memset( static_cast<void*>(&m_alpha.front()),
              static_cast<int>(AVP),
              static_cast<int>(SPB) );
   }
   else if (getDataObjectStatus() == OSSIM_EMPTY)
   {
      // Start with alpha being set to 0.  
      memset( static_cast<void*>(&m_alpha.front()),
              static_cast<int>(ANP),
              static_cast<int>(SPB) );
   }
   else // Partial must check each pixel.
   {
      const ossim_uint32 BANDS = getNumberOfBands();

      std::vector<T> null_pix(BANDS);
      std::vector<const T*> buf(BANDS); // Pointer to pixel data.
      ossim_uint32 band;
      for(band = 0; band < BANDS; ++band)
      {
         buf[band] = static_cast<const T*>(getBuf(band));
         null_pix[band] = static_cast<T>(m_nullPixelValue[band]);
      }
   
      for(ossim_uint32 i = 0; i < SPB; ++i)
      {
         //---
         // Start with alpha being set to 0.  If any band has a valid pixel set
         // to 255 and break from band loop.
         //---
         m_alpha[i] = ANP;
         for(band = 0; band < BANDS; ++band)
         {
            if (buf[band][i] != null_pix[band])
            {
               m_alpha[i] = AVP;
               break;
            }
         }
      }
   }
   
}  //  End: template <class T> void ossimImageData::computeAlphaChannel

ossim_uint32 ossimImageData::getWidth()const
{
   return m_spatialExtents[0];
}

ossim_uint32 ossimImageData::getHeight()const
{
   return m_spatialExtents[1];
}

void ossimImageData::getWidthHeight(ossim_uint32& w, ossim_uint32& h)
{
   w = m_spatialExtents[0];
   h = m_spatialExtents[1];
}

void ossimImageData::setWidth(ossim_uint32 width)
{
   m_spatialExtents[0] = width;
}

void ossimImageData::setHeight(ossim_uint32 height)
{
   m_spatialExtents[1] = height;
}

void ossimImageData::setWidthHeight(ossim_uint32 w, ossim_uint32 h)
{
   m_spatialExtents[0] = w;
   m_spatialExtents[1] = h;
}

void ossimImageData::setOrigin(const ossimIpt& origin)
{
   m_origin = origin;
}

ossim_uint32 ossimImageData::getDataSizeInBytes()const
{
   return getSizeInBytes();
}

void ossimImageData::copyLine(const void* src,
                              ossim_int32 lineNumber,
                              ossim_int32 lineStartSample,
                              ossim_int32 lineStopSample,
                              ossimInterleaveType lineInterleave)
{
   switch(m_scalarType)
   {
      case OSSIM_UINT8:
      {
         copyLineTemplate((ossim_uint8)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_SINT8:
      {
         copyLineTemplate((ossim_sint8)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
         
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         copyLineTemplate((ossim_uint16)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_SINT16:
      {
         copyLineTemplate((ossim_sint16)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_UINT32:
      {
         copyLineTemplate((ossim_uint32)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_SINT32:
      {
         copyLineTemplate((ossim_sint32)0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         copyLineTemplate((ossim_float32)0.0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
      {
         copyLineTemplate((ossim_float64)0.0, src, lineNumber, lineStartSample,
                          lineStopSample, lineInterleave);
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::copyLine Unsupported scalar type!"
            << std::endl;
         break;
      }
   }
   
} // End: void ossimImageData::copyLine

template <class T>
void ossimImageData::copyLineTemplate(T /* dummyTemplate */,
                                      const void* src,
                                      ossim_int32 lineNumber,
                                      ossim_int32 lineStartSample,
                                      ossim_int32 lineStopSample,
                                      ossimInterleaveType lineInterleave)
{
   if (src)
   {
      const ossimIrect RECT = getImageRectangle();

      // Check for intersect:
      if ( ( lineNumber       >= RECT.ul().y)     &&
           ( lineNumber       <= RECT.lr().y)     &&
           ( lineStartSample  <  lineStopSample)  &&
           ( lineStartSample  <= RECT.lr().x)     &&
           ( lineStopSample   >= RECT.ul().x) )
      {
         const ossim_int32 BANDS = static_cast<ossim_int32>(m_numberOfDataComponents);
         const ossim_int32 START_SAMP =
            (lineStartSample > RECT.ul().x)?lineStartSample:RECT.ul().x;
         const ossim_int32 STOP_SAMP  =
            (lineStopSample  < RECT.lr().x)?lineStopSample:RECT.lr().x;
         const ossim_int32 SAMPS = STOP_SAMP - START_SAMP + 1;

         std::vector<T*> d(BANDS);

         ossim_int32 band;
         for (band = 0; band < BANDS; ++band)
         {
            d[band] = static_cast<T*>(getBuf(band));

            // Position at start sample.
            d[band] +=  (lineNumber - RECT.ul().y) * RECT.width() + (START_SAMP - RECT.ul().x);
         }
         
         if (lineInterleave == OSSIM_BIP)
         {
            const T* S = static_cast<const T*>(src); // Source buffer:
            
            // Position at start sample.
            S += (START_SAMP - lineStartSample) * BANDS;
            
            ossim_int32 srcOffset = 0;
            for (ossim_int32 samp = 0; samp < SAMPS; ++samp)
            {
               for (band = 0; band < BANDS; ++band)
               {
                  d[band][samp] = S[srcOffset++];
               }
            }
         }
         else
         {
            const ossim_int32 W = lineStopSample - lineStartSample + 1;
            std::vector<const T*> S(BANDS);
            for (band = 0; band < BANDS; ++band)
            {
               S[band] = static_cast<const T*>(src) + (START_SAMP - lineStartSample);
               if (band)
               {
                  S[band] += band * W; // Move to line.
               }
            }

            for (band = 0; band < BANDS; ++band)
            {
               for (ossim_int32 samp = 0; samp < SAMPS; ++samp)
               {
                  d[band][samp] = S[band][samp];
               }
            }
         }
         
      } // intersect check
      
   } // if (src)
   
} // End: template <class T> void ossimImageData::copyLineTemplate

void ossimImageData::setIndexedFlag(bool flag)
{
   m_indexedFlag = flag;
}

bool ossimImageData::getIndexedFlag() const
{
   return m_indexedFlag;
}

bool ossimImageData::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   bool result = ossimRectilinearDataObject::saveState(kwl, prefix);
   ossimString null_pixels;
   ossimString min_pixels;
   ossimString max_pixels;
   ossim::toSimpleStringList(null_pixels, m_nullPixelValue);
   ossim::toSimpleStringList(min_pixels, m_minPixelValue);
   ossim::toSimpleStringList(max_pixels, m_maxPixelValue);
   
   kwl.add(prefix, "null_pixels", null_pixels, true);
   kwl.add(prefix, "min_pixels", min_pixels, true);
   kwl.add(prefix, "max_pixels", max_pixels, true);
   
   ossimString alpha;
   ossim::toSimpleStringList(alpha, m_alpha);
   kwl.add(prefix, "alpha", alpha, true);
   kwl.add(prefix, "indexed", m_indexedFlag, true);
   kwl.add(prefix, "origin", m_origin.toString(), true);
   return result;
   
}

bool ossimImageData::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = ossimRectilinearDataObject::loadState(kwl, prefix);
   m_spatialExtents.resize(2);
   if(result)
   {
      const char* null_pixels = kwl.find(prefix, "null_pixels");
      const char* min_pixels = kwl.find(prefix, "min_pixels");
      const char* max_pixels = kwl.find(prefix, "max_pixels");
      const char* alpha = kwl.find(prefix, "alpha");
      const char* origin = kwl.find(prefix, "origin");
      const char* indexed = kwl.find(prefix, "indexed");
      ossimString rectString = kwl.find(prefix, "rect");
      const char* numberOfBands = kwl.find(prefix, "number_bands");
      m_nullPixelValue.clear();
      m_minPixelValue.clear();
      m_maxPixelValue.clear();
      m_alpha.clear();
      if(null_pixels)
      {
         if(!ossim::toSimpleVector(m_nullPixelValue, ossimString(null_pixels)))
         {
            return false;
         }
      }
      if(min_pixels)
      {
         if(!ossim::toSimpleVector(m_minPixelValue, ossimString(min_pixels)))
         {
            return false;
         }
      }
      if(max_pixels)
      {
         if(!ossim::toSimpleVector(m_maxPixelValue, ossimString(max_pixels)))
         {
            return false;
         }
      }
      if(alpha)
      {
         if(!ossim::toSimpleVector(m_alpha, ossimString(alpha)))
         {
            return false;
         }
      }
      if(origin)
      {
         m_origin.toPoint(origin);
      }
      m_indexedFlag = false;
      if(indexed)
      {
         m_indexedFlag = ossimString(indexed).toBool();
      }
      if(!rectString.empty())
      {
        ossimIrect rect;

        if(rect.toRect(rectString))
        {
          setImageRectangle(rect);
        }
      }
      if(numberOfBands)
      {
        ossim_uint32 nBands = ossimString(numberOfBands).toUInt32();
        setNumberOfDataComponents(nBands);
        if(m_nullPixelValue.empty()||
          m_minPixelValue.empty()||
          m_maxPixelValue.empty())
          {
            initializeDefaults();
          }
      }
   }
   
   return result;
}                     

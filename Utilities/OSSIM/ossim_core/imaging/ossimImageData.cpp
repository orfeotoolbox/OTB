//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageData.cpp,v 1.149 2006/01/04 22:31:15 dburken Exp $

#include <iterator>

#include <imaging/ossimImageData.h>
#include <base/common/ossimSource.h>
#include <base/context/ossimErrorContext.h>
#include <base/data_types/ossimIrect.h>
#include <base/data_types/ossimMultiBandHistogram.h>
#include <base/data_types/ossimHistogram.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>

RTTI_DEF1(ossimImageData, "ossimImageData", ossimRectilinearDataObject)

ossimImageData::ossimImageData()
   : ossimRectilinearDataObject(2,            // 2d
                                NULL,         // owner
                                1,            // bands
                                OSSIM_UINT8), // scalar
      theNullPixelValue(NULL),
      theMinPixelValue(NULL),
      theMaxPixelValue(NULL),
      theOrigin(0, 0)
{
   ossimIpt defaultTileSize;
   ossimGetDefaultTileSize(defaultTileSize);
   theSpatialExtents[0] = defaultTileSize.x;
   theSpatialExtents[1] = defaultTileSize.y;
}

ossimImageData::ossimImageData(ossimSource*    owner,
                               ossimScalarType scalar,
                               ossim_uint32    bands)
   : ossimRectilinearDataObject(2,
                                owner,
                                bands,
                                scalar),
     theNullPixelValue(NULL),
     theMinPixelValue(NULL),
     theMaxPixelValue(NULL),
     theOrigin(0, 0)
{
   ossimIpt defaultTileSize;
   ossimGetDefaultTileSize(defaultTileSize);
   theSpatialExtents[0] = defaultTileSize.x;
   theSpatialExtents[1] = defaultTileSize.y;
   initializeDefaults();
}

ossimImageData::ossimImageData(ossimSource* owner,
                               ossimScalarType scalar,
                               ossim_uint32 bands,
                               ossim_uint32 width,
                               ossim_uint32 height)
   :ossimRectilinearDataObject(owner,
                               bands,
                               width, // 2-D array
                               height,
                               scalar),
      theNullPixelValue(NULL),
      theMinPixelValue(NULL),
      theMaxPixelValue(NULL),
      theOrigin(0, 0)
{   
   theSpatialExtents[0] = width;
   theSpatialExtents[1] = height;
   initializeDefaults();
}

ossimImageData::ossimImageData(const ossimImageData &rhs)
   :ossimRectilinearDataObject(rhs),
   theNullPixelValue(NULL),
   theMinPixelValue(NULL),
   theMaxPixelValue(NULL),
   theOrigin(rhs.theOrigin)
{   
   if(rhs.theNullPixelValue)
   {
      theNullPixelValue = new ossim_float64[getNumberOfBands()];
      
      memcpy((void*)theNullPixelValue,
             (const void*)rhs.theNullPixelValue,
             (sizeof(ossim_float64)*(int)getNumberOfBands()));
   }
   if(rhs.theMinPixelValue)
   {
      theMinPixelValue = new ossim_float64[getNumberOfBands()];
      
      memcpy((void*)theMinPixelValue,
             (const void*)rhs.theMinPixelValue,
             (sizeof(ossim_float64)*(int)getNumberOfBands()));
   }
   if(rhs.theMaxPixelValue)
   {
      theMaxPixelValue = new ossim_float64[getNumberOfBands()];
      
      memcpy((void*)theMaxPixelValue,
             (const void*)rhs.theMaxPixelValue,
             (sizeof(ossim_float64)*(int)getNumberOfBands()));
   }
}

ossimImageData::~ossimImageData()
{
   if(theMinPixelValue)
   {
      delete [] theMinPixelValue;
      theMinPixelValue = NULL;
   }
   if(theMaxPixelValue)
   {
      delete [] theMaxPixelValue;
      theMaxPixelValue = NULL;
   }
   if(theNullPixelValue)
   {
      delete [] theNullPixelValue;
      theNullPixelValue = NULL;
   }
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

const void* ossimImageData::getBuf() const
{
   return static_cast<const void*>(theDataBuffer);
}

void* ossimImageData::getBuf()
{
   return static_cast<void*>(theDataBuffer);
}

const void* ossimImageData::getBuf(ossim_uint32 band) const
{
   const ossim_uint8* b = theDataBuffer;
   
   if (isValidBand(band) && b != NULL)
   {
      b += (band * getSizePerBandInBytes());
      return static_cast<const void*>(b);
   }
   return NULL;
}

void* ossimImageData::getBuf(ossim_uint32 band)
{
   ossim_uint8* b = theDataBuffer;
   
   if (isValidBand(band) && b != NULL)
   {
      b += (band * getSizePerBandInBytes());
      return static_cast<void*>(b);
   }
   return NULL;
}

const ossim_uint8* ossimImageData::getUcharBuf() const
{
   if (theScalarType == OSSIM_UINT8)
   {
      return static_cast<const ossim_uint8*>(theDataBuffer);
   }
   return NULL;
}

const ossim_uint16* ossimImageData::getUshortBuf() const
{
   if (theScalarType == OSSIM_UINT16 ||
       theScalarType == OSSIM_USHORT11)
   {
      return static_cast<const ossim_uint16*>(getBuf());
   }
   return NULL;
}

const ossim_sint16* ossimImageData::getSshortBuf() const
{
   if (theScalarType == OSSIM_SINT16)
   {
      return static_cast<const ossim_sint16*>(getBuf());
   }
   return NULL;
}

const ossim_float32* ossimImageData::getFloatBuf() const
{
   if (theScalarType == OSSIM_FLOAT32 ||
       theScalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<const ossim_float32*>(getBuf());
   }
   return NULL;
}

const ossim_float64* ossimImageData::getDoubleBuf() const
{
   if (theScalarType == OSSIM_FLOAT64 ||
       theScalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<const ossim_float64*>(getBuf());
   }
   return NULL;
}

ossim_uint8* ossimImageData::getUcharBuf() 
{
   if (theScalarType == OSSIM_UINT8)
   {
      return theDataBuffer;
   }
   return NULL;
}

ossim_uint16* ossimImageData::getUshortBuf() 
{
   if (theScalarType == OSSIM_UINT16 ||
       theScalarType == OSSIM_USHORT11)
   {
      return static_cast<ossim_uint16*>(getBuf());
   }
   return NULL;
}

ossim_sint16* ossimImageData::getSshortBuf() 
{
   if (theScalarType == OSSIM_SINT16)
   {
      return static_cast<ossim_sint16*>(getBuf());
   }
   return NULL;
}

ossim_float32* ossimImageData::getFloatBuf() 
{
   if (theScalarType == OSSIM_FLOAT32 ||
       theScalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<ossim_float32*>(getBuf());
   }
   return NULL;
}

ossim_float64* ossimImageData::getDoubleBuf() 
{
   if (theScalarType == OSSIM_FLOAT64 ||
       theScalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<ossim_float64*>(getBuf());
   }
   return NULL;
}

const ossim_uint8* ossimImageData::getUcharBuf(ossim_uint32 band) const
{
   if (theScalarType == OSSIM_UINT8)
   {
      return static_cast<const ossim_uint8*>(getBuf(band));
   }
   return NULL;
}

const ossim_uint16* ossimImageData::getUshortBuf(ossim_uint32 band) const
{
   if (theScalarType == OSSIM_UINT16 ||
       theScalarType == OSSIM_USHORT11)
   {
      return static_cast<const ossim_uint16*>(getBuf(band));
   }
   return NULL;
}

const ossim_sint16* ossimImageData::getSshortBuf(ossim_uint32 band) const
{
   if (theScalarType == OSSIM_SINT16)
   {
      return static_cast<const ossim_sint16*>(getBuf(band));
   }
   return NULL;
}

const ossim_float32* ossimImageData::getFloatBuf(ossim_uint32 band) const
{
   if (theScalarType == OSSIM_FLOAT32 ||
       theScalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<const ossim_float32*>(getBuf(band));
   }
   return NULL;
}

const ossim_float64* ossimImageData::getDoubleBuf(ossim_uint32 band) const
{
   if (theScalarType == OSSIM_FLOAT64 ||
       theScalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<const ossim_float64*>(getBuf(band));
   }
   return NULL;
}

ossim_uint8* ossimImageData::getUcharBuf(ossim_uint32 band) 
{
   if (theScalarType == OSSIM_UINT8)
   {
      return static_cast<ossim_uint8*>(getBuf(band));
   }
   return NULL;
}

ossim_uint16* ossimImageData::getUshortBuf(ossim_uint32 band) 
{
   if (theScalarType == OSSIM_UINT16 ||
       theScalarType == OSSIM_USHORT11)
   {
      return static_cast<ossim_uint16*>(getBuf(band));
   }
   return NULL;
}

ossim_sint16* ossimImageData::getSshortBuf(ossim_uint32 band) 
{
   if (theScalarType == OSSIM_SINT16)
   {
      return static_cast<ossim_sint16*>(getBuf(band));
   }
   return NULL;
}

ossim_float32* ossimImageData::getFloatBuf(ossim_uint32 band) 
{
   if (theScalarType == OSSIM_FLOAT32 ||
       theScalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return static_cast<ossim_float32*>(getBuf(band));
   }
   return NULL;
}

ossim_float64* ossimImageData::getDoubleBuf(ossim_uint32 band) 
{
   if (theScalarType == OSSIM_FLOAT64 ||
       theScalarType == OSSIM_NORMALIZED_DOUBLE)
   {
      return static_cast<ossim_float64*>(getBuf(band));
   }
   return NULL;
}

void ossimImageData::getNormalizedFloat(ossim_uint32 offset,
                                        ossim_uint32 bandNumber,
                                        ossim_float32& result)const
{
   if(!result)
   {
      return;
   }
   
   // Make sure that the types and width and height are good.
   if( (getDataObjectStatus() == OSSIM_NULL) &&
       (bandNumber < getNumberOfDataComponents()) )
   {
      return;
   }
   
   ossim_float32 delta =  theMaxPixelValue[bandNumber] - theMinPixelValue[bandNumber];
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         const unsigned char* sourceBuf = getUcharBuf(bandNumber);
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_SINT8:
      {
         const ossim_sint8* sourceBuf = static_cast<const ossim_sint8*>(getBuf(bandNumber));
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         const ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_SINT16:
      {
         const ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_UINT32:
      {
         const ossim_uint32* sourceBuf =
            static_cast<const ossim_uint32*>(getBuf(bandNumber));
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_SINT32:
      {
         const ossim_sint32* sourceBuf = static_cast<const ossim_sint32*>(getBuf(bandNumber));
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         const ossim_float32* sourceBuf = getFloatBuf(bandNumber);
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         const ossim_float64* sourceBuf = getDoubleBuf(bandNumber);
         result = (sourceBuf[offset] - theMinPixelValue[bandNumber])/delta;
         break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         // Shouldn't hit this.
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::setNormalizedFloat Unsupported scalar type!"
            << std::endl;
         
         result = 0;
      }
   }
}

void ossimImageData::setNormalizedFloat(ossim_uint32 offset,
                                        ossim_uint32 bandNumber,
                                        ossim_float32 inputValue)
{
   // Make sure that the types and width and height are good.
   if( (getDataObjectStatus() == OSSIM_NULL)&&
       (bandNumber < getNumberOfDataComponents()) )
   {
      return;
   }
   
   ossim_float32 delta =  theMaxPixelValue[bandNumber] - theMinPixelValue[bandNumber];
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         unsigned char* sourceBuf = getUcharBuf(bandNumber);
         sourceBuf[offset] =
            static_cast<ossim_uint8>(theMinPixelValue[bandNumber] +
                                     delta*inputValue);
         
         break;
      }
      case OSSIM_SINT8:
      {
         ossim_sint8* sourceBuf = static_cast<ossim_sint8*>(getBuf(bandNumber));
         sourceBuf[offset] =
            static_cast<ossim_sint8>(theMinPixelValue[bandNumber] +
                                     delta*inputValue);
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         ossim_uint16* sourceBuf = getUshortBuf(bandNumber);
         sourceBuf[offset] =
            static_cast<ossim_uint16>(theMinPixelValue[bandNumber] +
                                      delta*inputValue);
         break;
      }
      case OSSIM_SINT16:
      {
         ossim_sint16* sourceBuf = getSshortBuf(bandNumber);
         sourceBuf[offset] =
            static_cast<ossim_sint16>(theMinPixelValue[bandNumber] +
                                      delta*inputValue);
         break;
      }
      case OSSIM_UINT32:
      {
         ossim_uint32* sourceBuf =
            static_cast<ossim_uint32*>(getBuf(bandNumber));
         sourceBuf[offset] =
            static_cast<ossim_uint32>(theMinPixelValue[bandNumber] +
                                      delta*inputValue);
         break;
      }
      case OSSIM_SINT32:
      {
         ossim_sint32* sourceBuf = static_cast<ossim_sint32*>(getBuf(bandNumber));
         sourceBuf[offset] =
            static_cast<ossim_sint32>(theMinPixelValue[bandNumber] +
                                      delta*inputValue);
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
      {
         ossim_float32* sourceBuf = getFloatBuf(bandNumber);
         sourceBuf[offset] =
            static_cast<ossim_float32>(theMinPixelValue[bandNumber] +
                                       delta*inputValue);
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         ossim_float64* sourceBuf = getDoubleBuf(bandNumber);
         sourceBuf[offset] =
            static_cast<ossim_float64>(theMinPixelValue[bandNumber]
                                       + delta*inputValue);
      break;
   }
   case OSSIM_SCALAR_UNKNOWN:
   default:
      // Shouldn't hit this.
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimImageData::setNormalizedFloat Unsupported scalar type!"
         << std::endl;
   }
}

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
      new ossimImageData(NULL,
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
   ossimImageData* result = new ossimImageData(NULL,
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
void ossimImageData::populateHistogram(ossimMultiBandHistogram* histo)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_uint8* buffer = (ossim_uint8*)getBuf(band);
         
            if(currentHisto)
            {
               ossimHistogram* currentHisto = histo->getHistogram(band);
               
               if(currentHisto)
               {
                  ossim_uint32 upperBound = getWidth()*getHeight();
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
	 
             ossim_uint16* buffer = (ossim_uint16*)getBuf(band);
             if(currentHisto)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_sint16* buffer = (ossim_sint16*)getBuf(band);
            
            if(currentHisto)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_sint32* buffer = (ossim_sint32*)getBuf(band);
            
            if(currentHisto)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_uint32* buffer = (ossim_uint32*)getBuf(band);
            
            if(currentHisto)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_float64* buffer = (ossim_float64*)getBuf(band);
            
            if(currentHisto)
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
            ossimHistogram* currentHisto = histo->getHistogram(band);
            ossim_float32* buffer = (ossim_float32*)getBuf(band);
            
            if(currentHisto)
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
   if (theDataBuffer == NULL)
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
      const T NP = static_cast<T>(theNullPixelValue[band]);
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
   if ( (theDataBuffer == NULL) || (getDataObjectStatus() == OSSIM_EMPTY) )
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
   if ( (theDataBuffer == NULL) || (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return; // nothing to do...
   }
   
   const ossim_uint32 BANDS = getNumberOfBands();
   const ossim_uint32 SPB   = getSizePerBand();
   
   for(ossim_uint32 band = 0; band < BANDS; ++band)
   {
      const T NP = static_cast<T>(theNullPixelValue[band]);
      T* p = static_cast<T*>(getBuf(band));
      for (ossim_uint32 i = 0; i < SPB; ++i)
      {
         p[i] = NP;
      }
   }
   
   setDataObjectStatus(OSSIM_EMPTY);
}

void ossimImageData::initialize()
{
   // let the base class allocate a buffer
   ossimRectilinearDataObject::initialize();
   
   if(!theMinPixelValue)
   {
      initializeDefaults();
   }
   
   if (theDataBuffer)
   {
      makeBlank();  // Make blank will set the status.
   }
}

bool ossimImageData::isWithin(ossim_int32 x, ossim_int32 y)
{
   return ((x >= theOrigin.x) &&
           (x <  theOrigin.x + static_cast<ossim_int32>(theSpatialExtents[0])) &&
           (y >= theOrigin.y) &&
           (y <  theOrigin.y + static_cast<ossim_int32>(theSpatialExtents[1])));
           
}

void ossimImageData::setValue(ossim_int32 x, ossim_int32 y, ossim_float64 color)
{
   if(theDataBuffer&&isWithin(x, y))
   {
      ossim_uint32 band=0;

      //***
      // Compute the offset into the buffer for (x,y).  This should always
      // come out positive.
      //***
      ossim_uint32 ux = static_cast<ossim_uint32>(x - theOrigin.x);
      ossim_uint32 uy = static_cast<ossim_uint32>(y - theOrigin.y);
      
      ossim_uint32 offset = uy * theSpatialExtents[0] + ux;
      
      switch (getScalarType())
      {
         case OSSIM_UINT8:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
            {
               unsigned char* buf = static_cast<unsigned char*>(getBuf(band))+
                  offset;
               *buf = (unsigned char)color;
            }
            break;
         }
         case OSSIM_SINT8:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
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
            for(band = 0; band < theNumberOfDataComponents; band++)
            {
               ossim_uint16* buf = static_cast<ossim_uint16*>(getBuf(band))+
                  offset;
               *buf = (ossim_uint16)color;
            }
            break;
         }
         case OSSIM_SINT16:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
            {
               signed short* buf = static_cast<signed short*>(getBuf(band))+
                  offset;
               *buf = (signed short)color;
            }
            break;
         }
         case OSSIM_UINT32:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
            {
               ossim_uint32* buf = static_cast<ossim_uint32*>(getBuf(band))+
                  offset;
               *buf = (ossim_uint32)color;
            }
            break;
         }
         case OSSIM_SINT32:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
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
            for(band = 0; band < theNumberOfDataComponents; band++)
            {
               ossim_float32* buf = static_cast<ossim_float32*>(getBuf(band))+offset;
               *buf = (ossim_float32)color;
            }
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            for(band = 0; band < theNumberOfDataComponents; band++)
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
   ossim_uint32 numberOfBands = getNumberOfBands();
   if(numberOfBands < 1)
   {
      return;
   }
   if(theMinPixelValue)
   {
      delete [] theMinPixelValue;
      theMinPixelValue = NULL;
   }
   if(theMaxPixelValue)
   {
      delete [] theMaxPixelValue;
      theMaxPixelValue = NULL;
   }
   if(theNullPixelValue)
   {
      delete [] theNullPixelValue;
      theNullPixelValue = NULL;
   }
   theMinPixelValue  = new ossim_float64[numberOfBands];
   theMaxPixelValue  = new ossim_float64[numberOfBands];
   theNullPixelValue = new ossim_float64[numberOfBands];
   ossim_uint32 band;
   switch (getScalarType())
   {
      case OSSIM_UINT8:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = 0.0;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_UCHAR;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_UCHAR;
         }
         break;
      }
      case OSSIM_SINT8:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_DEFAULT_MIN_PIX_SINT8-1;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_SINT8;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_SINT8;
         }
         break;
      }
      case OSSIM_UINT16:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_USHORT_NAN;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_UINT16;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_UINT16;
         }
         break;
      }
      case OSSIM_USHORT11:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_USHORT_NAN;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_UINT11;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_UINT11;
         }
         break;
      }
      case OSSIM_SINT16:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_SSHORT_NAN;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_SINT16;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_SINT16;
         }
         break;
      }
      case OSSIM_UINT32:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_UINT_NAN;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_UINT32;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_UINT32;
         }
         break;
      }
      case OSSIM_SINT32:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_DEFAULT_MIN_PIX_SINT32-1;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_SINT32;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_SINT32;
         }
         break;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = 0.0;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
         }
         break;
      }
      case OSSIM_FLOAT32:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_FLT_NAN;         
            theMinPixelValue[band]  =  OSSIM_DEFAULT_MIN_PIX_FLOAT;
            theMaxPixelValue[band]  =  OSSIM_DEFAULT_MAX_PIX_FLOAT;
         }
         break;
      }
      case OSSIM_FLOAT64:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = OSSIM_DBL_NAN;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_DOUBLE;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_DOUBLE;
         }
         break;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = 0.0;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_NORM_DOUBLE;
         }
         break;
      }
      default:
      {
         for(band = 0; band < numberOfBands; ++band)
         {
            theNullPixelValue[band] = 0.0;
            theMinPixelValue[band]  = OSSIM_DEFAULT_MIN_PIX_UCHAR;
            theMaxPixelValue[band]  = OSSIM_DEFAULT_MAX_PIX_UCHAR;
         }
         break;
      }
   }
}

bool ossimImageData::isEqualTo(const ossimDataObject& rhs,
                               bool deepTest)const
{
   ossimImageData* rhsPtr = PTR_CAST(ossimImageData, &rhs);
   if(!(&rhs)||(!rhsPtr)) return false;
   bool result = ( (theScalarType         == rhsPtr->theScalarType)&&
                   (theNumberOfDataComponents == rhsPtr->theNumberOfDataComponents)&&
                   (theOrigin             == rhsPtr->theOrigin)&&
                   (getWidth()            == rhsPtr->getWidth())&&
                   (getHeight()           == rhsPtr->getHeight()));

   if(result)
   {
      bool test=true;
      for(ossim_uint32 index = 0; index < theNumberOfDataComponents; ++index)
      {
         if(theMinPixelValue[index] != rhsPtr->theMinPixelValue[index])
         {
            test = false;
            break;
         }
         if(theMaxPixelValue[index] != rhsPtr->theMaxPixelValue[index])
         {
            test = false;
            break;
         }
         if(theNullPixelValue[index] != rhsPtr->theNullPixelValue[index])
         {
            test = false;
            break;
         }
      }
      result = test;
   }
   if(deepTest&&result)
   {
      if(theDataBuffer && rhsPtr->theDataBuffer)
      {
         if(memcmp(theDataBuffer, rhsPtr->theDataBuffer, getSizeInBytes()) != 0)
         {
            result = false;
         }
      }
      else if(!theDataBuffer&&!rhsPtr->theDataBuffer)
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
   ossimIpt relative( position.x - theOrigin.x,
                      position.y - theOrigin.y);
   return getPix((theSpatialExtents[0])*relative.y + relative.x, band);
}

ossim_float64 ossimImageData::getPix(ossim_uint32 offset, ossim_uint32 band) const
{
   switch(theScalarType)
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

   if (s == NULL) return; // nothing to do...

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
      if (value == theNullPixelValue[band])
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
   ossim_int32 xNew = (pt.x - theOrigin.x);
   ossim_int32 yNew = (pt.y - theOrigin.y);
   if(xNew < 0 || xNew >= (int)theSpatialExtents[0] ||
      yNew < 0 || yNew >= (int)theSpatialExtents[1])
   {
      return true;
   }
   ossim_uint32 offset = getWidth()*yNew + xNew;
   
   return isNull(offset);
}

bool ossimImageData::isNull(const ossimIpt& pt, ossim_uint32 band)const
{
   ossim_int32 xNew = (pt.x - theOrigin.x);
   ossim_int32 yNew = (pt.y - theOrigin.y);
   if(xNew < 0 || xNew >= (int)theSpatialExtents[0] ||
      yNew < 0 || yNew >= (int)theSpatialExtents[1])
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
   ossim_int32 xNew = (pt.x - theOrigin.x);
   ossim_int32 yNew = (pt.y - theOrigin.y);
   
   if(xNew < 0 || xNew >= (int)theSpatialExtents[0] ||
      yNew < 0 || yNew >= (int)theSpatialExtents[1])
   {
      return;
   }
   ossim_uint32 offset = ((int)getWidth())*yNew + xNew;
   
   setNull(offset);
}

void ossimImageData::setNull(const ossimIpt& pt, ossim_uint32 band)
{
   ossim_int32 xNew = (pt.x - theOrigin.x);
   ossim_int32 yNew = (pt.y - theOrigin.y);
   
   if(xNew < 0 || xNew >= (int)theSpatialExtents[0] ||
      yNew < 0 || yNew >= (int)theSpatialExtents[1])
   {
      return;
   }
   ossim_uint32 offset = ((int)getWidth())*yNew + xNew;
   
   setNull(offset, band);
}

void ossimImageData::setNullPix(ossim_float64 null_pix)
{
   if(!theNullPixelValue) initializeDefaults();
   if(!theNullPixelValue) return;
   ossim_uint32 num = getNumberOfBands();
   for(ossim_uint32 band = 0; band < num; ++band)
   {
      theNullPixelValue[band] = null_pix;
   }
}

void ossimImageData::setNullPix(ossim_float64 null_pix, ossim_uint32 band)
{
   if(!theNullPixelValue) initializeDefaults();
   if(!theNullPixelValue) return;
   theNullPixelValue[band] = null_pix;
}

void ossimImageData::setNullPix(const ossim_float64* nullPixArray, ossim_uint32 numberOfValues)
{
   if(!nullPixArray) return;
   if(!theNullPixelValue) initializeDefaults();
   if(!theNullPixelValue) return;
   for(ossim_uint32 band = 0;
       (band < numberOfValues)&&(band < theNumberOfDataComponents); ++band)
   {
      theNullPixelValue[band] = nullPixArray[band];
   }
}

void ossimImageData::setMinPix(ossim_float64 min_pix, ossim_uint32 band)
{
   if(!theMinPixelValue) initializeDefaults();
   if(!theMinPixelValue) return;
   theMinPixelValue[band] = min_pix;
}

void ossimImageData::setMinPix(const ossim_float64* minPixArray, ossim_uint32 numberOfValues)
{
   if(!minPixArray) return;
   if(!theMinPixelValue) initializeDefaults();
   if(!theMinPixelValue) return;
   for(ossim_uint32 band = 0; (band < numberOfValues)&&(band < theNumberOfDataComponents); ++band)
   {
      theMinPixelValue[band] = minPixArray[band];
   }
}

void ossimImageData::setMinPix(ossim_float64 min_pix)
{
   if(!theMinPixelValue) initializeDefaults();
   if(!theMinPixelValue) return;
   ossim_uint32 num = getNumberOfBands();
   for(ossim_uint32 band = 0; band < num; ++band)
   {
      theMinPixelValue[band] = min_pix;
   }
}

void ossimImageData::setMaxPix(ossim_float64 max_pix)
{
   if(!theMaxPixelValue) initializeDefaults();
   if(!theMaxPixelValue) return;
   ossim_uint32 num = getNumberOfBands();
   for(ossim_uint32 band = 0; band < num; ++band)
   {
      theMaxPixelValue[band] = max_pix;
   }
}

void ossimImageData::setMaxPix(ossim_float64 max_pix, ossim_uint32 band)
{
   if(!theMaxPixelValue) initializeDefaults();
   if(!theMaxPixelValue) return;
   theMaxPixelValue[band] = max_pix;
}

void ossimImageData::setMaxPix(const ossim_float64* maxPixArray, ossim_uint32 numberOfValues)
{
   if(!maxPixArray) return;
   if(!theMaxPixelValue) initializeDefaults();
   if(!theMaxPixelValue) return;
   for(ossim_uint32 band = 0; (band < numberOfValues)&&(band < theNumberOfDataComponents); ++band)
   {
      theMaxPixelValue[band] = maxPixArray[band];
   }
}

void ossimImageData::setNumberOfBands(ossim_uint32 bands,
                                      bool reallocate)
{
   ossim_uint32 b  = getNumberOfBands();
   if((b != bands)&&
      bands&&
      theNullPixelValue&&
      theMinPixelValue&&
      theMaxPixelValue)
   {
      setNumberOfDataComponents(bands);
      if(reallocate)
      {
         ossimRectilinearDataObject::initialize();
      }
      
      ossim_uint32 minBands = ossimMin(b, bands);
      ossim_float64 *newNull = new ossim_float64[bands];
      ossim_float64 *newMin  = new ossim_float64[bands];
      ossim_float64 *newMax  = new ossim_float64[bands];
      
      std::copy(theNullPixelValue,
                theNullPixelValue + minBands,
                newNull);
      std::copy(theMinPixelValue,
                theMinPixelValue + minBands,
                newMin);
      std::copy(theMaxPixelValue,
                theMaxPixelValue + minBands,
                newMax);
      
      if(bands > minBands)
      {
         std::fill(newNull+minBands,
                   newNull+bands,
                   theNullPixelValue[b - 1]);
         std::fill(newMin+minBands,
                   newMin+bands,
                   theMinPixelValue[b - 1]);
         std::fill(newMax+minBands,
                   newMax+bands,
                   theMaxPixelValue[b - 1]);
      }
      
      delete [] theNullPixelValue;
      delete [] theMinPixelValue;
      delete [] theMaxPixelValue;
      
      theNullPixelValue = newNull;
      theMinPixelValue  = newMin;
      theMaxPixelValue  = newMax;
      
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
      if(theMinPixelValue)
      {
         delete [] theMinPixelValue;
         theMinPixelValue = NULL;
      }
      if(theMaxPixelValue)
      {
         delete [] theMaxPixelValue;
         theMaxPixelValue = NULL;
      }
      if(theNullPixelValue)
      {
         delete [] theNullPixelValue;
         theNullPixelValue = NULL;
      }
      ossim_uint32 numberOfBands = getNumberOfBands();
      if(!numberOfBands)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageData::assign\n"
            << "Number of components is 0, can't assign" << std::endl;
         return;
      }
      theOrigin = data->theOrigin;
      theMinPixelValue  = new ossim_float64[numberOfBands];
      theMaxPixelValue  = new ossim_float64[numberOfBands];
      theNullPixelValue = new ossim_float64[numberOfBands];
      if(data->theMinPixelValue&&
         data->theMaxPixelValue&&
         data->theNullPixelValue)
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            theMinPixelValue[band]  = data->theMinPixelValue[band];
            theMaxPixelValue[band]  = data->theMaxPixelValue[band];
            theNullPixelValue[band] = data->theNullPixelValue[band];
         }
      }
      else
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
   if (s == NULL || d == NULL)
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
         loadBand(ossim_uint8(0), src, src_rect, band);
         return;

      case OSSIM_SINT8:
         loadBand(ossim_sint8(0), src, src_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadBand(ossim_uint16(0), src, src_rect, band);
         return;
         
      case OSSIM_SINT16:
         loadBand(ossim_sint16(0), src, src_rect, band);
         return;
         
      case OSSIM_UINT32:
         loadBand(ossim_uint32(0), src, src_rect, band);
         return;

      case OSSIM_SINT32:
         loadBand(ossim_sint32(0), src, src_rect, band);
         return;
         
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
         loadBand(ossim_float32(0), src, src_rect, band);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadBand(ossim_float64(0), src, src_rect, band);
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
         loadBand(ossim_uint8(0), src, src_rect, clip_rect, band);
         return;

      case OSSIM_SINT8:
         loadBand(ossim_sint8(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadBand(ossim_uint16(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_SINT16:
         loadBand(ossim_sint16(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT32:
         loadBand(ossim_uint32(0), src, src_rect, clip_rect, band);
         return;

      case OSSIM_SINT32:
         loadBand(ossim_sint32(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
         loadBand(ossim_float32(0), src, src_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadBand(ossim_float64(0), src, src_rect, clip_rect, band);
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

template <class T> void ossimImageData::loadBand(T, // dummy template variable
                                                 const void* src,
                                                 const ossimIrect& src_rect,
                                                 ossim_uint32 band)
{
   const ossimIrect img_rect = getImageRectangle();

   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   
   loadBand(T(0), src, src_rect, clip_rect, band);
}

template <class T> void ossimImageData::loadBand(T, // dummy template variable
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
//      cerr <<"Doing nothing" << std::endl;
      return; // Nothing to do here.
   }

   // Check the clip rect.
   if (!clip_rect.completely_within(img_rect))
   {
//       cerr << MODULE
//            << " ERROR: clip rect does not fall within tile rect!"
//            << std::endl;
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

void ossimImageData::loadTileFromBip(const void* src,
                                     const ossimIrect& src_rect)
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         loadTileFromBip(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBip(ossim_sint8(0), src, src_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBip(ossim_uint16(0), src, src_rect);
         return;
         
      case OSSIM_SINT16:
         loadTileFromBip(ossim_sint16(0), src, src_rect);
         return;
         
      case OSSIM_UINT32:
         loadTileFromBip(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBip(ossim_sint32(0), src, src_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBip(ossim_float32(0), src, src_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBip(ossim_float64(0), src, src_rect);
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
         loadTileFromBip(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBip(ossim_sint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBip(ossim_uint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBip(ossim_sint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBip(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBip(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBip(ossim_float32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBip(ossim_float64(0), src, src_rect, clip_rect);
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
         loadTileFromBil(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBil(ossim_sint8(0), src, src_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBil(ossim_uint16(0), src, src_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBil(ossim_sint16(0), src, src_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBil(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBil(ossim_sint32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBil(ossim_float32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBil(ossim_float64(0), src, src_rect);
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
         loadTileFromBil(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBil(ossim_sint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBil(ossim_uint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBil(ossim_sint16(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBil(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBil(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBil(ossim_float32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBil(ossim_float64(0), src, src_rect, clip_rect);
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
         loadTileFromBsq(ossim_uint8(0), src, src_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBsq(ossim_sint8(0), src, src_rect);
         return;

      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBsq(ossim_uint16(0), src, src_rect);
         return;
      
      case OSSIM_SINT16:
         loadTileFromBsq(ossim_sint16(0), src, src_rect);
         return;
      
      case OSSIM_UINT32:
         loadTileFromBsq(ossim_uint32(0), src, src_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBsq(ossim_sint32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBsq(ossim_float32(0), src, src_rect);
         return;
      
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBsq(ossim_float64(0), src, src_rect);
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
         loadTileFromBsq(ossim_uint8(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         loadTileFromBsq(ossim_sint8(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         loadTileFromBsq(ossim_uint16(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         loadTileFromBsq(ossim_sint16(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         loadTileFromBsq(ossim_uint32(0), src, src_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         loadTileFromBsq(ossim_sint32(0), src, src_rect, clip_rect);
         return;
      
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         loadTileFromBsq(ossim_float32(0), src, src_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         loadTileFromBsq(ossim_float64(0), src, src_rect, clip_rect);
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
                                      vector<ossim_float64>& maxBands)
{
   if ( (getDataObjectStatus() == OSSIM_NULL) || 
        (getDataObjectStatus() == OSSIM_EMPTY) )
   {
      return;
   }

   switch(theScalarType)
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
void ossimImageData::computeMinMaxPix(T dummyValue,
                                      vector<ossim_float64>& minBands,
                                      vector<ossim_float64>& maxBands)
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

template <class T>
void ossimImageData::loadTileFromBip(T, // dummy template variable
                                     const void* src,
                                     const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBip(T(0), src, src_rect, clip_rect);
}

template <class T>
void ossimImageData::loadTileFromBip(T, // dummy template variable
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
//       cerr << MODULE
//            << " ERROR: clip rect does not fall within tile rect!"
//            << std::endl;
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
void ossimImageData::loadTileFromBil(T, // dummy template variable
                                     const void* src,
                                     const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBil(T(0), src, src_rect, clip_rect);
}
   
template <class T>
void ossimImageData::loadTileFromBil(T, // dummy template variable
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
//       cerr << MODULE
//            << " ERROR: clip rect does not fall within tile rect!"
//            << std::endl;
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
void ossimImageData::loadTileFromBsq(T, // dummy template variable
                                     const void* src,
                                     const ossimIrect& src_rect)
{
   const ossimIrect img_rect = getImageRectangle();
   
   // Get the clip rect.
   const ossimIrect clip_rect = img_rect.clipToRect(src_rect);

   loadTileFromBsq(T(0), src, src_rect, clip_rect);
}

template <class T>
void ossimImageData::loadTileFromBsq(T, // dummy template variable
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
//       cerr << MODULE
//            << " ERROR: clip rect does not fall within tile rect!"
//            << std::endl;
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
   return ((point.x >= theOrigin.x)&&
           (point.y >= theOrigin.y)&&
           ((point.x - theOrigin.x)<static_cast<long>(theSpatialExtents[0]))&&
           ((point.y - theOrigin.y)<static_cast<long>(theSpatialExtents[1])));
}

bool ossimImageData::isPointWithin(ossim_int32 x, ossim_int32 y)const
{
   return ((x >= theOrigin.x)&&
           (y >= theOrigin.y)&&
           ((x - theOrigin.x) < static_cast<long>(theSpatialExtents[0]))&&
           ((y - theOrigin.y) < static_cast<long>(theSpatialExtents[1])));
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
   } // End of "switch (getScalarType())"
}

void ossimImageData::unloadTileToBip(void* dest,
                                     const ossimIrect& dest_rect,
                                     const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBip(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBip(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBip(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBip(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         unloadTileToBip(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT32:
         unloadTileToBip(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBip(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBip(ossim_float64(0), dest, dest_rect, clip_rect);
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

void ossimImageData::unloadTileToBil(void* dest,
                                     const ossimIrect& dest_rect,
                                     const ossimIrect& clip_rect) const
{
   switch (getScalarType())
   {
      case OSSIM_UINT8:
         unloadTileToBil(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBil(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBil(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBil(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_UINT32:
         unloadTileToBil(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         unloadTileToBil(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBil(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBil(ossim_float64(0), dest, dest_rect, clip_rect);
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
         unloadTileToBsq(ossim_uint8(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT8:
         unloadTileToBsq(ossim_sint8(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadTileToBsq(ossim_uint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_SINT16:
         unloadTileToBsq(ossim_sint16(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_UINT32:
         unloadTileToBsq(ossim_uint32(0), dest, dest_rect, clip_rect);
         return;

      case OSSIM_SINT32:
         unloadTileToBsq(ossim_sint32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadTileToBsq(ossim_float32(0), dest, dest_rect, clip_rect);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadTileToBsq(ossim_float64(0), dest, dest_rect, clip_rect);
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
         unloadBand(ossim_uint8(0), dest, dest_rect, clip_rect, band);
         return;

      case OSSIM_SINT8:
         unloadBand(ossim_sint8(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
         unloadBand(ossim_uint16(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_SINT16:
         unloadBand(ossim_sint16(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_UINT32:
         unloadBand(ossim_uint32(0), dest, dest_rect, clip_rect, band);
         return;

      case OSSIM_SINT32:
         unloadBand(ossim_sint32(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_FLOAT:
      case OSSIM_FLOAT32:
         unloadBand(ossim_float32(0), dest, dest_rect, clip_rect, band);
         return;
         
      case OSSIM_NORMALIZED_DOUBLE:
      case OSSIM_FLOAT64:
         unloadBand(ossim_float64(0), dest, dest_rect, clip_rect, band);
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

template <class T>
void ossimImageData::unloadBand(T, // dummy tmeplate variable
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
   
   ossim_uint32 num_bands = getNumberOfBands();

   ossim_uint32 buf_width = dest_rect.lr().x - dest_rect.ul().x + 1;

   // Get a pointer to the source band buffer.
   const T* s = reinterpret_cast<const T*>(getBuf(band));

   // Get the offset for each source band.
   ossim_uint32 src_offset = (output_clip_rect.ul().y - img_rect.ul().y) *
      getWidth() + output_clip_rect.ul().x - img_rect.ul().x;
   
   // Move the pointers to the start.
   T* d = reinterpret_cast<T*>(dest);
   
   d += (output_clip_rect.ul().y - dest_rect.ul().y) * buf_width * num_bands +
      output_clip_rect.ul().x - dest_rect.ul().x;
   
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
ossimImageData::unloadTileToBip(T, // dummy template variable
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
         nulls[band] = static_cast<T>(theNullPixelValue[band]);
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
      nulls = NULL;;
   }
}

template <class T> void
ossimImageData::unloadTileToBil(T,  // dummy template arg...
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
         nulls[band] = static_cast<T>(theNullPixelValue[band]);
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
   }
}

template <class T> void
ossimImageData::unloadTileToBsq(T,  // dummy template arg...
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

   ossim_uint32 num_bands     = getNumberOfBands();
   ossim_uint32 band          = 0;
   if(!dataIsNull)
   {
      ossim_uint32 num_bands     = getNumberOfBands();
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
         ossim_uint8 np = static_cast<ossim_uint8>(theNullPixelValue[band]);
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
void ossimImageData::copyTileToNormalizedBuffer(T dummyTemplate,
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
void ossimImageData::copyTileToNormalizedBuffer(T dummyTemplate,
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
void ossimImageData::copyTileToNormalizedBuffer(T dummyTemplate,
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
void ossimImageData::copyTileToNormalizedBuffer(T dummyTemplate,
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
void ossimImageData::copyNormalizedBufferToTile(T dummyTemplate,
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
void ossimImageData::copyNormalizedBufferToTile(T dummyTemplate,
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
void ossimImageData::copyNormalizedBufferToTile(T dummyTemplate,
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
void ossimImageData::copyNormalizedBufferToTile(T dummyTemplate,
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
   if (theScalarType == OSSIM_NORMALIZED_DOUBLE ||
       theScalarType == OSSIM_NORMALIZED_FLOAT)
   {
      return theMinPixelValue[0];  // Already normalized.
   }
   return (1.0 / (theMaxPixelValue[0] - theMinPixelValue[0] + 1.0));
}

std::ostream& ossimImageData::print(std::ostream& out) const
{
   out << "ossimImageData::print:"
      << "\ntheOrigin:  " << theOrigin << std::endl;
   if(theNullPixelValue)
   {
      out << "Null values: ";
      
      copy(theNullPixelValue,
           theNullPixelValue + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   if(theMinPixelValue)
   {
      out << "Min values:  ";
      
      copy(theMinPixelValue,
           theMinPixelValue + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   if(theMaxPixelValue)
   {
      out << "Max values:  ";
      
      copy(theMaxPixelValue,
           theMaxPixelValue + getNumberOfBands(),
           std::ostream_iterator<ossim_float64>(out, " "));
      out << "\n";
   }
   out << "width:            " << getWidth()
       << "\nheight:           " << getHeight()
       << "\nimage rectangle:  " << getImageRectangle()
       << std::endl;

   return ossimRectilinearDataObject::print(out);
}

ossim_uint32 ossimImageData::getWidth()const
{
   if(theSpatialExtents) return theSpatialExtents[0];
   return 0;
}

ossim_uint32 ossimImageData::getHeight()const
{
   if(theSpatialExtents) return theSpatialExtents[1];
   return 0;
}

void ossimImageData::getWidthHeight(ossim_uint32& w, ossim_uint32& h)
{
   if(theSpatialExtents)
   {
      w = theSpatialExtents[0];
      h = theSpatialExtents[1];
   }
}

void ossimImageData::setWidth(ossim_uint32 width)
{
   if(theSpatialExtents)
   {
      theSpatialExtents[0] = width;
   }
}

void ossimImageData::setHeight(ossim_uint32 height)
{
   if(theSpatialExtents)
   {
      theSpatialExtents[1] = height;
   }
}

void ossimImageData::setWidthHeight(ossim_uint32 w, ossim_uint32 h)
{
   if(theSpatialExtents)
   {
      theSpatialExtents[0] = w;
      theSpatialExtents[1] = h;
   }
}

void ossimImageData::setOrigin(const ossimIpt& origin)
{
   theOrigin = origin;
}

ossim_uint32 ossimImageData::getDataSizeInBytes()const
{
   return getSizeInBytes();
}

const ossimImageData& ossimImageData::operator=(const ossimImageData& /* rhs */)
{
   return *this;
}

//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimNBandToIndexFilter.cpp 19732 2011-06-06 22:24:54Z dburken $

#include <ossim/imaging/ossimNBandToIndexFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimNotifyContext.h>


static const char* KEEP_QUANTIZED_VALUE_FLAG_KW = "keep_quantized_value_flag";

RTTI_DEF1(ossimNBandToIndexFilter,
          "ossimNBandToIndexFilter",
          ossimImageSourceFilter);

ossimNBandToIndexFilter::ossimNBandToIndexFilter()
   :ossimImageSourceFilter(),
    theLut(0),
    theTile(0),
    theKeepQuantizedValueFlag(false)
{
}

ossimNBandToIndexFilter::~ossimNBandToIndexFilter()
{
}

void ossimNBandToIndexFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   // Force allocate on next getTile.
   theTile      = 0;
   if(theLut.valid())
   {
      if(theKeepQuantizedValueFlag)
      {
         ossimNBandLutDataObject::LUT_ENTRY_TYPE minValue;
         ossimNBandLutDataObject::LUT_ENTRY_TYPE maxValue;
         ossim_uint32 bands = theLut->getNumberOfBands();
         ossim_uint32 idx   = 0;
         if(theMinValues.size() != bands)
         {
            theMinValues.resize(bands);
            theMaxValues.resize(bands);
            theNullValues.resize(bands);
         }
         std::fill(theNullValues.begin(),
                   theNullValues.end(), 0);
         for(idx = 0; idx < bands; ++idx)
         {
            theLut->getMinMax(idx, minValue, maxValue);
            theMinValues[idx] = minValue;
            theMaxValues[idx] = maxValue;
            theNullValues[idx] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(ossimImageSourceFilter::getNullPixelValue(idx));
         }
      }
      else
      {
         if(theMinValues.size() != 1)
         {
            theMinValues.resize(1);
            theMaxValues.resize(1);
            theNullValues.resize(1);
         }
         theMinValues[0]  = 0;
         theMaxValues[0]  = theLut->getNumberOfEntries()-1;
         theNullValues[0] = theLut->getNullPixelIndex();
      }
   }
}

void ossimNBandToIndexFilter::allocate()
{
   theTile      = 0;
   
   if(isSourceEnabled()&&theInputConnection)
   {
      theTile      = ossimImageDataFactory::instance()->create(this, this);
      if(theTile.valid())
      {
         theTile->initialize();
      }
   }
}

void ossimNBandToIndexFilter::disableSource()
{
   ossimImageSourceFilter::disableSource();
   theTile = 0;
}

ossimRefPtr<ossimImageData> ossimNBandToIndexFilter::getTile(const ossimIrect& origin,
                                                             ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(origin, resLevel);
   }
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }

   if(!theTile.valid())
   {
      allocate();
   }
   
   ossimRefPtr<ossimImageData> input = theInputConnection->getTile(origin,
                                                                   resLevel);

   if(!theTile.valid() || !input.valid())
   {
      return input;
   }

   theTile->setImageRectangle(origin);
   theTile->setDataObjectStatus(OSSIM_FULL);
   theTile->makeBlank();

   return convertInputTile(input.get());
}

ossimRefPtr<ossimImageData> ossimNBandToIndexFilter::convertInputTile(ossimImageData* tile)
{
   switch(getOutputScalarType())
   {
      case OSSIM_UINT8:
      {
         return convertOutputTileTemplate((ossim_uint8)0,
                                          tile);
         break;
      }
      case OSSIM_UINT16:
      {
         return convertOutputTileTemplate((ossim_uint16)0,
                                          tile);
         break;
      }
      case OSSIM_UINT32:
      {
         return convertOutputTileTemplate((ossim_uint32)0,
                                         tile);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimNBandToIndexFilter::convertInputTile: Unsupported scalar type for conversion" << std::endl;
      }
   }

   return theTile;
}

template <class T>
ossimRefPtr<ossimImageData> ossimNBandToIndexFilter::convertOutputTileTemplate(T outputDummy,
                                                                               ossimImageData* inputTile)
{
   switch(inputTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         return convertInputTileToOutputTemplate((ossim_uint8)0,
                                                 outputDummy,
                                                 inputTile);
         break;
      }
      case OSSIM_SINT8:
      {
         return convertInputTileToOutputTemplate((ossim_sint8)0,
                                                 outputDummy,
                                                 inputTile);
      }
      case OSSIM_UINT16:
      {
         return convertInputTileToOutputTemplate((ossim_uint16)0,
                                                 outputDummy,
                                                 inputTile);
      }
      case OSSIM_SINT32:
      {
         return convertInputTileToOutputTemplate((ossim_sint32)0,
                                                 outputDummy,
                                                 inputTile);
      }
      case OSSIM_UINT32:
      {
         return convertInputTileToOutputTemplate((ossim_uint32)0,
                                                 outputDummy,
                                                 inputTile);
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN) << "convertInputTileToOutputTemplate::convertOutputTileTemplate: Unsupported scalar type for conversion." << std::endl;
      }
   }

   return theTile;
}

template <class T1, class T2>
ossimRefPtr<ossimImageData> ossimNBandToIndexFilter::convertInputTileToOutputTemplate(
   T1 /* inputDummy */,
   T2 /* outputDummy */,
   ossimImageData* inputTile)
{
   if (!inputTile)
   {
      return theTile;
   }
   
   const ossim_uint32 BANDS = inputTile->getNumberOfBands();

   if (!BANDS)
   {
      return theTile;
   }
//   T2** outBand = (T2*)theTile->getBuf();

   ossim_uint32 numberOfBands = theLut->getNumberOfBands();
   
   ossimNBandLutDataObject::LUT_ENTRY_TYPE* bandValues = new ossimNBandLutDataObject::LUT_ENTRY_TYPE[numberOfBands];

   memset(bandValues, 0, sizeof(ossimNBandLutDataObject::LUT_ENTRY_TYPE)*numberOfBands);
   numberOfBands = ossim::min(BANDS,
                            numberOfBands);
   if(!numberOfBands) return theTile;
   T1** band = new T1*[numberOfBands];
   T2** outBand = new T2*[numberOfBands];
   ossim_int32 idx   = 0;

   for(idx = 0; idx < (ossim_int32)numberOfBands; ++idx)
   {
      band[idx] = (T1*)inputTile->getBuf(idx);
      outBand[idx] = (T2*)theTile->getBuf(idx);
   }
   if(band[0])
   {
      if(inputTile->getDataObjectStatus() == OSSIM_FULL)
      {
         ossim_uint32 upper = inputTile->getWidth()*inputTile->getHeight();
         ossim_uint32 idx2=0;
         for(ossim_uint32 offset = 0; offset < upper; ++offset)
         {
            for(idx2 = 0; idx2 < numberOfBands; ++idx2)
            {
               bandValues[idx2] = *band[idx2];
               ++band[idx2];
            }
            //---
            // Note call the ossimNBandLutDataObject::findIndex that takes a size as the input
            // data may have dropped the alpha channel.
            //---
            idx = theLut->findIndex(bandValues, numberOfBands);

            if(!theKeepQuantizedValueFlag)
            {
               *outBand[0] = (T2)idx;
               ++outBand[0];
            }
            else
            {
               for(idx2 = 0; idx2 < numberOfBands; ++idx2)
               {
                  if(idx >=0)
                  {
                     *outBand[idx2] = (T2)(*theLut)[idx][idx2];
                     ++outBand[idx2];
                  }
               }
            }
         }
      }
      else if(inputTile->getDataObjectStatus() == OSSIM_PARTIAL)
      {
         ossim_uint32 upper = inputTile->getWidth()*inputTile->getHeight();
         ossim_uint32 idx2=0;
         for(ossim_uint32 offset = 0; offset < upper; ++offset)
         {
            for(idx2 = 0; idx2 < numberOfBands; ++idx2)
            {
               bandValues[idx2] = *band[idx2];
               ++band[idx2];
            }
            idx = -1;
            if(!inputTile->isNull(offset))
            {
               //---
               // Note call the ossimNBandLutDataObject::findIndex that takes a size as the input
               // data may have dropped the alpha channel.
               //---
               idx = theLut->findIndex(bandValues, numberOfBands);
            }
            if(!theKeepQuantizedValueFlag)
            {
               if(idx>=0)
               {
                  *outBand[0] = (T2)idx;
               }
               ++outBand[0];
            }
            else
            {
               for(idx2 = 0; idx2 < numberOfBands; ++idx2)
               {
                  if(idx >=0)
                  {
                     if(idx>=0)
                     {
                        *outBand[idx2] = (T2)(*theLut)[idx][idx2];
                     }
                  }
                  ++outBand[idx2];
               }
            }
         }
      }
   }
   if(theKeepQuantizedValueFlag)
   {
      theTile->validate();
   }
   else
   {
      theTile->setDataObjectStatus(OSSIM_FULL);
   }
   return theTile;
}

bool ossimNBandToIndexFilter::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   ossimString newPrefix = prefix;
   newPrefix = newPrefix + "lut.";

   if(theLut.valid())
   {
      theLut->saveState(kwl, newPrefix.c_str());
   }

   kwl.add(prefix,
           KEEP_QUANTIZED_VALUE_FLAG_KW,
           theKeepQuantizedValueFlag?"true":"false",
           true);
           
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimNBandToIndexFilter::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   ossimString newPrefix = prefix;
   newPrefix = newPrefix + "lut.";

   if(!theLut.valid())
   {
      theLut = new ossimNBandLutDataObject;
   }
   theLut->loadState(kwl, newPrefix.c_str());

   const char* flag = kwl.find(prefix, KEEP_QUANTIZED_VALUE_FLAG_KW);
   if(flag)
   {
      theKeepQuantizedValueFlag = ossimString(flag).toBool();
   }
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimNBandToIndexFilter::isSourceEnabled()const
{
   bool result = ossimSource::isSourceEnabled()&&theInputConnection;

   // only support 3 band integral values, no floating point for now.
   //
   if(result)
   {
      
      ossimScalarType inputScalarType = theInputConnection->getOutputScalarType();
      if(!theLut.valid() ||
         (theLut->getNumberOfEntries()<1)||
         (inputScalarType==OSSIM_FLOAT32)||
         (inputScalarType==OSSIM_FLOAT64)||
         (theInputConnection->getNumberOfOutputBands()!=3))
      {
         result = false;
      }
   }
   
   return result;
}

ossim_uint32 ossimNBandToIndexFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      if(theKeepQuantizedValueFlag)
      {
         if(theLut.valid())
         {
            return theLut->getNumberOfBands();
         }
      }
      else
      {
         return 1;
      }
   }
   
   return ossimImageSourceFilter::getNumberOfOutputBands();
}
   
ossimScalarType ossimNBandToIndexFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      
      ossim_uint32 numberOfEntries = theLut->getNumberOfEntries();
      
      if(numberOfEntries < 257)
      {
         return OSSIM_UCHAR;
      }
      else if(numberOfEntries < 65537)
      {
         return OSSIM_UINT16;
      }
      else
      {
         return OSSIM_UINT32;
      }
   }
   
   return ossimImageSourceFilter::getOutputScalarType();
}

void ossimNBandToIndexFilter::setLut(ossimNBandLutDataObject& lut)
{
   theLut = new ossimNBandLutDataObject(lut);
}

double ossimNBandToIndexFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(isSourceEnabled())
   {
      if(theLut->getNullPixelIndex() >= 0)
      {
         return theLut->getNullPixelIndex();
      }
   }
   
   return ossimImageSourceFilter::getNullPixelValue(band);
}

double ossimNBandToIndexFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(isSourceEnabled())
   {
      if(band < theMinValues.size())
      {
         return (double)theMinValues[band];
      }
   }
//    if(isSourceEnabled())
//    {
//       if(getNullPixelValue() != 0)
//       {
//          return 0;
//       }
//       else
//       {
//          return 1;
//       }
//       return 0;
//    }
   
   return ossimImageSourceFilter::getMinPixelValue();
}

double ossimNBandToIndexFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(isSourceEnabled())
   {
      if(band < theMaxValues.size())
      {
         return (double)theMaxValues[band];
      }
//       if(theLut->getNullPixelIndex() != ((ossim_int32)(theLut->getNumberOfEntries())-1))
//       {
//          return theLut->getNumberOfEntries();
//       }
//       else
//       {
//          return (theLut->getNumberOfEntries()-1);
//       }
   }
   
   return ossimImageSourceFilter::getMaxPixelValue();
}

void ossimNBandToIndexFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(property->getName() == ossimKeywordNames::FILENAME_KW)
   {
      ossimKeywordlist kwl;
      if(kwl.addFile(ossimFilename(property->valueToString())))
      {
         theLut = new ossimNBandLutDataObject;
         theLut->loadState(kwl);
      }
   }
   else if(property->getName() == KEEP_QUANTIZED_VALUE_FLAG_KW)
   {
      theKeepQuantizedValueFlag = property->valueToString().toBool();
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNBandToIndexFilter::getProperty(const ossimString& name)const
{
   if(name == ossimKeywordNames::FILENAME_KW)
   {
      ossimFilenameProperty* property = new ossimFilenameProperty(name, theLutFilename);
      property->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      property->setCacheRefreshBit();
      
      return property;
   }
   else if(name == KEEP_QUANTIZED_VALUE_FLAG_KW)
   {
      ossimBooleanProperty* property = new ossimBooleanProperty(name, theKeepQuantizedValueFlag);
      property->setCacheRefreshBit();

      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimNBandToIndexFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);

   propertyNames.push_back(ossimKeywordNames::FILENAME_KW);
   propertyNames.push_back(KEEP_QUANTIZED_VALUE_FLAG_KW);
}

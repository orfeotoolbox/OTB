//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimEdgeFilter.cpp,v 1.9 2005/08/17 19:18:12 gpotts Exp $
#include <imaging/tile_sources/ossimEdgeFilter.h>
#include <imaging/factory/ossimImageDataFactory.h>
#include <imaging/ossimImageData.h>
#include <base/property/ossimStringProperty.h>

RTTI_DEF1(ossimEdgeFilter, "ossimEdgeFilter", ossimImageSourceFilter);

ossimEdgeFilter::ossimEdgeFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theFilterType("Sobel")
{
}

ossimEdgeFilter::ossimEdgeFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(NULL),
    theFilterType("Sobel")
{
}

ossimEdgeFilter::ossimEdgeFilter(ossimObject* owner,
                                   ossimImageSource* inputSource)
   :ossimImageSourceFilter(owner, inputSource),
    theTile(NULL),
    theFilterType("Sobel")
{
}

ossimRefPtr<ossimImageData> ossimEdgeFilter::getTile(const ossimIrect& rect,
                                                     ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }

   // expand the rect out to cver the 3x3 horizontal and vertical
   // kernel.
   //
   ossimIrect requestRect = rect;

   adjustRequestRect(requestRect);
   
   ossimRefPtr<ossimImageData> inputData =
      ossimImageSourceFilter::getTile(requestRect, resLevel);

   if(!inputData.valid() || (!inputData->getBuf()))
   {
      return inputData;
   }

   if(!theTile.valid()) initialize();
   if(!theTile.valid()) return theTile;
      
   theTile->setImageRectangleAndBands(rect, inputData->getNumberOfBands());

   switch(theTile->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      runFilter((ossim_uint8)0,
                inputData);
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      runFilter((ossim_float32)0,
                inputData);
      break;
   }
   case OSSIM_USHORT16:
   case OSSIM_USHORT11:
   {
      runFilter((ossim_uint16)0,
                inputData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      runFilter((ossim_sint16)0,
                inputData);
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      runFilter((ossim_float64)0,
                inputData);
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimEdgeFilter::getTile WARN: Scalar type = " << theTile->getScalarType()
                                         << " Not supported by ossimEdgeFilter" << std::endl;
      break;
   }
   }

   return theTile;
}

void ossimEdgeFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile = NULL;

   if(!isSourceEnabled())
   {
      return;
   }
   
   theTile = ossimImageDataFactory::instance()->create(this, this);
   if(theTile.valid())
   {
      theTile->initialize();
   }
}

void ossimEdgeFilter::getFilterTypeNames(
   std::vector<ossimString>& filterNames)const
{
   filterNames.push_back("Laplacian");
   filterNames.push_back("Prewitt");
   filterNames.push_back("Roberts");
   filterNames.push_back("Sobel");
}

ossimString ossimEdgeFilter::getFilterType()const
{
   return theFilterType;
}

void ossimEdgeFilter::setFilterType(const ossimString& filterType)
{
   ossimString tempFilterType = filterType;
   tempFilterType = tempFilterType.downcase();
   
   if(tempFilterType.contains("sob"))
   {
      tempFilterType = "Sobel";
   }
   else if(tempFilterType.contains("lap"))
   {
      tempFilterType = "Laplacian";
   }
   else if(tempFilterType.contains("prew"))
   {
      tempFilterType = "Prewitt";
   }
   else if(tempFilterType.contains("rob"))
   {
      tempFilterType = "Roberts";
   }
   else
   {
      tempFilterType = "Sobel";
   }
}

void ossimEdgeFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   
   if(property->getName() == "Edge type")
   {
      theFilterType = property->valueToString();
   }
}

ossimRefPtr<ossimProperty> ossimEdgeFilter::getProperty(const ossimString& name)const
{
   if(name == "Edge type")
   {
      std::vector<ossimString> filterNames;
      
      getFilterTypeNames(filterNames);
      ossimStringProperty* stringProp = new ossimStringProperty("Edge type",
								theFilterType,
								false,
								filterNames);
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();

      return stringProp;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimEdgeFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back("Edge type");
}


void ossimEdgeFilter::adjustRequestRect(ossimIrect& requestRect)const
{
   ossimString filterType = theFilterType;
   filterType = filterType.downcase();
   ossimIrect rect = requestRect;
   if(filterType.contains("sob")||
      filterType.contains("lap")||
      filterType.contains("pre"))
   {
      requestRect = ossimIrect(rect.ul().x - 1,
                               rect.ul().y - 1,
                               rect.lr().x + 1,
                               rect.lr().y + 1);
   }
   else if(filterType.contains("rob"))
   {
      requestRect = ossimIrect(rect.ul().x,
                               rect.ul().y,
                               rect.lr().x + 1,
                               rect.lr().y + 1);
   }
   else
   {
      requestRect = ossimIrect(rect.ul().x - 1,
                               rect.ul().y - 1,
                               rect.lr().x + 1,
                               rect.lr().y + 1);
   }
}

template <class T>
void ossimEdgeFilter::runFilter(T dummyVariable,
                                ossimRefPtr<ossimImageData> inputData)
{
   ossimString filterType = theFilterType;
   filterType = filterType.downcase();
   
   if(filterType.contains("sobel"))
   {
      runSobelFilter(dummyVariable, inputData);
   }
   else if(filterType.contains("lap"))
   {
      runLaplacianFilter(dummyVariable, inputData);
   }
   else if(filterType.contains("pre"))
   {
      runPrewittFilter(dummyVariable, inputData);
   }
   else if(filterType.contains("rob"))
   {
      runRobertsFilter(dummyVariable, inputData);
   }
   else 
   {
      theTile->makeBlank();
   }
}

template <class T>
void ossimEdgeFilter::runSobelFilter(T dummyVariable,
                                     ossimRefPtr<ossimImageData> inputData)
{
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   double horizontalValue = 0.0;
   double verticalValue = 0.0;
   double value = 0.0;
   // ossim_uint32 valueIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_int32 rowIncrement  = inputData->getWidth();
   ossim_int32 rowIncrement2 = 2*inputData->getWidth();
   
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(theTile->getNullPix(bandIdx));
      T minP       = static_cast<T>(theTile->getMinPix(bandIdx));
      T maxP       = static_cast<T>(theTile->getMaxPix(bandIdx));

      if(inputBuf&&outputBuf)
      {
         for(y = 0; y < height; ++y)
         {
            for(x = 0; x < width; ++x)
            {
               if( (*(inputBuf + rowIncrement + 1) != np))
               {
                  horizontalValue = ((double)inputBuf[0] - (double)inputBuf[rowIncrement2]) +
                                    ((double)(inputBuf[1]*2.0) - (double)( inputBuf[rowIncrement2+1]*2.0)) +
                                    ((double)(inputBuf[2]) - (double)(inputBuf[rowIncrement2+2]));

                   verticalValue   = ((double)(inputBuf[2]) + (double)inputBuf[rowIncrement+2]*2.0 + (double)inputBuf[rowIncrement2+2]) -
                                     (double)(inputBuf[0] + 2.0*(double)inputBuf[rowIncrement] + (double)inputBuf[rowIncrement2]);
                  
                  value = sqrt(horizontalValue*horizontalValue +  verticalValue*verticalValue);

                  if((value == np) ||
                     (value < minP))
                  {
                     *outputBuf = (static_cast<T>(minP));
                  }
                  else if(value > maxP)
                  {
                     *outputBuf = (static_cast<T>(maxP));
                  }
                  else
                  {
                     *outputBuf = (static_cast<T>(value));
                  }
                  
               }
               else
               {
                  *outputBuf = np;
               }
               ++outputBuf;
               ++inputBuf;
            }
            inputBuf+=2;
         }
      }
   }
   theTile->validate();
}

template <class T>
void ossimEdgeFilter::runPrewittFilter(T dummyVariable,
                                       ossimRefPtr<ossimImageData> inputData)
{
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   double horizontalValue = 0.0;
   double verticalValue = 0.0;
   double value = 0.0;
   // ossim_uint32 valueIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_int32 rowIncrement  = inputData->getWidth();
   ossim_int32 rowIncrement2 = 2*inputData->getWidth();
   
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(theTile->getNullPix(bandIdx));
      T minP       = static_cast<T>(theTile->getMinPix(bandIdx));
      T maxP       = static_cast<T>(theTile->getMaxPix(bandIdx));

      if(inputBuf&&outputBuf)
      {
         for(y = 0; y < height; ++y)
         {
            for(x = 0; x < width; ++x)
            {
               if( (*(inputBuf + rowIncrement + 1) != np))
               {
                  horizontalValue = ((double)inputBuf[0] - (double)inputBuf[rowIncrement2]) +
                                    ((double)(inputBuf[1]) - (double)( inputBuf[rowIncrement2+1])) +
                                    ((double)(inputBuf[2]) - (double)(inputBuf[rowIncrement2+2]));

                   verticalValue   = ((double)(inputBuf[2]) + (double)inputBuf[rowIncrement+2] + (double)inputBuf[rowIncrement2+2]) -
                                     ((double)inputBuf[0] + (double)inputBuf[rowIncrement] + (double)inputBuf[rowIncrement2]);
                  
                  value = sqrt(horizontalValue*horizontalValue +  verticalValue*verticalValue);

                  if((value == np) ||
                     (value < minP))
                  {
                     *outputBuf = (static_cast<T>(minP));
                  }
                  else if(value > maxP)
                  {
                     *outputBuf = (static_cast<T>(maxP));
                  }
                  else
                  {
                     *outputBuf = (static_cast<T>(value));
                  }
                  
               }
               else
               {
                  *outputBuf = np;
               }
               ++outputBuf;
               ++inputBuf;
            }
            inputBuf+=2;
         }
      }
   }
   theTile->validate();
}

template <class T>
void ossimEdgeFilter::runRobertsFilter(T dummyVariable,
                                       ossimRefPtr<ossimImageData> inputData)
{
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   double v1 = 0.0;
   double v2 = 0.0;
   double value = 0.0;
   // ossim_uint32 valueIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_int32 rowIncrement  = inputData->getWidth();
   
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(theTile->getNullPix(bandIdx));
      T minP       = static_cast<T>(theTile->getMinPix(bandIdx));
      T maxP       = static_cast<T>(theTile->getMaxPix(bandIdx));

      if(inputBuf&&outputBuf)
      {
         for(y = 0; y < height; ++y)
         {
            for(x = 0; x < width; ++x)
            {
               if( (*inputBuf) != np)
               {
                  v1 = (double)inputBuf[0] - (double)(inputBuf[rowIncrement+1]);

                  v2   = (double)inputBuf[1] - (double)inputBuf[rowIncrement];
                  
                  value = sqrt(v1*v1 +  v2*v2);

                  if((value == np) ||
                     (value < minP))
                  {
                     *outputBuf = (static_cast<T>(minP));
                  }
                  else if(value > maxP)
                  {
                     *outputBuf = (static_cast<T>(maxP));
                  }
                  else
                  {
                     *outputBuf = (static_cast<T>(value));
                  }
                  
               }
               else
               {
                  *outputBuf = np;
               }
               ++outputBuf;
               ++inputBuf;
            }
            ++inputBuf;
         }
      }
   }
   theTile->validate();
}

template <class T>
void ossimEdgeFilter::runLaplacianFilter(T dummyVariable,
                                         ossimRefPtr<ossimImageData> inputData)
{
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   // double horizontalValue = 0.0;
   // double verticalValue = 0.0;
   double value = 0.0;
   // ossim_uint32 valueIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_int32 rowIncrement  = inputData->getWidth();
   ossim_int32 rowIncrement2 = 2*inputData->getWidth();
   
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(theTile->getNullPix(bandIdx));
      T minP       = static_cast<T>(theTile->getMinPix(bandIdx));
      T maxP       = static_cast<T>(theTile->getMaxPix(bandIdx));

      if(inputBuf&&outputBuf)
      {
         for(y = 0; y < height; ++y)
         {
            for(x = 0; x < width; ++x)
            {
               if( (*(inputBuf + rowIncrement + 1) != np))
               {
                  
                  value = fabs(((double)inputBuf[rowIncrement + 1]*4.0) -
                               ((double)inputBuf[1] + (double)inputBuf[rowIncrement] + (double)inputBuf[rowIncrement + 2] + (double)inputBuf[rowIncrement2+1]));

                  if((value == np) ||
                     (value < minP))
                  {
                     *outputBuf = (static_cast<T>(minP));
                  }
                  else if(value > maxP)
                  {
                     *outputBuf = (static_cast<T>(maxP));
                  }
                  else
                  {
                     *outputBuf = (static_cast<T>(value));
                  }
                  
               }
               else
               {
                  *outputBuf = np;
               }
               ++outputBuf;
               ++inputBuf;
            }
            inputBuf+=2;
         }
      }
   }
   theTile->validate();
}

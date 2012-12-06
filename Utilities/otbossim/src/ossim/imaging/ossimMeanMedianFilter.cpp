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
//  $Id: ossimMeanMedianFilter.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimMeanMedianFilter.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/imaging/ossimImageData.h>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;


RTTI_DEF1(ossimMeanMedianFilter,
          "ossimMeanMedianFilter",
          ossimImageSourceFilter);

// Keywords used throughout.
static const ossimString WINDOW_SIZE_KW = "window_size";
static const ossimString FILTER_TYPE_KW = "filter_type";
static const ossimString AUTO_GROW_KW   = "auto_grow_rectangle_flag";

ossimMeanMedianFilter::ossimMeanMedianFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(0),
    theFilterType(OSSIM_MEDIAN),
    theWindowSize(3),
    theEnableFillNullFlag(false),
    theAutoGrowRectFlag(false)
{
   setDescription(ossimString("Mean Median Filter"));
}

ossimMeanMedianFilter::~ossimMeanMedianFilter()
{
}

ossimRefPtr<ossimImageData> ossimMeanMedianFilter::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }

   ossim_uint32 halfSize = getWindowSize()>>1;

   ossimIrect requestRect(rect.ul().x - halfSize,
                          rect.ul().y - halfSize,
                          rect.lr().x + halfSize,
                          rect.lr().y + halfSize);

   ossimRefPtr<ossimImageData> inputData =
      ossimImageSourceFilter::getTile(requestRect, resLevel);
   if(!inputData.valid() || !inputData->getBuf())
   {
      return inputData;
   }

   if(!theTile.valid())
   {
      theTile = (ossimImageData*)inputData->dup();
      theTile->setImageRectangle(rect);
   }
   else
   {
      theTile->setImageRectangleAndBands(rect, inputData->getNumberOfBands());
   }

   applyFilter(inputData);

   theTile->setDataObjectStatus(inputData->getDataObjectStatus());
   if(theEnableFillNullFlag)
   {
      theTile->validate();
   }

   return theTile;
}

void ossimMeanMedianFilter::setWindowSize(ossim_uint32 windowSize)
{
   theWindowSize = windowSize;
}

ossim_uint32 ossimMeanMedianFilter::getWindowSize()const
{
   return theWindowSize;
}

void ossimMeanMedianFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile = NULL;
}

void ossimMeanMedianFilter::applyFilter(ossimRefPtr<ossimImageData>& input)
{
   switch(input->getScalarType())
   {
      case OSSIM_UINT8:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_uint8(0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_uint8(0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_uint8(0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_uint8(0), input);
               
            default:
               break;
         }
         break;
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_uint16(0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_uint16(0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_uint16(0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_uint16(0), input);
               break;
               
            default:
               break;
         }
         break;
      }
      case OSSIM_SINT16:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_sint16(0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_sint16(0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_sint16(0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_sint16(0), input);
               break;
               
            default:
               break;
         }
         break;
      }
      case OSSIM_UINT32:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_uint32(0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_uint32(0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_uint32(0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_uint32(0), input);
               break;
               
            default:
               break;
         }
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_float32(0.0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_float32(0.0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_float32(0.0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_float32(0.0), input);
               break;
               
            default:
               break;
         }
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         switch (theFilterType)
         {
            case OSSIM_MEDIAN:
            case OSSIM_MEDIAN_FILL_NULLS:
               applyMedian(ossim_float64(0.0), input);
               break;
               
            case OSSIM_MEDIAN_NULL_CENTER_ONLY:
               applyMedianNullCenterOnly(ossim_float64(0.0), input);
               break;
               
            case OSSIM_MEAN:
            case OSSIM_MEAN_FILL_NULLS:
               applyMean(ossim_float64(0.0), input);
               break;

            case OSSIM_MEAN_NULL_CENTER_ONLY:
               applyMeanNullCenterOnly(ossim_float64(0.0), input);
               break;
               
            default:
               break;
         }
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimMeanMedianFilter::applyFilter WARNING:\n"
            << "Unhandled scalar type!" << endl;
      }
   }
}

template <class T>
void ossimMeanMedianFilter::applyMean(T /* dummyVariable */,
                                      ossimRefPtr<ossimImageData>& inputData)
{
   ossim_uint32 halfWindow = (theWindowSize >> 1);
   ossim_uint32 bandIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 kernelX   = 0;
   ossim_uint32 kernelY   = 0;
   ossim_uint32 kernelIdx = 0;
   ossim_uint32 iw  = inputData->getWidth();
   ossim_uint32 ow  = theTile->getWidth();
   ossim_uint32 oh = theTile->getHeight();
   ossim_uint32 numberOfBands = ossim::min(theTile->getNumberOfBands(),
                                         inputData->getNumberOfBands());
   ossimDataObjectStatus status = inputData->getDataObjectStatus();
   std::vector<double> values(theWindowSize*theWindowSize);

   if(status == OSSIM_FULL)
   {
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);

         if(inputBuf&&outputBuf)
         {
            for(y = 0; y < oh; ++y)
            {
               for(x = 0; x < ow; ++x)
               {
                  kernelIdx = 0;
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        values[kernelIdx] = *(inputBuf+kernelX + kernelY*iw);
                        ++kernelIdx;
                     }
                  }


                  if(values.size() > 0)
                  {
                     double sum = std::accumulate(values.begin(),
                                                  values.end(),
                                                  0.0);
                     double average = sum/(double)values.size();
                     (*outputBuf) = (T)average;
                  }
                  ++inputBuf;
                  ++outputBuf;
               }

               inputBuf+=(halfWindow<<1);
            }
         }
      }
   }
   else
   {
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);
         T np            = (T)inputData->getNullPix(bandIdx);
         if(inputBuf&&outputBuf)
         {
            for(y = 0; y < oh; ++y)
            {
               for(x = 0; x < ow; ++x)
               {
                  values.clear();
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        T tempValue = *(inputBuf+kernelX + kernelY*iw);

                        if(tempValue != np)
                        {
                           values.push_back((double)tempValue);
                        }
                     }
                  }


                  if(values.size() > 0)
                  {
                     double accumulate = std::accumulate(values.begin(),
                                                         values.end(),
                                                         0.0);
                     double average = accumulate/(double)values.size();
                     if(*(inputBuf+halfWindow + halfWindow*iw) == np)
                     {
                        if(theEnableFillNullFlag)
                        {
                           (*outputBuf) = (T)average;
                        }
                        else
                        {
                           (*outputBuf) = np;
                        }
                     }
                     else
                     {
                        (*outputBuf) = (T)average;
                     }
                  }
                  else
                  {
                     *outputBuf = np;
                  }
                  ++inputBuf;
                  ++outputBuf;
               }

               inputBuf+=(halfWindow<<1);
            }
         }
      }
   }
}

template <class T> void ossimMeanMedianFilter::applyMeanNullCenterOnly(
   T /* dummyVariable */,
   ossimRefPtr<ossimImageData>& inputData)
{
   ossim_uint32 halfWindow = (theWindowSize >> 1);
   ossim_uint32 bandIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 kernelX   = 0;
   ossim_uint32 kernelY   = 0;
   ossim_uint32 iw  = inputData->getWidth();
   ossim_uint32 ow  = theTile->getWidth();
   ossim_uint32 oh = theTile->getHeight();
   ossim_uint32 numberOfBands = ossim::min(theTile->getNumberOfBands(),
                                         inputData->getNumberOfBands());
   ossimDataObjectStatus status = inputData->getDataObjectStatus();
   std::vector<double> values;

   if(status == OSSIM_FULL)
   {
      // Nothing to do just copy the tile.
      theTile->loadTile(inputData.get());
   }
   else
   {
      // Partial tile with nulls in it.
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);
         if (!inputBuf || !outputBuf)
         {
            return; // Shouldn't happen...
         }
         
         const T NP = (T)inputData->getNullPix(bandIdx);

         for(y = 0; y < oh; ++y)
         {
            for(x = 0; x < ow; ++x)
            {
               // Get the center input pixel.
               const T CP = *(inputBuf+halfWindow + halfWindow*iw);
               if (CP == NP)
               {
                  values.clear();
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        T tempValue = *(inputBuf+kernelX + kernelY*iw);
                        
                        if(tempValue != NP)
                        {
                           values.push_back((double)tempValue);
                        }
                     }
                  }
                  
                  if(values.size() > 0)
                  {
                     double accumulate = std::accumulate(values.begin(),
                                                         values.end(),
                                                         0.0);
                     double average = accumulate/(double)values.size();
                     (*outputBuf) = (T)average;
                  }
                  else
                  {
                     (*outputBuf) = NP;
                  }
                  
               }
               else // Center pixel (CP) not null.
               {
                  (*outputBuf) = CP;
               }
               
               // Move over...
               ++inputBuf;
               ++outputBuf;
               
            } // End of loop in x direction.

            // Move down...
            inputBuf+=(halfWindow<<1);
            
         }  // End of loop in y direction.
         
      }  // End of band loop.
      
   }  // End of else "partial tile" block.
}

template <class T>
void ossimMeanMedianFilter::applyMedian(T /* dummyVariable */,
                                        ossimRefPtr<ossimImageData>& inputData)
{
   ossim_uint32 halfWindow = (theWindowSize >> 1);
   ossim_uint32 bandIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 kernelX   = 0;
   ossim_uint32 kernelY   = 0;
   ossim_uint32 kernelIdx = 0;
   ossim_uint32 iw  = inputData->getWidth();
   ossim_uint32 ow  = theTile->getWidth();
   ossim_uint32 oh = theTile->getHeight();
   ossim_uint32 numberOfBands = ossim::min(theTile->getNumberOfBands(),
                                         inputData->getNumberOfBands());
   ossimDataObjectStatus status = inputData->getDataObjectStatus();
   std::vector<T> values(theWindowSize*theWindowSize);

   if(status == OSSIM_FULL)
   {
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);

         if(inputBuf&&outputBuf)
         {
            for(y = 0; y < oh; ++y)
            {
               for(x = 0; x < ow; ++x)
               {
                  kernelIdx = 0;
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        values[kernelIdx] = *(inputBuf+kernelX + kernelY*iw);
                        ++kernelIdx;
                     }
                  }

                  std::sort(values.begin(),
                            values.end());

                  if(values.size() > 0)
                  {
                     (*outputBuf) = values[values.size()>>1];
                  }
                  ++inputBuf;
                  ++outputBuf;
               }

               inputBuf+=(halfWindow<<1);
            }
         }
      }
   }
   else
   {
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);
         T np            = (T)inputData->getNullPix(bandIdx);
         if(inputBuf&&outputBuf)
         {
            for(y = 0; y < oh; ++y)
            {
               for(x = 0; x < ow; ++x)
               {
                  values.clear();
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        T tempValue = *(inputBuf+kernelX + kernelY*iw);

                        if(tempValue != np)
                        {
                           values.push_back(tempValue);
                        }
                     }
                  }

                  std::sort(values.begin(),
                            values.end());

                  if(values.size() > 0)
                  {
                     if(*(inputBuf+halfWindow + halfWindow*iw) == np)
                     {
                        if(theEnableFillNullFlag)
                        {
                           (*outputBuf) = values[values.size()>>1];
                        }
                        else
                        {
                           (*outputBuf) = np;
                        }
                     }
                     else
                     {
                        (*outputBuf) = values[values.size()>>1];
                     }
                  }
                  else
                  {
                     *outputBuf = np;
                  }
                  ++inputBuf;
                  ++outputBuf;
               }

               inputBuf+=(halfWindow<<1);
            }
         }
      }
   }
}

template <class T> void ossimMeanMedianFilter::applyMedianNullCenterOnly (
   T /* dummyVariable */,
   ossimRefPtr<ossimImageData>& inputData)
{
   ossim_uint32 halfWindow = (theWindowSize >> 1);
   ossim_uint32 bandIdx = 0;
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 kernelX   = 0;
   ossim_uint32 kernelY   = 0;
   ossim_uint32 iw  = inputData->getWidth();
   ossim_uint32 ow  = theTile->getWidth();
   ossim_uint32 oh = theTile->getHeight();
   ossim_uint32 numberOfBands = ossim::min(theTile->getNumberOfBands(),
                                         inputData->getNumberOfBands());
   ossimDataObjectStatus status = inputData->getDataObjectStatus();
   std::vector<T> values;

   if(status == OSSIM_FULL)
   {
      // Nothing to do just copy the tile.
      theTile->loadTile(inputData.get());
   }
   else
   {
       // Partial tile with nulls in it.
      for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
      {
         T* inputBuf     = (T*)inputData->getBuf(bandIdx);
         T* outputBuf    = (T*)theTile->getBuf(bandIdx);
         if (!inputBuf || !outputBuf)
         {
            return; // Shouldn't happen...
         }
         
         const T NP = (T)inputData->getNullPix(bandIdx);

         for(y = 0; y < oh; ++y)
         {
            for(x = 0; x < ow; ++x)
            {
               // Get the center input pixel.
               const T CP = *(inputBuf+halfWindow + halfWindow*iw);
               if (CP == NP)
               {
                  values.clear();
                  for(kernelY = 0; kernelY < theWindowSize; ++kernelY)
                  {
                     for(kernelX = 0; kernelX < theWindowSize;++kernelX)
                     {
                        T tempValue = *(inputBuf+kernelX + kernelY*iw);
                        
                        if(tempValue != NP)
                        {
                           values.push_back(tempValue);
                        }
                     }
                  }

                  std::sort(values.begin(),
                            values.end());
                  
                  if(values.size() > 0)
                  {
                     (*outputBuf) = values[values.size()>>1];
                  }
                  else
                  {
                     (*outputBuf) = NP;
                  }
               }
               else // Center pixel (CP) not null.
               {
                  (*outputBuf) = CP;
               }

               // Move over...
               ++inputBuf;
               ++outputBuf;
               
            }  // End of loop in x direction.

            // Move down...
            inputBuf+=(halfWindow<<1);
            
         }  // End of loop in y direction.
         
      }  // End of band loop.
      
   }  // End of else "partial tile" block.
}

void ossimMeanMedianFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid())
   {
      return;
   }

   ossimString name = property->getName();

   if (name == WINDOW_SIZE_KW)
   {
      theWindowSize = property->valueToString().toUInt32();
   }
   else if (name == FILTER_TYPE_KW)
   {
      ossimString value = property->valueToString();
      setFilterType(value);
   }
   else if (name == AUTO_GROW_KW)
   {
      ossimString value;
      property->valueToString(value);
      setAutoGrowRectFlag(value.toBool());
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimMeanMedianFilter::getProperty(const ossimString& name)const
{
   if (name == WINDOW_SIZE_KW)
   {
      ossimProperty* prop =
         new ossimNumericProperty(WINDOW_SIZE_KW,
                                  ossimString::toString(theWindowSize),
                                  3,
                                  25);
      prop->setCacheRefreshBit();

      return prop;
   }
   else if (name == FILTER_TYPE_KW)
   {
      std::vector<ossimString> constraintList;
      getFilterTypeList(constraintList);
      ossimString value = getFilterTypeString();
      ossimProperty* prop = new ossimStringProperty(FILTER_TYPE_KW,
                                                    value,
                                                    false,
                                                    constraintList);
      prop->setCacheRefreshBit();
      
      return prop;
   }
   else if (name == AUTO_GROW_KW)
   {
      ossimRefPtr<ossimProperty> p = new ossimBooleanProperty(
         AUTO_GROW_KW, getAutoGrowRectFlag());
      p->setFullRefreshBit();
      return p;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimMeanMedianFilter::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(WINDOW_SIZE_KW);
   propertyNames.push_back(FILTER_TYPE_KW);
   propertyNames.push_back(AUTO_GROW_KW);

   ossimImageSourceFilter::getPropertyNames(propertyNames);
}

bool ossimMeanMedianFilter::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   kwl.add(prefix,
           WINDOW_SIZE_KW.c_str(),
           theWindowSize,
           true);
   kwl.add(prefix,
           FILTER_TYPE_KW.c_str(),
           getFilterTypeString(),
           true);
   kwl.add(prefix,
           AUTO_GROW_KW.c_str(),
           (theAutoGrowRectFlag?"true":"false"),
           true);   

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimMeanMedianFilter::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   const char* lookup = NULL;
   
   lookup = kwl.find(prefix, WINDOW_SIZE_KW.c_str());
   if(lookup)
   {
      theWindowSize = ossimString(lookup).toUInt32();
   }
   
   lookup = kwl.find(prefix, FILTER_TYPE_KW.c_str());
   if(lookup)
   {
      ossimString type = lookup;
      setFilterType(type);
   }

   lookup = kwl.find(prefix, AUTO_GROW_KW.c_str());
   if(lookup)
   {
      ossimString flag = lookup;
      setAutoGrowRectFlag(flag.toBool());
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}
void ossimMeanMedianFilter::setFilterType(ossimMeanMedianFilterType type)
{
   theFilterType = type;
}

void ossimMeanMedianFilter::setFilterType(const ossimString& type)
{
   ossimString s = type;
   s.downcase();

   std::vector<ossimString> list;
   getFilterTypeList(list);
   
   for (ossim_uint32 i = 0; i < list.size(); ++i)
   {
      if (s == list[i])
      {
         theFilterType = static_cast<ossimMeanMedianFilterType>(i);
      }
   }

   if ( (theFilterType == OSSIM_MEDIAN_FILL_NULLS) ||
        (theFilterType == OSSIM_MEAN_FILL_NULLS) )
   {
      theEnableFillNullFlag = true;
   }
   else
   {
      theEnableFillNullFlag = false;
   }
}

void ossimMeanMedianFilter::setAutoGrowRectFlag(bool flag)
{
   theAutoGrowRectFlag = flag;
}

bool ossimMeanMedianFilter::getAutoGrowRectFlag() const
{
   return theAutoGrowRectFlag;
}

ossimString ossimMeanMedianFilter::getFilterTypeString() const
{
   std::vector<ossimString> list;
   getFilterTypeList(list);
   return list[theFilterType];
}

void ossimMeanMedianFilter::getFilterTypeList(
   std::vector<ossimString>& list) const
{
   list.resize(OSSIM_MEAN_NULL_CENTER_ONLY+1);

   list[0] = ossimString("median");
   list[1] = ossimString("median_fill_nulls");
   list[2] = ossimString("median_null_center_only");
   list[3] = ossimString("mean");
   list[4] = ossimString("mean_fill_nulls");
   list[5] = ossimString("mean_null_center_only");
}

ossimIrect ossimMeanMedianFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   // Get the input rectangle.
   ossimIrect rect = ossimImageSourceFilter::getBoundingRect(resLevel);
   if (!theAutoGrowRectFlag || rect.hasNans())
   {
      return rect; // Not in autogrow mode or no input connection yet...
   }

   if ( (theFilterType == OSSIM_MEDIAN_FILL_NULLS)       ||
        (theFilterType == OSSIM_MEDIAN_NULL_CENTER_ONLY) ||
        (theFilterType == OSSIM_MEAN_FILL_NULLS)         ||
        (theFilterType == OSSIM_MEAN_NULL_CENTER_ONLY) )
   {
      ossimIpt pt = rect.ul();
      const ossim_int32 HW = (theWindowSize >> 1); // half window size.

      // Adjust the upper left.
      pt.x = pt.x-HW;
      pt.y = pt.y-HW;
      rect.set_ul(pt);

      // Adjust the lower right.
      pt = rect.lr();
      pt.x = pt.x+HW;
      pt.y = pt.y+HW;
      rect.set_lr(pt);
   }

   return rect;
}

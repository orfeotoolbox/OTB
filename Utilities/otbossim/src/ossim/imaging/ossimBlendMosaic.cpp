//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimBlendMosaic.cpp 20696 2012-03-19 12:36:40Z dburken $

#include <ossim/imaging/ossimBlendMosaic.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimBlendMosaic, "ossimBlendMosaic", ossimImageMosaic)

ossimBlendMosaic::ossimBlendMosaic()
   : ossimImageMosaic(),
     theNormResult(NULL)
{
}

ossimBlendMosaic::ossimBlendMosaic(ossimConnectableObject::ConnectableObjectList& inputSources)
   : ossimImageMosaic(inputSources),
     theNormResult(NULL)
{
   initialize();
}

ossimBlendMosaic::~ossimBlendMosaic()
{
}

void ossimBlendMosaic::initialize()
{   
   ossimImageMosaic::initialize();

   allocate();

   // only allocate this space if we have to
   if(hasDifferentInputs())
   {
      theNormResult = new ossimImageData(this,
                                         OSSIM_NORMALIZED_FLOAT,
                                         theLargestNumberOfInputBands);
      theNormResult->initialize();
   }
      
   if(theWeights.size() < theInputObjectList.size())
   {
      for(ossim_uint32 index =(ossim_uint32)theWeights.size();
          index < theInputObjectList.size();
          ++index)
      {
         theWeights.push_back(1.0);
      }
   }
}

ossimRefPtr<ossimImageData> ossimBlendMosaic::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{   
   // ossimIpt origin = tileRect.ul();
   if(!isSourceEnabled())
   {
      return ossimImageMosaic::getTile(tileRect, resLevel);
   }
   
   ossim_uint32 size = getNumberOfInputs();
   if(!theTile.get())
   {
      // try to initialize
      initialize();

      // if we still don't have a buffer
      // then we will leave
      if(!theTile.get())
      {
         return theTile;
      }
   }

   if(size == 0)
   {
      return ossimRefPtr<ossimImageData>();
   }
   if(size == 1)
   {
      return ossimImageMosaic::getTile(tileRect, resLevel);
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   if(theNormResult.valid())
   {
      theNormResult->setImageRectangle(tileRect);
      theNormResult->makeBlank();
   }
   
   switch(theTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint8>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint8>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_SINT8:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint8>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint8>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_FLOAT32: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<float>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<float>(0), tileRect, resLevel);
         }
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint16>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint16>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_SINT16:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint16>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint16>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_SINT32:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_sint32>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_sint32>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_UINT32:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<ossim_uint32>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<ossim_uint32>(0), tileRect,
                               resLevel);
         }
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         if(!hasDifferentInputs())
         {
            return combine(static_cast<double>(0), tileRect, resLevel);
         }
         else
         {
            return combineNorm(static_cast<double>(0), tileRect, resLevel);
         }
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_NOTICE)
            << "ossimBlendMosaic::getTile NOTICE:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimImageMosaic" << endl;
       }
   }

   return ossimRefPtr<ossimImageData>();   
}

template <class T> ossimRefPtr<ossimImageData> ossimBlendMosaic::combine(
   T,
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
  ossimRefPtr<ossimImageData> currentImageData=NULL;
  ossim_uint32 band;
  double currentWeight = 1.0;
  double previousWeight = 1.0;
  // double sumOfWeights   = 1;
  long offset = 0;
  long row    = 0;
  long col    = 0;
  ossim_uint32 layerIdx = 0;
  currentImageData = getNextTile(layerIdx, 0, tileRect, resLevel);
  
  if(!currentImageData.get()) // if we don't have one then return theTile
  {
     return theTile;
  }
  
  T** srcBands  = new T*[theLargestNumberOfInputBands];
  T** destBands = new T*[theLargestNumberOfInputBands];
  T*  nullPix   = new T[theTile->getNumberOfBands()];
  
  previousWeight = theWeights[layerIdx];
  //    // now get the previous weight and then combine the two into one.
	 // let's assign the bands
  for(band = 0; band < theLargestNumberOfInputBands; ++band)
  {
     destBands[band] = static_cast<T*>(theTile->getBuf(band));
     nullPix[band]   = static_cast<T>(theTile->getNullPix(band));
  }
  while(currentImageData.get())
  {
      ossimDataObjectStatus currentStatus =
         currentImageData->getDataObjectStatus();
      
      // set the current weight for the current tile.
      currentWeight = theWeights[layerIdx];

      // sumOfWeights = previousWeight+currentWeight;
      if( (currentStatus != OSSIM_EMPTY) &&
	  (currentStatus != OSSIM_NULL))
       {	 
	 long h = (long)currentImageData->getHeight();
	 long w = (long)currentImageData->getWidth();
	 offset = 0;
	 ossim_uint32 minNumberOfBands = currentImageData->getNumberOfBands();
	 
	 // let's assign the bands
	 for(band = 0; band < minNumberOfBands; ++band)
	   {
	     srcBands[band]  = static_cast<T*>(currentImageData->getBuf(band));
	   }
	 for(;band < theLargestNumberOfInputBands; ++band)
	   {
	     srcBands[band]  = srcBands[minNumberOfBands - 1];
	   }

 	 if(currentStatus == OSSIM_PARTIAL)
         {
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                  if(!currentImageData->isNull(offset))
                  {
                     for(band = 0; band < theLargestNumberOfInputBands; ++band)
                     {
                        if(destBands[band][offset] != nullPix[band])
                        {
//                            destBands[band][offset] = static_cast<T>((destBands[band][offset]*(1.0-currentWeight) +  
//                                                                      srcBands[band][offset]*currentWeight));
                           destBands[band][offset] = static_cast<T>((destBands[band][offset]*previousWeight + 
                                                                     srcBands[band][offset]*currentWeight)/(previousWeight+currentWeight));
                        }
                        else
                        {
                           destBands[band][offset] = srcBands[band][offset];
                        }
                     }	 
                  }
                  ++offset;
               }
            }
         }
	 else
         {
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                  for(band = 0; band < theLargestNumberOfInputBands; ++band)
                  {			
                     if(destBands[band][offset] != nullPix[band])
                     {
                        destBands[band][offset] = static_cast<T>((destBands[band][offset]*previousWeight+ 
                                                                  srcBands[band][offset]*currentWeight)/(previousWeight + currentWeight));
                     }
                     else
                     {
                        destBands[band][offset] = srcBands[band][offset];
                     }
                  }
                  ++offset;
               }
            }
         }
       }
      currentImageData = getNextTile(layerIdx, tileRect, resLevel);
      previousWeight   = (previousWeight+currentWeight)/2.0;
    }
  delete [] srcBands;
  delete [] destBands;
  delete [] nullPix;
  theTile->validate();

  return theTile;   
}

template <class T> ossimRefPtr<ossimImageData> ossimBlendMosaic::combineNorm(
   T, const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> currentImageData=NULL;
   ossim_uint32 band;
   double currentWeight = 1.0;
   double previousWeight = 1.0;
   // double sumOfWeights   = 1;
   long offset = 0;
   long row    = 0;
   long col    = 0;
   ossim_uint32 layerIdx = 0;
   currentImageData = getNextNormTile(layerIdx, 0, tileRect, resLevel);
  
   if(!currentImageData.get()) // if we don't have one then return theTile
   {
      return theTile;
   }
   theNormResult->makeBlank();
   float** srcBands  = new float*[theLargestNumberOfInputBands];
   float** destBands = new float*[theLargestNumberOfInputBands];
   float*  nullPix   = new float[theTile->getNumberOfBands()];
  
   previousWeight = theWeights[layerIdx];
   //    // now get the previous weight and then combine the two into one.
   // let's assign the bands
   for(band = 0; band < theLargestNumberOfInputBands; ++band)
   {
      destBands[band] = static_cast<float*>(theNormResult->getBuf(band));
      nullPix[band]   = static_cast<float>(theNormResult->getNullPix(band));
   }
   while(currentImageData.get())
   {
      ossimDataObjectStatus currentStatus     = currentImageData->getDataObjectStatus();
     
      // set the current weight for the current tile.
      currentWeight = theWeights[layerIdx];
     
      // sumOfWeights = previousWeight+currentWeight;
      if( (currentStatus != OSSIM_EMPTY) &&
          (currentStatus != OSSIM_NULL))
      {	 
         long h = (long)currentImageData->getHeight();
         long w = (long)currentImageData->getWidth();
         offset = 0;
         ossim_uint32 minNumberOfBands = currentImageData->getNumberOfBands();
        
         // let's assign the bands
         for(band = 0; band < minNumberOfBands; ++band)
         {
            srcBands[band]  = static_cast<float*>(currentImageData->getBuf(band));
         }
         for(;band < theLargestNumberOfInputBands; ++band)
         {
            srcBands[band]  = srcBands[minNumberOfBands - 1];
         }
        
         if(currentStatus == OSSIM_PARTIAL)
         {
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                  if(!currentImageData->isNull(offset))
                  {
                     for(band = 0; band < theLargestNumberOfInputBands; ++band)
                     {
                        if(destBands[band][offset] != nullPix[band])
                        {
                           destBands[band][offset] = static_cast<float>((destBands[band][offset]*previousWeight + 
                                                                         srcBands[band][offset]*currentWeight)/(previousWeight+currentWeight));
                        }
                        else
                        {
                           destBands[band][offset] = srcBands[band][offset];
                        }
                     }	 
                  }
                  ++offset;
               }
            }
         }
         else
         {
            for(row = 0; row < h; ++row)
            {
               for(col = 0; col < w; ++col)
               {
                  for(band = 0; band < theLargestNumberOfInputBands; ++band)
                  {			
                     if(destBands[band][offset] != nullPix[band])
                     {
                        destBands[band][offset] = static_cast<float>((destBands[band][offset]*previousWeight+ 
                                                                      srcBands[band][offset]*currentWeight)/(previousWeight + currentWeight));
                     }
                     else
                     {
                        destBands[band][offset] = srcBands[band][offset];
                     }
                  }
                  ++offset;
               }
            }
         }
      }
      currentImageData = getNextNormTile(layerIdx, tileRect, resLevel);
      previousWeight   = (previousWeight+currentWeight)/2.0;
   }
   theNormResult->validate();
   theTile->copyNormalizedBufferToTile((float*)theNormResult->getBuf());
   delete [] srcBands;
   delete [] destBands;
   delete [] nullPix;
   theTile->validate();

   return theTile;   
}

bool ossimBlendMosaic::saveState(ossimKeywordlist& kwl,
                                 const char* prefix)const
{
   bool result = ossimImageMosaic::saveState(kwl, prefix);

   ossimString copyPrefix    = prefix;
   for(ossim_uint32 index = 0; index < theWeights.size(); ++index)
   {
      ossimString weightStr = ossimString("weight") +
                              ossimString::toString(index);
      kwl.add(copyPrefix.c_str(),
              weightStr.c_str(),
              theWeights[index],
              true);
   }
   
   return result;
}

bool ossimBlendMosaic::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   bool result = ossimImageMosaic::loadState(kwl, prefix);
   ossim_uint32 count = 0;
   theWeights.clear();
   if(result)
   {
      ossimString copyPrefix    = prefix;
      ossimString regExpression =  ossimString("^(") + copyPrefix + "weight[0-9]+)";
      ossim_uint32 result = kwl.getNumberOfSubstringKeys(regExpression);
            
      ossim_uint32 numberOfMatches = 0;
      while(numberOfMatches < result)
      {
         ossimString value = ossimString("weight") + ossimString::toString(count);

         const char* lookup = kwl.find(copyPrefix.c_str(),
                                       value.c_str());
         
         if(lookup)
         {
            ++numberOfMatches;
            theWeights.push_back(ossimString(lookup).toDouble());
         }

         ++count;
      }
   }
   
   return result;
}


void ossimBlendMosaic::setNumberOfWeights(ossim_uint32 numberOfWeights)
{
   if(numberOfWeights > theWeights.size())
   {
      ossim_uint32 length = numberOfWeights - (ossim_uint32) theWeights.size();
      for(ossim_uint32 index= 0; index < length; ++ index)
      {
         theWeights.push_back(1.0);
      }
   }
   else if(numberOfWeights < theWeights.size())
   {
      // copy the list
      vector<double> temp(theWeights.begin(),
                          theWeights.begin()+numberOfWeights);
      // now shrink
      theWeights = temp;
   }
}

void ossimBlendMosaic::setAllWeightsTo(double value)
{
   for(ossim_uint32 index = 0; index < theWeights.size(); ++index)
   {
      theWeights[index] = value;
   }
}

void ossimBlendMosaic::normalizeWeights()
{
  double sumWeights=0.0;
  ossim_uint32 index = 0;
  for(index = 0; index < theWeights.size(); ++index)
    {
      sumWeights+=theWeights[index];
    }
  if(sumWeights > 0.0)
    {
      for(index = 0; index < theWeights.size(); ++index)
	{
	  theWeights[index] /= sumWeights;
	}
    }
}

void ossimBlendMosaic::findMinMax(double& minValue, double& maxValue)const
{
   if(theWeights.size() > 0)
   {
      minValue = maxValue = theWeights[0];

      for(ossim_uint32 index = 1; index < theWeights.size(); ++index)
      {
         minValue = minValue < theWeights[index]?minValue:theWeights[index];
         maxValue = maxValue > theWeights[index]?maxValue:theWeights[index];
      }
   }
}

void ossimBlendMosaic::setWeight(ossim_uint32 index, double weight)
{
   if(index < theWeights.size())
   {
      theWeights[index] = weight;
   }
}

void ossimBlendMosaic::setWeights(const std::vector<double>& weights)
{
   theWeights = weights;
}

const vector<double>& ossimBlendMosaic::getWeights()const
{
   return theWeights;
}

double ossimBlendMosaic::getWeight(ossim_uint32 index) const
{
   if(index < theWeights.size())
   {
      return theWeights[index];
   }
   if(theWeights.size())
   {
      return theWeights[theWeights.size()-1];
   }
   return 0;
}

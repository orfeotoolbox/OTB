//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimBandClipFilter.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimBandClipFilter.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageData.h>

RTTI_DEF1(ossimBandClipFilter, "ossimBandClipFilter", ossimImageSourceFilter);

ossimBandClipFilter::ossimBandClipFilter()
   :ossimImageSourceFilter(),
    theClipType(ossimBandClipType_NONE),
    theTile(NULL)
{
   theTile = new ossimImageData(this, // I am the owner,
                                OSSIM_NORMALIZED_FLOAT,
                                1);
   theTile->initialize();
}

ossimBandClipFilter::ossimBandClipFilter(ossimImageSource* inputSource,
                                           const vector<double>& minPix,
                                           const vector<double>& maxPix,
                                           ossimBandClipType clipType)
   :ossimImageSourceFilter(inputSource),
    theClipType(clipType),
    theTile(NULL)
{
   theTile = new ossimImageData(this, // I am the owner,
                                OSSIM_NORMALIZED_FLOAT,
                                1);
   theTile->initialize();
   setMinMaxPix(minPix, maxPix);
}

ossimBandClipFilter::ossimBandClipFilter(ossimImageSource* inputSource,
                                         double minPix,
                                         double maxPix,
                                         ossimBandClipType clipType)
   :ossimImageSourceFilter(inputSource),
    theClipType(clipType),
    theTile(NULL)
{
   theTile = new ossimImageData(this, // I am the owner,
                                OSSIM_NORMALIZED_FLOAT,
                                1);
   theTile->initialize();
   
   theMinPix.push_back(minPix);
   theMaxPix.push_back(maxPix);
}

ossimBandClipFilter::~ossimBandClipFilter()
{
}

void ossimBandClipFilter::setClipType(ossimBandClipType clipType)
{
   theClipType = clipType;
}

ossimBandClipFilter::ossimBandClipType ossimBandClipFilter::getClipType()const
{
   return theClipType;
}

ossim_uint32 ossimBandClipFilter::getNumberOfValues()const
{
   // We make sure that all arrays have the same number
   // of elements so we can use the size of any of them.
   return (ossim_uint32)theMinPix.size();
}

void ossimBandClipFilter::setNumberOfValues(ossim_uint32 size)
{

   // lets do a non destructive set.  That way we can
   // grow or shrink the list without destroying
   // the original values.
   //
   if(!size)
   {
      theMinPix.clear();
      theMaxPix.clear();
      theMedian.clear();
      return;
   }

   
   vector<double> tempMin    = theMinPix;
   vector<double> tempMax    = theMaxPix;
   
   theMinPix.resize(size);
   theMaxPix.resize(size);
   theMedian.resize(size);
    
   ossim_uint32 upperBound = (ossim_uint32)min(theMinPix.size(), tempMin.size());
   ossim_uint32 index = 0;

   for(index = 0; index < upperBound; ++index)
   {
      theMinPix[index] = tempMin[index];
      theMaxPix[index] = tempMax[index];
      
      theMinPix[index] = theMinPix[index]>1?1:theMinPix[index];
      theMinPix[index] = theMinPix[index]<0?0:theMinPix[index];
      theMaxPix[index] = theMaxPix[index]>1?1:theMaxPix[index];
      theMaxPix[index] = theMaxPix[index]<0?0:theMaxPix[index];
      
      theMedian[index] = (theMaxPix[index] + theMinPix[index])/2.0;
   }
   
   for(;index < theMinPix.size();++index)
   {
      theMinPix[index] = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
      theMaxPix[index] = OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
      theMedian[index] = (theMaxPix[index] + theMinPix[index])/2.0;
            
      theMedian[index] = (theMaxPix[index] + theMinPix[index])/2.0;
   }
}

void ossimBandClipFilter::setMinMaxPix(const vector<double>& minPix,
                                       const vector<double>& maxPix)
{
   theMinPix = minPix;
   theMaxPix = maxPix;

   setNumberOfValues((ossim_uint32)max(theMinPix.size(), theMaxPix.size()));
}

const std::vector<double>& ossimBandClipFilter::getMinPixList()const
{
   return theMinPix;
}

const std::vector<double> ossimBandClipFilter::getMaxPixList()const
{
   return theMaxPix;
}

double ossimBandClipFilter::getMinPix(ossim_uint32 index)const
{
   return index<theMinPix.size()?theMinPix[index]:0;
}

double ossimBandClipFilter::getMaxPix(ossim_uint32 index)const
{
   return index<theMaxPix.size()?theMaxPix[index]:0;
}

ossimRefPtr<ossimImageData> ossimBandClipFilter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }
   
   ossimRefPtr<ossimImageData> data =
      theInputConnection->getTile(rect, resLevel);
   if(!data.get())
   {
      return data;
   }
   ossimDataObjectStatus status = data->getDataObjectStatus();
   if((status != OSSIM_NULL) &&
      (status != OSSIM_EMPTY))
   {
      ossim_uint32 dw = data->getWidth();
      ossim_uint32 dh = data->getHeight();
      ossim_uint32 db = data->getNumberOfBands();

      ossim_uint32 tw = theTile->getWidth();
      ossim_uint32 th = theTile->getHeight();
      ossim_uint32 tb = theTile->getNumberOfBands();

      if(((tw*th)!=(dw*dh))||
         (tb != db))
      {
         theTile = new ossimImageData(this,
                                      OSSIM_NORMALIZED_FLOAT,
                                      db,
                                      dw,
                                      dh);
         theTile->initialize();
      }

      if(getNumberOfValues() != theTile->getNumberOfBands())
      {
         // Should this go on??? (drb)
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimBandClipFilter::getTile\n"
            << "getNumberOfValues() != theTile->getNumberOfBands"
            << endl;
      }
      
      data->copyTileToNormalizedBuffer(static_cast<float*>(theTile->getBuf()));
      theTile->setDataObjectStatus(data->getDataObjectStatus());
      
      switch(theClipType)
      {
         case ossimBandClipType_CLIP:
         {
            runClip();
            break;
         }
         case ossimBandClipType_CLAMP:
         {
            runClamp();
            break;
         }
         case ossimBandClipType_LINEAR_STRETCH:
         {
            runLinearStretch();
            break;
         }
         case ossimBandClipType_MEDIAN_STRETCH:
         {
            runMedianStretch();
            break;
         }
         default:
            break;
      }
      data->copyNormalizedBufferToTile(static_cast<float*>(theTile->getBuf()));
   }
   
   return data;
}

void ossimBandClipFilter::runClip()
{
   ossim_uint32 offset = 0;
   ossim_uint32 upperBound = theTile->getWidth()*theTile->getHeight();
   for(ossim_uint32 band =0; band < theTile->getNumberOfBands(); ++band)
   {
      float *buf     = static_cast<float*>(theTile->getBuf(band));
      float  minPix  = theMinPix[band];
      float  maxPix  = theMaxPix[band];
      float  nullPix = theTile->getNullPix(band);
      
      if(buf)
      {
         for(offset = 0; offset < upperBound; ++offset)
         {
            if((*buf < minPix)||
               (*buf > maxPix))
            {
               *buf = nullPix;
            }
            ++buf;
         }
      }
   }
   theTile->validate();
}

void ossimBandClipFilter::runClamp()
{
   ossim_uint32 offset = 0;
   ossim_uint32 upperBound = theTile->getWidth()*theTile->getHeight();
   for(ossim_uint32 band =0; band < theTile->getNumberOfBands(); ++band)
   {
      float *buf     = static_cast<float*>(theTile->getBuf(band));
      float  minPix  = theMinPix[band];
      float  maxPix  = theMaxPix[band];
      float  nullPix = theTile->getNullPix(band);
      
      if(buf)
      {
         if(theTile->getDataObjectStatus() == OSSIM_PARTIAL)
         {
            for(offset = 0; offset < upperBound; ++offset)
            {
               if(*buf != nullPix)
               {
                  if(*buf < minPix)
                  {
                     *buf = minPix;
                  }
                  else if(*buf > maxPix)
                  {
                     *buf = maxPix;
                  }
               }
               ++buf;
            }
         }
         else
         {
            for(offset = 0; offset < upperBound; ++offset)
            {
               if(*buf < minPix)
               {
                  *buf = minPix;
               }
               else if(*buf > maxPix)
               {
                  *buf = maxPix;
               }
               ++buf;
            }
         }
      }
   }
}

void ossimBandClipFilter::runLinearStretch()
{   
   ossim_uint32 bands = (ossim_uint32)min(theTile->getNumberOfBands(), (ossim_uint32)theMinPix.size());
   ossim_uint32 w     = theTile->getWidth();
   ossim_uint32 h     = theTile->getHeight();
   ossim_uint32 upperBound = w*h;
   const double* minPixArray = theTile->getMinPix();
   const double* maxPixArray = theTile->getMaxPix();
      
   float* *bandArray = new float*[bands];
   ossim_uint32 band = 0;
   for(band = 0; band < bands; ++band)
   {
      bandArray[band] = static_cast<float*>(theTile->getBuf(band));
   }
   
   if(theTile->getDataObjectStatus() == OSSIM_PARTIAL)
   {
      for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
      {
         if(!theTile->isNull(offset))
         {
            for(band = 0; band < bands; ++band)
            {
               double delta = theMaxPix[band] - theMinPix[band];
               double t     = (bandArray[band][offset] - theMinPix[band])/delta;
               if(t < 0)
               {
                 bandArray[band][offset] = minPixArray[band];
               }
               else if(t > 1)
               {
                 bandArray[band][offset] = maxPixArray[band];
               }
               else
               {
                  bandArray[band][offset] = t;
               }
            }
         }
      }
   }
   else if(theTile->getDataObjectStatus() == OSSIM_FULL)
   {
      for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
         {
            for(band = 0; band < bands; ++band)
            {
               double delta = theMaxPix[band] - theMinPix[band];
               double t     = (bandArray[band][offset] - theMinPix[band])/delta;
               
              if(t < 0)
              {
                 bandArray[band][offset] = minPixArray[band];
              }
              else if(t > 1)
              {
                 bandArray[band][offset] = maxPixArray[band];
              }
              else
              {
                 bandArray[band][offset] = t;
              }
            }
         }
   }
   delete [] bandArray;
   
}

void ossimBandClipFilter::runMedianStretch()
{
   ossim_uint32 bands = theTile->getNumberOfBands();
   float* *bandArray = new float*[bands];
   ossim_uint32 band = 0;

   for(band = 0; band < bands; ++band)
   {
      bandArray[band] = static_cast<float*>(theTile->getBuf(band));
   }
   ossim_uint32 upperBound = theTile->getWidth()*theTile->getHeight();
   
   if(theTile->getDataObjectStatus() == OSSIM_PARTIAL)
   {
      for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
      {
         if(!theTile->isNull(offset))
         {
            for(band = 0; band < bands; ++band)
            {
               double side = (theMedian[band] - (double)bandArray[band][offset]);
               
               if(side > 0) // on the left
               {
                  double delta = fabs(theMedian[band] - theMinPix[band]);
                  double t     = ((double)bandArray[band][offset] - theMinPix[band])/delta;
                  if(t < 0)
                  {
                     bandArray[band][offset] = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
                  }
                  else if(t > 1)
                  {
                     bandArray[band][offset] = theMedian[band];
                  }
                  else
                  {
                     bandArray[band][offset] = theMedian[band]*t;
                  }
               }
               else // on the right
               {
                  double delta = theMaxPix[band] - theMedian[band];
                  double t = ((double)bandArray[band][offset] - theMedian[band])/delta;
                  
                  if(t < 0)
                  {
                     bandArray[band][offset] = theMedian[band];
                  }
                  else if(t > 1)
                  {
                     bandArray[band][offset] = OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
                  }
                  else
                  {
                     bandArray[band][offset] = theMedian[band]+delta*t;
                  }
               }
            }
         }
      }
   }
   else if(theTile->getDataObjectStatus() == OSSIM_FULL)
   {
      for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
      {
         for(band = 0; band < bands; ++band)
         {
            double side = (theMedian[band] - (double)bandArray[band][offset]);
            
            if(side > 0) // on the left
            {
               double delta = fabs(theMedian[band] - theMinPix[band]);
               double t     = ((double)bandArray[band][offset] - theMinPix[band])/delta;
               if(t < 0)
               {
                  bandArray[band][offset] = OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
               }
               else if(t > 1)
               {
                  bandArray[band][offset] = theMedian[band];
               }
               else
               {
                  bandArray[band][offset] = theMedian[band]*t;
               }
            }
            else // on the right
            {
               double delta = theMaxPix[band] - theMedian[band];
               double t = ((double)bandArray[band][offset] - theMedian[band])/delta;
               
               if(t < 0)
               {
                  bandArray[band][offset] = theMedian[band];
               }
               else if(t > 1)
               {
                  bandArray[band][offset] = OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
               }
               else
               {
                  bandArray[band][offset] = theMedian[band]+delta*t;
               }
            }
         }
      }
   }

   delete [] bandArray;
}   

void ossimBandClipFilter::initialize()
{
   ossimImageSourceFilter::initialize();
   
   if(theInputConnection)
   {
      ossim_uint32 bands = theInputConnection->getNumberOfOutputBands();

      if(bands)
      {
         if(theMinPix.size() != bands)
         {
            setNumberOfValues(bands);
         }
      }
   }
}

bool ossimBandClipFilter::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   bool result = ossimImageSourceFilter::loadState(kwl, prefix);

   if(result)
   {
      ossimString minRegExpression    = ossimString("^(") +
                                        ossimString(prefix) +
                                        "min[0-9]+)";
      ossimString maxRegExpression    = ossimString("^(") +
                                        ossimString(prefix) +
                                        "max[0-9]+)";
      ossimString medianRegExpression = ossimString("^(") +
                                        ossimString(prefix) +
                                        "median[0-9]+)";
      
      ossim_uint32 numberOfMins    = kwl.getNumberOfSubstringKeys(minRegExpression);
      ossim_uint32 numberOfMaxs    = kwl.getNumberOfSubstringKeys(maxRegExpression);
      ossim_uint32 numberOfMedians = kwl.getNumberOfSubstringKeys(medianRegExpression);
      theMinPix.clear();
      theMaxPix.clear();
      theMedian.clear();

      if(numberOfMins != numberOfMaxs)
      {
         ossim_int32 temp = (ossim_int32)min(numberOfMins, numberOfMaxs);
         numberOfMins = temp;
         numberOfMaxs = temp;
      }
      if(numberOfMins> 0)
      {
         ossimString prefixMin    = ossimString(prefix) + "min";
         ossimString prefixMax    = ossimString(prefix) + "max";
         ossimString prefixMedian = ossimString(prefix) + "median";
         
         ossim_uint32 numberOfMatches = 0;
         ossim_uint32 index = 0;
         const char* value=(const char*)NULL;
         
         while(numberOfMatches < numberOfMins)
         {
            value = kwl.find(prefixMin.c_str(),
                             ossimString::toString(index).c_str());
            if(value)
            {
               theMinPix.push_back(ossimString(value).toDouble());
               ++numberOfMatches;
            }
            ++index;
         }
         index = 0;
         numberOfMatches = 0;
         while(numberOfMatches < numberOfMaxs)
         {
            value = kwl.find(prefixMax.c_str(),
                             ossimString::toString(index).c_str());
            if(value)
            {
               theMaxPix.push_back(ossimString(value).toDouble());
               ++numberOfMatches;
            }
            ++index;
         }
         if(numberOfMedians != numberOfMins)
         {
            for(index = 0; index <  theMaxPix.size(); ++index)
            {
               theMedian.push_back((theMinPix[index]+theMaxPix[index])/2.0);
            }
         }
         else
         {
            index = 0;
            numberOfMatches = 0;
            while(numberOfMatches < numberOfMedians)
            {
               value = kwl.find(prefixMedian.c_str(),
                                ossimString::toString(index).c_str());
               if(value)
               {
                  theMedian.push_back(ossimString(value).toDouble());
                  ++numberOfMatches;
               }
               ++index;
            }
         }
      }
   }
   const char* clip_type = kwl.find(prefix, "clip_type");
   if(clip_type)
   {
     ossimString clipType = clip_type;
      clipType = clipType.upcase().trim();

      if(clipType == "NONE")
      {
         theClipType = ossimBandClipType_NONE;
      }
      else if(clipType == "CLIP")
      {
         theClipType = ossimBandClipType_CLIP;
      }
      else if(clipType == "LINEAR_STRETCH")
      {
         theClipType = ossimBandClipType_LINEAR_STRETCH;
      }
      else if(clipType == "MEDIAN_STRETCH")
      {
         theClipType = ossimBandClipType_MEDIAN_STRETCH;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "ossimBandClipFilter::loadState NOTICE:\n"
         << "Clip type not given defaulting to NONE" << endl;
      theClipType = ossimBandClipType_NONE;
   }

   return result;
}

bool ossimBandClipFilter::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   ossimString minPrefix    = ossimString("min");
   ossimString maxPrefix    = ossimString("max");
   ossimString medianPrefix = ossimString("median");

   for(ossim_uint32 index = 1; index <= getNumberOfValues(); ++index)
   {
      ossimString value = (minPrefix+ossimString::toString(index));
      kwl.add(prefix,
              value.c_str(),
              theMinPix[index-1],
              true);
      value = (maxPrefix+ossimString::toString(index));
      kwl.add(prefix,
              value.c_str(),
              theMaxPix[index-1],
              true);
      value = (medianPrefix+ossimString::toString(index));
      kwl.add(prefix,
              value.c_str(),
              theMedian[index-1],
              true);
   }
   ossimString clipType;
   switch(theClipType)
   {
      case ossimBandClipType_NONE:
      {
         clipType = "NONE";
         break;
      }
      case ossimBandClipType_CLIP:
      {
         clipType = "CLIP";
         break;
      }
      case ossimBandClipType_LINEAR_STRETCH:
      {
         clipType = "LINEAR_STRETCH";
         break;
      }
      case ossimBandClipType_MEDIAN_STRETCH:
      {
         clipType = "MEDIAN_STRETCH";
         break;
      }
      default:
         break;
   }
   kwl.add(prefix,
           "clip_type",
           clipType.c_str(),
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

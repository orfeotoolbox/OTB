//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimMultiBandHistogramTileSource.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/imaging/ossimMultiBandHistogramTileSource.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <algorithm>

static const char* MIN_VALUE_PERCENT_KW = "min_percent";
static const char* MAX_VALUE_PERCENT_KW = "max_percent";
static const char* ALGORITHM_KW         = "algorithm";

RTTI_DEF1(ossimMultiBandHistogramTileSource,
          "ossimMultiBandHistogramTileSource",
          ossimImageSourceHistogramFilter)
   
ossimMultiBandHistogramTileSource::ossimMultiBandHistogramTileSource()
   : ossimImageSourceHistogramFilter(),
     theTile(0),
     theAlgorithm(OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM)
{
   disableSource();
}

ossimMultiBandHistogramTileSource::ossimMultiBandHistogramTileSource(
   double /* minValuePercent */,
   double /* maxValuePercent */,
   ossimImageSource* inputSource,
   ossimMultiResLevelHistogram* histogram)
   : ossimImageSourceHistogramFilter(inputSource, histogram),
     theTile(0),
     theAlgorithm(OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM)
{
   disableSource();
}

ossimMultiBandHistogramTileSource::~ossimMultiBandHistogramTileSource()
{
}

ossimRefPtr<ossimImageData> ossimMultiBandHistogramTileSource::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection) return 0;
   
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(tileRect,
                                                                       resLevel);
   
   if(!isSourceEnabled() || !inputTile.valid())
   {
      return inputTile;
   }

   if(!theTile.valid())
   {
      allocate(); // First time through...
   }
   
   if( !theTile.valid() ||
       !theMinValuePercentArray.size() ||
       !theMaxValuePercentArray.size() )
   {
      return inputTile;
   }
   
   theTile->setImageRectangle(tileRect);
   if(inputTile->getBuf()&&
      (inputTile->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      theTile->loadTile(inputTile.get());
      switch(inputTile->getScalarType())
      {
         case OSSIM_UCHAR:
         {
            return runLinearStretchAlgorithm((ossim_uint8)0,
                                             theTile);
         }
         case OSSIM_USHORT16:
         case OSSIM_USHORT11:
         {
            return runLinearStretchAlgorithm((ossim_uint16)0,
                                             theTile);
         }
         case OSSIM_SSHORT16:
         {
            return runLinearStretchAlgorithm(static_cast<ossim_sint16>(0),
                                             theTile);
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            return runLinearStretchAlgorithm(static_cast<double>(0),
                                             theTile);
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            return runLinearStretchAlgorithm(static_cast<float>(0),
                                             theTile);
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            ossimSetError("ossimMultiBandHistogramTileSource",
                          ossimErrorCodes::OSSIM_ERROR,
                          "Unknown scalar type");
            break;
         }
      }
      theTile->validate();
      return theTile;
   }
   
   return inputTile;
}

void ossimMultiBandHistogramTileSource::initialize()
{
   ossimImageSourceHistogramFilter::initialize();

   // Force an allocate on the next getTile.
   theTile = 0;
}

void ossimMultiBandHistogramTileSource::allocate()
{
   theTile = ossimImageDataFactory::instance()->create(this, this);

   if(theTile.valid())
   {
      theTile->initialize();

      ossim_uint32 numberOfBands = theTile->getNumberOfBands();
      ossim_uint32 i = 0;
      if(numberOfBands)
      {
         std::vector<double> copyArray;
         
         if(numberOfBands > theMinValuePercentArray.size())
         {
            copyArray = theMinValuePercentArray;
            theMinValuePercentArray.resize(numberOfBands);
            if(numberOfBands < copyArray.size())
            {
               std::copy(copyArray.begin(),
                         copyArray.end(),
                         theMinValuePercentArray.begin());
            }
            else
            {
               std::copy(copyArray.begin(),
                         copyArray.begin() + copyArray.size(),
                         theMinValuePercentArray.begin());
            }
         }
         if(numberOfBands != theMaxValuePercentArray.size())
         {
            copyArray = theMaxValuePercentArray;
            theMaxValuePercentArray.resize(numberOfBands);
            if(numberOfBands < copyArray.size())
            {
               std::copy(copyArray.begin(),
                         copyArray.end(),
                         theMaxValuePercentArray.begin());
            }
            else
            {
               std::copy(copyArray.begin(),
                         copyArray.begin() + copyArray.size(),
                         theMaxValuePercentArray.begin());
            }
         }
         if(numberOfBands > theMinValuePercentArray.size())
         {
            for(i = (ossim_uint32)theMinValuePercentArray.size(); i < numberOfBands; ++i)
            {
               theMinValuePercentArray[i] = 0.0;
               theMaxValuePercentArray[i] = 0.0;
            }
         }
      }
   }
}

bool ossimMultiBandHistogramTileSource::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   theMinValuePercentArray.clear();
   theMaxValuePercentArray.clear();
   if(ossimImageSourceHistogramFilter::loadState(kwl, prefix))
   {
      ossimNotify(ossimNotifyLevel_INFO) << "ossimMultiBandHistogramTileSource::loadState INFO: histogram filter was loaded" << std::endl;
      ossimString regExpression = ossimString("^(") + ossimString(prefix) + ossimString(MIN_VALUE_PERCENT_KW) + ossimString("[0-9]+)");

      long numberOfSources = kwl.getNumberOfSubstringKeys(regExpression);
      
      unsigned long numberOfMatches = 0;
      ossim_uint32 i = 0;
      while(numberOfMatches < static_cast<unsigned long>(numberOfSources))
      {
         ossimString key = ossimString(MIN_VALUE_PERCENT_KW) + ossimString::toString(i);
         const char* value = kwl.find(prefix, key); 
         if(value)
         {
            theMinValuePercentArray.push_back(ossimString(value).toDouble());
            ++numberOfMatches;
         }      
         ++i;
      }

      numberOfMatches = 0;
      i               = 0;
      regExpression = ossimString("^(") + ossimString(prefix) + ossimString(MAX_VALUE_PERCENT_KW) + ossimString("[0-9]+)");
      numberOfSources = kwl.getNumberOfSubstringKeys(regExpression);
      while(numberOfMatches < static_cast<unsigned long>(numberOfSources))
      {
         ossimString key = ossimString(MAX_VALUE_PERCENT_KW) + ossimString::toString(i);
         const char* value = kwl.find(prefix, key); 

         if(value)
         {
            theMaxValuePercentArray.push_back(ossimString(value).toDouble());
            ++numberOfMatches;
         }      
         ++i;
      }

      ossimString algorithm = kwl.find(prefix,
                                       ALGORITHM_KW);
      algorithm = algorithm.downcase();
      theAlgorithm = OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM;
      if(algorithm != "")
      {
         if(algorithm.contains("linear"))
         {
           theAlgorithm = OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM;
         }
      }
   }
   return true;
}

bool ossimMultiBandHistogramTileSource::saveState(ossimKeywordlist& kwl,
                                           const char* prefix)const
{
   ossimImageSourceHistogramFilter::saveState(kwl, prefix);
   ossim_int32 i = 0;

   for(i = 0; i< (ossim_int32)theMinValuePercentArray.size();++i)
   {
      kwl.add(prefix,
              ossimString(MIN_VALUE_PERCENT_KW) + ossimString::toString(i),
              theMinValuePercentArray[i],
              true);
   }
   for(i = 0; i< (ossim_int32)theMaxValuePercentArray.size();++i)
   {
      kwl.add(prefix,
              ossimString(MAX_VALUE_PERCENT_KW) + ossimString::toString(i),
              theMaxValuePercentArray[i],
              true);
   }
   ossimString algorithmString;

   switch(theAlgorithm)
   {
   case OSSIM_HISTOGRAM_LINEAR_STRETCH_ALGORITHM:
   {
      algorithmString = "linear_stretch";
      break;
   }
   case OSSIM_HISTOGRAM_NO_ALGORITHM:
   {
      algorithmString = "none";
      break;
   }
   default:
   {
      algorithmString = "none";
      break;
   }
   }

   kwl.add(prefix,
           ALGORITHM_KW,
           algorithmString.c_str(),
           true);
   
   return true;
}

template <class T>
ossimRefPtr<ossimImageData> ossimMultiBandHistogramTileSource::runLinearStretchAlgorithm(
   T /* dummyVariable */, ossimRefPtr<ossimImageData>& tile)
{
   if(!getHistogram())
   {
      return tile;
   }

   ossimRefPtr<ossimMultiBandHistogram> histo    = getHistogram()->getMultiBandHistogram(theCurrentResLevel);
   if(histo.valid())
   {
      ossim_uint32 maxBands = ( (histo->getNumberOfBands() >
                           tile->getNumberOfBands())?
                          tile->getNumberOfBands():
                          histo->getNumberOfBands());

      long offsetUpperBound = tile->getHeight()*tile->getWidth();

      for(ossim_uint32 band = 0; band < maxBands; ++band)
      {
         ossimRefPtr<ossimHistogram> h  = histo->getHistogram(band);
         
         T* buf   = static_cast<T*>(tile->getBuf(band));
         T np     = static_cast<T>(tile->getNullPix(band));
         T minPix = static_cast<T>(tile->getMinPix(band));
         T maxPix = static_cast<T>(tile->getMaxPix(band));
         T range  = (maxPix - minPix);
         if(h.valid()&&buf)
         {
            
            double maxClip = minPix + (h->HighClipVal(theMaxValuePercentArray[band])/(double)h->GetRes())*range;
            double minClip = minPix + (h->LowClipVal(theMinValuePercentArray[band])/(double)h->GetRes())*range;
            double delta   = (maxClip - minClip);
            
            if(fabs(delta) > 0.0)
            {
               for(long offset = 0; offset < offsetUpperBound; ++offset)
               {
                  if(buf[offset] != np)
                  {
                     if(buf[offset] < minClip)
                     {
                        buf[offset] = minPix;
                     }
                     else if(buf[offset] > maxClip)
                     {
                        buf[offset] = maxPix;
                     }
                     else
                     {
                        T value = static_cast<T>(minPix + ((buf[offset] - minClip)/delta)*range);
                        buf[offset] = value>maxPix?maxPix:value;
                     }
                  }
               }
            }
         }
      }

      tile->validate();
   }
   
   return tile;
}

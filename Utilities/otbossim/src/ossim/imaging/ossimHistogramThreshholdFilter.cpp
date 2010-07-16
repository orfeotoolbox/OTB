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
// $Id: ossimHistogramThreshholdFilter.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/imaging/ossimHistogramThreshholdFilter.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimErrorContext.h>

static const ossimKeyword MIN_VALUE_PERCENT_KW("min_percent",
                                               "min percentage clip");

static const ossimKeyword MAX_VALUE_PERCENT_KW("max_percent",
                                               "max percentage value");


RTTI_DEF1(ossimHistogramThreshholdFilter, "ossimHistogramThreshholdFilter", ossimImageSourceHistogramFilter)
   
ossimHistogramThreshholdFilter::ossimHistogramThreshholdFilter()
   : ossimImageSourceHistogramFilter(),
     theMinValuePercent(0.0),
     theMaxValuePercent(0.0)
{
}

ossimHistogramThreshholdFilter::ossimHistogramThreshholdFilter(double minValuePercent,
                                                               double maxValuePercent,
                                                               ossimImageSource* inputSource,
                                                               ossimMultiResLevelHistogram* histogram)
   : ossimImageSourceHistogramFilter(inputSource, histogram),
     theMinValuePercent(minValuePercent),
     theMaxValuePercent(maxValuePercent)
{
}

ossimHistogramThreshholdFilter::~ossimHistogramThreshholdFilter()
{
}

ossimRefPtr<ossimImageData> ossimHistogramThreshholdFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection) return NULL;
   
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tileRect, resLevel);

   if(!isSourceEnabled())
   {
      return inputTile;
   }
   if(inputTile.valid()  &&
      inputTile->getBuf() &&
      (inputTile->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      switch(inputTile->getScalarType())
      {
         case OSSIM_UCHAR:
         {
            return runThreshholdStretchAlgorithm((ossim_uint8)0,
                                                 inputTile);
         }
         case OSSIM_USHORT16:
         case OSSIM_USHORT11:
         {
            return runThreshholdStretchAlgorithm((ossim_uint16)0,
                                                 inputTile);
         }
         case OSSIM_SSHORT16:
         {
            return runThreshholdStretchAlgorithm(static_cast<ossim_sint16>(0),
                                                 inputTile);
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            return runThreshholdStretchAlgorithm(static_cast<double>(0),
                                                 inputTile);
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            return runThreshholdStretchAlgorithm(static_cast<float>(0),
                                                 inputTile);
         }
         case OSSIM_SCALAR_UNKNOWN:
         default:
         {
            ossimSetError("ossimHistogramThreshholdFilter",
                          ossimErrorCodes::OSSIM_ERROR,
                          "Unknown scalar type");
            break;
         }
      }
   }

   return inputTile;
}


bool ossimHistogramThreshholdFilter::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   if(ossimImageSourceHistogramFilter::loadState(kwl, prefix))
   {
      const char* minPercent = kwl.find(prefix, MIN_VALUE_PERCENT_KW);
      const char* maxPercent = kwl.find(prefix, MAX_VALUE_PERCENT_KW);

      if(minPercent)
      {
         theMinValuePercent = ossimString(minPercent).toDouble();
      }
      else
      {
         theMinValuePercent = 0.0;
      }
      if(maxPercent)
      {
         theMaxValuePercent = ossimString(maxPercent).toDouble();
      }
      else
      {
         theMaxValuePercent = 0.0;
      }
   }
   else
   {
      return false;
   }

   return true;
}

bool ossimHistogramThreshholdFilter::saveState(ossimKeywordlist& kwl,
                                           const char* prefix)const
{
   if(ossimImageSourceHistogramFilter::saveState(kwl, prefix))
   {
      kwl.add(prefix,
              MIN_VALUE_PERCENT_KW,
              theMinValuePercent,
              true);
      kwl.add(prefix,
              MAX_VALUE_PERCENT_KW,
              theMaxValuePercent,
              true);
   }
   else
   {
      return false;
   }

   return true;
}

void ossimHistogramThreshholdFilter::initialize()
{
   ossimImageSourceHistogramFilter::initialize();
}

template <class T>
ossimRefPtr<ossimImageData> ossimHistogramThreshholdFilter::runThreshholdStretchAlgorithm(
   T /* dummyVariable */,
   ossimRefPtr<ossimImageData>& tile)
{
   if(!getHistogram())
   {
      return tile;
   }
   
   ossimRefPtr<ossimMultiBandHistogram> histo    = getHistogram()->getMultiBandHistogram(0);
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
         
         if(h.valid()&&buf)
         {
            T np     = static_cast<T>(tile->getNullPix(band));
            T minPix = static_cast<T>(tile->getMinPix(band));
            T maxPix = static_cast<T>(tile->getMaxPix(band));
            double range  = (maxPix - minPix);
            double maxClip = (h->HighClipVal(theMaxValuePercent/100.0)/(double)h->GetRes());
            double minClip = (h->LowClipVal(theMinValuePercent/100.0)/(double)h->GetRes());
            ossim_float64 normPix;
            double delta   = fabs(maxClip - minClip);
            if(delta > 0.0)
            {
               for(long offset = 0; offset < offsetUpperBound; ++offset)
               {
                  if(buf[offset] != np)
                  {
                     normPix = ((double)buf[offset]-minPix)/range;
                     if(normPix <= minClip)
                     {
                        buf[offset] = minPix;
                     }
                     else if(normPix >= maxClip)
                     {
                        buf[offset] = maxPix;
                     }
                     else 
                     {
                        double t = (normPix - minClip)/delta;
                        T value = static_cast<T>(minPix + range*t);
                        buf[offset] = value;
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

void ossimHistogramThreshholdFilter::setMinValuePercent(double minValue)
{
   theMinValuePercent = minValue;
}

void ossimHistogramThreshholdFilter::setMaxValuePercent(double maxValue)
{
   theMaxValuePercent = maxValue;
}

double ossimHistogramThreshholdFilter::getMinValuePercent()const
{
   return theMinValuePercent;
}

double ossimHistogramThreshholdFilter::getMaxValuePercent()const
{
   return theMaxValuePercent;
}

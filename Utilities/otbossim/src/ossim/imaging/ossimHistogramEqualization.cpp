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
// $Id: ossimHistogramEqualization.cpp 14110 2009-03-14 15:46:40Z dburken $
#include <ossim/imaging/ossimHistogramEqualization.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeyword.h>

RTTI_DEF1(ossimHistogramEqualization, "ossimHistogramEqualization", ossimImageSourceHistogramFilter)
   
static const ossimKeyword HISTOGRAM_INVERSE_FLAG_KW("inverse_flag",
                                                    "specifies whether the inverse should be done for the getTile");

static ossimTrace traceDebug("ossimHistogramEqualization:debug");

ossimHistogramEqualization::ossimHistogramEqualization()
   : ossimImageSourceHistogramFilter(),
     theTile(NULL),
     theAccumulationHistogram(NULL),
     theInverseFlag(false)
{
}

ossimHistogramEqualization::ossimHistogramEqualization(ossimImageSource* inputSource,
                                                       ossimRefPtr<ossimMultiResLevelHistogram> histogram)
   : ossimImageSourceHistogramFilter(inputSource, histogram),
     theTile(0),
     theAccumulationHistogram(0),
     theInverseFlag(false)
{
   if(getHistogram().valid())
   {
      theAccumulationHistogram = getHistogram()->createAccumulationLessThanEqual();
      initializeLuts();
   }
}

ossimHistogramEqualization::ossimHistogramEqualization(ossimImageSource* inputSource,
                                                       ossimRefPtr<ossimMultiResLevelHistogram> histogram,
                                                       bool inverseFlag)
   : ossimImageSourceHistogramFilter(inputSource, histogram),
     theTile(NULL),
     theAccumulationHistogram(0),
     theInverseFlag(inverseFlag)
{
   if(getHistogram().valid())
   {
      theAccumulationHistogram = getHistogram()->createAccumulationLessThanEqual();
      initializeLuts();
   }
}

ossimHistogramEqualization::~ossimHistogramEqualization()
{
   deleteLuts();
}

ossimRefPtr<ossimImageData> ossimHistogramEqualization::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }
   
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(tileRect,
                                                                       resLevel);

   if(!inputTile.valid()) return inputTile;
   
   if (!isSourceEnabled())
   {
      return inputTile;
   }
   
   if(!theTile.valid())
   {
      allocate(); // First time through...
   }
   
   // if (!theTile) throw exception!
   
   // Set the origin, resize if needed of the output tile.
   theTile->setImageRectangleAndBands(inputTile->getImageRectangle(),
				      inputTile->getNumberOfBands());
   
   if( !inputTile.valid() ||
       inputTile->getDataObjectStatus() == OSSIM_NULL ||
       inputTile->getDataObjectStatus() == OSSIM_EMPTY )
   {
      return theTile;
   }

   if(!theAccumulationHistogram)
   {
      computeAccumulationHistogram();
   }
   if(theForwardLut.size() == 0 ||
      theInverseLut.size() == 0)
   {
      initializeLuts();
   }
   if((theForwardLut.size() == 0) ||
      (theInverseLut.size() == 0))
   {
      return inputTile;
   }
   
   switch(inputTile->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         return runEqualizationAlgorithm(static_cast<ossim_uint8>(0),
                                         inputTile);
      }
      case OSSIM_USHORT11:
      case OSSIM_UINT16:
      {
         return runEqualizationAlgorithm(static_cast<ossim_uint16>(0),
                                         inputTile);
      }
      case OSSIM_SINT16:
      {
         return runEqualizationAlgorithm(static_cast<ossim_sint16>(0),
                                         inputTile);
      }
      case OSSIM_UINT32:
      {
         return runEqualizationAlgorithm(static_cast<ossim_uint32>(0),
                                         inputTile);
      }
      case OSSIM_SINT32:
      {
         return runEqualizationAlgorithm(static_cast<ossim_sint32>(0),
                                         inputTile);
      }
      case OSSIM_FLOAT32:
      {
         return runEqualizationAlgorithm(static_cast<ossim_float32>(0),
                                         inputTile);
      }
      case OSSIM_FLOAT64:
      {
         return runEqualizationAlgorithm(static_cast<ossim_float64>(0),
                                         inputTile);
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimHistogramEqualization::getTile WARNING: Unsupported scalar type."
            << endl;
         break;
      }
   }
   
   return inputTile;
}

void ossimHistogramEqualization::setHistogram(ossimRefPtr<ossimMultiResLevelHistogram> histogram)
{
   ossimImageSourceHistogramFilter::setHistogram(histogram);
   computeAccumulationHistogram();
   initializeLuts();
}

bool ossimHistogramEqualization::saveState(ossimKeywordlist& kwl,
                                           const char* prefix)const
{
   bool result = ossimImageSourceHistogramFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           HISTOGRAM_INVERSE_FLAG_KW,
           theInverseFlag,
           true);
   
   return result;
}

bool ossimHistogramEqualization::loadState(const ossimKeywordlist& kwl,
                                           const char* prefix)
{
   static const char MODULE[] = "ossimHistogramEqualization::loadState";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << " Entered..."
         << "\nprefix:  " << prefix << endl;
   }

   const char* lookup = kwl.find(prefix,
                                 HISTOGRAM_INVERSE_FLAG_KW);
   if(lookup)
   {
      theInverseFlag = ossimString(lookup).toBool();
   }
   if(ossimImageSourceHistogramFilter::loadState(kwl, prefix))
   {
//      computeAccumulationHistogram();
//      initializeLuts();
   }
   else
   {
      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:" << MODULE;
      this->print(ossimNotify(ossimNotifyLevel_DEBUG));
      ossimNotify(ossimNotifyLevel_DEBUG) << "\nExited..." << endl;
   }
   
   return true;
}

void ossimHistogramEqualization::computeAccumulationHistogram()
{
   if(getHistogram().valid())
   {
      theAccumulationHistogram = getHistogram()->createAccumulationLessThanEqual();
   }
}

template <class T>
ossimRefPtr<ossimImageData> ossimHistogramEqualization::runEqualizationAlgorithm(T, ossimRefPtr<ossimImageData> tile)
{
   
   if(!theAccumulationHistogram ||
      !getHistogram())
   {
      return tile;
   }

   // for now we will always pull from res 0 information
   ossimRefPtr<ossimMultiBandHistogram> histo = getHistogram()->getMultiBandHistogram(0);

   if(histo.valid())
   {
      ossim_uint32 maxBands = ( (histo->getNumberOfBands() >
                                 tile->getNumberOfBands())?
                                tile->getNumberOfBands():
                                histo->getNumberOfBands());
      
      long offsetUpperBound = tile->getHeight()*tile->getWidth();

      for(ossim_uint32 band = 0; band < maxBands; ++band)
      {
         ossimRefPtr<ossimHistogram> bandHisto = histo->getHistogram(band);
         T* buf = static_cast<T*>(tile->getBuf(band));
         double *histoLut = band<theForwardLut.size()?theForwardLut[band]:NULL;
         ossim_uint32 actualBand = theBandList[band];
         if(bandHisto.valid())
         {
            if(buf&&histoLut&&(actualBand <  histo->getNumberOfBands()))
            {
               if(theInverseFlag)
               {
                  histoLut = theInverseLut[actualBand];
               }
               if(histoLut)
               {
                  if(tile->getDataObjectStatus() == OSSIM_FULL)
                  {
                     T minPix = (T)tile->getMinPix(actualBand);
                     T maxPix = (T)tile->getMaxPix(actualBand);
                     for(long offset = 0; offset < offsetUpperBound; ++offset)
                     {
                        ossim_int32 idx = bandHisto->GetIndex(buf[offset]);
                           
                        if(idx>=0)
                        {
                           T value = (T)(histoLut[idx]);

                           //---
                           // Assign clamping to min max.
                           // 
                           // ESH 03/2009 -- Clamping to within min-max fixed
                           //--- 
                           buf[offset] = value < minPix ? minPix :
                              (value > maxPix ? maxPix : value);
                        }
                     }
                  }
                  else
                  {
                     T minPix  = (T)tile->getMinPix(actualBand);
                     T maxPix  = (T)tile->getMaxPix(actualBand);
                     T nullPix = (T)tile->getNullPix(actualBand);
                     for(long offset = 0; offset < offsetUpperBound; ++offset)
                     {
                        ossim_int32 idx = bandHisto->GetIndex(buf[offset]);
                        
                        if((buf[offset]!=nullPix)&&(idx>=0))
                        {
                           T value = (T)(histoLut[idx]);

                           //---
                           // Assign clamping to min max.
                           // 
                           // ESH 03/2009 -- Clamping to within min-max fixed
                           //--- 
                           buf[offset] = value < minPix ? minPix :
                              (value > maxPix ? maxPix : value);
                        }
                        else
                        {
                           buf[offset] = nullPix;
                        }
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

void ossimHistogramEqualization::initialize()
{
   // Base class will recapture "theInputConnection".
   ossimImageSourceHistogramFilter::initialize();
   if(theTile.valid())
   {
      theTile = NULL;
   }
}

void ossimHistogramEqualization::allocate()
{
   theBandList.clear();
   
   if(theTile.valid())
   {
      theTile = NULL;
   }
   
   if(theInputConnection)
   {
      theInputConnection->getOutputBandList(theBandList);
      theTile = ossimImageDataFactory::instance()->create(this, this);
      if(theTile.valid())
      {
         theTile->initialize();
      }
   }
}

void ossimHistogramEqualization::initializeLuts()
{
   if(theForwardLut.size() != 0 ||
      theInverseLut.size() != 0)
   {
      deleteLuts();
   }
   if(!theAccumulationHistogram||!getHistogram())
   {
      return;
   }
   ossimRefPtr<ossimMultiBandHistogram> accumHisto = theAccumulationHistogram->getMultiBandHistogram(0);
   ossimRefPtr<ossimMultiBandHistogram> histogram  = getHistogram()->getMultiBandHistogram(0);
   ossimKeywordlist kwl;
   theAccumulationHistogram->saveState(kwl);

   if(accumHisto.valid()&&histogram.valid())
   {
      long maxBands = accumHisto->getNumberOfBands();
      
      for(long band = 0; band < maxBands; ++band)
      {
         // first we grab pointers to the histogram and the accumulation
         // histogram
         ossimRefPtr<ossimHistogram> h  = accumHisto->getHistogram(band);
         ossimRefPtr<ossimHistogram> h2 = histogram->getHistogram(band);

         if(h.valid()&&h2.valid())
         {
            // lets get the number of indices.
            ossim_uint32 numberOfIndices = (ossim_uint32)h2->GetRes();
            vector<double> countForInverse(numberOfIndices);
            
            theForwardLut.push_back(new double[numberOfIndices]);
            theInverseLut.push_back(new double[numberOfIndices]);
            
            // let's grab the counts array
            const float* histoCounts = h->GetCounts();

//         double maxIntensity      = h2->GetMaxVal();
            double maxIntensity      = h2->GetRangeMax();
            double maxCount          = h->GetMaxCount();
            
            // now pre compute the transforms
            double *forwardLut = theForwardLut[band];
            double *inverseLut = theInverseLut[band];
            
//         double minIntensity  = h2->GetMinVal();
            double minIntensity  = h2->GetRangeMin();
            double delta = maxIntensity-minIntensity;
            ossim_uint32 idx = 0;
            
            // clear out the inverse
            //
            for(idx = 0; idx < numberOfIndices; ++ idx)
            {
               inverseLut[idx] = ossim::nan();
            }
            for(idx = 0; idx < numberOfIndices; ++ idx)
            {
               forwardLut[idx]   = minIntensity + (histoCounts[idx]/maxCount)*delta;
               
               ossim_int32 inverseIdx = h2->GetIndex(forwardLut[idx]);
               if(inverseIdx >= 0)
               {
                  inverseLut[inverseIdx]   = minIntensity + delta*(idx/(ossim_float32)numberOfIndices);
               }
            }
            
            // now solve the inverse lut
            //
            ossim_uint32 idxStart = 0;
            ossim_uint32 idxEnd = 0;
            
            while((ossim::isnan(inverseLut[idxEnd]))&&(idxEnd <numberOfIndices)){ ++idxEnd;}
            if((idxStart!=idxEnd)&&(idxEnd<numberOfIndices))
            {
               std::fill(inverseLut,
                         inverseLut+idxEnd,
                         inverseLut[idxEnd]);
            }
            idxStart = numberOfIndices-1;
            while((ossim::isnan(inverseLut[idxStart]))&&(idxStart > 0)){ --idxStart;}
            if(idxStart !=0)
            {
               std::fill(inverseLut+idxStart,
                         inverseLut+numberOfIndices,
                         inverseLut[idxStart]);
            }
            
            idxStart = 0;
            idxEnd   = 0;
            ossim_float32 valueStart = 0.0;
            ossim_float32 valueEnd = 0.0;
            
            while(idxStart < numberOfIndices)
            {
               idxEnd = idxStart;
               if(ossim::isnan(inverseLut[idxStart]))
               {
                  while(ossim::isnan(inverseLut[idxEnd])&&(idxEnd < (numberOfIndices-1))) ++idxEnd;
                  double length = (idxEnd-idxStart)+1;
                  valueEnd = inverseLut[idxEnd];
                  double deltaVal = (valueEnd-valueStart);
                  ossim_uint32 tempIdx = idxStart;
                  ossim_float32 count = 1.0;
                  double t = 0.0;
                  while(tempIdx < idxEnd)
                  {
                     t = (count/length);
                     t = t>1.0?1.0:t;
                     inverseLut[tempIdx] = valueStart + deltaVal*t;
                     ++count;
                     ++tempIdx;
                  }
                  idxStart = idxEnd;
                  valueStart = valueEnd;
               }
               else
               {
                  valueStart = inverseLut[idxStart];
                  ++idxStart;
               }
            }
         }
      }
   }
}

bool ossimHistogramEqualization::setHistogram(const ossimFilename& filename)
{
   return ossimImageSourceHistogramFilter::setHistogram(filename);
}

bool ossimHistogramEqualization::getInverseFlag()const
{
   return theInverseFlag;
}

void ossimHistogramEqualization::setInverseFlag(bool inverseFlag)
{
   theInverseFlag = inverseFlag;
}

void ossimHistogramEqualization::deleteLuts()
{
   long band = 0;
   
   for(band = 0; band < (long)theForwardLut.size(); ++band)
   {
      delete [] theForwardLut[band];
   }
   theForwardLut.clear();
   
   for(band = 0; band < (long)theInverseLut.size(); ++band)
   {
      delete [] theInverseLut[band];      
   }
   
   theInverseLut.clear();
}

const ossimFilename& ossimImageSourceHistogramFilter::getHistogramFilename() const
{
   return theFilename;
}

std::ostream& ossimHistogramEqualization::print(std::ostream& out) const
{
   // Base class...
   return ossimImageSourceHistogramFilter::print(out);
}

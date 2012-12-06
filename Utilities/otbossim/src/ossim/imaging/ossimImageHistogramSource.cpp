//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageHistogramSource.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimImageHistogramSource:debug");

  RTTI_DEF3(ossimImageHistogramSource, "ossimImageHistogramSource", ossimHistogramSource, ossimConnectableObjectListener, ossimProcessInterface);

ossimImageHistogramSource::ossimImageHistogramSource(ossimObject* owner)
   :ossimHistogramSource(owner,
                         1,     // one input
                         0,     // no outputs
                         true,  // input list is fixed
                         false),// output can still grow though
    theHistogramRecomputeFlag(true),
    theMaxNumberOfResLevels(1),
    theComputationMode(OSSIM_HISTO_MODE_NORMAL),
    theNumberOfTilesToUseInFastMode(100)
{
   theAreaOfInterest.makeNan();
   addListener((ossimConnectableObjectListener*)this);
	
   theMinValueOverride     = ossim::nan();
   theMaxValueOverride     = ossim::nan();
   theNumberOfBinsOverride = -1;
}

ossimImageHistogramSource::~ossimImageHistogramSource()
{
   removeListener((ossimConnectableObjectListener*)this);
}

ossimObject* ossimImageHistogramSource::getObject()
{
   return this;
}

const ossimObject* ossimImageHistogramSource::getObject()const
{
   return this;
}

void ossimImageHistogramSource::setAreaOfInterest(const ossimIrect& rect)
{
   if(rect != theAreaOfInterest)
   {
      theHistogramRecomputeFlag = true;
   }
   theAreaOfInterest = rect;
}

ossimIrect ossimImageHistogramSource::getAreaOfInterest()const
{
   return theAreaOfInterest;
}

void ossimImageHistogramSource::getAreaOfInterest(ossimIrect& rect)const
{
   rect = theAreaOfInterest;
}

ossim_uint32 ossimImageHistogramSource::getMaxNumberOfRLevels()const
{
   return theMaxNumberOfResLevels;
}

void ossimImageHistogramSource::setMaxNumberOfRLevels(ossim_uint32 number)
{
   if(number != theMaxNumberOfResLevels)
   {
      theHistogramRecomputeFlag = true;
   }
   theMaxNumberOfResLevels = number;
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimImageHistogramSource::getHistogram(const ossimIrect& rect)
{
   if((theAreaOfInterest != rect)||
      (theAreaOfInterest.hasNans()))
   {
      theAreaOfInterest = rect;
      theHistogramRecomputeFlag = true;
   }
	
   return getHistogram();
}

bool ossimImageHistogramSource::execute()
{
   if(!isSourceEnabled())
   {
      return theHistogram.valid();
   }
   
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   if(theHistogramRecomputeFlag)
   {
      if(theAreaOfInterest.hasNans())
      {
         ossimImageSource* interface = PTR_CAST(ossimImageSource, getInput(0));
         if(interface)
         {
            theAreaOfInterest = interface->getBoundingRect();
         }
      }
      switch(theComputationMode)
      {
         case OSSIM_HISTO_MODE_FAST:
         {
            computeFastModeHistogram();
            break;
         }
         case OSSIM_HISTO_MODE_NORMAL:
         default:
         {
            computeNormalModeHistogram();
            break;
         }
      }
   }
   
   if (needsAborting())
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_ABORTED);
      theHistogramRecomputeFlag = false;
   }
   else
   {
      setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);
   }
   
   return true;
}

bool ossimImageHistogramSource::canConnectMyInputTo(ossim_int32 myInputIndex,
                                                    const ossimConnectableObject* object)const
{
   return ((myInputIndex==0)&&PTR_CAST(ossimImageSource,
                                       object));
}

void ossimImageHistogramSource::setNumberOfBinsOverride(ossim_int32 numberOfBinsOverride)
{
   theNumberOfBinsOverride = numberOfBinsOverride;
}

void ossimImageHistogramSource::setMinValueOverride(ossim_float32 minValueOverride)
{
   theMinValueOverride = minValueOverride;
}
void ossimImageHistogramSource::setMaxValueOverride(ossim_float32 maxValueOverride)
{
   theMaxValueOverride = maxValueOverride;
}

ossimHistogramMode ossimImageHistogramSource::getComputationMode()const
{
   return theComputationMode;
}

void ossimImageHistogramSource::setComputationMode(ossimHistogramMode mode)
{
   theComputationMode = mode;
}

void ossimImageHistogramSource::propertyEvent(ossimPropertyEvent& /* event */)
{
   theHistogramRecomputeFlag = true;
}

void ossimImageHistogramSource::connectInputEvent(ossimConnectionEvent& /* event */)
{
   theHistogramRecomputeFlag = true;
}

ossimRefPtr<ossimMultiResLevelHistogram> ossimImageHistogramSource::getHistogram()
{
   execute();
   return theHistogram;
}

void ossimImageHistogramSource::getBinInformation(ossim_uint32& numberOfBins,
                                                  ossim_float64& minValue,
                                                  ossim_float64& maxValue)const
{
   numberOfBins = 0;
   minValue     = 0;
   maxValue     = 0;
	
   ossimImageSource* input = PTR_CAST(ossimImageSource, getInput(0));
   if(input)
   {
      switch(input->getOutputScalarType())
      {
         case OSSIM_UINT8:
         {
            minValue     = 0;
            maxValue     = OSSIM_DEFAULT_MAX_PIX_UCHAR;
            numberOfBins = 256;
				
            break;
         }
         case OSSIM_USHORT11:
         {
            minValue     = 0;
            maxValue     = OSSIM_DEFAULT_MAX_PIX_UINT11;
            numberOfBins = OSSIM_DEFAULT_MAX_PIX_UINT11 + 1;
				
            break;
         }
         case OSSIM_UINT16:
         case OSSIM_UINT32:
         {
            minValue     = 0;
            maxValue     = OSSIM_DEFAULT_MAX_PIX_UINT16;
            numberOfBins = OSSIM_DEFAULT_MAX_PIX_UINT16 + 1;
				
            break;
         }
         case OSSIM_SINT16:
         case OSSIM_SINT32:
         case OSSIM_FLOAT32:
         case OSSIM_FLOAT64:
         {
            minValue     = OSSIM_DEFAULT_MIN_PIX_SINT16;
            maxValue     = OSSIM_DEFAULT_MAX_PIX_SINT16;
            numberOfBins = (OSSIM_DEFAULT_MAX_PIX_SINT16-OSSIM_DEFAULT_MIN_PIX_SINT16) + 1;
				
            break;
         }
         case OSSIM_NORMALIZED_FLOAT:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            minValue     = 0;
            maxValue     = 1.0;
            numberOfBins = OSSIM_DEFAULT_MAX_PIX_UINT16+1;
            break;
         }
         default:
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "Unsupported scalar type in ossimImageHistogramSource::computeHistogram()" << endl;
            }
            return;
         }
      }
   }
	
   if(ossim::isnan(theMinValueOverride) == false)
   {
      minValue = (float)theMinValueOverride;
   }
   if(ossim::isnan(theMaxValueOverride) == false)
   {
      maxValue = (float)theMaxValueOverride;
   }
   if(theNumberOfBinsOverride > 0)
   {
      numberOfBins = theNumberOfBinsOverride;
   }
}

void ossimImageHistogramSource::computeNormalModeHistogram()
{
   // ref ptr, not a leak.
   theHistogram = new ossimMultiResLevelHistogram;
	
   ossimImageSource* input = PTR_CAST(ossimImageSource, getInput(0));
   if(!input)
   {
      setPercentComplete(100.0);
      return;
   }
   
   if(getInput(0))
   {
      // sum up all tiles needing processing.  We will use the sequncer.
      //      ossim_uint32 numberOfResLevels = input->getNumberOfDecimationLevels();
      ossim_uint32 index = 0;
      double tileCount   = 0.0;
      double totalTiles  = 0.0;
      ossim_uint32 numberOfBands = input->getNumberOfOutputBands();
      ossim_uint32 numberOfBins  = 0;
      ossim_float64 minValue     = 0;
      ossim_float64 maxValue     = 0;
      getBinInformation(numberOfBins, minValue, maxValue);
		
      ossimRefPtr<ossimImageSourceSequencer> sequencer = new ossimImageSourceSequencer;
      sequencer->connectMyInputTo(0, getInput(0));
      sequencer->initialize();
      
      vector<ossimDpt> decimationFactors;
      input->getDecimationFactors(decimationFactors);
      if ( !decimationFactors.size() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimImageHistogramSource::computeNormalModeHistogram WARNING:"
            << "\nNo decimation factors from input.  returning..." << std::endl;
         return;
      }
      
      ossim_uint32 resLevelsToCompute = ossim::min((ossim_uint32)theMaxNumberOfResLevels,
                                                   (ossim_uint32)decimationFactors.size());
         
      if( decimationFactors.size() < theMaxNumberOfResLevels)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "Number Decimations is smaller than the request number of r-levels defaulting to the smallest of the 2 numbers" << endl;
      }

      theHistogram->create(resLevelsToCompute);
      for(index = 0; index < resLevelsToCompute; ++index)
      {
         sequencer->setAreaOfInterest(theAreaOfInterest*decimationFactors[index]);
         
         totalTiles += sequencer->getNumberOfTiles();
      }
      
      
      if(numberOfBins > 0)
      {
         setPercentComplete(0.0);
         for(index = 0;
             (index < resLevelsToCompute);
             ++index)
         {
            // Check for abort request.
            if (needsAborting())
            {
               setPercentComplete(100);
               break;
            }
            
            //sequencer->setAreaOfInterest(input->getBoundingRect(index));
            sequencer->setAreaOfInterest(theAreaOfInterest*decimationFactors[index]);
            
            sequencer->setToStartOfSequence();
            theHistogram->getMultiBandHistogram(index)->create(numberOfBands,
                                                               numberOfBins,
                                                               minValue,
                                                               maxValue);
            
            ossimRefPtr<ossimImageData> data = sequencer->getNextTile(index);
            ++tileCount;
            setPercentComplete((100.0*(tileCount/totalTiles)));
            
            ossim_uint32 resLevelTotalTiles = sequencer->getNumberOfTiles();
            for (ossim_uint32 resLevelTileCount = 0;
                 resLevelTileCount < resLevelTotalTiles;
                 ++resLevelTileCount)
            {
               if(data.valid()&&data->getBuf()&&(data->getDataObjectStatus() != OSSIM_EMPTY))
               {
                  data->populateHistogram(theHistogram->getMultiBandHistogram(index));
               }
               
               // Check for abort request.
               if (needsAborting())
               {
                  setPercentComplete(100);
                  break;
               }
               
               
               data = sequencer->getNextTile(index);
               ++tileCount;
               setPercentComplete((100.0*(tileCount/totalTiles)));
            }
         }
      }
      sequencer->disconnect();
      sequencer = 0;
   }
}

void ossimImageHistogramSource::computeFastModeHistogram()
{
   // We will only compute a full res histogram in fast mode.  and will only do a MAX of 100 tiles.
   //
   ossim_uint32 resLevelsToCompute = 1;
	
   // ref ptr, not a leak.
   theHistogram = new ossimMultiResLevelHistogram;
   theHistogram->create(resLevelsToCompute);
   ossimImageSource* input = PTR_CAST(ossimImageSource, getInput(0));
   if(!input)
   {
      setPercentComplete(100.0);
      return;
   }
   // sum up all tiles needing processing.  We will use the sequencer.
   //      ossim_uint32 numberOfResLevels = input->getNumberOfDecimationLevels();
   double tileCount   = 0.0;
   double totalTiles  = 0.0;
   ossim_uint32 numberOfBands = input->getNumberOfOutputBands();
   ossim_uint32 numberOfBins  = 0;
   ossim_float64 minValue     = 0;
   ossim_float64 maxValue     = 0;
   getBinInformation(numberOfBins, minValue, maxValue);
	
   ossimIrect tileBoundary = theAreaOfInterest;
   ossimIpt tileSize(ossim::max((ossim_uint32)input->getTileWidth(), (ossim_uint32)64),
                     ossim::max((ossim_uint32)input->getTileHeight(), (ossim_uint32)64));
	
   tileBoundary.stretchToTileBoundary(tileSize);
   ossim_uint32 tilesWide = (tileBoundary.width()/tileSize.x);
   ossim_uint32 tilesHigh = (tileBoundary.height()/tileSize.y);
   totalTiles = tilesWide*tilesHigh;
   
   if(totalTiles > theNumberOfTilesToUseInFastMode)
   {
      ossim_uint32 testTiles = (ossim_uint32) (std::sqrt((double)theNumberOfTilesToUseInFastMode)+.5);
      tilesWide = testTiles>tilesWide?tilesWide:testTiles;
      tilesHigh = testTiles>tilesHigh?tilesHigh:testTiles;
   }
   if(numberOfBins > 0)
   {
      ossimIpt origin = theAreaOfInterest.ul();
		
      ossim_uint32 widthWithExcess  = (ossim_uint32)(((ossim_float64)tileBoundary.width()/(tilesWide*tileSize.x))*tileSize.x);
      ossim_uint32 heightWithExcess = ((ossim_uint32)((ossim_float64)tileBoundary.height()/(tilesHigh*tileSize.y))*tileSize.y);
      theHistogram->getMultiBandHistogram(0)->create(numberOfBands,
                                                     numberOfBins,
                                                     minValue,
                                                     maxValue);
		
      ossim_uint32 x = 0;
      ossim_uint32 y = 0;
      tileCount = 0;
      totalTiles = tilesWide*tilesHigh;
      for(y = 0; y < tilesHigh; ++y)
      {
         for(x = 0; x < tilesWide; ++x)
         {
            ossimIpt ul(origin.x + (x*widthWithExcess),
                        origin.y + (y*heightWithExcess));
            ossimIrect tileRect(ul.x, ul.y, ul.x + tileSize.x-1, ul.y + tileSize.y-1);
            ossimRefPtr<ossimImageData> data = input->getTile(tileRect);
				
            if(data.valid()&&data->getBuf()&&(data->getDataObjectStatus() != OSSIM_EMPTY))
            {
               data->populateHistogram(theHistogram->getMultiBandHistogram(0));
            }
            ++tileCount;
            setPercentComplete((100.0*(tileCount/totalTiles)));
         }
      }
   }
}

bool ossimImageHistogramSource::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   ossimHistogramSource::loadState(kwl, prefix);  
   setNumberOfInputs(2);
   ossimString rect = kwl.find(prefix, "rect");

   if(!rect.empty())
   {
      loadState(kwl, prefix);
   }
   else 
   {
      ossimString newPrefix = ossimString(prefix) + "area_of_interest.";
      theAreaOfInterest.loadState(kwl, newPrefix);
   }
   
   ossimString mode = kwl.find(prefix, "mode");
   mode = mode.downcase();
   if(mode == "normal")
   {
      theComputationMode = OSSIM_HISTO_MODE_NORMAL;
   }
   else if(mode == "fast")
   {
      theComputationMode = OSSIM_HISTO_MODE_FAST;
   }
   if(getNumberOfInputs()!=1)
   {
      setNumberOfInputs(1);
   }
   
   ossimString numberOfTiles = kwl.find(prefix, "number_of_tiles");
   if(!numberOfTiles.empty())
   {
      theNumberOfTilesToUseInFastMode = numberOfTiles.toUInt32();
   }
   theInputListIsFixedFlag = true;
   theOutputListIsFixedFlag = false;
	
   return true;
}

bool ossimImageHistogramSource::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   bool result = ossimHistogramSource::saveState(kwl, prefix);
   if(result)
   {
      ossimString newPrefix = ossimString(prefix) + "area_of_interest.";
      theAreaOfInterest.saveState(kwl, newPrefix);
   }
   return result;
}

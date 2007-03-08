//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageHistogramSource.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>
#include <ossim/base/ossimMultiBandHistogram.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>

RTTI_DEF3(ossimImageHistogramSource, "ossimImageHistogramSource", ossimHistogramSource, ossimConnectableObjectListener, ossimProcessInterface);

ossimImageHistogramSource::ossimImageHistogramSource(ossimObject* owner)
   :ossimHistogramSource(owner,
                         1,     // one input
                         0,     // no outputs
                         true,  // input list is fixed
                         false),// output can still grow though
    theHistogramRecomputeFlag(true),
    theMaxNumberOfResLevels(1)
{
   theAreaOfInterest.makeNan();
   addListener((ossimConnectableObjectListener*)this);
   disableSource();

   theMinValueOverride     = OSSIM_DBL_NAN;
   theMaxValueOverride     = OSSIM_DBL_NAN;
   theNumberOfBinsOverride = -1;
}

ossimImageHistogramSource::ossimImageHistogramSource(ossimObject* owner,
                                                     ossim_uint32 numberOfInputs,
                                                     ossim_uint32 numberOfOutputs,
                                                     ossim_uint32 inputListFixedFlag,
                                                     ossim_uint32 outputListFixedFlag)
   :ossimHistogramSource(owner,
                         numberOfInputs,
                         numberOfOutputs,
                         inputListFixedFlag,
                         outputListFixedFlag),
    theHistogramRecomputeFlag(true)
{
   theAreaOfInterest.makeNan();
   addListener((ossimConnectableObjectListener*)this);
   disableSource();
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

ossimMultiResLevelHistogram* ossimImageHistogramSource::getHistogram(const ossimIrect& rect)
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
      return theHistogram;
   }
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   if(theHistogramRecomputeFlag)
   {
      if(theAreaOfInterest.hasNans())
      {
         ossimImageSourceInterface* interface = PTR_CAST(ossimImageSourceInterface, getInput(0));
         if(interface)
         {
            theAreaOfInterest = interface->getBoundingRect();
         }
      }
      computeHistogram();
      if(getProcessStatus() != ossimProcessInterface::PROCESS_STATUS_ABORTED)
      {
         theHistogramRecomputeFlag = false;
         disableSource();
      }
   }
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);
   
   return true;
}

bool ossimImageHistogramSource::canConnectMyInputTo(ossim_int32 myInputIndex,
                                                    const ossimConnectableObject* object)const
{
   return ((myInputIndex==0)&&PTR_CAST(ossimImageSourceInterface,
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

void ossimImageHistogramSource::propertyEvent(ossimPropertyEvent& /* event */)
{
   theHistogramRecomputeFlag = true;
}

void ossimImageHistogramSource::connectInputEvent(ossimConnectionEvent& /* event */)
{
   theHistogramRecomputeFlag = true;
}

ossimMultiResLevelHistogram* ossimImageHistogramSource::getHistogram()
{
  execute();
  return theHistogram;
}

void ossimImageHistogramSource::computeHistogram()
{
   if(theHistogram)
   {
      delete theHistogram;
      theHistogram = NULL;
   }   
   theHistogram = new ossimMultiResLevelHistogram;

   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(0));
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
      float minValue     = 0;
      float maxValue     = 0;
      
      ossimImageSourceSequencer* sequencer = new ossimImageSourceSequencer;
      sequencer->connectMyInputTo(0, getInput(0));
      sequencer->initialize();
      
      vector<ossimDpt> decimationFactors;
      input->getDecimationFactors(decimationFactors);

      if( decimationFactors.size() < theMaxNumberOfResLevels)
      {
         cerr << "Number Decimations is smaller than the request number of r-levels" << endl;
         return;
      }
      theHistogram->create(theMaxNumberOfResLevels);//numberOfResLevels);
      for(index = 0; index < theMaxNumberOfResLevels; ++index)
      {
         sequencer->setAreaOfInterest(theAreaOfInterest*decimationFactors[index]);
         
         totalTiles += sequencer->getNumberOfTiles();
      }
      
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
         cerr << "Unsupported scalar type in ossimImageHistogramSource::computeHistogram()" << endl;
         return;
      }
      }

      if(theMinValueOverride != OSSIM_DBL_NAN)
      {
         minValue = (float)theMinValueOverride;
      }
      if(theMaxValueOverride != OSSIM_DBL_NAN)
      {
         maxValue = (float)theMaxValueOverride;
      }
      if(theNumberOfBinsOverride > 0)
      {
         numberOfBins = theNumberOfBinsOverride;
      }
      ossimProcessInterface::ossimProcessStatus processStatus = getProcessStatus();
      if(numberOfBins > 0)
      {
	setPercentComplete(0.0);
	for(index = 0;
	    ((index < theMaxNumberOfResLevels)&&//numberOfResLevels)&&
	     (processStatus != ossimProcessInterface::PROCESS_STATUS_ABORTED));
	    ++index)
         {
            //sequencer->setAreaOfInterest(input->getBoundingRect(index));
            sequencer->setAreaOfInterest(theAreaOfInterest*decimationFactors[index]);
            
            sequencer->setToStartOfSequence();
            theHistogram->getMultiBandHistogram(index)->create(numberOfBands,
                                                               numberOfBins,
                                                               minValue,
                                                               maxValue);
            
            processStatus = getProcessStatus();
            if(processStatus!=ossimProcessInterface::PROCESS_STATUS_ABORTED)
            {
               ossimRefPtr<ossimImageData> data =
                  sequencer->getNextTile(index);
               ++tileCount;
               setPercentComplete((100.0*(tileCount/totalTiles)));
               processStatus = getProcessStatus();
               long resLevelTotalTiles = sequencer->getNumberOfTiles();
               long resLevelTileCount  = 1;
               while((resLevelTileCount < resLevelTotalTiles)&&
                     (processStatus!=ossimProcessInterface::PROCESS_STATUS_ABORTED))
               {
                  if(data.valid())
                  {
                     data->populateHistogram(theHistogram->getMultiBandHistogram(index));
                  }
                  data = sequencer->getNextTile(index);
                  processStatus = getProcessStatus();
                  ++tileCount;
                  ++resLevelTileCount;
                  setPercentComplete((100.0*(tileCount/totalTiles)));
               }
            }
         }
      }
      delete sequencer;
      sequencer = NULL;
   }
   //setPercentComplete(100);
}

bool ossimImageHistogramSource::loadState(const ossimKeywordlist& kwl,
					  const char* prefix)
{
  ossimHistogramSource::loadState(kwl,
				  prefix);  

  ossimString newPrefix = ossimString(prefix) + "rect.";
  theAreaOfInterest.loadState(kwl, newPrefix);
  
  if(getNumberOfInputs()!=1)
    {
      setNumberOfInputs(1);
    }
  theInputListIsFixedFlag = true;
  theOutputListIsFixedFlag = false;
  
  return true;
}

bool ossimImageHistogramSource::saveState(ossimKeywordlist& kwl,
					  const char* prefix)const
{
   bool result = ossimHistogramSource::saveState(kwl,
                                                 prefix);
   
   if(result)
   {
      ossimString newPrefix = ossimString(prefix) + "rect.";
      theAreaOfInterest.saveState(kwl, newPrefix);
   }
   
  return result;
}

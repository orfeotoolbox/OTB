
//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimImageSourceFilter.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimProperty.h>
 
static ossimTrace traceDebug("ossimImageSourceFilter:degug");

RTTI_DEF2(ossimImageSourceFilter, "ossimImageSourceFilter", ossimImageSource, ossimConnectableObjectListener)

ossimImageSourceFilter::ossimImageSourceFilter(ossimObject* owner)
   : ossimImageSource(owner,
                      1, // number of inputs
                      0, // number of outputs
                      true, // input's fixed
                      false), // outputs ar not fixed
     theInputConnection(NULL)
{
   addListener((ossimConnectableObjectListener*)this);
}

ossimImageSourceFilter::ossimImageSourceFilter(ossimImageSource* inputSource)
   : ossimImageSource(NULL,
                      1,
                      0,
                      true,
                      false),
     theInputConnection(inputSource)
{
   if(inputSource)
   {
     connectMyInputTo(0, inputSource);
   }
   addListener((ossimConnectableObjectListener*)this);
}

ossimImageSourceFilter::ossimImageSourceFilter(ossimObject* owner,
                                               ossimImageSource* inputSource)
   : ossimImageSource(owner,
                      1,
                      0,
                      true,
                      false),
     theInputConnection(inputSource)
{
   if(inputSource)
   {
      theInputObjectList[0] = inputSource;
      inputSource->connectMyOutputTo(this, false);
   }
   addListener((ossimConnectableObjectListener*)this);
}


ossimImageSourceFilter::~ossimImageSourceFilter()
{
   removeListener((ossimConnectableObjectListener*)this);
   theInputConnection = NULL;
}

ossimRefPtr<ossimImageData> ossimImageSourceFilter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(theInputConnection)
   {
      return theInputConnection->getTile(rect, resLevel);
   }
   
   return ossimRefPtr<ossimImageData>(NULL);
}

ossimIrect ossimImageSourceFilter::getBoundingRect(ossim_uint32 resLevel) const
{
   if(theInputConnection)
   {
      return theInputConnection->getBoundingRect(resLevel);
   }
   ossimIrect rect;
   rect.makeNan();
   
   return rect;
}

void ossimImageSourceFilter::getDecimationFactor(ossim_uint32 resLevel,
                                                 ossimDpt& result) const
{
   if(theInputConnection)
   {
      theInputConnection->getDecimationFactor(resLevel,
                                              result);
   }
   else // Ok assume square and powers of 2
   {
      double dec = 1.0/pow((double)2.0,(double)resLevel);
      result.x = dec;
      result.y = dec;
   }
}

void ossimImageSourceFilter::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   if(theInputConnection)
   {
      theInputConnection->getDecimationFactors(decimations);
   }   
}

ossim_uint32 ossimImageSourceFilter::getNumberOfDecimationLevels()const
{
   if(theInputConnection)
   {
      return theInputConnection->getNumberOfDecimationLevels();
   }

   return 0;
}

ossim_uint32 ossimImageSourceFilter::getNumberOfInputBands()const
{
   if(theInputConnection)
   {
      return theInputConnection->getNumberOfOutputBands();
   }

   return 0;
}

ossimScalarType ossimImageSourceFilter::getOutputScalarType() const
{
   if(theInputConnection)
   {
      return theInputConnection->getOutputScalarType();
   }

   return OSSIM_SCALAR_UNKNOWN;
}

void ossimImageSourceFilter::getValidImageVertices(vector<ossimIpt>& validVertices,
                                                   ossimVertexOrdering ordering,
                                                   ossim_uint32 resLevel)const
{
   if(theInputConnection)
   {
      theInputConnection->getValidImageVertices(validVertices, ordering, resLevel);
   }
}

ossim_uint32 ossimImageSourceFilter::getTileWidth() const
{
   if(theInputConnection)
   {
      return theInputConnection->getTileWidth();
   }
   
   return 0;
}

ossim_uint32 ossimImageSourceFilter::getTileHeight() const
{
   if(theInputConnection)
   {
      return theInputConnection->getTileHeight();
   }

   return 0;
}

void ossimImageSourceFilter::initialize()
{
   theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput(0));
}

bool ossimImageSourceFilter::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   bool result = ossimImageSource::loadState(kwl, prefix);

   // make sure we have 1 input.
   setNumberOfInputs(1);

   // we will fix the input
   if(!theInputListIsFixedFlag)
   {
      theInputListIsFixedFlag = true;
   }
   
   return result;
}

bool ossimImageSourceFilter::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   return ossimImageSource::saveState(kwl, prefix);
}


bool ossimImageSourceFilter::canConnectMyInputTo(ossim_int32 inputIndex,
						 const ossimConnectableObject* object)const
{
  
  return ((PTR_CAST(ossimImageSourceInterface, object)||!object) && ( inputIndex == 0 ) );
}

void ossimImageSourceFilter::connectInputEvent(ossimConnectionEvent& event)
{
  if(traceDebug())
    {
       ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::connectInputEvent: ";

       if(getInput())
       {
          ossimNotify(ossimNotifyLevel_DEBUG) << "to" <<  getInput()->getClassName() << std::endl;
       }
       else
       {
          ossimNotify(ossimNotifyLevel_DEBUG) << "to NULL" << std::endl;
       }
    }
  theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput(0));
  initialize();
  if(traceDebug())
  {
     if(theInputConnection)
     {
        ossimNotify(ossimNotifyLevel_DEBUG) << "theInputConnection was set to " << getInput()->getClassName() << std::endl;
     }
     else
     {
        ossimNotify(ossimNotifyLevel_DEBUG) << "theInputConnection was set to NULL" <<  std::endl;
     }
     ossimNotify(ossimNotifyLevel_DEBUG) << "Leaving ossimImageSourceFilter::connectInput" << std::endl;
  }
}
void ossimImageSourceFilter::disconnectInputEvent(ossimConnectionEvent& event)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::disconnectInputEvent" << std::endl;
   }
   theInputConnection = PTR_CAST(ossimImageSourceInterface, getInput(0));
   initialize();
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "Leaving ossimImageSourceFilter::disconnectInput" << std::endl;
   }
}
void ossimImageSourceFilter::propertyEvent(ossimPropertyEvent& event)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::propertyEvent DEBUG: Entering..." << std::endl;
   }
   initialize();
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::propertyEvent DEBUG: Leaving..." << std::endl;
   }
}

void ossimImageSourceFilter::refreshEvent(ossimRefreshEvent& event)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::refreshEvent " << std::endl;
   }
   initialize();
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimImageSourceFilter::refreshEvent " << std::endl;
   }
}

double ossimImageSourceFilter::getNullPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      return theInputConnection->getNullPixelValue(band);
   }
   return ossimImageSource::getNullPixelValue(band);
}

double ossimImageSourceFilter::getMinPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      return theInputConnection->getMinPixelValue(band);
   }
   return ossimImageSource::getMinPixelValue(band);
}

double ossimImageSourceFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if(theInputConnection)
   {
      return theInputConnection->getMaxPixelValue(band);
   }
   return ossimImageSource::getMaxPixelValue(band);
}

void ossimImageSourceFilter::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   if(theInputConnection)
   {
      return theInputConnection->getOutputBandList(bandList);
   }

   return ossimImageSource::getOutputBandList(bandList);
}

void ossimImageSourceFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimImageSource::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimImageSourceFilter::getProperty(const ossimString& name)const
{
   return ossimImageSource::getProperty(name);
   
}

void ossimImageSourceFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSource::getPropertyNames(propertyNames);
}

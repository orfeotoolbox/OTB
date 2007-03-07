//----------------------------------------------------------------------------
// Copyright (C) 2004 Garrett Potts, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: 
//
// $Id: ossimVpfTileSource.cpp,v 1.9 2005/02/11 15:07:32 dburken Exp $
//----------------------------------------------------------------------------
#include "ossimVpfTileSource.h"

RTTI_DEF2(ossimVpfTileSource, "ossimVpfTileSource", ossimImageHandler, ossimViewInterface);

ossimVpfTileSource::ossimVpfTileSource()
   :
      ossimViewInterface()
{
   theObject = this;
   theAnnotationSource = new ossimVpfAnnotationSource;
   theAnnotationSource->setNumberOfBands(3);
}

ossimVpfTileSource::~ossimVpfTileSource()
{
   if(theAnnotationSource)
   {
      delete theAnnotationSource;
      theAnnotationSource = NULL;
   }
}

bool ossimVpfTileSource::saveState(ossimKeywordlist& kwl,
				   const char* prefix)const
{
   return theAnnotationSource->saveState(kwl, prefix);
}

bool ossimVpfTileSource::loadState(const ossimKeywordlist& kwl,
				   const char* prefix)
{
   return theAnnotationSource->loadState(kwl, prefix);
}

void ossimVpfTileSource::close()
{
   theAnnotationSource->close();
}

bool ossimVpfTileSource::open()
{
   return theAnnotationSource->open(theImageFile);
}

ossimRefPtr<ossimImageData> ossimVpfTileSource::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   return theAnnotationSource->getTile(tileRect, resLevel);
}

ossim_uint32 ossimVpfTileSource::getNumberOfInputBands() const
{
   return theAnnotationSource->getNumberOfOutputBands();
}

ossim_uint32 ossimVpfTileSource::getNumberOfOutputBands() const
{
   return theAnnotationSource->getNumberOfOutputBands();
}

ossim_uint32 ossimVpfTileSource::getNumberOfLines(ossim_uint32 /* reduced_res_level */) const
{
   ossimIrect theBounds = theAnnotationSource->getBoundingRect();
   
   if(theBounds.hasNans())
   {
      return theBounds.ul().x;
   }
   
   return theBounds.height();
}

ossim_uint32 ossimVpfTileSource::getNumberOfSamples(ossim_uint32 /* reduced_res_level */) const
{
   ossimIrect theBounds = theAnnotationSource->getBoundingRect();
   
   if(theBounds.hasNans())
   {
      return theBounds.ul().x;
   }
   
   return theBounds.height();
}

ossim_uint32 ossimVpfTileSource::getNumberOfDecimationLevels() const
{
   return 32;
}

ossimIrect ossimVpfTileSource::getImageRectangle(ossim_uint32 /* reduced_res_level */) const
{
   return theAnnotationSource->getBoundingRect();
}

bool ossimVpfTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                          const char* prefix)
{
   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }
   
   ossimObject* view = theAnnotationSource->getView();
   if(view)
   {
      bool result = view->saveState(kwl, prefix);
      if (result)
      {
         // Capture for next time...
         setImageGeometry(kwl);
      }
      return result;
   }
   
   return false;
}

ossimScalarType ossimVpfTileSource::getOutputScalarType() const
{
   return theAnnotationSource->getOutputScalarType();
}

ossim_uint32 ossimVpfTileSource::getTileWidth() const
{
   return theAnnotationSource->getTileWidth();         
}

ossim_uint32 ossimVpfTileSource::getTileHeight() const
{
   return theAnnotationSource->getTileHeight();
}

ossim_uint32 ossimVpfTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimVpfTileSource::getImageTileHeight() const
{
   return 0;
}

bool ossimVpfTileSource::isOpen()const
{
   return theAnnotationSource->isOpen();
}
   
double ossimVpfTileSource::getNullPixelValue(ossim_uint32 band)const
{
   return theAnnotationSource->getNullPixelValue(band);
}

double ossimVpfTileSource::getMinPixelValue(ossim_uint32 band)const
{
   return theAnnotationSource->getMinPixelValue(band);
}
      
double ossimVpfTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   return theAnnotationSource->getMaxPixelValue(band);
}

ossimObject* ossimVpfTileSource::getView()
{
   return theAnnotationSource->getView();
}

const ossimObject* ossimVpfTileSource::getView()const
{
   return theAnnotationSource->getView();
}
   
bool ossimVpfTileSource::setView(ossimObject*  baseObject,
                                 bool ownsTheView)
{
   return theAnnotationSource->setView(baseObject, ownsTheView);
}

void ossimVpfTileSource::getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList)
{
   if(theAnnotationSource)
   {
      theAnnotationSource->getAllFeatures(featureList);
   }
}

void ossimVpfTileSource::setAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList)
{
   if(theAnnotationSource)
   {
      theAnnotationSource->setAllFeatures(featureList);
   }
}

void ossimVpfTileSource::transformObjects()
{
   theAnnotationSource->transformObjects();
}

void ossimVpfTileSource::computeBoundingRect()
{
   theAnnotationSource->computeBoundingRect();
}

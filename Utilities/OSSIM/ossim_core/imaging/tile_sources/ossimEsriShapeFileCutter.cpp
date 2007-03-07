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
// $Id: ossimEsriShapeFileCutter.cpp,v 1.6 2005/09/15 12:28:44 gpotts Exp $
#include <imaging/tile_sources/ossimEsriShapeFileCutter.h>

RTTI_DEF2(ossimEsriShapeFileCutter,
          "ossimEsriShapeFileCutter",
          ossimImageSourceFilter,
          ossimViewInterface);

ossimEsriShapeFileCutter::ossimEsriShapeFileCutter()
   :ossimImageSourceFilter(),
    ossimViewInterface(),
    theEsriShapeFile(NULL),
    theMaskFilter(NULL)
{
   theObject = this;
   theEsriShapeFile = new ossimEsriShapeFileFilter;
   theEsriShapeFile->setFillFlag(true);

   theMaskFilter    = new ossimMaskFilter;

   theMaskFilter->connectMyInputTo(1,
                                   theEsriShapeFile);
}

ossimEsriShapeFileCutter::~ossimEsriShapeFileCutter()
{
   if(theEsriShapeFile)
   {
      delete theEsriShapeFile;
      theEsriShapeFile = NULL;
   }

   if(theMaskFilter)
   {
      delete theMaskFilter;
      theMaskFilter = NULL;
   }
}

ossimRefPtr<ossimImageData> ossimEsriShapeFileCutter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(isSourceEnabled())
   {
      if(theMaskFilter)
      {
         return theMaskFilter->getTile(rect, resLevel);
      }
   }
   else
   {
      if(getInput())
      {
         ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                                     getInput());
         
         if(input)
         {
            return input->getTile(rect, resLevel);
         }
      }
   }
   
   return NULL;
}

void ossimEsriShapeFileCutter::initialize()
{
   if(theEsriShapeFile)
   {
      theEsriShapeFile->initialize();
   }

   
   if(theMaskFilter)
   {
      theMaskFilter->disconnectMyInput((ossim_int32)0);
      theMaskFilter->disconnectMyInput((ossim_int32)1);
      theMaskFilter->connectMyInputTo((int)0,
                                      getInput(0));
      theMaskFilter->connectMyInputTo(1,
                                      theEsriShapeFile);
   }
}

bool ossimEsriShapeFileCutter::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   ossimString shapeFilePrefix = ossimString(prefix) + "shape_file_filter.";
   ossimString maskPrefix      = ossimString(prefix) + "mask.";
                                 
   theEsriShapeFile->loadState(kwl, shapeFilePrefix.c_str());
   theMaskFilter->loadState(kwl, maskPrefix.c_str());

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimEsriShapeFileCutter::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   ossimString shapeFilePrefix = ossimString(prefix) + "shape_file_filter.";
   ossimString maskPrefix      = ossimString(prefix) + "mask.";
                                 
   theEsriShapeFile->saveState(kwl, shapeFilePrefix.c_str());
   theMaskFilter->saveState(kwl, maskPrefix.c_str());
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

void ossimEsriShapeFileCutter::getDecimationFactor(ossim_uint32 resLevel,
                                                   ossimDpt& result)const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(input)
   {
      input->getDecimationFactor(resLevel, result);
   }
}

ossimIrect ossimEsriShapeFileCutter::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;
   result.makeNan();
   
   if(theMaskFilter)
   {
      result = theMaskFilter->getBoundingRect(resLevel);
   }
   
   return result;
}

void ossimEsriShapeFileCutter::getDecimationFactors(
   vector<ossimDpt>& decimations)const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      input->getDecimationFactors(decimations);
   }     
}

ossim_uint32 ossimEsriShapeFileCutter::getNumberOfDecimationLevels()const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getNumberOfDecimationLevels();
   } 
   return 0;
}

ossim_uint32 ossimEsriShapeFileCutter::getNumberOfOutputBands() const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getNumberOfOutputBands();
   }
   return 0;
}

ossimScalarType ossimEsriShapeFileCutter::getOutputScalarType() const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getOutputScalarType();
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

void ossimEsriShapeFileCutter::getValidImageVertices(
   vector<ossimIpt>& validVertices,
   ossimVertexOrdering ordering,
   ossim_uint32 resLevel) const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      input->getValidImageVertices(validVertices, ordering, resLevel);
   }
}

ossim_uint32 ossimEsriShapeFileCutter::getTileWidth() const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getTileWidth();
   }
   
   return 0;
}

ossim_uint32 ossimEsriShapeFileCutter::getTileHeight() const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getTileHeight();
   }
   return 0;
}

double ossimEsriShapeFileCutter::getNullPixelValue(ossim_uint32 band)const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getNullPixelValue(band);
   }
   return 0;
}
   
double ossimEsriShapeFileCutter::getMinPixelValue(ossim_uint32 band)const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                               getInput(0));
   if(input)
   {
      return input->getMinPixelValue(band);
   }
   return 0;
}
   
double ossimEsriShapeFileCutter::getMaxPixelValue(ossim_uint32 band)const
{
   ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(0));
   if(input)
   {
      return input->getMaxPixelValue(band);
   }
   return 0;
}

void ossimEsriShapeFileCutter::loadFile(const ossimFilename& filename)
{
   if(theEsriShapeFile)
   {
      theEsriShapeFile->loadShapeFile(filename);
   }
}

bool ossimEsriShapeFileCutter::setView(ossimObject* baseObject,
                                       bool ownsTheView)
{
   if(theEsriShapeFile)
   {
      return theEsriShapeFile->setView(baseObject, ownsTheView);
   }
   
   return false;
}

ossimObject* ossimEsriShapeFileCutter::getView()
{
   if(theEsriShapeFile)
   {
      return theEsriShapeFile->getView();
   }
   
   return NULL;;
}

const ossimObject* ossimEsriShapeFileCutter::getView()const
{
   if(theEsriShapeFile)
   {
      return theEsriShapeFile->getView();
   }
   
   return NULL;;
}

ossimFilename ossimEsriShapeFileCutter::getFilename()const
{
   if(theEsriShapeFile)
   {
      return theEsriShapeFile->getFilename();
   }
   
   return ossimFilename();
}

void ossimEsriShapeFileCutter::setMaskType(
   ossimMaskFilter::ossimFileSelectionMaskType type)
{
   if(theMaskFilter)
   {
      theMaskFilter->setMaskType(type);
   }
}

ossimMaskFilter::ossimFileSelectionMaskType ossimEsriShapeFileCutter::getMaskType()const
{
   if(theMaskFilter)
   {
      return theMaskFilter->getMaskType();
   }
   
   return ossimMaskFilter::OSSIM_MASK_TYPE_SELECT;
}

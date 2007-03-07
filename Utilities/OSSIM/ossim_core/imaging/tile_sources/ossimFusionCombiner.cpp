//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: implementation for base fusion
//
//*************************************************************************
// $Id: ossimFusionCombiner.cpp,v 1.10 2005/12/08 21:12:10 dburken Exp $

#include <imaging/tile_sources/ossimFusionCombiner.h>
#include <imaging/factory/ossimImageDataFactory.h>
RTTI_DEF1(ossimFusionCombiner, "ossimFusionCombiner", ossimImageCombiner);

ossimFusionCombiner::ossimFusionCombiner()
  :ossimImageCombiner(NULL, 2, 0, true, false),
   theTile(NULL),
   theNormTile(NULL),
   theNormIntensity(NULL),
   theInputConnection(NULL),
   theIntensityConnection(NULL)
{
}

ossimFusionCombiner::ossimFusionCombiner(ossimObject* owner)
  :ossimImageCombiner(owner, 2, 0, true, false),
   theTile(NULL),
   theNormTile(NULL),
   theNormIntensity(NULL),
   theInputConnection(NULL),
   theIntensityConnection(NULL)
{}

ossimFusionCombiner::~ossimFusionCombiner()
{
   theInputConnection     = NULL;
   theIntensityConnection = NULL;
}

ossimIrect ossimFusionCombiner::getBoundingRect(ossim_uint32 resLevel) const
{
  ossimIrect result;
  result.makeNan();
  if(theIntensityConnection)
    {
      result = theIntensityConnection->getBoundingRect(resLevel);
    }
  else if(theInputConnection)
    {
      result = theInputConnection->getBoundingRect(resLevel);
    }
  
  return result;
}

bool ossimFusionCombiner::canConnectMyInputTo(ossim_int32 inputIndex,
					      const ossimConnectableObject* object)const
{
  return ((inputIndex<2)&&
	  (PTR_CAST(ossimImageSourceInterface, object)||!object));
}

ossimScalarType ossimFusionCombiner::getOutputScalarType() const
{
   if(theInputConnection)
   {
      return theInputConnection->getOutputScalarType();
   }

   return ossimImageCombiner::getOutputScalarType();
}


ossimRefPtr<ossimImageData> ossimFusionCombiner::getNormIntensity(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(theIntensityConnection)
   {
      ossimRefPtr<ossimImageData> data =
         theIntensityConnection->getTile(rect, resLevel);
      if(data.valid() && data->getBuf())
      {
         if(!theNormIntensity.valid())
         {
            theNormIntensity = new ossimImageData(this,
                                                  OSSIM_NORMALIZED_FLOAT,
                                                  data->getNumberOfBands(),
                                                  rect.width(),
                                                  rect.height());
            theNormIntensity->initialize();
         }

         theNormIntensity->setImageRectangleAndBands(rect,
                                                     data->getNumberOfBands());

         data->copyTileToNormalizedBuffer((float*)theNormIntensity->getBuf());
         theNormIntensity->setDataObjectStatus(data->getDataObjectStatus());
         return theNormIntensity;
      }
   }
   return ossimRefPtr<ossimImageData>();
}

ossimRefPtr<ossimImageData> ossimFusionCombiner::getNormTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> data = theInputConnection->getTile(rect,
                                                                     resLevel);
      if(data.valid() && data->getBuf())
      {
         // make sure the tile result is updated if changed.
         //
         if((data->getNumberOfBands() != theTile->getNumberOfBands())||
            (data->getScalarType()    != theTile->getScalarType()))
         {
            theTile = ossimImageDataFactory::instance()->create(this,
                                                                theInputConnection);
            theTile->initialize();
         }

         if(!theNormTile.valid())
         {
            theNormTile = new ossimImageData(this,
                                             OSSIM_NORMALIZED_FLOAT,
                                             data->getNumberOfBands(),
                                             rect.width(),
                                             rect.height());
            theNormTile->initialize();
         }
         else if(theNormTile->getNumberOfBands() != data->getNumberOfBands())
         {
            theNormTile = new ossimImageData(this,
                                             OSSIM_NORMALIZED_FLOAT,
                                             data->getNumberOfBands(),
                                             rect.width(),
                                             rect.height());
            theNormTile->initialize();
            
         }

         theNormTile->setImageRectangleAndBands(rect,
                                                data->getNumberOfBands());

         data->copyTileToNormalizedBuffer((float*)theNormTile->getBuf());
         theNormTile->setDataObjectStatus(data->getDataObjectStatus());
         return theNormTile;
      }
   }
   return ossimRefPtr<ossimImageData>();
}

void ossimFusionCombiner::initialize()
{
   ossimImageCombiner::initialize();
   
   theInputConnection     = PTR_CAST(ossimImageSourceInterface, getInput(0));
   theIntensityConnection = PTR_CAST(ossimImageSourceInterface, getInput(1));

   if(getInput(0)&&getInput(1))
   {
      ossimImageSourceInterface* temp  = PTR_CAST(ossimImageSourceInterface,
                                                  getInput(0));
      ossimImageSourceInterface* temp2 = PTR_CAST(ossimImageSourceInterface,
                                                  getInput(1));

      if(temp&&temp2)
      {
         if((temp->getNumberOfOutputBands()==1)&&
            (temp2->getNumberOfOutputBands()!=1))
         {
            theIntensityConnection = PTR_CAST(ossimImageSourceInterface,
                                              getInput(0));
            theInputConnection     = PTR_CAST(ossimImageSourceInterface,
                                              getInput(1));
         }
      }
   }
   
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();
   }
}

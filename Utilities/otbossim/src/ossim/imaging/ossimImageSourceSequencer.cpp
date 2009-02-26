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
//  $Id: ossimImageSourceSequencer.cpp 13374 2008-08-04 13:02:22Z gpotts $
#include <ossim/imaging/ossimImageSourceSequencer.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF2(ossimImageSourceSequencer, "ossimImageSourceSequencer",
          ossimImageSource, ossimConnectableObjectListener);

static ossimTrace traceDebug("ossimImageSourceSequencer:debug");
   
ossimImageSourceSequencer::ossimImageSourceSequencer(ossimImageSource* inputSource,
                                                     ossimObject* owner)
   :ossimImageSource(owner,
                     1,
                     1,
                     true,
                     true),
    theBlankTile(NULL),
    theAreaOfInterest(0,0,0,0),
    theTileSize(OSSIM_DEFAULT_TILE_WIDTH, OSSIM_DEFAULT_TILE_HEIGHT),
    theNumberOfTilesHorizontal(0),
    theNumberOfTilesVertical(0),
    theCurrentTileNumber(0)
{
   ossim::defaultTileSize(theTileSize);
   theAreaOfInterest.makeNan();
   theInputConnection    = inputSource;
   if(inputSource)
   {
     connectMyInputTo(0, inputSource);
     initialize();
   }
   addListener( (ossimConnectableObjectListener*)this);
}

ossimImageSourceSequencer::~ossimImageSourceSequencer()
{
   removeListener((ossimConnectableObjectListener*)this);
}

ossim_uint32 ossimImageSourceSequencer::getNumberOfTiles()const
{
   return (theNumberOfTilesHorizontal*theNumberOfTilesVertical);
}


ossim_uint32 ossimImageSourceSequencer::getNumberOfTilesHorizontal()const
{
   return theNumberOfTilesHorizontal;
}

ossim_uint32 ossimImageSourceSequencer::getNumberOfTilesVertical()const
{
   return theNumberOfTilesVertical;
}

ossimScalarType ossimImageSourceSequencer::getOutputScalarType() const
{
   if(theInputConnection)
   {
      return theInputConnection->getOutputScalarType();
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

ossimIpt ossimImageSourceSequencer::getTileSize()const
{
   return theTileSize;
}

void ossimImageSourceSequencer::setTileSize(const ossimIpt& tileSize)
{
   theTileSize = tileSize;
   updateTileDimensions();
//   initialize();
}

void ossimImageSourceSequencer::setTileSize(int width, int height)
{
   setTileSize(ossimIpt(width, height));
}

void ossimImageSourceSequencer::updateTileDimensions()
{
   if(theAreaOfInterest.hasNans() ||
      theTileSize.hasNans())
   {
      theNumberOfTilesHorizontal = 0;
      theNumberOfTilesVertical   = 0;
   }
   else
   {
      ossim_int32 width  = theAreaOfInterest.width();
      ossim_int32 height = theAreaOfInterest.height();

      theNumberOfTilesHorizontal = static_cast<ossim_uint32>(ceil((double)width/(double)theTileSize.x));
      theNumberOfTilesVertical   = static_cast<ossim_uint32>(ceil((double)height/(double)theTileSize.y));
   }
}

void ossimImageSourceSequencer::initialize()
{
   theInputConnection = PTR_CAST(ossimImageSource, getInput(0));

   if(theInputConnection)
   {
      if(theTileSize.hasNans())
      {
         theTileSize.x = theInputConnection->getTileWidth();
         theTileSize.y = theInputConnection->getTileHeight();
      }

      ossimDrect rect = theInputConnection->getBoundingRect();
      if(rect.hasNans())
      {
         theAreaOfInterest.makeNan();
      }
      else
      {
         rect.stretchOut();
         setAreaOfInterest(rect);
      }
      updateTileDimensions();
      
      theBlankTile  = ossimImageDataFactory::instance()->create(this,
                                                                this);
      if(theBlankTile.valid())
      {
         theBlankTile->initialize();
      }
   }
}

bool ossimImageSourceSequencer::canConnectMyInputTo(ossim_int32 inputIndex,
						    const ossimConnectableObject* object)const
{      
  return (object&& PTR_CAST(ossimImageSource, object));
}

void ossimImageSourceSequencer::connectInputEvent(ossimConnectionEvent& event)
{
  theInputConnection = PTR_CAST(ossimImageSource, getInput(0));
  initialize();
}

void ossimImageSourceSequencer::disconnectInputEvent(ossimConnectionEvent& event)
{
  theInputConnection = PTR_CAST(ossimImageSource, getInput(0));
}

ossimIrect ossimImageSourceSequencer::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimDrect temp;
   temp.makeNan();

   if(!theInputConnection) return temp;
   
   if(theAreaOfInterest.hasNans())
   {
      return theInputConnection->getBoundingRect(resLevel);
   }
      
   return theAreaOfInterest;
}

void ossimImageSourceSequencer::getDecimationFactor(ossim_uint32 resLevel,
                                                    ossimDpt& result) const
{
   if(theInputConnection)
   {
      theInputConnection->getDecimationFactor(resLevel,
                                              result);
   }
   result.makeNan();
}

void ossimImageSourceSequencer::getDecimationFactors(vector<ossimDpt>& decimations) const
{
   if(theInputConnection)
   {
      theInputConnection->getDecimationFactors(decimations);
   }   
}

ossim_uint32 ossimImageSourceSequencer::getNumberOfDecimationLevels()const
{
   if(theInputConnection)
   {
      return theInputConnection->getNumberOfDecimationLevels();
   }

   return 0;
}

void ossimImageSourceSequencer::setAreaOfInterest(const ossimIrect& areaOfInterest)
{
   if(areaOfInterest.hasNans())
   {
      theAreaOfInterest.makeNan();
      theNumberOfTilesHorizontal = 0;
      theNumberOfTilesVertical   = 0;
   }

   // let's round it to the nearest pixel value before setting it.
   theAreaOfInterest = areaOfInterest;
   updateTileDimensions();
}

const ossimIrect& ossimImageSourceSequencer::getAreaOfInterest()const
{
   return theAreaOfInterest;
}

void ossimImageSourceSequencer::setToStartOfSequence()
{
   theCurrentTileNumber = 0;
}

ossimRefPtr<ossimImageData> ossimImageSourceSequencer::getTile(
   const ossimIrect& rect, ossim_uint32 resLevel)
{
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> tile =
         theInputConnection->getTile(rect, resLevel);
      if (tile.valid()&&tile->getBuf())
      {
         return tile;
      }
      else
      {
         // We should return a valid tile for the writers.
         theBlankTile->setImageRectangle(rect);
         return theBlankTile;
      }
   }

   return 0;
}

ossimRefPtr<ossimImageData> ossimImageSourceSequencer::getNextTile(
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }
   ossimIpt origin;
   if(getTileOrigin(theCurrentTileNumber, origin))
   {
      ++theCurrentTileNumber;

      ossimIrect tileRect(origin.x,
			  origin.y,
			  origin.x + (theTileSize.x - 1),
			  origin.y + (theTileSize.y - 1));
      ossimRefPtr<ossimImageData> data = theInputConnection->getTile(tileRect,
                                                                     resLevel);
      if(!data.valid()||!data->getBuf())
	  {	 
	     theBlankTile->setImageRectangle(tileRect);
		 	
         return theBlankTile;
      }
      
      return data;
   }

   
   return ossimRefPtr<ossimImageData>();
}

ossimRefPtr<ossimImageData> ossimImageSourceSequencer::getTile(
   ossim_int32 id, ossim_uint32 resLevel)
{
   static const char* MODULE= "ossimImageSourceSequencer::getTile(id, resLevel)";
   if(traceDebug())
   {
      CLOG << "entering.."<<endl;
   }
   if(!theInputConnection)
   {
      if(traceDebug())
      {
         CLOG << "No input connection so returing NULL" << endl;
      }
      return NULL;
   }
   // if we have no tiles try to initialize.
   if(getNumberOfTiles() == 0)
   {
      initialize();
   }
   ossimIpt origin;
   
   if(getTileOrigin(id, origin))
   {
      if(traceDebug())
      {
         CLOG << "returning tile" << endl;
      }
      ossimIrect tRect(origin.x,
                       origin.y,
                       origin.x + theTileSize.x - 1,
                       origin.y + theTileSize.y - 1);
      
      ossimRefPtr<ossimImageData> temp = theInputConnection->getTile(tRect,
                                                                     resLevel);


      theBlankTile->setImageRectangle(tRect);
      
      if(temp.valid())
      {
         if(!temp->getBuf())
         {
            return theBlankTile;
         }
      }
      else
      {
         return theBlankTile;
      }
      return temp;
   }
   else
   {
      if(traceDebug())
      {
         CLOG << "was not able to get an origin for id = " << id << endl;
      }
   }
   if(traceDebug())
   {
      CLOG << "leaving.."<<endl;
   }
   return 0;
}


bool ossimImageSourceSequencer::getTileOrigin(ossim_int32 id,
                                              ossimIpt& origin)const
{
   if(id < 0)
   {
      return false;
   }
   if((theNumberOfTilesHorizontal > 0)&&
      (theCurrentTileNumber < getNumberOfTiles()))
   {
      ossim_int32 y = id/static_cast<ossim_int32>(theNumberOfTilesHorizontal);
      ossim_int32 x = id%static_cast<ossim_int32>(theNumberOfTilesHorizontal);
      if((x < static_cast<ossim_int32>(theNumberOfTilesHorizontal)) &&
         (y < static_cast<ossim_int32>(theNumberOfTilesVertical)))
      {           
         origin.x = theAreaOfInterest.ul().x + x*theTileSize.x;
         origin.y = theAreaOfInterest.ul().y + y*theTileSize.y;
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   return true;
}

double ossimImageSourceSequencer::getNullPixelValue(ossim_uint32 band)const
{
   if (theInputConnection)
   {
      return theInputConnection->getNullPixelValue(band);
   }

   return ossimImageSource::getNullPixelValue(band);
}

double ossimImageSourceSequencer::getMinPixelValue(ossim_uint32 band)const
{
   if (theInputConnection)
   {
      return theInputConnection->getMinPixelValue(band);
   }
   
   return ossimImageSource::getMinPixelValue(band);  
}

double ossimImageSourceSequencer::getMaxPixelValue(ossim_uint32 band)const
{
   if (theInputConnection)
   {
      return theInputConnection->getMaxPixelValue(band);
   }
   
   return ossimImageSource::getMaxPixelValue(band);  
}

ossim_uint32 ossimImageSourceSequencer::getNumberOfInputBands()const
{
   if(theInputConnection)
   {
      return theInputConnection->getNumberOfOutputBands();
   }

   return 0;
}

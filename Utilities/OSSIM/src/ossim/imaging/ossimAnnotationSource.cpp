//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationSource.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationObjectFactory.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/imaging/ossimU8ImageData.h>

#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimAnnotationSource, "ossimAnnotationSource", ossimImageSourceFilter)
   
static ossimTrace traceDebug("ossimAnnotationSource:debug");

ossimAnnotationSource::ossimAnnotationSource(ossimImageSource* inputSource)
   :
      ossimImageSourceFilter(inputSource),
      theRectangle(0, 0, 0, 0),
      theNumberOfBands(1),
      theImage(NULL),
      theTile(NULL),
      theAnnotationObjectList()
{
   theRectangle.makeNan();
}

ossimAnnotationSource::~ossimAnnotationSource()
{
   deleteAll();
   destroy();
}

void ossimAnnotationSource::destroy()
{
   if(theImage)
   {
      delete theImage;
      theImage = NULL;
   }
}

void ossimAnnotationSource::allocate(const ossimIrect& rect)
{
   destroy();

   ossim_uint32 width  = rect.width();
   ossim_uint32 height = rect.height();
   
   theImage = new ossimRgbImage();
   
   theTile = new ossimU8ImageData(this,
                                  theNumberOfBands,
                                  width,
                                  height);
   
   theTile->initialize();
}

ossimScalarType ossimAnnotationSource::getOutputScalarType() const
{
   if(theInputConnection && !isSourceEnabled())
   {
      return theInputConnection->getOutputScalarType();
   }
   return OSSIM_UCHAR;
}

ossim_uint32 ossimAnnotationSource::getNumberOfOutputBands()const
{
   if(theInputConnection && !isSourceEnabled())
   {
      return theInputConnection->getNumberOfOutputBands();
   }
   // this source is an RGB source
   return theNumberOfBands;
}


void ossimAnnotationSource::initialize()
{
   //---
   // Call the base class initialize.
   // Note:  This will reset "theInputConnection" if it changed...
   //---
   ossimImageSourceFilter::initialize();

   if (theInputConnection)
   {
      computeBoundingRect();

      ossim_uint32 bands = theInputConnection->getNumberOfOutputBands();
      if (bands == 1 || bands == 3)
      {
         setNumberOfBands(bands);
      }

      if (theTile.valid())  // Check for number of band change.
      {
         if ( theInputConnection->getNumberOfOutputBands() !=
              theTile->getNumberOfBands())
         {
            //---
            // This will wide things slick and force an allocate()
            // call on first getTile().
            //---
            destroy();
         }
      }
   }
   else
   {
      destroy();
   }
}

ossimIrect ossimAnnotationSource::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;
   result.makeNan();
   
   if(theInputConnection)
   {
      ossimIrect temp = theInputConnection->getBoundingRect(resLevel);
      result = temp;
   }
   
   if(result.hasNans())
   {
      result = theRectangle;
   }
   else if(!theRectangle.hasNans())
   {
      result.combine(theRectangle);
   }
      
   return result;
}


void ossimAnnotationSource::setNumberOfBands(ossim_uint32 bands)
{
   theNumberOfBands = bands;

   if(theNumberOfBands == 2) theNumberOfBands = 3;
   if(theNumberOfBands > 3) theNumberOfBands  = 3;
   if(theNumberOfBands < 1) theNumberOfBands  = 1;
}

ossimRefPtr<ossimImageData> ossimAnnotationSource::getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> inputTile = NULL;

   if(theInputConnection)
   {
   // Fetch tile from pointer from the input source.
      inputTile = theInputConnection->getTile(tile_rect,
                                              resLevel);
   }
   
   // Check for remap bypass:
   if ( !theEnableFlag)
   {
      return inputTile;
   }

   // Check for first time through or size or band change.
   if ( !theTile ||  
        tile_rect.height() != theTile->getHeight() ||
        tile_rect.width()  != theTile->getWidth())
   {
      allocate(tile_rect);
   }
   if(!theTile)
   {
      return theTile;
   }
   
   theTile->makeBlank();

   // Set the origin of the output tile.
   theTile->setImageRectangle(tile_rect);
   
   if(inputTile.valid() &&
      inputTile->getBuf()&&
      (inputTile->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      if(theTile->getNumberOfBands() !=
         inputTile->getNumberOfBands())
      {
         int maxBands = ossimMin(theTile->getNumberOfBands(),
                                 inputTile->getNumberOfBands());
         int idx = 0;
         for(idx = 0; idx < maxBands; ++idx)
         {
            theTile->loadBand(inputTile->getBuf(idx),
                              inputTile->getImageRectangle(),
                              idx);
         }
         int saveIdx = idx - 1;
         for(;idx<(int)theTile->getNumberOfBands();++idx)
         {
            theTile->loadBand(inputTile->getBuf(saveIdx),
                              inputTile->getImageRectangle(),
                              idx);
         }
      }
      else
      {
         theTile->loadTile(inputTile.get());
      }
   }

   drawAnnotations(theTile);

   theTile->validate();
      
   return theTile;
}

bool ossimAnnotationSource::addObject(ossimAnnotationObject* anObject)
{
   if(anObject)
   {
      theAnnotationObjectList.push_back(anObject);
      return true;
   }

   return false;
}

bool ossimAnnotationSource::deleteObject(ossimAnnotationObject* anObject)
{
   if(anObject)
   {
      vector<ossimAnnotationObject*>::iterator current = theAnnotationObjectList.begin();
      while(current != theAnnotationObjectList.end())
      {
         if(*current == anObject)
         {
            delete anObject;
            theAnnotationObjectList.erase(current);
            return true;
         }
         ++current;
      }
   }

   return false;
}

bool ossimAnnotationSource::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   // Save the state of all annotation objects we have.
   vector<ossimAnnotationObject*>::const_iterator obj =
      theAnnotationObjectList.begin();
   ossim_uint32 objIdx = 0;
   while (obj < theAnnotationObjectList.end())
   {
      ossimString newPrefix = prefix;
      
      newPrefix += (ossimString("object") +
                    ossimString::toString(objIdx) +
                    ossimString("."));
      (*obj)->saveState(kwl, newPrefix.c_str());
      ++objIdx;
      ++obj;
   }
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimAnnotationSource::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   ossim_uint32 index = 0;
   ossimString copyPrefix = prefix;
   ossim_uint32 result = kwl.getNumberOfSubstringKeys(copyPrefix +
                                                      "object[0-9]+\\.");
   
   ossim_uint32 numberOfMatches = 0;
   const ossim_uint32 MAX_INDEX = result + 100;
   
   while(numberOfMatches < result)
   {
      ossimString newPrefix = copyPrefix;
      newPrefix += ossimString("object");
      newPrefix += ossimString::toString(index);
      newPrefix += ossimString(".");

      const char* lookup = kwl.find(newPrefix.c_str(), "type");
      if (lookup)
      {
         ++numberOfMatches;
         
         ossimAnnotationObject* obj = NULL;
         obj = ossimAnnotationObjectFactory::instance()->
            create(kwl, newPrefix.c_str());
         if (obj)
         {
            if(!addObject(obj))
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimGeoAnnotationSource::loadState\n"
                  << "Object " << obj->getClassName()
                  << " is not a geographic object" << endl;
               delete obj;
            }
         }
      }

      ++index;
      
      if (index > MAX_INDEX) // Avoid infinite loop...
      {
         break;
      }
   }

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimAnnotationSource::computeBoundingRect()
{   
   theRectangle.makeNan();

   if(theAnnotationObjectList.size()>0)
   {
      ossimDrect rect;
      theAnnotationObjectList[0]->computeBoundingRect();
      
      theAnnotationObjectList[0]->getBoundingRect(theRectangle);
      
      vector<ossimAnnotationObject*>::iterator object = (theAnnotationObjectList.begin()+1);
      while(object != theAnnotationObjectList.end())
      {
         (*object)->computeBoundingRect();
         (*object)->getBoundingRect(rect);
         if(theRectangle.hasNans())
         {
            theRectangle = rect;
         }
         else if(!rect.hasNans())
         {  
            theRectangle = theRectangle.combine(rect);
         }
         ++object;
      }
   }
}

vector<ossimAnnotationObject*> ossimAnnotationSource::pickObjects(const ossimDpt& imagePoint)
{
   vector<ossimAnnotationObject*> result;
   vector<ossimAnnotationObject*>::iterator currentObject;

   currentObject = theAnnotationObjectList.begin();

   while(currentObject != theAnnotationObjectList.end())
   {
      if((*currentObject)->isPointWithin(imagePoint))
      {
         result.push_back(*currentObject);
      }
      
      ++currentObject;
   }
   return result;
}

vector<ossimAnnotationObject*> ossimAnnotationSource::pickObjects(const ossimDrect& imageRect)
{
   vector<ossimAnnotationObject*> result;
   vector<ossimAnnotationObject*>::iterator currentObject;

   currentObject = theAnnotationObjectList.begin();

   while(currentObject != theAnnotationObjectList.end())
   {
      ossimAnnotationObject* current = (*currentObject);
      if(current->isPointWithin(imageRect.ul()))
      {
         result.push_back(*currentObject);
      }
      else if(current->isPointWithin(imageRect.ll()))
      {
         result.push_back(*currentObject);
      }
      else if(current->isPointWithin(imageRect.lr()))
      {
         result.push_back(*currentObject);
      }
      else if(current->isPointWithin(imageRect.ur()))
      {
         result.push_back(*currentObject);
      }
      
      ++currentObject;
   }
   return result;
}

void ossimAnnotationSource::deleteAll()
{
   vector<ossimAnnotationObject*>::iterator obj;

   obj = theAnnotationObjectList.begin();

   while(obj != theAnnotationObjectList.end())
   {
      if(*obj)
      {
         delete *obj;
         *obj = NULL;
      }
      
      ++obj;
   }

   theAnnotationObjectList.clear();
}

void ossimAnnotationSource::drawAnnotations(ossimRefPtr<ossimImageData> tile)
{
   theImage->setCurrentImageData(tile);

   if(theImage->getImageData().valid())
   {
      vector<ossimAnnotationObject*>::iterator object = theAnnotationObjectList.begin();
      while(object != theAnnotationObjectList.end())
      {
         if(*object)
         {
            (*object)->draw(*theImage);
         }
         ++object;
      }      
   }
}

const vector<ossimAnnotationObject*>& ossimAnnotationSource::getObjectList()const
{
   return theAnnotationObjectList;
}

vector<ossimAnnotationObject*>& ossimAnnotationSource::getObjectList()
{
   return theAnnotationObjectList;
}

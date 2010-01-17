//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationSource.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimAnnotationObjectFactory.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimAnnotationSource,
          "ossimAnnotationSource",
          ossimImageSourceFilter)
   
ossimAnnotationSource::ossimAnnotationSource(ossimImageSource* inputSource)
   :
      ossimImageSourceFilter(inputSource),
      theRectangle(0, 0, 0, 0),
      theNumberOfBands(1),
      theImage(0),
      theTile(0),
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
   // theImage is an ossimRefPtr<ossimRgbImage> so this is not a leak.
   theImage = 0;

   // theTile is an ossimRefPtr<ossimImageData> so this is not a leak.
   theTile = 0; 
}

void ossimAnnotationSource::allocate(const ossimIrect& rect)
{
   if (!theImage)
   {
      theImage = new ossimRgbImage();
   }

   if (!theTile)
   {
      theTile = new ossimU8ImageData( this,
                                      theNumberOfBands,
                                      rect.width(),
                                      rect.height() );
      theTile->initialize();
   }
}

ossimScalarType ossimAnnotationSource::getOutputScalarType() const
{
   if(theInputConnection && !isSourceEnabled())
   {
      return theInputConnection->getOutputScalarType();
   }
   return OSSIM_UINT8;
}

ossim_uint32 ossimAnnotationSource::getNumberOfOutputBands()const
{
   if(theInputConnection && !isSourceEnabled())
   {
      return theInputConnection->getNumberOfOutputBands();
   }
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

      // This will call destroy on band count change.
      setNumberOfBands(theInputConnection->getNumberOfOutputBands());
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
   if (theTile.get())
   {
      if ( theNumberOfBands != theTile->getNumberOfBands() )
      {
         //---
         // This will wide things slick and force an allocate()
         // call on first getTile().
         //---
         destroy();
      }
   }
}

ossimRefPtr<ossimImageData> ossimAnnotationSource::getTile(
   const ossimIrect& tile_rect, ossim_uint32 resLevel)
{
   ossimRefPtr<ossimImageData> inputTile = 0;

   //---
   // NOTE:
   // This source is written to be used with or without an input connection.
   // So any call to inputTile should be preceeded by: if (inputTile.valid())"
   //---
   if(theInputConnection)
   {
      // Fetch tile from pointer from the input source.
      inputTile = theInputConnection->getTile(tile_rect, resLevel);
   }
   
   // Check for remap bypass:
   if ( !theEnableFlag )
   {
      return inputTile;
   }

   // Check for first time through or reallocation force by band change.
   if ( !theTile )
   {
      allocate(tile_rect);
   }

   // Allocation failed!
   if(!theTile)
   {
      return inputTile;
   }

   //---
   // Set the image rectangle and bands.  This will set the origin.
   // 
   // NOTE:  We do this before the "theTile->makeBlank()" call for efficiency
   // since this will force a "ossimImageData::initialize()", which performs
   // a "makeBlank" if a resize is needed due to tile rectangle or number
   // of band changes.
   //---
   theTile->setImageRectangleAndBands( tile_rect, theNumberOfBands );

   //---
   // Start with a blank tile.
   //
   // NOTE: This will not do anything if already blank.
   //---
   theTile->makeBlank();
   
   if(inputTile.valid() &&
      inputTile->getBuf()&&
      (inputTile->getDataObjectStatus()!=OSSIM_EMPTY))
   {
      //---
      // Copy the input tile to the output tile performing scalar remap if
      // needed.
      //---
      theTile->loadTile(inputTile.get());
   }

   // Annotate the output tile.
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
      AnnotationObjectListType::iterator current =
         theAnnotationObjectList.begin();
      while(current != theAnnotationObjectList.end())
      {
         if(*current == anObject)
         {
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
   AnnotationObjectListType::const_iterator obj =
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
         
         ossimRefPtr<ossimAnnotationObject> obj = 0;
         obj = ossimAnnotationObjectFactory::instance()->
            create(kwl, newPrefix.c_str());
         if (obj.valid())
         {
            if(!addObject(obj.get()))
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimGeoAnnotationSource::loadState\n"
                  << "Object " << obj->getClassName()
                  << " is not a geographic object" << endl;
               obj = 0;
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
      
      AnnotationObjectListType::iterator object =
         (theAnnotationObjectList.begin()+1);
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

ossimAnnotationSource::AnnotationObjectListType ossimAnnotationSource::pickObjects(
   const ossimDpt& imagePoint)
{
   AnnotationObjectListType result;
   AnnotationObjectListType::iterator currentObject;

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

ossimAnnotationSource::AnnotationObjectListType ossimAnnotationSource::pickObjects(
   const ossimDrect& imageRect)
{
   AnnotationObjectListType result;
   AnnotationObjectListType::iterator currentObject;

   currentObject = theAnnotationObjectList.begin();

   while(currentObject != theAnnotationObjectList.end())
   {
      ossimRefPtr<ossimAnnotationObject> current = (*currentObject);
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
   AnnotationObjectListType::iterator obj;

   theAnnotationObjectList.clear();
}

void ossimAnnotationSource::drawAnnotations(ossimRefPtr<ossimImageData> tile)
{
   theImage->setCurrentImageData(tile);

   if(theImage->getImageData().valid())
   {
      AnnotationObjectListType::iterator object =
         theAnnotationObjectList.begin();
      while(object != theAnnotationObjectList.end())
      {
         if((*object).valid())
         {
            (*object)->draw(*theImage);
         }
         ++object;
      }      
   }
}

const ossimAnnotationSource::AnnotationObjectListType&
ossimAnnotationSource::getObjectList()const
{
   return theAnnotationObjectList;
}

ossimAnnotationSource::AnnotationObjectListType& ossimAnnotationSource::getObjectList()
{
   return theAnnotationObjectList;
}

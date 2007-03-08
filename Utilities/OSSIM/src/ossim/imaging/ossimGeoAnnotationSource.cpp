//************************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationSource.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimGeoAnnotationObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

RTTI_DEF2(ossimGeoAnnotationSource,
          "ossimGeoAnnotationSource",
          ossimAnnotationSource,
          ossimViewInterface);
   

ostream& operator <<(ostream& out, const ossimGeoAnnotationSource& rhs)
{
   const vector<ossimAnnotationObject*> &tempList = rhs.getObjectList();
   for(ossim_uint32 index = 0; index < tempList.size(); ++index)
   {
      out << "output " << index << endl;
      tempList[index]->print(out);
   }

   return out;
}

ossimGeoAnnotationSource::ossimGeoAnnotationSource(ossimProjection* projection,
                                                   bool ownsProjectionFlag)
   :ossimAnnotationSource(),
    ossimViewInterface(),
    theProjection(projection),
    theOwnsProjectionFlag(ownsProjectionFlag)
{
   ossimViewInterface::theObject = this;
}

ossimGeoAnnotationSource::ossimGeoAnnotationSource(ossimImageSource* inputSource,
                                                   ossimProjection* projection,
                                                   bool ownsProjectionFlag)
   :ossimAnnotationSource(inputSource),
    ossimViewInterface(),
    theProjection(projection),
    theOwnsProjectionFlag(ownsProjectionFlag)
{
   ossimViewInterface::theObject = this;
}

ossimGeoAnnotationSource::~ossimGeoAnnotationSource()
{
   removeProjection();
}

bool ossimGeoAnnotationSource::addObject(ossimAnnotationObject* anObject)
{
   ossimGeoAnnotationObject *objectToAdd = PTR_CAST(ossimGeoAnnotationObject, anObject);
   
   if(objectToAdd)
   {
      ossimAnnotationSource::addObject(objectToAdd);
      if(theProjection)
      {
         objectToAdd->transform(theProjection);
         computeBoundingRect();
      }
      return true;
   }

   return false;
}

void ossimGeoAnnotationSource::setProjection(ossimProjection* projection,
                                             bool ownsProjectionFlag)
{
   // remove our current set projection first
   removeProjection();

   theProjection         = projection;
   theOwnsProjectionFlag = ownsProjectionFlag;
   transformObjects(theProjection);
   
}

bool ossimGeoAnnotationSource::setView(ossimObject* baseObject,
                                       bool ownsTheView)
{
   ossimProjection* proj = PTR_CAST(ossimProjection, baseObject);
   if(proj)
   {
      setProjection(proj, ownsTheView);

      return true;
   }

   return false;
}

ossimObject* ossimGeoAnnotationSource::getView()
{
   return theProjection;
}

const ossimObject* ossimGeoAnnotationSource::getView()const
{
   return theProjection;
}

void ossimGeoAnnotationSource::computeBoundingRect()
{
//   static const char *MODULE = "ossimAnnotationSource::computeBoundingRect";
   
   theRectangle.makeNan();

   if(theAnnotationObjectList.size()>0)
   {
      ossimDrect rect;
      theAnnotationObjectList[0]->getBoundingRect(theRectangle);
      
      vector<ossimAnnotationObject*>::iterator object = (theAnnotationObjectList.begin()+1);
      while(object != theAnnotationObjectList.end())
      {
         (*object)->getBoundingRect(rect);
         theRectangle = theRectangle.combine(rect);
         ++object;
      }
   }
}

void ossimGeoAnnotationSource::transformObjects(ossimProjection* projection)
{
   if(!theProjection && !projection)
   {
      return;
   }
   if(!projection)
   {
      projection = theProjection;
   }

   if(!theProjection) return;
   vector<ossimAnnotationObject*>::iterator currentObject;

   currentObject = theAnnotationObjectList.begin();

   while(currentObject != theAnnotationObjectList.end())
   {
      // this is safe since we trapped all adds to make
      // sure that each object added to the list is
      // geographic.
      ossimGeoAnnotationObject* object = static_cast<ossimGeoAnnotationObject*>(*currentObject);

      // transform the object to image space.
      object->transform(projection);

      ++currentObject;
   }
   computeBoundingRect();
}

void ossimGeoAnnotationSource::removeProjection()
{
   if(theProjection&&theOwnsProjectionFlag)
   {
      delete theProjection;
   }

   theProjection = 0;
}

bool ossimGeoAnnotationSource::getImageGeometry(ossimKeywordlist& kwl,
						const char* prefix)
{
  if(theProjection)
    {
      theProjection->saveState(kwl, prefix);
      return true;
    }
  return ossimImageSource::getImageGeometry(kwl, prefix);
}

bool ossimGeoAnnotationSource::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   return ossimAnnotationSource::saveState(kwl, prefix);
}


bool ossimGeoAnnotationSource::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl,"view_proj.");

   if(proj)
   {
      removeProjection();
      theOwnsProjectionFlag = true;
      theProjection = proj;
   }
   
   return ossimAnnotationSource::loadState(kwl, prefix);
}

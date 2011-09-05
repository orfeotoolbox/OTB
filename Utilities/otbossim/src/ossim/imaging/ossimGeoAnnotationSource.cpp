//************************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationSource.cpp 19900 2011-08-04 14:19:57Z dburken $

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
   const ossimAnnotationSource::AnnotationObjectListType &tempList = rhs.getObjectList();
   for(ossim_uint32 index = 0; index < tempList.size(); ++index)
   {
      out << "output " << index << endl;
      tempList[index]->print(out);
   }

   return out;
}

ossimGeoAnnotationSource::ossimGeoAnnotationSource(ossimImageGeometry* geom,
                                                   bool /* ownsProjectionFlag */ )
   :ossimAnnotationSource(),
    ossimViewInterface(),
    m_geometry(geom)
{
   ossimViewInterface::theObject = this;
}

ossimGeoAnnotationSource::ossimGeoAnnotationSource(ossimImageSource* inputSource,
                                                   ossimImageGeometry* geom,
                                                   bool /* ownsProjectionFlag */)
   :ossimAnnotationSource(inputSource),
    ossimViewInterface(),
    m_geometry(geom)
{
   ossimViewInterface::theObject = this;
}

ossimGeoAnnotationSource::~ossimGeoAnnotationSource()
{
}

bool ossimGeoAnnotationSource::addObject(ossimAnnotationObject* anObject)
{
   ossimGeoAnnotationObject *objectToAdd = PTR_CAST(ossimGeoAnnotationObject, anObject);
   
   if(objectToAdd)
   {
      ossimAnnotationSource::addObject(objectToAdd);
      if(m_geometry.valid())
      {
         objectToAdd->transform(m_geometry.get());
         computeBoundingRect();
      }
      return true;
   }

   return false;
}

void ossimGeoAnnotationSource::setGeometry(ossimImageGeometry* geom)
{
   m_geometry = geom;
   transformObjects(m_geometry.get());
}

bool ossimGeoAnnotationSource::setView(ossimObject* baseObject)
{
   bool result = false;
   ossimProjection* proj = PTR_CAST(ossimProjection, baseObject);
   if(proj)
   {
      if(m_geometry.valid())
      {
         m_geometry->setProjection(proj);
      }
      else
      {
         m_geometry = new ossimImageGeometry(0, proj);
      }
      setGeometry(m_geometry.get());
      result = true;
   }
   else
   {
      m_geometry = dynamic_cast<ossimImageGeometry*>(baseObject);
      if ( m_geometry.valid() )
      {
         result = true;
      }
   }
   return result;
}

ossimObject* ossimGeoAnnotationSource::getView()
{
   return m_geometry.get();
}

const ossimObject* ossimGeoAnnotationSource::getView()const
{
   return m_geometry.get();
}

void ossimGeoAnnotationSource::computeBoundingRect()
{
//   static const char *MODULE = "ossimAnnotationSource::computeBoundingRect";
   
   theRectangle.makeNan();

   if(theAnnotationObjectList.size()>0)
   {
      ossimDrect rect;
      theAnnotationObjectList[0]->getBoundingRect(theRectangle);
      
      AnnotationObjectListType::iterator object = (theAnnotationObjectList.begin()+1);
      while(object != theAnnotationObjectList.end())
      {
         (*object)->getBoundingRect(rect);
         theRectangle = theRectangle.combine(rect);
         ++object;
      }
   }
}

void ossimGeoAnnotationSource::transformObjects(ossimImageGeometry* geom)
{
   ossimRefPtr<ossimImageGeometry> tempGeom = geom;
   if(!tempGeom)
   {
      tempGeom = m_geometry.get();
   }

   if(!tempGeom) return;
   AnnotationObjectListType::iterator currentObject;

   currentObject = theAnnotationObjectList.begin();

   while(currentObject != theAnnotationObjectList.end())
   {
      // this is safe since we trapped all adds to make
      // sure that each object added to the list is
      // geographic.
      ossimGeoAnnotationObject* object = static_cast<ossimGeoAnnotationObject*>((*currentObject).get());

      // transform the object to image space.
      object->transform(m_geometry.get());

      ++currentObject;
   }
   computeBoundingRect();
}

ossimRefPtr<ossimImageGeometry> ossimGeoAnnotationSource::getImageGeometry()
{
   if(!m_geometry.valid())
   {
      return ossimImageSource::getImageGeometry();
   }
   return m_geometry;
}

bool ossimGeoAnnotationSource::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   return ossimAnnotationSource::saveState(kwl, prefix);
}


bool ossimGeoAnnotationSource::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   m_geometry = new ossimImageGeometry;
   
   ossimString newPrefix = ossimString(prefix)+"view_proj.";
   if(!m_geometry->loadState(kwl, newPrefix.c_str()))
   {
      m_geometry = 0;
   }
   else
   {
      if(!m_geometry->hasProjection())
      {
         m_geometry = 0;
      }
   }
   
   return ossimAnnotationSource::loadState(kwl, prefix);
}

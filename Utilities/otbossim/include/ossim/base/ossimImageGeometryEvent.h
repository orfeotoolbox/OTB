//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageGeometryEvent.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageGeometryEvent_HEADER
#define ossimImageGeometryEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>

class ossimImageGeometryEvent : public ossimEvent
{
public:
   ossimImageGeometryEvent(ossimObject* object=0,
                           long id=OSSIM_EVENT_IMAGE_GEOMETRY_ID)
      :ossimEvent(object, id)
      {
      }
   
   ossimImageGeometryEvent(const ossimKeywordlist& geomKwl,
                           ossimObject* object=NULL,
                           long id=OSSIM_EVENT_IMAGE_GEOMETRY_ID)
      :ossimEvent(object, id),
       theGeomKwl(geomKwl)
      {
      }
   ossimImageGeometryEvent(const ossimImageGeometryEvent& rhs)
      :ossimEvent(rhs),
       theGeomKwl(rhs.theGeomKwl)
      {
      }
   virtual ~ossimImageGeometryEvent(){}
   virtual ossimObject* dup()const
      {
         return new ossimImageGeometryEvent(*this);
      }
   void setGeometry(const ossimKeywordlist& geomKwl)
      {
         theGeomKwl = geomKwl;
      }
   void getGeometry(ossimKeywordlist& geomKwl)const
      {
         geomKwl = theGeomKwl;
      }
   const ossimKeywordlist& getGeometry()const
      {
         return theGeomKwl;
      }
   bool isGeometrySet()const
      {
         return (theGeomKwl.getSize() != 0);
      }
   void clearGeometry()
      {
         theGeomKwl.clear();
      }
protected:
   ossimKeywordlist theGeomKwl;

TYPE_DATA
};

#endif

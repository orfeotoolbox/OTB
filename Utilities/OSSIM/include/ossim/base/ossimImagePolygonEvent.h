//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImagePolygonEvent.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImagePolygonEvent_HEADER
#define ossimImagePolygonEvent_HEADER
#include <vector>
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>


class OSSIMDLLEXPORT ossimImagePolygonEvent : public ossimEvent
{
public:
   ossimImagePolygonEvent(const std::vector<ossimIpt>& polygon,
                          ossimObject* obj=NULL)
      : ossimEvent(obj,OSSIM_EVENT_AOI_POLYGON_ID) ,
        thePolygon(polygon)
      {
      }
   ossimImagePolygonEvent(const std::vector<ossimDpt>& polygon,
                          ossimObject* obj=NULL)
      : ossimEvent(obj,OSSIM_EVENT_AOI_POLYGON_ID) ,
        thePolygon(polygon.begin(),
                   polygon.end())
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimImagePolygonEvent(*this);
      }
   
   const std::vector<ossimIpt>& getpolygon()const
      {
         return thePolygon;
      }
   const std::vector<ossimIpt>& getPolygon()const
      {
         return thePolygon;
      }
   void setPolygon(const std::vector<ossimIpt>& polygon)
      {
         thePolygon = polygon;
      }
   void setPolygon(const std::vector<ossimDpt>& polygon)
      {
         thePolygon.clear();
         thePolygon.insert(thePolygon.begin(),
                           polygon.begin(),
                           polygon.end());
      }
   
protected:
   std::vector<ossimIpt> thePolygon;
   
};

#endif

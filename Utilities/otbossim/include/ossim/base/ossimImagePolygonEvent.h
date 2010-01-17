//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImagePolygonEvent.h 13016 2008-06-10 16:06:58Z dburken $
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
                          ossimObject* obj=0)
      : ossimEvent(obj,OSSIM_EVENT_AOI_POLYGON_ID) ,
        thePolygon(polygon)
      {
      }
      
   ossimImagePolygonEvent(const std::vector<ossimDpt>& polygon,
                          ossimObject* obj=0)
      : ossimEvent(obj,OSSIM_EVENT_AOI_POLYGON_ID),
        thePolygon(polygon.size())
      {
         for (std::vector<ossimDpt>::size_type i = 0; i < polygon.size(); ++i)
         {
            thePolygon[i] = polygon[i];
         }
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
         thePolygon.resize(polygon.size());
         for (std::vector<ossimDpt>::size_type i = 0; i < polygon.size(); ++i)
         {
            thePolygon[i] = polygon[i];
         }
      }
   
protected:
   std::vector<ossimIpt> thePolygon;
   
};

#endif

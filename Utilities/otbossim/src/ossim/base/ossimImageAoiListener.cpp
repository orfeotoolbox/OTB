//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImageAoiListener.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimImageAoiListener.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimImageRectangleEvent.h>
#include <ossim/base/ossimImagePolygonEvent.h>

void ossimImageAoiListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
   case OSSIM_EVENT_AOI_RECTANGLE_ID:
   {
      ossimImageRectangleEvent* eventCast = static_cast<ossimImageRectangleEvent*>(&event);
      imageRectangleEvent(*eventCast);
      break;
   }
   case OSSIM_EVENT_AOI_POLYGON_ID:
   {
      ossimImagePolygonEvent* eventCast = static_cast<ossimImagePolygonEvent*>(&event);
      imagePolygonEvent(*eventCast);
      break;
   }
   default:
   {
      ossimListener::processEvent(event);
      break;
   }
   }
}

//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageGeometryEventListener.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimImageGeometryEventListener.h>

RTTI_DEF1(ossimImageGeometryEventListener,
          "ossimImageGeometryEventListener",
          ossimListener);

void ossimImageGeometryEventListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
   case  OSSIM_EVENT_IMAGE_GEOMETRY_ID:
   {
      ossimImageGeometryEvent* tempCast = (ossimImageGeometryEvent*)(&event);
      imageGeometryEvent(*tempCast);
      
      break;
   }
   default:
   {
      ossimListener::processEvent(event);
   }
   }
}

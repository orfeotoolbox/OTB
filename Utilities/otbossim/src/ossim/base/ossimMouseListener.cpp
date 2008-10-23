//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimMouseListener.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/base/ossimMouseListener.h>
#include <ossim/base/ossimMouseEvent.h>

RTTI_DEF1(ossimMouseListener,"ossimMouseListener",ossimListener);

void ossimMouseListener::processEvent(ossimEvent& event)
{
   ossimMouseEvent* mEvent = PTR_CAST(ossimMouseEvent, &event);

   if(mEvent)
   {
      mouseEvent(*mEvent);
   }
   else
   {
      ossimListener::processEvent(event);
   }
//    switch(event.getId())
//    {
//    case OSSIM_MOUSE_EVENT_ID:
//    {
//        = static_cast<ossimMouseEvent*>(&event);
//       mouseEvent(*mEvent);
//       break;
//    }
//    default:
//    {
//       ossimListener::processEvent(event);
//    }
//    }
}

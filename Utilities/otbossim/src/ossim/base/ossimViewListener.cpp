//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimViewListener.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimViewListener.h>

RTTI_DEF1(ossimViewListener, "ossimViewListener", ossimListener);
void ossimViewListener::processEvent(ossimEvent& event)
{
   ossimViewEvent* evt = PTR_CAST(ossimViewEvent, &event);

   if(evt)
   {
      switch(evt->getViewEventType())
      {
      case  ossimViewEvent::OSSIM_VIEW_EVENT_SCALE_CHANGE:
      {
         viewScaleChangeEvent(*evt);
         break;
      }
      case  ossimViewEvent::OSSIM_VIEW_EVENT_TYPE_GENERIC:
      {
         viewEvent(*evt);
         break;
      }
      case ossimViewEvent::OSSIM_VIEW_EVENT_TRANSFORM_CHANGE:
      {
         viewTransformChangeEvent(*evt);
         break;
      }
      case ossimViewEvent::OSSIM_VIEW_EVENT_VIEW_TYPE_CHANGE:
      {
         viewTypeChangeEvent(*evt);
         break;
      }
      }
      allViewEvents(*evt);
   }
}

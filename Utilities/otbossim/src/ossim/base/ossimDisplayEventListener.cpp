//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id

#include <ossim/base/ossimDisplayEventListener.h>
#include <ossim/base/ossimDisplayListEvent.h>
#include <ossim/base/ossimDisplayRefreshEvent.h>

RTTI_DEF1(ossimDisplayEventListener, "ossimDisplayEventListener", ossimListener);

ossimDisplayEventListener::ossimDisplayEventListener()
   : ossimListener()
{
}

ossimDisplayEventListener::~ossimDisplayEventListener()
{
}

void ossimDisplayEventListener::processEvent(ossimEvent& event)
{
   ossimDisplayListEvent* displayListEvtPtr = PTR_CAST(ossimDisplayListEvent, &event);

   if(displayListEvtPtr)
   {
      displayListEvent(*displayListEvtPtr);
   }

   ossimDisplayRefreshEvent* displayRefreshEvt = PTR_CAST(ossimDisplayRefreshEvent, &event);
   if(displayRefreshEvt)
   {
      displayRefreshEvent(*displayRefreshEvt);
   }
   
}

void ossimDisplayEventListener::displayListEvent(ossimDisplayListEvent&)
{
}

void ossimDisplayEventListener::displayRefreshEvent(ossimDisplayRefreshEvent&)
{
}

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts 
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimRefreshEvent.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimRefreshEvent_HEADER
#define ossimRefreshEvent_HEADER
#include <ossim/base/ossimEvent.h>

class OSSIMDLLEXPORT ossimRefreshEvent : public ossimEvent
{
public:

   ossimRefreshEvent(ossimObject* object=NULL)  // the object associated with the event if any
      :ossimEvent(object, OSSIM_EVENT_REFRESH_ID)
      {}
   ossimRefreshEvent(const ossimRefreshEvent& rhs)
      :ossimEvent(rhs)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimRefreshEvent(*this);
      }
 
TYPE_DATA
};

#endif

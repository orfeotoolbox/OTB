//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimEvent.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimEvent_HEADER
#define ossimEvent_HEADER
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimEvent : public ossimObject
{
public:

   /**
    * @param object The object associated with the event if any.
    *
    * @param id The event id.
    */
   ossimEvent(ossimObject* object=NULL,
              long id=OSSIM_EVENT_NULL_ID);

   ossimEvent(const ossimEvent& rhs);

   long getId()const;
   void setId(long id);

   bool isConsumed()const;
   void setConsumedFlag(bool flag=true);
   void consume();

   const ossimObject* getObject()const;
   ossimObject*       getObject();

   void setObject(ossimObject* object);

protected:
   ossimObject* theObject;
   long         theId;
   bool         theIsConsumedFlag;

TYPE_DATA
};

#endif /* #ifndef ossimEvent_HEADER */

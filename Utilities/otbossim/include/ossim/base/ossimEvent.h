//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimEvent.h 13362 2008-08-01 14:02:32Z gpotts $
#ifndef ossimEvent_HEADER
#define ossimEvent_HEADER
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimEvent : public ossimObject
{
public:
   enum PropagationType
   {
      PROPAGATION_NONE = 0,
      PROPAGATION_INPUT = 1,
      PROPAGATION_OUTPUT = 2
   };
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

   /**
    * This is the originating object that originally 
    * produced the event.
    */
   const ossimObject* getObject()const;
   ossimObject*       getObject();
   const ossimObject* getCurrentObject()const;
   ossimObject*       getCurrentObject();

   void setObject(ossimObject* object);
   void setCurrentObject(ossimObject* object);

   void setPropagationType(PropagationType type);
   bool isPropagatingToOutputs()const;
   bool isPropagatingToInputs()const;
protected:
   ossimObject* theObject;
   ossimObject* theCurrentObject;
   long         theId;
   bool         theIsConsumedFlag;
   PropagationType thePropagationType;

TYPE_DATA
};

#endif /* #ifndef ossimEvent_HEADER */

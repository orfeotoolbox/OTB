//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimContainerEvent_HEADER
#define ossimContainerEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>

class OSSIMDLLEXPORT ossimContainerEvent : public ossimEvent
{
public:

   /** constuctor */
   ossimContainerEvent(ossimObject* obj1,// the object theEvent was sent to.
                       ossimObject* obj2, // the object associated 
                       long id);

   ossimObject* getAssociatedObject();
   const ossimObject* getAssociatedObject()const;
   void setAssociatedObject(ossimObject* obj);
   
protected:
   
   /*!
    * This is the object associated with the container event.
    * if this was an add then it s the object added to the
    * container.
    */
   ossimObject* theAssociatedObject;

TYPE_DATA
};

#endif /* end of: "#ifndef ossimContainerEvent_HEADER" */

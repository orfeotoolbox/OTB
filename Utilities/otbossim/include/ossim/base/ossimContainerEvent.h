//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.h 19966 2011-08-16 18:12:56Z gpotts $
#ifndef ossimContainerEvent_HEADER
#define ossimContainerEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimRefPtr.h>
#include <vector>

class OSSIMDLLEXPORT ossimContainerEvent : public ossimEvent
{
public:

   typedef std::vector<ossimRefPtr<ossimObject> > ObjectList;
   
   /** constuctor */
   ossimContainerEvent(ossimObject* obj1,// the object firing the event.
                       long id);

   void setObjectList(ossimObject* obj);
   void setObjectList(ObjectList& objects);
   ObjectList& getObjectList(){return m_objectList;}
   const ObjectList& getObjectList()const{return m_objectList;}
   
protected:
   
   /*!
    * This is the object associated with the container event.
    * if this was an add then it s the object added to the
    * container.
    */
      ObjectList m_objectList;

TYPE_DATA
};

#endif /* end of: "#ifndef ossimContainerEvent_HEADER" */

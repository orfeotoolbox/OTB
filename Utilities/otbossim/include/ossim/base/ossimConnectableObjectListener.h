//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimConnectableObjectListener.h 19962 2011-08-16 18:10:52Z gpotts $
#ifndef ossimConnectableObjectListener_HEADER
#define ossimConnectableObjectListener_HEADER

#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimConnectionEvent.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimContainerEvent.h>
#include <ossim/base/ossimRefreshEvent.h>

class OSSIMDLLEXPORT ossimConnectableObjectListener : public ossimListener
{
public:
   ossimConnectableObjectListener():ossimListener(){}
   virtual ~ossimConnectableObjectListener(){}
   
   virtual void processEvent(ossimEvent& event);
   
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& /*event*/)
   {}
   virtual void connectionEvent(ossimConnectionEvent& /* event */)
   {}
   virtual void disconnectInputEvent(ossimConnectionEvent& /* event */)
   {}
   virtual void disconnectOutputEvent(ossimConnectionEvent& /* event */)
   {}
   virtual void connectInputEvent(ossimConnectionEvent& /* event */)
   {}
   virtual void connectOutputEvent(ossimConnectionEvent& /* event */)
   {}
   
   virtual void propertyEvent(ossimPropertyEvent& /* event */)
   {}
   
   /*!
    * Typically isued by objects that contain children.  If anyone is
    * interested, can latch on to this event.  Other objects within the
    * system might be interest in this event even 
    */
   virtual void addObjectEvent(ossimContainerEvent& /* event */)
   {}
   
   virtual void removeObjectEvent(ossimContainerEvent& /* event */)
   {}
   
   virtual void containerEvent(ossimContainerEvent& /* event */)
   {}
   
   virtual void refreshEvent(ossimRefreshEvent& /* event */)
   {}
   TYPE_DATA
};

#endif

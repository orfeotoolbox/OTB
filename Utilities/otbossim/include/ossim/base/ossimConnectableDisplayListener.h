//----------------------------------------------------------------------------
// Copyright (c) 2004, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimConnectableDisplayListener
// 
// This class is intended for connectable displays that wants to be a
// ossimConnectableObjectListener and a ossimDisplayEventListener to avoid
// a triangle inheritance.
//
//----------------------------------------------------------------------------
// $Id: ossimConnectableDisplayListener.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimConnectableDisplayListener_HEADER
#define ossimConnectableDisplayListener_HEADER

#include <ossim/base/ossimListener.h>

class ossimConnectionEvent;
class ossimObjectDestructingEvent;
class ossimPropertyEvent;
class ossimContainerEvent;
class oossimDisplayListEvent;
class ossimRefreshEvent;
class ossimDisplayListEvent;
class ossimDisplayRefreshEvent;

/**
 * This class is intended for connectable displays that wants to be a
 * ossimConnectableObjectListener and a ossimDisplayEventListener to avoid
 * a triangle inheritance.
 */
class OSSIMDLLEXPORT ossimConnectableDisplayListener : public ossimListener
{
public:

   /** default constructor */
   ossimConnectableDisplayListener();

   /**
    * Method called by listener managers.  This will in turn call the correct
    * xxxEvent method.
    *
    * @param event Event to handle.
    */
   virtual void processEvent(ossimEvent& event);

   /** @param event Event to handle signifying an object destroying. */
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   /** @param event Event to handle signifying an object connecting. */
   virtual void connectionEvent(ossimConnectionEvent& event);

   /** @param event Event to handle signifying a disconnect input event. */
   virtual void disconnectInputEvent(ossimConnectionEvent& event);

   /** @param event Event to handle signifying an output disconnect event. */
   virtual void disconnectOutputEvent(ossimConnectionEvent& event);

   /** @param event Event to handle signifying a connect input event. */
   virtual void connectInputEvent(ossimConnectionEvent& event);

   /** @param event Event to handle signifying a connect  output event. */
   virtual void connectOutputEvent(ossimConnectionEvent& event);

   /**
    * @param event Event to handle signifying a property has changed  event.
    */
   virtual void propertyEvent(ossimPropertyEvent& event);

   /**
    * @param event Event to handle signifying a dislay list  has changed
    * event.
    */
   virtual void displayListEvent(ossimDisplayListEvent&);

   /**
    * @param event Event to handle signifying a dislay refresh
    * is needed.
    */
   virtual void displayRefreshEvent(ossimDisplayRefreshEvent&);

   /**
    * @param event Event to handle signifying a refresh is needed.
    */
   virtual void refreshEvent(ossimRefreshEvent& event);

   /**
    * Container Events:
    * Typically isued by objects that contain children.  If anyone is
    * interested, can latch on to this event.  Other objects within the
    * system might be interest in this event even 
    */

   /** @param event Event signifying an object was added to a container. */
   virtual void addObjectEvent(ossimContainerEvent& event);

   /** @param event Event signifying an object was removed from a container. */
   virtual void removeObjectEvent(ossimContainerEvent& event);

TYPE_DATA
};

#endif

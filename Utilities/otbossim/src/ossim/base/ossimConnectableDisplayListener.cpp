
//----------------------------------------------------------------------------
// Copyright (c) 2004, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: 
//
//----------------------------------------------------------------------------
// $Id: ossimConnectableDisplayListener.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimConnectableDisplayListener.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimDisplayListEvent.h>
#include <ossim/base/ossimDisplayRefreshEvent.h>

#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimConnectableDisplayListener,
          "ossimConnectableDisplayListener",
          ossimListener);

ossimConnectableDisplayListener::ossimConnectableDisplayListener()
   : ossimListener()
{}

void ossimConnectableDisplayListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
      case OSSIM_EVENT_OBJECT_DESTRUCTING_ID:
      {
         ossimObjectDestructingEvent* eventCast =
            static_cast<ossimObjectDestructingEvent*>(&event);
         objectDestructingEvent(*eventCast);
         
         break;
      }
      case OSSIM_EVENT_CONNECTION_CONNECT_ID:
      case OSSIM_EVENT_CONNECTION_DISCONNECT_ID:
      {
         ossimConnectionEvent* eventCast =
            static_cast<ossimConnectionEvent*>(&event);
         
         connectionEvent(*eventCast);
         if(event.getId() == OSSIM_EVENT_CONNECTION_DISCONNECT_ID)
         {
            if(eventCast->isInputDirection())
            {
               disconnectInputEvent(*eventCast);
            }
            else if(eventCast->isOutputDirection())
            {
               disconnectOutputEvent(*eventCast);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimConnectableDisplayListener::processEvent, Direction not set\n";
            }
         }
         else
         {
            ossimConnectionEvent* eventCast = static_cast<ossimConnectionEvent*>(&event);
            
            if(eventCast->isInputDirection())
            {
               connectInputEvent(*eventCast);
            }
            else if(eventCast->isOutputDirection())
            {
               connectOutputEvent(*eventCast);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN) << "ossimConnectableDisplayListener::processEvent, Direction not set\n";
            }
         }
         break;
      }
      case OSSIM_EVENT_PROPERTY_ID:
      {
         ossimPropertyEvent* eventCast = static_cast<ossimPropertyEvent*>(&event);
         propertyEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_ADD_OBJECT_ID:
      {
         ossimContainerEvent* eventCast = static_cast<ossimContainerEvent*>(&event);
         addObjectEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_REMOVE_OBJECT_ID:
      {
         ossimContainerEvent* eventCast = static_cast<ossimContainerEvent*>(&event);
         removeObjectEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_REFRESH_ID:
      {
         ossimRefreshEvent* eventCast = static_cast<ossimRefreshEvent*>(&event);
         refreshEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_DISPLAY_LIST_ID:
      {
         ossimDisplayListEvent* eventCast =
            static_cast<ossimDisplayListEvent*>(&event);
         displayListEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_DISPLAY_REFRESH_ID:
      {
         ossimDisplayRefreshEvent* eventCast =
            static_cast<ossimDisplayRefreshEvent*>(&event);
         displayRefreshEvent(*eventCast);
         break;
      }
      default:
      {
         ossimListener::processEvent(event);
         break;
      }
   }
}

void ossimConnectableDisplayListener::objectDestructingEvent(
   ossimObjectDestructingEvent& /*event*/)
{
}

void ossimConnectableDisplayListener::connectionEvent(
   ossimConnectionEvent& /* event */)
{}

void ossimConnectableDisplayListener::disconnectInputEvent(
   ossimConnectionEvent& /* event */)
{}

void ossimConnectableDisplayListener::disconnectOutputEvent(
   ossimConnectionEvent& /* event */)
{}

void ossimConnectableDisplayListener::connectInputEvent(
   ossimConnectionEvent& /* event */)
{}

void ossimConnectableDisplayListener::connectOutputEvent(
   ossimConnectionEvent& /* event */)
{}

void ossimConnectableDisplayListener::propertyEvent(
   ossimPropertyEvent& /* event */)
{}

void ossimConnectableDisplayListener::displayListEvent(ossimDisplayListEvent&)
{}

void ossimConnectableDisplayListener::displayRefreshEvent(
   ossimDisplayRefreshEvent&)
{}

void ossimConnectableDisplayListener::addObjectEvent(
   ossimContainerEvent& /* event */)
{}

void ossimConnectableDisplayListener::removeObjectEvent(
   ossimContainerEvent& /* event */)
{}

void ossimConnectableDisplayListener::refreshEvent(
   ossimRefreshEvent& /* event */)
{}


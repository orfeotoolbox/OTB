//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectableObjectListener.cpp 19961 2011-08-16 18:10:36Z gpotts $

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimConnectableObjectListener,
          "ossimConnectableObjectListener",
          ossimListener);

void ossimConnectableObjectListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
      case OSSIM_EVENT_OBJECT_DESTRUCTING_ID:
      {
         ossimObjectDestructingEvent* eventCast = dynamic_cast<ossimObjectDestructingEvent*>(&event);
         if(eventCast) objectDestructingEvent(*eventCast);
         
         break;
      }
      case OSSIM_EVENT_CONNECTION_CONNECT_ID:
      case OSSIM_EVENT_CONNECTION_DISCONNECT_ID:
      {
         ossimConnectionEvent* eventCast = dynamic_cast<ossimConnectionEvent*>(&event);
         
         if(eventCast)
         {
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
                  ossimNotify(ossimNotifyLevel_WARN) << "ossimConnectableObjectListener::processEvent, Direction not set\n";
               }
            }
            else
            {
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
                  ossimNotify(ossimNotifyLevel_WARN) << "ossimConnectableObjectListener::processEvent, Direction not set\n";
               }
            }
         }
         break;
      }
      case OSSIM_EVENT_PROPERTY_ID:
      {
         ossimPropertyEvent* eventCast = dynamic_cast<ossimPropertyEvent*>(&event);
         if(eventCast) propertyEvent(*eventCast);
         break;
      }
      case OSSIM_EVENT_ADD_OBJECT_ID:
      {
         ossimContainerEvent* eventCast = dynamic_cast<ossimContainerEvent*>(&event);
         if(eventCast)
         {
            containerEvent(*eventCast);
            addObjectEvent(*eventCast);
         }
         break;
      }
      case OSSIM_EVENT_REMOVE_OBJECT_ID:
      {
         ossimContainerEvent* eventCast = dynamic_cast<ossimContainerEvent*>(&event);
         if(eventCast)
         {
            containerEvent(*eventCast);
            removeObjectEvent(*eventCast);
         }
         break;
      }
      case OSSIM_EVENT_REFRESH_ID:
      {
         ossimRefreshEvent* eventCast = dynamic_cast<ossimRefreshEvent*>(&event);
         if(eventCast) refreshEvent(*eventCast);
         break;
      }
      default:
      {
         ossimListener::processEvent(event);
         break;
      }
   }
}

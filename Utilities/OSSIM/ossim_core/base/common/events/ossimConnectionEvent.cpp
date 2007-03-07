//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectionEvent.cpp,v 1.3 2005/09/17 20:27:50 dburken Exp $

#include <base/common/events/ossimConnectionEvent.h>

RTTI_DEF1(ossimConnectionEvent, "ossimConnectionEvent", ossimEvent);

ossimConnectionEvent::ossimConnectionEvent(ossimObject* object, long id)
   :ossimEvent(object, id),
    theDirectionType(OSSIM_DIRECTION_UNKNOWN)
{
}

ossimConnectionEvent::ossimConnectionEvent(
   ossimObject* object,
   long id,
   const vector<ossimConnectableObject*>& newList,
   const vector<ossimConnectableObject*>& oldList,
   ossimConnectionDirectionType whichDirection)
   : ossimEvent(object, id),
     theNewObjectList(newList),
     theOldObjectList(oldList),
     theDirectionType(whichDirection)
{
}

ossimConnectionEvent::ossimConnectionEvent(
   ossimObject* object,
   long id,
   ossimConnectableObject* newConnectableObject,
   ossimConnectableObject* oldConnectableObject,
   ossimConnectionDirectionType whichDirection)
   : ossimEvent(object, id),
     theDirectionType(whichDirection)
{
   if(newConnectableObject)
   {
      theNewObjectList.push_back(newConnectableObject);
   }
   if(oldConnectableObject)
   {
      theOldObjectList.push_back(oldConnectableObject);
   }
}
   
ossimConnectionEvent::ossimConnectionEvent(const ossimConnectionEvent& rhs)
   : ossimEvent(rhs),
     theNewObjectList(rhs.theNewObjectList),
     theOldObjectList(rhs.theOldObjectList),
     theDirectionType(rhs.theDirectionType)
{
}

ossimObject* ossimConnectionEvent::dup()const
{
   return new ossimConnectionEvent(*this);
}

void ossimConnectionEvent::setDirection(
   ossimConnectionDirectionType direction)
{
   theDirectionType   = direction;
}

ossimConnectionEvent::ossimConnectionDirectionType ossimConnectionEvent::getDirection()const
{
   return theDirectionType;
}

ossim_uint32 ossimConnectionEvent::getNumberOfNewObjects()const
{
   return theNewObjectList.size();
}

ossim_uint32 ossimConnectionEvent::getNumberOfOldObjects()const
{
   return theOldObjectList.size();
}

ossimConnectableObject* ossimConnectionEvent::getOldObject(ossim_uint32 i)const
{
   if(i < getNumberOfOldObjects())
   {
      return theOldObjectList[i];
   }
   
   return (ossimConnectableObject*)NULL;
}

ossimConnectableObject* ossimConnectionEvent::getNewObject(ossim_uint32 i)const
{
   if(i < getNumberOfNewObjects())
   {
      return theNewObjectList[i];
   }
   
   return (ossimConnectableObject*)NULL;
}

bool ossimConnectionEvent::isDisconnect()const
{
   return (getId()==OSSIM_EVENT_CONNECTION_DISCONNECT_ID);
}

bool ossimConnectionEvent::isConnect()const
{
   return (getId()==OSSIM_EVENT_CONNECTION_CONNECT_ID);
}

bool ossimConnectionEvent::isInputDirection()const
{
   return ((long)theDirectionType  & (long)OSSIM_INPUT_DIRECTION);
}

bool ossimConnectionEvent::isOutputDirection()const
{
   return ((long)theDirectionType & OSSIM_OUTPUT_DIRECTION);
}

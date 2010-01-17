//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimConnectionEvent.cpp 15833 2009-10-29 01:41:53Z eshirschorn $

#include <ossim/base/ossimConnectionEvent.h>

RTTI_DEF1(ossimConnectionEvent, "ossimConnectionEvent", ossimEvent);

ossimConnectionEvent::ossimConnectionEvent(ossimObject* object, long id)
   :ossimEvent(object, id),
    theDirectionType(OSSIM_DIRECTION_UNKNOWN)
{
}

ossimConnectionEvent::ossimConnectionEvent(
   ossimObject* object,
   long id,
   const ossimConnectableObject::ConnectableObjectList& newList,
   const ossimConnectableObject::ConnectableObjectList& oldList,
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
   return (ossim_uint32)theNewObjectList.size();
}

ossim_uint32 ossimConnectionEvent::getNumberOfOldObjects()const
{
   return (ossim_uint32)theOldObjectList.size();
}

ossimConnectableObject* ossimConnectionEvent::getOldObject(ossim_uint32 i)
{
   if(i < getNumberOfOldObjects())
   {
      return theOldObjectList[i].get();
   }
   
   return (ossimConnectableObject*)NULL;
}

ossimConnectableObject* ossimConnectionEvent::getNewObject(ossim_uint32 i)
{
   if(i < getNumberOfNewObjects())
   {
      return theNewObjectList[i].get();
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

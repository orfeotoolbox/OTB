//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.cpp,v 1.1 2005/09/17 20:25:33 dburken Exp $

#include <base/common/events/ossimContainerEvent.h>

RTTI_DEF1(ossimContainerEvent, "ossimContainerEvent", ossimEvent);

ossimContainerEvent::ossimContainerEvent(ossimObject* obj1,
                                         ossimObject* obj2,
                                         long id)
   :
   ossimEvent(obj1, id),
   theAssociatedObject(obj2)
{
}

ossimObject* ossimContainerEvent::getAssociatedObject()
{
   return theAssociatedObject;
}

const ossimObject* ossimContainerEvent::getAssociatedObject()const
{
   return theAssociatedObject;
}

void ossimContainerEvent::setAssociatedObject(ossimObject* obj)
{
   theAssociatedObject = obj;
}

//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimContainerEvent.h>

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

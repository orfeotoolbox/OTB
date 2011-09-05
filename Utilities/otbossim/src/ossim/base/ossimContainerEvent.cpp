//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimContainerEvent.cpp 19965 2011-08-16 18:12:15Z gpotts $

#include <ossim/base/ossimContainerEvent.h>

RTTI_DEF1(ossimContainerEvent, "ossimContainerEvent", ossimEvent);

ossimContainerEvent::ossimContainerEvent(ossimObject* obj,
                                         long id)
   :ossimEvent(obj, id)
{
}

void ossimContainerEvent::setObjectList(ossimObject* obj)
{
   m_objectList.clear();
   m_objectList.push_back(obj);
}

void ossimContainerEvent::setObjectList(ObjectList& objects)
{
   m_objectList = objects;
}

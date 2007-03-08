//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimEvent.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/base/ossimEvent.h>

RTTI_DEF1(ossimEvent, "ossimEvent", ossimObject);


ossimEvent::ossimEvent(ossimObject* object, long id)
   :
      ossimObject(),
      theObject(object),
      theId(id),
      theIsConsumedFlag(false)
{
}

ossimEvent::ossimEvent(const ossimEvent& rhs)
   :
      ossimObject(),
      theObject(rhs.theObject),
      theId(rhs.theId),
      theIsConsumedFlag(rhs.theIsConsumedFlag)
{
}

long ossimEvent::getId() const
{
   return theId;
}

void ossimEvent::setId(long id)
{
   theId = id;
}

bool ossimEvent::isConsumed() const
{
   return theIsConsumedFlag;
}

void ossimEvent::setConsumedFlag(bool flag)
{
   theIsConsumedFlag = flag;
}

void ossimEvent::consume()
{
   setConsumedFlag(true);
}

const ossimObject* ossimEvent::getObject() const
{
   return theObject;
}

ossimObject* ossimEvent::getObject()
{
   return theObject;
}

void ossimEvent::setObject(ossimObject* object)
{
   theObject = object;
}

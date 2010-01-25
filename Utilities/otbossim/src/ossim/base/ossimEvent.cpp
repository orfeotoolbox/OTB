//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimEvent.cpp 13362 2008-08-01 14:02:32Z gpotts $
#include <ossim/base/ossimEvent.h>

RTTI_DEF1(ossimEvent, "ossimEvent", ossimObject);


ossimEvent::ossimEvent(ossimObject* object, long id)
:
ossimObject(),
theObject(object),
theCurrentObject(object),
theId(id),
theIsConsumedFlag(false),
thePropagationType(PROPAGATION_NONE)
{
}

ossimEvent::ossimEvent(const ossimEvent& rhs)
:
ossimObject(),
theObject(rhs.theObject),
theCurrentObject(rhs.theCurrentObject),
theId(rhs.theId),
theIsConsumedFlag(rhs.theIsConsumedFlag),
thePropagationType(PROPAGATION_NONE)

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

const ossimObject* ossimEvent::getCurrentObject() const
{
   return theCurrentObject;
}

ossimObject* ossimEvent::getCurrentObject()
{
   return theCurrentObject;
}

void ossimEvent::setObject(ossimObject* object)
{
   theObject = object;
}

void ossimEvent::setCurrentObject(ossimObject* object)
{
   theCurrentObject = object;
}

void ossimEvent::setPropagationType(PropagationType type)
{
   thePropagationType = type;
}

bool ossimEvent::isPropagatingToOutputs()const
{
   return thePropagationType&PROPAGATION_OUTPUT;
}

bool ossimEvent::isPropagatingToInputs()const
{
   return thePropagationType&PROPAGATION_INPUT;
}


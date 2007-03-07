//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimEvent.cpp,v 1.3 2004/06/10 17:57:10 dburken Exp $
#include "ossimEvent.h"

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

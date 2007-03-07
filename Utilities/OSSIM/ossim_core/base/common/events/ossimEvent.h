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
// $Id: ossimEvent.h,v 1.10 2005/09/09 19:23:58 gpotts Exp $
#ifndef ossimEvent_HEADER
#define ossimEvent_HEADER
#include "ossimEventIds.h"
#include "base/common/ossimObject.h"

class OSSIMDLLEXPORT ossimEvent : public ossimObject
{
public:

   /**
    * @param object The object associated with the event if any.
    *
    * @param id The event id.
    */
   ossimEvent(ossimObject* object=NULL,
              long id=OSSIM_EVENT_NULL_ID);

   ossimEvent(const ossimEvent& rhs);

   long getId()const;
   void setId(long id);

   bool isConsumed()const;
   void setConsumedFlag(bool flag=true);
   void consume();

   const ossimObject* getObject()const;
   ossimObject*       getObject();

   void setObject(ossimObject* object);

protected:
   ossimObject* theObject;
   long         theId;
   bool         theIsConsumedFlag;

TYPE_DATA
};

#endif /* #ifndef ossimEvent_HEADER */

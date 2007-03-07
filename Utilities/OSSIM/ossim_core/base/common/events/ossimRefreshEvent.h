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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimRefreshEvent.h,v 1.2 2003/01/06 19:02:30 gpotts Exp $
#ifndef ossimRefreshEvent_HEADER
#define ossimRefreshEvent_HEADER
#include "ossimEvent.h"

class OSSIMDLLEXPORT ossimRefreshEvent : public ossimEvent
{
public:

   ossimRefreshEvent(ossimObject* object=NULL)  // the object associated with the event if any
      :ossimEvent(object, OSSIM_EVENT_REFRESH_ID)
      {}
   ossimRefreshEvent(const ossimRefreshEvent& rhs)
      :ossimEvent(rhs)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimRefreshEvent(*this);
      }
 
TYPE_DATA
};

#endif

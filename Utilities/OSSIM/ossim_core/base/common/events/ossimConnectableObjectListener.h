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
// $Id: ossimConnectableObjectListener.h,v 1.19 2005/10/13 13:30:33 gpotts Exp $
#ifndef ossimConnectableObjectListener_HEADER
#define ossimConnectableObjectListener_HEADER

#include "ossimListener.h"
#include "ossimConnectionEvent.h"
#include "ossimObjectDestructingEvent.h"
#include "ossimPropertyEvent.h"
#include "ossimContainerEvent.h"
#include "ossimRefreshEvent.h"

class OSSIMDLLEXPORT ossimConnectableObjectListener : public ossimListener
{
public:
   ossimConnectableObjectListener():ossimListener(){}
   virtual ~ossimConnectableObjectListener(){}

   virtual void processEvent(ossimEvent& event);

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& /*event*/)
      {}
   virtual void connectionEvent(ossimConnectionEvent& /* event */)
      {}
   virtual void disconnectInputEvent(ossimConnectionEvent& /* event */)
      {}
   virtual void disconnectOutputEvent(ossimConnectionEvent& /* event */)
      {}
   virtual void connectInputEvent(ossimConnectionEvent& /* event */)
      {}
   virtual void connectOutputEvent(ossimConnectionEvent& /* event */)
      {}

   virtual void propertyEvent(ossimPropertyEvent& /* event */)
      {}

   /*!
    * Typically isued by objects that contain children.  If anyone is
    * interested, can latch on to this event.  Other objects within the
    * system might be interest in this event even 
    */
   virtual void addObjectEvent(ossimContainerEvent& /* event */)
      {}

   virtual void removeObjectEvent(ossimContainerEvent& /* event */)
      {}

   virtual void refreshEvent(ossimRefreshEvent& /* event */)
      {}
TYPE_DATA
};

#endif

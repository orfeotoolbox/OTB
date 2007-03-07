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
// Author: Garrett Potts
//
//*************************************************************************
// $Id

#include "ossimDisplayEventListener.h"
#include "ossimDisplayListEvent.h"
#include "ossimDisplayRefreshEvent.h"

RTTI_DEF1(ossimDisplayEventListener, "ossimDisplayEventListener", ossimListener);

ossimDisplayEventListener::ossimDisplayEventListener()
   : ossimListener()
{
}

ossimDisplayEventListener::~ossimDisplayEventListener()
{
}

void ossimDisplayEventListener::processEvent(ossimEvent& event)
{
   ossimDisplayListEvent* displayListEvtPtr = PTR_CAST(ossimDisplayListEvent, &event);

   if(displayListEvtPtr)
   {
      displayListEvent(*displayListEvtPtr);
   }

   ossimDisplayRefreshEvent* displayRefreshEvt = PTR_CAST(ossimDisplayRefreshEvent, &event);
   if(displayRefreshEvt)
   {
      displayRefreshEvent(*displayRefreshEvt);
   }
   
}

void ossimDisplayEventListener::displayListEvent(ossimDisplayListEvent&)
{
}

void ossimDisplayEventListener::displayRefreshEvent(ossimDisplayRefreshEvent&)
{
}

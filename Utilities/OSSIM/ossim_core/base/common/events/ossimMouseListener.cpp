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
//
//*************************************************************************
// $Id: ossimMouseListener.cpp,v 1.4 2002/09/25 12:01:18 gpotts Exp $

#include "base/common/events/ossimMouseListener.h"
#include "base/common/events/ossimMouseEvent.h"

RTTI_DEF1(ossimMouseListener,"ossimMouseListener",ossimListener);

void ossimMouseListener::processEvent(ossimEvent& event)
{
   ossimMouseEvent* mEvent = PTR_CAST(ossimMouseEvent, &event);

   if(mEvent)
   {
      mouseEvent(*mEvent);
   }
   else
   {
      ossimListener::processEvent(event);
   }
//    switch(event.getId())
//    {
//    case OSSIM_MOUSE_EVENT_ID:
//    {
//        = static_cast<ossimMouseEvent*>(&event);
//       mouseEvent(*mEvent);
//       break;
//    }
//    default:
//    {
//       ossimListener::processEvent(event);
//    }
//    }
}

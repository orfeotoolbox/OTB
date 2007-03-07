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
// $Id: ossimImageAoiListener.cpp,v 1.2 2002/04/09 21:20:03 gpotts Exp $
#include "ossimImageAoiListener.h"
#include "ossimEventIds.h"
#include "ossimImageRectangleEvent.h"
#include "ossimImagePolygonEvent.h"

void ossimImageAoiListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
   case OSSIM_EVENT_AOI_RECTANGLE_ID:
   {
      ossimImageRectangleEvent* eventCast = static_cast<ossimImageRectangleEvent*>(&event);
      imageRectangleEvent(*eventCast);
      break;
   }
   case OSSIM_EVENT_AOI_POLYGON_ID:
   {
      ossimImagePolygonEvent* eventCast = static_cast<ossimImagePolygonEvent*>(&event);
      imagePolygonEvent(*eventCast);
      break;
   }
   default:
   {
      ossimListener::processEvent(event);
      break;
   }
   }
}

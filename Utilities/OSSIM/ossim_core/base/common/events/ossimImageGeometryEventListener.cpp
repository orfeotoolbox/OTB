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
// $Id: ossimImageGeometryEventListener.cpp,v 1.1 2003/12/03 13:57:46 gpotts Exp $
#include "ossimImageGeometryEventListener.h"

RTTI_DEF1(ossimImageGeometryEventListener, "ossimImageGeometryEventListener", ossimListener);

void ossimImageGeometryEventListener::processEvent(ossimEvent& event)
{
   switch(event.getId())
   {
   case  OSSIM_EVENT_IMAGE_GEOMETRY_ID:
   {
      ossimImageGeometryEvent* tempCast = (ossimImageGeometryEvent*)(&event);
      imageGeometryEvent(*tempCast);
      
      break;
   }
   default:
   {
      ossimListener::processEvent(event);
   }
   }
}

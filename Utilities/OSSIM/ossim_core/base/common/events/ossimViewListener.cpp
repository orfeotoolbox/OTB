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
// $Id: ossimViewListener.cpp,v 1.3 2002/12/20 15:14:44 gpotts Exp $
#include "ossimViewListener.h"

RTTI_DEF1(ossimViewListener, "ossimViewListener", ossimListener);
void ossimViewListener::processEvent(ossimEvent& event)
{
   ossimViewEvent* evt = PTR_CAST(ossimViewEvent, &event);

   if(evt)
   {
      switch(evt->getViewEventType())
      {
      case  ossimViewEvent::OSSIM_VIEW_EVENT_SCALE_CHANGE:
      {
         viewScaleChangeEvent(*evt);
         break;
      }
      case  ossimViewEvent::OSSIM_VIEW_EVENT_TYPE_GENERIC:
      {
         viewEvent(*evt);
         break;
      }
      case ossimViewEvent::OSSIM_VIEW_EVENT_TRANSFORM_CHANGE:
      {
         viewTransformChangeEvent(*evt);
         break;
      }
      case ossimViewEvent::OSSIM_VIEW_EVENT_VIEW_TYPE_CHANGE:
      {
         viewTypeChangeEvent(*evt);
         break;
      }
      }
      allViewEvents(*evt);
   }
}

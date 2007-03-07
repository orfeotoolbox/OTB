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
//
//*************************************************************************
// $Id: ossimMouseEvent.h,v 1.11 2003/01/06 19:01:24 gpotts Exp $

#ifndef ossimMouseEvent_HEADER
#define ossimMouseEvent_HEADER
#include "ossimEvent.h"
#include "base/data_types/ossimIpt.h"

class OSSIMDLLEXPORT ossimMouseEvent : public ossimEvent
{
public:
   ossimMouseEvent(const ossimDpt& scenePoint=ossimDpt(0,0),
                   const ossimDpt& relViewPoint=ossimDpt(0,0),
                   ossimObject* object = NULL,
                   long id = OSSIM_MOUSE_EVENT_ID,
                   long mouseType = 0)
      :ossimEvent(object, id),
       theScenePoint(scenePoint),
       theRelViewPoint(relViewPoint),
       theMouseType(mouseType)
      {
      }
   
   virtual ossimObject* dup()const{return new ossimMouseEvent(*this);}
   
   bool controlDown() const { return theControlKeyDown; }
   bool altDown() const { return theAltKeyDown; }
   bool shiftDown() const { return theShiftKeyDown; }
   
   // Find which event was just generated
   bool leftDown() const{   return (theMouseType == OSSIM_MOUSE_EVENT_LEFT_DOWN_ID); }
   bool middleDown() const{ return (theMouseType == OSSIM_MOUSE_EVENT_MIDDLE_DOWN_ID); }
   bool rightDown() const{  return (theMouseType == OSSIM_MOUSE_EVENT_RIGHT_DOWN_ID); }
   bool leftUp() const{   return (theMouseType == OSSIM_MOUSE_EVENT_LEFT_UP_ID); }
   bool middleUp() const{ return (theMouseType == OSSIM_MOUSE_EVENT_MIDDLE_UP_ID); }
   bool rightUp() const{  return (theMouseType == OSSIM_MOUSE_EVENT_RIGHT_UP_ID); }
   
   bool leftDClick() const { return (theMouseType == OSSIM_MOUSE_EVENT_LEFT_DCLICK_ID); }
   bool middleDClick() const { return (theMouseType == OSSIM_MOUSE_EVENT_MIDDLE_DCLICK_ID); }
   bool rightDClick() const { return (theMouseType == OSSIM_MOUSE_EVENT_RIGHT_DCLICK_ID); }
   
   // Find the current state of the mouse buttons (regardless
   // of current event type)
   bool leftIsDown() const   { return theLeftDown; }
   bool middleIsDown() const { return theMiddleDown; }
   bool rightIsDown() const  { return theRightDown; }
   
   // True if a button is down and the mouse is moving
   bool dragging() const
      {
         return (moving() &&
                 (leftIsDown() || middleIsDown() || rightIsDown()));
      }

   // True if the mouse is moving, and no button is down
   bool moving() const { return (theMouseType == OSSIM_MOUSE_EVENT_MOTION_ID); }
   
   // True if the mouse is just entering the window
   bool entering() const { return (theMouseType == OSSIM_MOUSE_EVENT_ENTER_WINDOW_ID); }
   
   // True if the mouse is just leaving the window
   bool leaving() const { return (theMouseType   == OSSIM_MOUSE_EVENT_LEAVE_WINDOW_ID); }
   
   ossimDpt theScenePoint;
   ossimDpt theRelViewPoint;
   int     theMouseType;
   bool     theAltKeyDown;
   bool     theControlKeyDown;
   bool     theShiftKeyDown;
   bool     theLeftDown;
   bool     theRightDown;
   bool     theMiddleDown;
   
TYPE_DATA
};

#endif

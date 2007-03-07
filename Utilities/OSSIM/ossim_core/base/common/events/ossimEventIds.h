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
// $Id: ossimEventIds.h,v 1.16 2003/12/03 13:57:16 gpotts Exp $
#ifndef ossimEventIds_HEADER
#define ossimEventIds_HEADER

#define OSSIM_EVENT_NULL_ID  -1


// destruction event
#define OSSIM_EVENT_OBJECT_DESTRUCTING_ID     1

// connection events
#define OSSIM_EVENT_CONNECTION_DISCONNECT_ID        3
#define OSSIM_EVENT_CONNECTION_CONNECT_ID           4

#define OSSIM_EVENT_REFRESH_ID  5
#define OSSIM_EVENT_PROCESS_PROGRESS_ID  20

#define OSSIM_EVENT_ADD_OBJECT_ID     40
#define OSSIM_EVENT_REMOVE_OBJECT_ID  41


#define OSSIM_EVENT_PROPERTY_ID       80
#define OSSIM_EVENT_STATE_CHANGED_ID  90


#define OSSIM_EVENT_AOI_RECTANGLE_ID  200
#define OSSIM_EVENT_AOI_POLYGON_ID    201


#define OSSIM_MOUSE_EVENT_ID               400
#define OSSIM_MOUSE_EVENT_LEAVE_WINDOW_ID  401
#define OSSIM_MOUSE_EVENT_MOTION_ID        402
#define OSSIM_MOUSE_EVENT_LEFT_DOWN_ID     403
#define OSSIM_MOUSE_EVENT_RIGHT_DOWN_ID    404
#define OSSIM_MOUSE_EVENT_MIDDLE_DOWN_ID   405
#define OSSIM_MOUSE_EVENT_LEFT_UP_ID       406
#define OSSIM_MOUSE_EVENT_RIGHT_UP_ID      407
#define OSSIM_MOUSE_EVENT_MIDDLE_UP_ID     408

#define OSSIM_MOUSE_EVENT_LEFT_DCLICK_ID       409
#define OSSIM_MOUSE_EVENT_RIGHT_DCLICK_ID      410
#define OSSIM_MOUSE_EVENT_MIDDLE_DCLICK_ID     411
#define OSSIM_MOUSE_EVENT_ENTER_WINDOW_ID  412

#define OSSIM

#define OSSIM_KEYBOARD_EVENT_ID  500

#define OSSIM_EVENT_VIEW_ID      600


#define OSSIM_EVENT_DISPLAY_LIST_ID           700
#define OSSIM_EVENT_DISPLAY_REFRESH_ID        701

#define OSSIM_EVENT_IMAGE_GEOMETRY_ID         800

// this is the first user defined id.  If users want to add
// additional id's they need to start at this id.  That way
// we have no conflicts.
//
#define OSSIM_FIRST_USER_DEFINED_EVENT_ID     1000000

#endif

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
// $Id: ossimMouseListener.h,v 1.2 2002/11/25 18:49:10 gpotts Exp $
#ifndef ossimMouseListener_HEADER
#define ossimMouseListener_HEADER
#include "ossimListener.h"
#include "ossimMouseEvent.h"

class OSSIMDLLEXPORT ossimMouseListener : public ossimListener
{
public:
   ossimMouseListener():ossimListener(){}
   virtual ~ossimMouseListener(){}
   
   virtual void processEvent(ossimEvent& event);

   /*!
    * processes all mouse events
    */
   virtual void mouseEvent(ossimMouseEvent& event)
      {}

TYPE_DATA
};
#endif

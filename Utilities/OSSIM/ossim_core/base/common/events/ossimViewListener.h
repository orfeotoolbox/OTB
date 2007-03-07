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
// $Id: ossimViewListener.h,v 1.3 2002/12/20 15:14:38 gpotts Exp $
#ifndef ossimViewListener_HEADER
#define ossimViewListener_HEADER
#include "ossimListener.h"
#include "ossimViewEvent.h"

class OSSIMDLLEXPORT ossimViewListener : public ossimListener
{
public:
   ossimViewListener():ossimListener(){}
   virtual ~ossimViewListener(){}
   
   virtual void processEvent(ossimEvent& event);
   virtual void viewScaleChangeEvent(ossimViewEvent& event)
      {}
   virtual void viewTypeChangeEvent(ossimViewEvent& event)
      {}
   virtual void viewEvent(ossimViewEvent& event)
      {}
   virtual void viewTransformChangeEvent(ossimViewEvent& event)
      {}

   virtual void allViewEvents(ossimViewEvent& event)
      {}
TYPE_DATA
};

#endif

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
// $Id: ossimImageRectangleEvent.h,v 1.3 2003/01/06 19:01:05 gpotts Exp $
#ifndef ossimImageRectangleEvent_HEADER
#define ossimImageRectangleEvent_HEADER
#include "ossimEvent.h"
#include "ossimEventIds.h"
#include "base/data_types/ossimIrect.h"

class OSSIMDLLEXPORT ossimImageRectangleEvent : public ossimEvent
{
public:
   ossimImageRectangleEvent(const ossimIrect& rect,
                            ossimObject* obj=NULL)
      : ossimEvent(obj,OSSIM_EVENT_AOI_RECTANGLE_ID) ,
        theRectangle(rect)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimImageRectangleEvent(*this);
      }
   
   const ossimIrect& getRectangle()const
      {
         return theRectangle;
      }
   void setRectangle(const ossimIrect& rect)
      {
         theRectangle = rect;
      }
   
protected:
   ossimIrect theRectangle;
   
};

#endif

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
// $Id: ossimViewEvent.h,v 1.8 2003/01/06 19:03:02 gpotts Exp $
#ifndef ossimViewEvent_HEADER
#define ossimViewEvent_HEADER
#include "ossimEvent.h"
#include "base/data_types/ossimGpt.h"

class OSSIMDLLEXPORT ossimViewEvent : public ossimEvent
{
public:
   enum ossimViewEventType
   {
      OSSIM_VIEW_EVENT_TYPE_GENERIC     = 0,
      OSSIM_VIEW_EVENT_SCALE_CHANGE,
      OSSIM_VIEW_EVENT_TRANSFORM_CHANGE,
      OSSIM_VIEW_EVENT_VIEW_TYPE_CHANGE 
   };
   enum ossimViewPropagateType
   {
      OSSIM_VIEW_EVENT_PROPAGATE_NONE         = 0,
      OSSIM_VIEW_EVENT_PROPAGATE_ALL_DISPLAYS = 1,
      OSSIM_VIEW_EVENT_PROPAGATE_ALL_CHAINS   = 2,
   };
   
   ossimViewEvent(ossimObject* view,
                  ossimGpt     centerPt,
                  ossimViewPropagateType propagateType = OSSIM_VIEW_EVENT_PROPAGATE_NONE,
                  ossimViewEventType eventType = OSSIM_VIEW_EVENT_TYPE_GENERIC,
                  ossimObject* object=NULL)
      :ossimEvent(object, OSSIM_EVENT_VIEW_ID),
       theView(view),
       theCenterPoint(centerPt),
       theEventType(eventType),
       thePropagateType(propagateType),
       theUpdateInputViewFlag(true),
       theRefreshDisplayFlag(true)
      {
      }
   ossimViewEvent(const ossimViewEvent& rhs)
      :ossimEvent(rhs),
       theView(rhs.theView),
       theCenterPoint(rhs.theCenterPoint),
       theEventType(rhs.theEventType),
       thePropagateType(rhs.thePropagateType),
       theUpdateInputViewFlag(rhs.theUpdateInputViewFlag),
       theRefreshDisplayFlag(rhs.theRefreshDisplayFlag)
      {
      }
   virtual ossimObject* dup()const
      {
         return new ossimViewEvent(*this);
      }
   virtual void setView(ossimObject* view)
      {
         theView = view;
      }
   virtual ossimObject* getView()
      {
         return theView;
      }
   void disablePropagation()
      {
         thePropagateType = OSSIM_VIEW_EVENT_PROPAGATE_NONE;
      }
   void setPropagateType(ossimViewPropagateType type)
      {
         thePropagateType = type;         
      }
   void setViewEventType(ossimViewEventType eventType)
      {
         theEventType = eventType;
      }
   void setEventTypeViewTypeChange()
      {
         theEventType = OSSIM_VIEW_EVENT_VIEW_TYPE_CHANGE;
      }
   void setEventTypeTransformChange()
      {
         theEventType = OSSIM_VIEW_EVENT_TRANSFORM_CHANGE;
      }
   void setEventTypeGeneric()
      {
         theEventType = OSSIM_VIEW_EVENT_TYPE_GENERIC;
      }
   void setCenterGroundPoint(const ossimGpt& gpt)
      {
         theCenterPoint = gpt;
      }
   const ossimGpt& getCenterGroundPoint()const
      {
         return theCenterPoint;
      }
   ossimViewEventType getViewEventType()const
      {
         return theEventType;
      }
   bool isViewChange()const
      {
         return (theEventType == OSSIM_VIEW_EVENT_VIEW_TYPE_CHANGE);
      }
   bool isTransformChange()const
      {
         return (theEventType == OSSIM_VIEW_EVENT_TRANSFORM_CHANGE);
      }
   bool isScaleChange()const
      {
         return (theEventType == OSSIM_VIEW_EVENT_SCALE_CHANGE);
         
      }
   bool isPropagateEnabled()const
      {
         return (thePropagateType != OSSIM_VIEW_EVENT_PROPAGATE_NONE);
      }
   bool isPropagateToDisplays()const
      {
         return (thePropagateType & OSSIM_VIEW_EVENT_PROPAGATE_ALL_DISPLAYS);
      }
   bool isPropagateToChains()const
      {
         return (thePropagateType & OSSIM_VIEW_EVENT_PROPAGATE_ALL_CHAINS);
      }
   void setUpdateInputViewFlag(bool flag)
      {
         theUpdateInputViewFlag = flag;
      }
   bool getUpdateInputViewFlag()const
      {
         return theUpdateInputViewFlag;
      }
   void setRefreshDisplayFlag(bool flag)
      {
         theRefreshDisplayFlag = flag;
      }
   bool getRefreshDisplayFlag()const
      {
         return theRefreshDisplayFlag;
      }
protected:
   ossimObject*           theView;
   ossimGpt               theCenterPoint;
   ossimViewEventType     theEventType;
   ossimViewPropagateType thePropagateType;
   bool                   theUpdateInputViewFlag;
   bool                   theRefreshDisplayFlag;
TYPE_DATA
};
#endif

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimViewEvent.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimViewEvent_HEADER
#define ossimViewEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimGpt.h>
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

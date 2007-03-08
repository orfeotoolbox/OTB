//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageRectangleEvent.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageRectangleEvent_HEADER
#define ossimImageRectangleEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimIrect.h>

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

//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Scott Bortman
//
// Description:   Region Of Interest (ROI) Event
//
// $Id: ossimROIEvent.cpp 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------
#include <ossim/base/ossimROIEvent.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimPolyLine.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF1(ossimROIEvent, "ossimROIEvent", ossimEvent);

ossimROIEvent::ossimROIEvent(ossimObject* object,
                             long id)
   : ossimEvent( object, id ),
     theType(OSSIM_RECTANGLE_ROI),
     theMovingFlag(false)
   
{
}

ossimROIEvent::ossimROIEvent( const ossimROIEvent& rhs )
   : ossimEvent    ( rhs ),
     theType       ( rhs.theType ),
     theMovingFlag ( rhs.theMovingFlag)
{
}

ossimObject* ossimROIEvent::dup() const
{
   return new ossimROIEvent( *this );
}

const vector<ossimIpt>& ossimROIEvent::getPoints()const
{
   return thePoints;
}

void ossimROIEvent::getPoint(vector<ossimIpt>& points)const 
{
   points = thePoints;
}

void ossimROIEvent::setPoints( const vector<ossimIpt>& points )
{
   thePoints = points;
}

void ossimROIEvent::setRect(const ossimIrect& rect)
{
   thePoints.clear();
   thePoints.push_back(rect.ul());
   thePoints.push_back(rect.lr());
}

void ossimROIEvent::setEventType( ossimRegionType eventType )
{
   theType = eventType;
}

ossimROIEvent::ossimRegionType ossimROIEvent::getEventType()const
{
   return theType;
}

void ossimROIEvent::setTypeToPolygon()
{
   theType = OSSIM_POLYGON_ROI;
}

void ossimROIEvent::setTypeToRectangle()
{
   theType = OSSIM_RECTANGLE_ROI;
}

void ossimROIEvent::setTypeToPolyline()
{
   theType = OSSIM_POLYLINE_ROI;
}

bool ossimROIEvent::isRectangleRegion()const
{
   return (theType == OSSIM_RECTANGLE_ROI);
}

bool ossimROIEvent::isPolygonRegion()const
{
   return (theType == OSSIM_POLYGON_ROI);
}

bool ossimROIEvent::isPolylineRegion()const
{
   return (theType == OSSIM_POLYLINE_ROI);
}

void ossimROIEvent::getRect(ossimIrect& rect)const
{
   rect = ossimIrect(thePoints);
}

void ossimROIEvent::getPolygon(ossimPolygon& polygon)const
{
   polygon = ossimPolygon(thePoints);
}

void ossimROIEvent::getPolyLine(ossimPolyLine& polyline)const
{
   polyline = ossimPolyLine(thePoints);
}

void ossimROIEvent::setMovingFlag(bool flag)
{
   theMovingFlag = flag;
}

bool ossimROIEvent::getMovingFlag() const
{
   return theMovingFlag;
}

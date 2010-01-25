//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Scott Bortman
//
// Description:   Region Of Interest (ROI) Event
//
// $Id: ossimROIEvent.h 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimROIEvent_HEADER
#define ossimROIEvent_HEADER

#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimIpt.h>
#include <vector>

class ossimPolygon;
class ossimPolyLine;
class ossimIrect;
class OSSIMDLLEXPORT ossimROIEvent : public ossimEvent
{
public:
   ossimROIEvent( ossimObject* object=NULL,
                  long id=OSSIM_EVENT_NULL_ID );
   ossimROIEvent( const ossimROIEvent& rhs );
   virtual ossimObject* dup() const;
   
   enum ossimRegionType
   {
      OSSIM_RECTANGLE_ROI      = 0,
      OSSIM_POLYGON_ROI        = 1,
      OSSIM_POLYLINE_ROI       = 2
   };

   const std::vector<ossimIpt>& getPoints()const;
   void getPoint(std::vector<ossimIpt>& points)const ;
   void setPoints( const std::vector<ossimIpt>& points );
   void setRect(const ossimIrect& rect);
   void setEventType( ossimRegionType eventType );
   ossimRegionType getEventType()const;
   void setTypeToPolygon();
   void setTypeToRectangle();
   void setTypeToPolyline();
   bool isRectangleRegion()const;
   bool isPolygonRegion()const;
   bool isPolylineRegion()const;
   void getRect(ossimIrect& rect)const;
   void getPolygon(ossimPolygon& polygon)const;
   void getPolyLine(ossimPolyLine& polyline)const;

   /**
    * @param flag If true indicates region of interest is moving and size is
    * not to change.
    */
   void setMovingFlag(bool flag);

   /**
    * @return Returns theMoving flag.  If true indicates region of interest
    * is moving and size is not to change.
    */
   bool getMovingFlag() const;
   
 protected:
   std::vector<ossimIpt> thePoints;
   ossimRegionType  theType;

   // Indicates moving so keep the same size on a redraw.
   bool             theMovingFlag;

TYPE_DATA
};

#endif

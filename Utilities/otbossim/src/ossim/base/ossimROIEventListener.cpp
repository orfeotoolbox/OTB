#include <ossim/base/ossimROIEventListener.h>
#include <ossim/base/ossimROIEvent.h>


RTTI_DEF1(ossimROIEventListener, "ossimROIEventListener", ossimListener);

void ossimROIEventListener::processEvent(ossimEvent& event)
{
   if(event.isConsumed()) return;
   
   ossimROIEvent* roiEvent = PTR_CAST(ossimROIEvent,
                                      &event);
   
   if(roiEvent)
   {
      const int type = roiEvent->getEventType();
      
      switch ( type )
      {
      case ossimROIEvent::OSSIM_RECTANGLE_ROI:
         handleRectangleROIEvent( *roiEvent );
         break;

      case ossimROIEvent::OSSIM_POLYGON_ROI:
         handlePolygonROIEvent( *roiEvent );
         break;


      case ossimROIEvent::OSSIM_POLYLINE_ROI:
         handlePolylineROIEvent( *roiEvent );
         break;

         default:
         break;
      }
   }
}

void ossimROIEventListener::handleRectangleROIEvent( ossimROIEvent& /* event */)
{
}

void ossimROIEventListener::handlePolygonROIEvent( ossimROIEvent& /* event */)
{
}

void ossimROIEventListener::handlePolylineROIEvent( ossimROIEvent& /* event */)
{
}

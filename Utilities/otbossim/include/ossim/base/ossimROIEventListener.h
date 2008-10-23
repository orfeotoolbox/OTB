#ifndef ossimROIEventListener_HEADER
#define ossimROIEventListener_HEADER

#include <ossim/base/ossimListener.h>
#include <ossim/base/ossimROIEvent.h>
#include <ossim/base/ossimROIEvent.h>

class OSSIMDLLEXPORT ossimROIEventListener : public ossimListener
{
public:
   ossimROIEventListener()
      {
      }
   virtual void processEvent(ossimEvent& event);
   virtual void handleRectangleROIEvent( ossimROIEvent& event );
   virtual void handlePolygonROIEvent( ossimROIEvent& event );
   virtual void handlePolylineROIEvent( ossimROIEvent& event );

TYPE_DATA   
};

#endif

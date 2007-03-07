#ifndef ossimElevationManagerEventListener_HEADER
#define ossmiElevationManagerEventListener_HEADER

#include "base/common/events/ossimListener.h"
#include "base/common/events/ossimElevationManagerEvent.h"

class OSSIMDLLEXPORT ossimElevationManagerEventListener : public ossimListener
{
 public:
   ossimElevationManagerEventListener();
   virtual ~ossimElevationManagerEventListener();
   virtual void processEvent( ossimEvent& event );
   virtual void processEvent( ossimElevationManagerEvent& event );

TYPE_DATA
};

#endif

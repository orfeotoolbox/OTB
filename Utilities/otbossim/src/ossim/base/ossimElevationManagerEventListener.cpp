#include <ossim/base/ossimElevationManagerEventListener.h>
#include <ossim/base/ossimElevationManagerEvent.h>

RTTI_DEF1(ossimElevationManagerEventListener, "ossimElevationManagerEventListener",
          ossimListener);

ossimElevationManagerEventListener::ossimElevationManagerEventListener()
      : ossimListener()
{
}


ossimElevationManagerEventListener::~ossimElevationManagerEventListener()
{
}

void ossimElevationManagerEventListener::processEvent( ossimEvent& event )
{
   ossimElevationManagerEvent* elevationManagerEvent =
      PTR_CAST( ossimElevationManagerEvent, &event );

   if ( elevationManagerEvent )
   {
      processEvent( *elevationManagerEvent );
   }
}

void ossimElevationManagerEventListener::processEvent( ossimElevationManagerEvent& /* event */)
{

}

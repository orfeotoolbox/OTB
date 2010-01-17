#ifndef ossimElevationManagerEvent_HEADER
#define ossimElevationManagerEvent_HEADER

#include <ossim/base/ossimEvent.h>

class ossimObject;

class OSSIMDLLEXPORT ossimElevationManagerEvent : public ossimEvent
{
 public:
   ossimElevationManagerEvent( ossimObject* object=NULL,
                                    long id=OSSIM_EVENT_NULL_ID )
      : ossimEvent( object, id )
      {
      }
   ossimElevationManagerEvent( const ossimElevationManagerEvent& rhs )
      : ossimEvent( rhs )
      {
      }
   virtual ossimObject* dup() const
      {
         return new ossimElevationManagerEvent( *this );
      }

TYPE_DATA   
};

#endif

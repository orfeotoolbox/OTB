#ifndef ossimObjectDestructingEvent_HEADER
#define ossimObjectDestructingEvent_HEADER

#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>

class ossimObject;

class OSSIMDLLEXPORT ossimObjectDestructingEvent : public ossimEvent
{
public:
   ossimObjectDestructingEvent(ossimObject* objectDestructing=NULL)
      :ossimEvent(objectDestructing, OSSIM_EVENT_OBJECT_DESTRUCTING_ID)
      {}
   virtual ossimObject* dup()const{return new ossimObjectDestructingEvent(*this);}

TYPE_DATA
};
#endif

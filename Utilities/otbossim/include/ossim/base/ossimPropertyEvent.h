#ifndef ossimPropertyEvent_HEADER
#define ossimPropertyEvent_HEADER
#include <ossim/base/ossimEvent.h>

class OSSIMDLLEXPORT ossimPropertyEvent : public ossimEvent
{
public:
   ossimPropertyEvent(ossimObject* object) // the event id
      :ossimEvent(object, OSSIM_EVENT_PROPERTY_ID)
      {}
   virtual ossimObject* dup()const{return new ossimPropertyEvent(*this);}
};

#endif

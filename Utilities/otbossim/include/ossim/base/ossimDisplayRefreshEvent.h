#ifndef ossimDisplayRefreshEvent_HEADER
#define ossimDisplayRefreshEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <map>

class OSSIMDLLEXPORT ossimDisplayRefreshEvent : public ossimEvent
{
public:
   ossimDisplayRefreshEvent(ossimObject* obj = NULL)
      :ossimEvent(obj, OSSIM_EVENT_DISPLAY_REFRESH_ID)
      {}

TYPE_DATA
};

#endif

#ifndef ossimDisplayListEvent_HEADER
#define ossimDisplayListEvent_HEADER
#include <ossim/base/ossimEvent.h>
#include <map>

class OSSIMDLLEXPORT ossimDisplayListEvent : public ossimEvent
{
public:
   ossimDisplayListEvent(ossimObject* obj = NULL)
      :ossimEvent(obj, OSSIM_EVENT_DISPLAY_LIST_ID)
      {
         theDisplayIterator = theDisplayMap.end();
      }
   void addDisplay(void* display);
   virtual ossimObject* dup()const
  {
    return new ossimDisplayListEvent(*this);
  }
   
   void* firstDisplay();
   void* nextDisplay();
   void  clearList();
   
protected:
   std::map<void*, void*> theDisplayMap;
   std::map<void*, void*>::iterator theDisplayIterator;


TYPE_DATA
};

#endif

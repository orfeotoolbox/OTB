#include <ossim/base/ossimDisplayListEvent.h>

RTTI_DEF1(ossimDisplayListEvent, "ossimDisplayListEvent", ossimEvent);

void ossimDisplayListEvent::addDisplay(void* display)
{
   std::map<void*, void*>::iterator iter = theDisplayMap.find(display);

   if(iter == theDisplayMap.end())
   {
      theDisplayMap.insert(std::make_pair(display, display));
   }
}

void* ossimDisplayListEvent::firstDisplay()
{
   theDisplayIterator = theDisplayMap.begin();

   if(theDisplayIterator != theDisplayMap.end())
   {
      return (*theDisplayIterator).first;
   }

   return NULL;
}

void* ossimDisplayListEvent::nextDisplay()
{
   ++theDisplayIterator;

   if(theDisplayIterator != theDisplayMap.end())
   {
      return (*theDisplayIterator).first;
   }

   return NULL;
}

void ossimDisplayListEvent::clearList()
{
   theDisplayMap.clear();
   theDisplayIterator = theDisplayMap.end();
}


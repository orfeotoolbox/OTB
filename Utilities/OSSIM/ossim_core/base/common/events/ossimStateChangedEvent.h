#ifndef ossimStateChangedEvent_HEADER
#define ossimStateChangedEvent_HEADER

#include "ossimEvent.h"
#include "ossimEventIds.h"
#include "base/data_types/ossimKeywordlist.h"

class OSSIMDLLEXPORT ossimStateChangedEvent : public ossimEvent
{
public:
   ossimStateChangedEvent( ossimObject* object=NULL,
                           long id=OSSIM_EVENT_STATE_CHANGED_ID );

   void setObjId( string objId );
   void setNewKeywordlist( ossimKeywordlist kwl );
   void setOldKeywordlist( ossimKeywordlist kwl );

   string getObjId();
   ossimKeywordlist getNewKeywordlist();
   ossimKeywordlist getOldKeywordlist();

   // HACK - Need to set keywordlists too!!!
   virtual ossimObject* dup()const{return new ossimStateChangedEvent(*this);}     

private:
   string theObjId;
   ossimKeywordlist theNewKeywordlist;
   ossimKeywordlist theOldKeywordlist;
};

#endif

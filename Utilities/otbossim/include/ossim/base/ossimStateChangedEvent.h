#ifndef ossimStateChangedEvent_HEADER
#define ossimStateChangedEvent_HEADER

#include <string>

#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimEventIds.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>

class OSSIMDLLEXPORT ossimStateChangedEvent : public ossimEvent
{
public:
   ossimStateChangedEvent( ossimObject* object=NULL,
                           long id=OSSIM_EVENT_STATE_CHANGED_ID );

   void setObjId( std::string objId );
   void setNewKeywordlist( ossimKeywordlist kwl );
   void setOldKeywordlist( ossimKeywordlist kwl );

   std::string getObjId();
   ossimKeywordlist getNewKeywordlist();
   ossimKeywordlist getOldKeywordlist();

   // HACK - Need to set keywordlists too!!!
   virtual ossimObject* dup()const{return new ossimStateChangedEvent(*this);}     

private:
   std::string theObjId;
   ossimKeywordlist theNewKeywordlist;
   ossimKeywordlist theOldKeywordlist;
};

#endif

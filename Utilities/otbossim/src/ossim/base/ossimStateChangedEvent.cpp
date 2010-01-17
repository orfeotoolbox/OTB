#include <ossim/base/ossimStateChangedEvent.h>

ossimStateChangedEvent::ossimStateChangedEvent( ossimObject* object,
                                                 long id )
   : ossimEvent( object, id )
{
}

void ossimStateChangedEvent::setNewKeywordlist( ossimKeywordlist kwl )
{
   theNewKeywordlist = kwl;
}

ossimKeywordlist ossimStateChangedEvent::getNewKeywordlist()
{
   return theNewKeywordlist;
}


void ossimStateChangedEvent::setOldKeywordlist( ossimKeywordlist kwl )
{
   theOldKeywordlist = kwl;
}

ossimKeywordlist ossimStateChangedEvent::getOldKeywordlist()
{
   return theOldKeywordlist;
}

void ossimStateChangedEvent::setObjId( std::string objId )
{
   theObjId = objId;
}

std::string ossimStateChangedEvent::getObjId()
{
   return theObjId;
}

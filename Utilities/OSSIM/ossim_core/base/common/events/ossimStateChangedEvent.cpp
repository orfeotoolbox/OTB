#include "ossimStateChangedEvent.h"

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

void ossimStateChangedEvent::setObjId( string objId )
{
   theObjId = objId;
}

string ossimStateChangedEvent::getObjId()
{
   return theObjId;
}

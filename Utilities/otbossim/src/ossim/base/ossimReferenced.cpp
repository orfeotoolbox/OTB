/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts
*/
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimNotifyContext.h>

ossimReferenced::~ossimReferenced()
{
   if(theRefMutex)
   {
      delete theRefMutex;
      theRefMutex = 0;
   }
   if (theRefCount>0)
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"Warning: deleting still referenced object "<<this<<std::endl;
      ossimNotify(ossimNotifyLevel_WARN)<<"         the final reference count was "<<theRefCount
                                        <<", memory corruption possible."<<std::endl;
   }
}

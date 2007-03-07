/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts
*/
#include <base/common/ossimReferenced.h>
#include <base/context/ossimNotifyContext.h>

ossimReferenced::~ossimReferenced()
{
   if (theRefCount>0)
   {
      ossimNotify(ossimNotifyLevel_WARN)<<"Warning: deleting still referenced object "<<this<<std::endl;
      ossimNotify(ossimNotifyLevel_WARN)<<"         the final reference count was "<<theRefCount
                                        <<", memory corruption possible."<<std::endl;
   }
}

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: A brief description of the contents of the file.
//
//
//*************************************************************************
// $Id: ossimRefreshEvent.cpp 19717 2011-06-03 17:26:52Z gpotts $
#include <ossim/base/ossimRefreshEvent.h>

RTTI_DEF1(ossimRefreshEvent, "ossimRefreshEvent", ossimEvent);


void ossimRefreshEvent::setRefreshType(int refreshType, bool on)
{
   if(on)
   {
      m_refreshType = static_cast<RefreshType>(m_refreshType | refreshType);
   }
   else 
   {
      m_refreshType = static_cast<RefreshType>(((~refreshType)&(m_refreshType))&REFRESH_ALL);
   }
}

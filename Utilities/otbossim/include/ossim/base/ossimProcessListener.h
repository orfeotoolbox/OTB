//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// $Id: ossimProcessListener.h 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimProcessListener_HEADER
#define ossimProcessListener_HEADER
#include <ossim/base/ossimListener.h>
class OSSIMDLLEXPORT ossimProcessProgressEvent;

class OSSIMDLLEXPORT ossimProcessListener : public ossimListener
{
public:
   ossimProcessListener();
   virtual ~ossimProcessListener();
   virtual void processEvent(ossimEvent& event);
   virtual void processProgressEvent(ossimProcessProgressEvent& event);

TYPE_DATA
};

#endif

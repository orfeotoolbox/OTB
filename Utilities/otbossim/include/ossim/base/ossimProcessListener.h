//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// $Id: ossimProcessListener.h 14799 2009-06-30 08:54:44Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimProcessListener_HEADER
#define ossimProcessListener_HEADER
#include <ossim/base/ossimListener.h>

class ossimProcessProgressEvent;

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

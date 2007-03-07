//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// $Id: ossimProcessListener.h,v 1.5 2004/08/17 16:49:03 dburken Exp $
//----------------------------------------------------------------------------
#ifndef ossimProcessListener_HEADER
#define ossimProcessListener_HEADER
#include "ossimListener.h"
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

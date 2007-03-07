//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimStdOutProgress.h,v 1.16 2004/10/06 15:35:17 dburken Exp $
#ifndef ossimStdOutProgress_HEADER
#define ossimStdOutProgress_HEADER
#include <iostream>
#include <base/common/events/ossimProcessListener.h>
#include <base/common/events/ossimProcessProgressEvent.h>
#include <base/context/ossimNotifyContext.h>
class OSSIMDLLEXPORT ossimStdOutProgress : public ossimProcessListener
{
public:
   ossimStdOutProgress(long precision = 0, bool flushStream=false);

   virtual void processProgressEvent(ossimProcessProgressEvent& event);
      
   virtual void setFlushStreamFlag(bool flag);

protected:
   long thePrecision;
   bool theFlushStreamFlag;
TYPE_DATA
};
OSSIMDLLEXPORT extern ossimStdOutProgress theStdOutProgress;
#endif

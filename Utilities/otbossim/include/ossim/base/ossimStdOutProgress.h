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
// $Id: ossimStdOutProgress.h 10422 2007-02-06 04:03:13Z gpotts $
#ifndef ossimStdOutProgress_HEADER
#define ossimStdOutProgress_HEADER

#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimNotifyContext.h>

class OSSIM_DLL ossimStdOutProgress : public ossimProcessListener
{
public:
   ossimStdOutProgress(ossim_uint32 precision = 0, bool flushStream=false);

   virtual void processProgressEvent(ossimProcessProgressEvent& event);
      
   virtual void setFlushStreamFlag(bool flag);

protected:
   ossim_uint32 thePrecision;
   bool         theFlushStreamFlag;
TYPE_DATA
};

OSSIM_DLL extern ossimStdOutProgress theStdOutProgress;
#endif

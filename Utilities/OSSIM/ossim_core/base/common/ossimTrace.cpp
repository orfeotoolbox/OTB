//*****************************************************************************
// FILE: ossimTrace.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimTrace
//
// SOFTWARE HISTORY:
//>
//   24Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id: ossimTrace.cpp,v 1.7 2005/04/18 20:46:13 dburken Exp $

#include <base/common/ossimTrace.h>
#include <base/common/ossimTraceManager.h>
#include <base/common/ossimCommon.h>
//*****************************************************************************
//  CONSTRUCTOR: ossimTrace
//  
//*****************************************************************************
ossimTrace::ossimTrace(const ossimString& trace_name)
   :
      theTraceName   (trace_name),
      theEnabledFlag (false)
{
   ossimTraceManager::instance()->addTrace(this);
}

ossimTrace::~ossimTrace()
{
   ossimTraceManager::instance()->removeTrace(this);
}


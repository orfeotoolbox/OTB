//*****************************************************************************
// FILE: ossimTrace.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains declaration of class ossimTrace. Used for tracing code execution.
//   Implemented as a singluy linked list of ossimTrace objects.
//
// SOFTWARE HISTORY:
//   24Apr2001  Oscar Kramer
//              Initial coding.
//*****************************************************************************
// $Id: ossimTrace.h,v 1.12 2005/04/18 20:46:13 dburken Exp $

#ifndef ossimTrace_HEADER
#define ossimTrace_HEADER

#include <iostream>  // for "clog"
#include <base/data_types/ossimString.h>
#include <base/context/ossimNotifyContext.h>

// Macro for use with trace...
#define CLOG ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " (\"" __FILE__ "\", line " << __LINE__ << ") DEBUG: "


class OSSIMDLLEXPORT ossimTrace
{
public:
   ossimTrace( const ossimString& trace_name );
   ~ossimTrace();

   /**
    * @returns true if enabled false if not.
    */
   bool isEnabled() const { return theEnabledFlag; }

   /**
    * Returns "theTraceName" as an ossimString.
    */
   ossimString getTraceName() const { return theTraceName; }


   /**
    * Sets "theEnabled" flag.
    *
    * @param flag true == enabled, false == disabled.
    */
   void setTraceFlag(bool flag) { theEnabledFlag = flag; }
   
   /**
    * Operator() for ossimTrace.  Given the static instance:
    * static ossimTrace traceDebug("myTrace");
    *
    * You can do:
    * if (traceDebug())
    * {
    *     CLOG << "Your trace stuff goes here..." << endl;
    * }
    */
   bool operator()() const { return theEnabledFlag; }

private:
   ossimString                theTraceName;
   bool                       theEnabledFlag;
};

#endif

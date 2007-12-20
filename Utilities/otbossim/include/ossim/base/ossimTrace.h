//*****************************************************************************
// FILE: ossimTrace.h
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of class ossimTrace. Used for tracing code execution.
//   Implemented as a singluy linked list of ossimTrace objects.
//
// SOFTWARE HISTORY:
//   24Apr2001  Oscar Kramer
//              Initial coding.
//*****************************************************************************
// $Id: ossimTrace.h 11650 2007-08-24 12:02:47Z dburken $

#ifndef ossimTrace_HEADER
#define ossimTrace_HEADER

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotify.h>

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

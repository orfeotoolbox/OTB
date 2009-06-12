//*****************************************************************************
// Copyright (C) 2005 Garrett Potts, all rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
// 
// DESCRIPTION:
//   Contains declaration of class ossimTraceManager.
//
//*****************************************************************************
// $Id: ossimTraceManager.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimTraceManager_HEADER
#define ossimTraceManager_HEADER

#include <ossim/base/ossimString.h>

class ossimTrace;

class OSSIMDLLEXPORT ossimTraceManager
{
public:
   /**
    * @return ossimTraceManager* to instance of the ossim trace manager.
    */
   static ossimTraceManager* instance();

   /**
    * Sets "thePattern" to "pattern", then calls setTraceFlags(true).
    *
    * @param pattern Regular expression to enable trace for.
    */
   void setTracePattern(const ossimString& pattern);

   /**
    * @param traceObj ossimTrace* to add to "theTraceList".
    */
   void addTrace(ossimTrace* traceObj);

   /**
    * @param traceObj ossimTrace* to remove from "theTraceList".
    */
   void removeTrace(ossimTrace* traceObj);
   
protected:
   /** Protected default constructor. */
   ossimTraceManager();

private:
   /**
    * Loops through "theTraceList" and sets any trace object's enable flag to
    * flag if it matches regular expression of "thePattern".
    * For trace objects not matching "thePattern" no action is taken.
    */
   void setTraceFlags(bool flag);

   /** The instance of this manager. */
   static ossimTraceManager* theInstance;

   /** The regular expression pattern like "ossimDynamic|ossimGdal". */
   ossimString               thePattern;

   /** The list of trace objects. */
   std::vector<ossimTrace*>  theTraceList;
};

#endif

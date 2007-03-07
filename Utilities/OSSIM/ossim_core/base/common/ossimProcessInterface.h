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
// $Id: ossimProcessInterface.h,v 1.13 2004/10/06 15:32:02 dburken Exp $
#ifndef ossimProcessInterface_HEADER
#define ossimProcessInterface_HEADER
#include <iostream>
#include "base/common/ossimRtti.h"
#include "base/data_types/ossimString.h"
#include "base/common/events/ossimProcessProgressEvent.h"
#include "base/common/events/ossimListenerManager.h"

class OSSIMDLLEXPORT ossimObject;
class OSSIMDLLEXPORT ossimListenerManager;

class OSSIMDLLEXPORT ossimProcessInterface
{
public:

   enum ossimProcessStatus
   {
      PROCESS_STATUS_UNKNOWN       = 0,
      PROCESS_STATUS_EXECUTING     = 1,
      PROCESS_STATUS_ABORTED       = 2,
      PROCESS_STATUS_ABORT_REQUEST = 3,
      PROCESS_STATUS_NOT_EXECUTING = 4
   };
   
   ossimProcessInterface();
   
   virtual ~ossimProcessInterface();
   
   virtual ossimObject* getObject()=0;
   virtual const ossimObject* getObject()const=0;
   virtual bool execute()=0;
   
   virtual void abort();

   bool needsAborting()const;

   bool isAbortRequested()const;

   bool isAborted()const;

   bool isExecuting()const;

   ossimProcessStatus getProcessStatus()const;

   virtual void setProcessStatus(ossimProcessStatus processStatus);

   virtual double getPercentComplete()const;

   virtual void setPercentComplete(double percentComplete);

   virtual ossimListenerManager* getManager();

   void enableEvents();
   
   void disableEvents();

   void setCurrentMessage(const ossimString& message);

   /*!
    *  Called by friend operator<< function, derived classes should override
    *  if something different is desired.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimProcessInterface& data);
   
protected:
   double              thePercentComplete;
   ossimProcessStatus  theProcessStatus;
   ossimString         theMessage;
   bool                theEventFlag;

TYPE_DATA
};

#endif

//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProcessInterface.h 14789 2009-06-29 16:48:14Z dburken $
#ifndef ossimProcessInterface_HEADER
#define ossimProcessInterface_HEADER
#include <iosfwd>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimListenerManager.h>

class ossimObject;
class ossimListenerManager;

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

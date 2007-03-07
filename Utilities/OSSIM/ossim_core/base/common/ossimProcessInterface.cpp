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
// $Id: ossimProcessInterface.cpp,v 1.8 2004/10/06 15:33:40 dburken Exp $
#include "ossimProcessInterface.h"
#include "base/common/events/ossimListenerManager.h"

using namespace std;

RTTI_DEF(ossimProcessInterface, "ossimProcessInterface");

ossimProcessInterface::ossimProcessInterface()
   :thePercentComplete(0.0),
    theProcessStatus(PROCESS_STATUS_NOT_EXECUTING),
    theMessage(""),
    theEventFlag(true)
{
}

ossimProcessInterface::~ossimProcessInterface()
{
}

void ossimProcessInterface::abort()
{
   if(theProcessStatus == PROCESS_STATUS_EXECUTING)
   {
      theProcessStatus =  PROCESS_STATUS_ABORT_REQUEST;
   }
}

bool ossimProcessInterface::needsAborting() const
{
   return ( isAborted()|| isAbortRequested() );
}

bool ossimProcessInterface::isAbortRequested()const
{
   return (theProcessStatus == PROCESS_STATUS_ABORT_REQUEST);
}

bool ossimProcessInterface::isAborted()const
{
   return (theProcessStatus==PROCESS_STATUS_ABORTED);
}

bool ossimProcessInterface::isExecuting()const
{
   return ((theProcessStatus==PROCESS_STATUS_EXECUTING)||
           (theProcessStatus==PROCESS_STATUS_ABORT_REQUEST));
}

ossimProcessInterface::ossimProcessStatus ossimProcessInterface::getProcessStatus()const
{
   return theProcessStatus;
}

void ossimProcessInterface::setProcessStatus(ossimProcessStatus processStatus)
{
   theProcessStatus = processStatus;
}

double ossimProcessInterface::getPercentComplete()const
{
   return thePercentComplete;
}

void ossimProcessInterface::setPercentComplete(double percentComplete)
{
   thePercentComplete = percentComplete;

   ossimListenerManager* manager = getManager();
   if(theEventFlag&&manager)
   {
      ossimProcessProgressEvent event(getObject(),
                                      thePercentComplete,
                                      theMessage,
                                      false);
      manager->fireEvent(event);
   }
}

ossimListenerManager* ossimProcessInterface::getManager()
{
   return PTR_CAST(ossimListenerManager, getObject());
}

void ossimProcessInterface::enableEvents()
{
   theEventFlag = true;
}
   
void ossimProcessInterface::disableEvents()
{
   theEventFlag = false;
}

void ossimProcessInterface::setCurrentMessage(const ossimString& message)
{
   theMessage = message;

   ossimListenerManager* manager = getManager();
   if(theEventFlag&&manager)
   {
      ossimProcessProgressEvent event(getObject(),
                                      thePercentComplete,
                                      theMessage,
                                      true);
      manager->fireEvent(event);
   }   
}

ostream& ossimProcessInterface::print(ostream& out) const
{
   out << "process status: ";
   if(theProcessStatus == PROCESS_STATUS_EXECUTING)
   {
      out << "executing" << endl;
   }
   else if(theProcessStatus == PROCESS_STATUS_ABORTED)
   {
      out << "aborted" << endl;
   }
   else if(theProcessStatus == PROCESS_STATUS_NOT_EXECUTING)
   {
      out << "not executing" << endl;
   }
   out << "percent_complete: " << thePercentComplete;
   
   return out;
}

ostream& operator<<(ostream& out, const ossimProcessInterface& data)
{
   return data.print(out);
}

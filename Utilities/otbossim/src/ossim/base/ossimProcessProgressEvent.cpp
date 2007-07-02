//----------------------------------------------------------------------------
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description: Event for process progress.
//
// $Id: ossimProcessProgressEvent.cpp 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------
#include <ossim/base/ossimProcessProgressEvent.h>

RTTI_DEF1(ossimProcessProgressEvent, "ossimProcessProgressEvent", ossimEvent);

ossimProcessProgressEvent::ossimProcessProgressEvent(ossimObject* owner,
                                                     double percentComplete,
                                                     const ossimString message,
                                                     bool outputMessageFlag)
   :
      ossimEvent(owner, OSSIM_EVENT_PROCESS_PROGRESS_ID),
      thePercentComplete(percentComplete),
      theMessage(message),
      theOutputMessageFlag(outputMessageFlag)
{
}

ossimObject* ossimProcessProgressEvent::dup()const
{
   return new ossimProcessProgressEvent(*this);
}

double ossimProcessProgressEvent::getPercentComplete()const
{
   return thePercentComplete;
}
   
ossimString ossimProcessProgressEvent::getMessage()const
{
   return theMessage;
}

void ossimProcessProgressEvent::getMessage(ossimString& message)const
{
   message = theMessage;
}
   
void ossimProcessProgressEvent::setPercentComplete(double percentComplete)
{
   thePercentComplete = percentComplete;
}

void ossimProcessProgressEvent::setMessage(const ossimString& message)
{
   theMessage = message;
}

void ossimProcessProgressEvent::setOutputMessageFlag(bool flag)
{
   theOutputMessageFlag = flag;
}

bool ossimProcessProgressEvent::getOutputMessageFlag() const
{
   return theOutputMessageFlag;
}

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
// $Id: ossimProcessProgressEvent.h,v 1.8 2005/09/09 19:23:58 gpotts Exp $
//----------------------------------------------------------------------------
#ifndef ossimProcessProgressEvent_HEADER
#define ossimProcessProgressEvent_HEADER

#include <base/common/events/ossimEvent.h>
#include <base/data_types/ossimString.h>

class OSSIMDLLEXPORT ossimProcessProgressEvent : public ossimEvent
{
public:
   ossimProcessProgressEvent(ossimObject* owner=NULL,
                             double percentComplete=0.0,
                             const ossimString message="",
                             bool outputMessageFlag=false);

   virtual ossimObject* dup()const;
   
   double getPercentComplete()const;
   
   ossimString getMessage()const;

   void getMessage(ossimString& message)const;
   
   void setPercentComplete(double percentComplete);
   
   void setMessage(const ossimString& message);

   void setOutputMessageFlag(bool flag);

   bool getOutputMessageFlag() const;
      
protected:
   double      thePercentComplete;
   ossimString theMessage;
   bool        theOutputMessageFlag;

TYPE_DATA
};
#endif

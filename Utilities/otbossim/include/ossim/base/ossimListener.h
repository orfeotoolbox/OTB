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
// $Id: ossimListener.h 14789 2009-06-29 16:48:14Z dburken $
#ifndef ossimListener_HEADER
#define ossimListener_HEADER
#include <ossim/base/ossimObject.h>

class ossimEvent;

/*!
 * Base class for all listners.  Listners nned to derive from this
 * class and override the processEvent method.
 */
class OSSIMDLLEXPORT ossimListener
{
public:

   ossimListener();

   virtual ~ossimListener();

   /**
    * ProcessEvent.  The defaul is to do nothing.  Derived
    * classes need to override this class.
    */
   virtual void processEvent(ossimEvent& event);

   void enableListener();

   void disableListener();

   void setListenerEnableFlag(bool flag);

   bool isListenerEnabled() const;

   bool getListenerEnableFlag() const;

protected:
   bool theListenerEnableFlag;
TYPE_DATA
};

#endif

//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  Garrett Potts 
//
//*******************************************************************
//  $Id: ossimListener.cpp,v 1.5 2005/07/15 20:43:51 dburken Exp $
#include <base/common/events/ossimListener.h>

RTTI_DEF(ossimListener, "ossimListener");

ossimListener::ossimListener()
  :theListenerEnableFlag(true)
{}

ossimListener::~ossimListener()
{
}

void ossimListener::processEvent(ossimEvent& /* event */)
{
}

void ossimListener::enableListener()
{
  theListenerEnableFlag = true;
}

void ossimListener::disableListener()
{
  theListenerEnableFlag = false;
}

void ossimListener::setListenerEnableFlag(bool flag)
{
  theListenerEnableFlag = flag;
}

bool ossimListener::isListenerEnabled()const
{
  return theListenerEnableFlag;
}

bool ossimListener::getListenerEnableFlag()const
{
  return theListenerEnableFlag;
}

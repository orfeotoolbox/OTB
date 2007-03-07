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
// $Id: ossimDisplayInterface.cpp,v 1.2 2005/07/15 19:04:28 dburken Exp $
#include <base/common/ossimDisplayInterface.h>

RTTI_DEF(ossimDisplayInterface, "ossimDisplayInterface");

ossimDisplayInterface::ossimDisplayInterface()
{
}

ossimDisplayInterface::~ossimDisplayInterface()
{
}

ossimString ossimDisplayInterface::getTitle()const
{
   ossimString result;
   
   getTitle(result);
   
   return result;
}

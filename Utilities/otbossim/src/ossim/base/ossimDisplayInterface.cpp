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
// $Id: ossimDisplayInterface.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimDisplayInterface.h>

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

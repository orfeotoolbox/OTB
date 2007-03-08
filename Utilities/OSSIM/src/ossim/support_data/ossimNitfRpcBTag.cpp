//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpcBTag.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/support_data/ossimNitfRpcBTag.h>

RTTI_DEF1(ossimNitfRpcBTag, "ossimNitfRpcBTag", ossimNitfRpcBase);

ossimNitfRpcBTag::ossimNitfRpcBTag()
   : ossimNitfRpcBase()
{
}

ossimString ossimNitfRpcBTag::getRegisterTagName() const
{
   return ossimString("RPC00B");
}

//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcATag.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/support_data/ossimNitfRpcATag.h>

RTTI_DEF1(ossimNitfRpcATag, "ossimNitfRpcATag", ossimNitfRpcBase);

ossimNitfRpcATag::ossimNitfRpcATag()
   : ossimNitfRpcBase()
{
}

ossimString ossimNitfRpcATag::getRegisterTagName() const
{
   return ossimString("RPC00A");
}

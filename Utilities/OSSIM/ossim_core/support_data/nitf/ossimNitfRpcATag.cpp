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
// $Id: ossimNitfRpcATag.cpp,v 1.4 2005/09/30 20:21:11 dburken Exp $

#include <support_data/nitf/ossimNitfRpcATag.h>

RTTI_DEF1(ossimNitfRpcATag, "ossimNitfRpcATag", ossimNitfRpcBase);

ossimNitfRpcATag::ossimNitfRpcATag()
   : ossimNitfRpcBase()
{
}

ossimString ossimNitfRpcATag::getRegisterTagName() const
{
   return ossimString("RPC00A");
}

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
// $Id: ossimNitfRpcBTag.cpp,v 1.3 2005/09/28 18:29:29 dburken Exp $

#include <support_data/nitf/ossimNitfRpcBTag.h>

RTTI_DEF1(ossimNitfRpcBTag, "ossimNitfRpcBTag", ossimNitfRpcBase);

ossimNitfRpcBTag::ossimNitfRpcBTag()
   : ossimNitfRpcBase()
{
}

ossimString ossimNitfRpcBTag::getRegisterTagName() const
{
   return ossimString("RPC00B");
}

//*******************************************************************
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpcBTag.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/support_data/ossimNitfRpcBTag.h>

RTTI_DEF1(ossimNitfRpcBTag, "ossimNitfRpcBTag", ossimNitfRpcBase);

ossimNitfRpcBTag::ossimNitfRpcBTag()
   : ossimNitfRpcBase()
{
}

std::string ossimNitfRpcBTag::getRegisterTagName() const
{
   return std::string("RPC00B");
}

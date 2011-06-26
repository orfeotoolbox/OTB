//*******************************************************************
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
// $Id: ossimNitfRpcATag.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/support_data/ossimNitfRpcATag.h>

RTTI_DEF1(ossimNitfRpcATag, "ossimNitfRpcATag", ossimNitfRpcBase);

ossimNitfRpcATag::ossimNitfRpcATag()
   : ossimNitfRpcBase()
{
}

std::string ossimNitfRpcATag::getRegisterTagName() const
{
   return std::string("RPC00A");
}

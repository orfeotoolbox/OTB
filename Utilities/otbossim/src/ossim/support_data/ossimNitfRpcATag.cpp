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
// $Id: ossimNitfRpcATag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <ossim/support_data/ossimNitfRpcATag.h>

RTTI_DEF1(ossimNitfRpcATag, "ossimNitfRpcATag", ossimNitfRpcBase);

ossimNitfRpcATag::ossimNitfRpcATag()
   : ossimNitfRpcBase()
{
   // Set the tag name in base.
   setTagName(std::string("RPC00A"));
}

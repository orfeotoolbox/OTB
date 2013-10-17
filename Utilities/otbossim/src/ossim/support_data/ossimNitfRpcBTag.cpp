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
// $Id: ossimNitfRpcBTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <ossim/support_data/ossimNitfRpcBTag.h>

RTTI_DEF1(ossimNitfRpcBTag, "ossimNitfRpcBTag", ossimNitfRpcBase);

ossimNitfRpcBTag::ossimNitfRpcBTag()
   : ossimNitfRpcBase()
{
   // Set the tag name in base.
   setTagName(std::string("RPC00B"));
}

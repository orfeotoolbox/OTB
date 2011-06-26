//*******************************************************************
// Copyright (C) 2004 Garrett Potts
//
// LICENSE: LGPL see top level LICENSE.txt for more details
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfCompressionHeader.cpp 18413 2010-11-11 19:56:22Z gpotts $
#include <ossim/support_data/ossimNitfCompressionHeader.h>
#include <ossim/base/ossimKeywordlist.h>
#include <sstream>

RTTI_DEF1(ossimNitfCompressionHeader, "ossimNitfCompressionHeader", ossimObject);

bool ossimNitfCompressionHeader::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   return ossimObject::saveState(kwl, prefix);
}

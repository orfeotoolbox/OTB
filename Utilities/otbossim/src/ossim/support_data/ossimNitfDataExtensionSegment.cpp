//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfDataExtensionSegment.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ostream>

#include <ossim/support_data/ossimNitfDataExtensionSegment.h>
RTTI_DEF1(ossimNitfDataExtensionSegment,
          "ossimNitfDataExtensionSegment",
          ossimObject)

std::ostream& operator <<(std::ostream &out,
                          const ossimNitfDataExtensionSegment &data)
{
   data.print(out);
   
   return out;
}

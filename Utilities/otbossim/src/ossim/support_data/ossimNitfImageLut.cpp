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
// $Id: ossimNitfImageLut.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <cstring> // memcpy
#include <ostream>

#include <ossim/support_data/ossimNitfImageLut.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimNitfImageLut &data)
{
   data.print(out);

   return out;
}

RTTI_DEF1(ossimNitfImageLut, "ossimNitfImageLut", ossimObject);
ossimNitfImageLut::ossimNitfImageLut()
{
}

ossimNitfImageLut::~ossimNitfImageLut()
{
}


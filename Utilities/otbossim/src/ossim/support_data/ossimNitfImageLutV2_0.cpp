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
// $Id: ossimNitfImageLutV2_0.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <cstring> // memcpy
#include <istream>
#include <ostream>

#include <ossim/support_data/ossimNitfImageLutV2_0.h>

ossimNitfImageLutV2_0::ossimNitfImageLutV2_0()
{
}

ossimNitfImageLutV2_0::~ossimNitfImageLutV2_0()
{
}

void ossimNitfImageLutV2_0::parseStream(std::istream &in)
{
   if (!in) return;

   ossim_uint32 count = 0;
   while(count < theNumberOfEntries)
   {
      in.read((char*)&theData[count], 1);
      
      ++count;
   }
}

std::ostream& ossimNitfImageLutV2_0::print(std::ostream& out)const
{
   out << "theNumberOfEntries:          " << theNumberOfEntries;

   return out;
}

ossim_uint32 ossimNitfImageLutV2_0::getNumberOfEntries()const
{
   return theNumberOfEntries;
}

void ossimNitfImageLutV2_0::setNumberOfEntries(ossim_uint32 numberOfEntries)
{
   theData.resize(numberOfEntries);
   theNumberOfEntries = numberOfEntries;
}

ossim_uint8 ossimNitfImageLutV2_0::getValue(ossim_uint32 index)const
{
   return theData[index];
}

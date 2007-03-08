//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*************************************************************************
// $Id: ossimId.cpp 9430 2006-08-16 19:04:16Z gpotts $

#include <iostream>
#include <ossim/base/ossimId.h>

const ossim_int64 ossimId::INVALID_ID = -1;

std::ostream& operator <<(std::ostream &out, const ossimId& data)
{
   data.print(out);
   return out;
}

void ossimId::print(std::ostream& out)const
{
   out << "id: " << theId;
}

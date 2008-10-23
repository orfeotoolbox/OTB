//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
// Base class for all ossim streams...  Derives from ossimReferenced.
//
//*******************************************************************
//  $Id: ossimStreamBase.cpp 11177 2007-06-07 19:47:04Z dburken $

#include <ossim/base/ossimStreamBase.h>

ossimStreamBase::ossimStreamBase()
   : ossimReferenced()
{
}

ossimStreamBase::~ossimStreamBase()
{
}

bool ossimStreamBase::isCompressed() const
{
   return false;
}

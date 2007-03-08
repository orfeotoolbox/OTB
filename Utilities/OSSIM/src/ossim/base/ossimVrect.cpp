//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class declaration for vrect.
// Container class for four double points representing a rectangle
// where y is up
// 
//*******************************************************************
//  $Id: ossimVrect.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimVrect.h>

//*******************************************************************
// Public Method:
//*******************************************************************
void ossimVrect::print(std::ostream& os) const
{
   os << theUlCorner << theLrCorner;
}

//*******************************************************************
// friend function:
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimVrect& rect)
{
   rect.print(os);

   return os;
}

ossimVrect::~ossimVrect()
{
}

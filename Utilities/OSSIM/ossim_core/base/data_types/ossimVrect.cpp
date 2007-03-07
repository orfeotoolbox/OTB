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
//  $Id: ossimVrect.cpp,v 1.3 2005/07/15 18:51:25 dburken Exp $

#include "ossimVrect.h"

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

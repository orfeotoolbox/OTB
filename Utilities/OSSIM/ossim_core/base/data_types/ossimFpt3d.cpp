//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definitions for ossimFpt3d.
//
//*******************************************************************
//  $Id: ossimFpt3d.cpp,v 1.1.1.1 2001/02/05 20:13:22 dburken Exp $

#include <iostream>
#include <iomanip>
using namespace std;

#include "ossimFpt3d.h"

//*******************************************************************
// Public Method:
//*******************************************************************
ostream& ossimFpt3d::print(ostream& os) const
{
   os << setiosflags(ios::fixed) << setprecision(14)
      << "( " << x << " " << y << " " << z << " )";
   return os;
}

//*******************************************************************
// friend function:
//*******************************************************************
ostream& operator<<(ostream& os, const ossimFpt3d& pt)
{
   return pt.print(os);
}

//*******************************************************************
// friend function:
//*******************************************************************
istream& operator>>(istream& is, ossimFpt3d& pt)
{
   is >> pt.x >> pt.y >> pt.z;

   return is;
}

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definitions for ossimFpt3d.
//
//*******************************************************************
//  $Id: ossimFpt3d.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <iostream>
#include <iomanip>
using namespace std;

#include <ossim/base/ossimFpt3d.h>

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

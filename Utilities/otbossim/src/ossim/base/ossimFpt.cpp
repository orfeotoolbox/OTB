//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
// Description:
//
// Contains class definitions for ossimFpt.
// 
//*******************************************************************
//  $Id: ossimFpt.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <iostream>
#include <iomanip>

#include <ossim/base/ossimFpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimFpt::ossimFpt(const ossimIpt& pt)
   :
      x(pt.x), y(pt.y)
{
   if(pt.hasNans())
   {
      makeNan();
   }
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimFpt::ossimFpt(const ossimDpt& pt)
   :
      x(pt.x), y(pt.y)
{
   if(pt.hasNans())
   {
      makeNan();
   }
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimFpt& ossimFpt::operator=(const ossimDpt& pt)
{
   x = pt.x;
   y = pt.y;
   
   if(pt.hasNans())
   {
      makeNan();
   }
   return *this;
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimFpt& ossimFpt::operator=(const ossimIpt& pt)
{
   x = pt.x;
   y = pt.y;
   if(pt.hasNans())
   {
      makeNan();
   }
   
   return *this;
}

//*******************************************************************
// Public Method:
//*******************************************************************
void ossimFpt::print(std::ostream& os) const
{
   os << std::setiosflags(std::ios::fixed) << std::setprecision(15)
      << "( " << x << " " << y << " )";
}

//*******************************************************************
// friend function:
//*******************************************************************
std::ostream& operator<<(std::ostream& os, const ossimFpt& pt)
{
   pt.print(os);
   
   return os;
}

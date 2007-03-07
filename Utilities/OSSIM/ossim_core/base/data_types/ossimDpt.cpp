//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
// Contains class definitions for ossimDpt.
// 
//*******************************************************************
//  $Id: ossimDpt.cpp,v 1.7 2005/12/16 14:25:11 dburken Exp $

#include <iostream>
#include <iomanip>
#include <sstream>

#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimDpt3d.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimFpt.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimString.h>


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimDpt::ossimDpt(const ossimFpt& pt)
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
ossimDpt::ossimDpt(const ossimIpt& pt)
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
ossimDpt::ossimDpt(const ossimDpt3d &pt)
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
ossimDpt::ossimDpt(const ossimGpt &pt)
   :
      x(pt.lon), y(pt.lat)
{
   if(pt.isLatNan() || pt.isLonNan())
   {
      makeNan();
   }
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimDpt& ossimDpt::operator=(const ossimFpt& pt)
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
const ossimDpt& ossimDpt::operator=(const ossimIpt& pt)
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
const ossimDpt& ossimDpt::operator=(const ossimDpt3d& pt)
{
   x = pt.x;
   y = pt.y;
   
   if(pt.hasNans())
   {
      makeNan();
   }
   
   return *this;
}

const ossimDpt& ossimDpt::operator=(const ossimGpt& pt)
{
   x = pt.lon;
   y = pt.lat;
   return *this;
}

std::ostream& ossimDpt::print(std::ostream& os, ossim_uint32 precision) const
{
   os << std::setiosflags(std::ios::fixed) << std::setprecision(precision)
      << "( ";

   if (x != OSSIM_DBL_NAN)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   
   os << ", ";

   if (y != OSSIM_DBL_NAN)
   {
      os << y;
   }
   else
   {
      os << "nan";
   }

   os << " )";

   return os;
}

std::ostream& operator<<(std::ostream& os, const ossimDpt& pt)
{
   return pt.print(os);
}

ossimString ossimDpt::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   print(os, precision);
   return ossimString(os.str());
}

std::istream& operator>>(std::istream& is, ossimDpt &pt)
{
   //---
   // Expected input format:
   // ( 30.00000000000000, -90.00000000000000 )
   //   --------x--------  ---------y--------
   //---

   // Start with a nan point.
   pt.makeNan();

   // Check the stream.
   if (!is) return is;

   const int SZ = 64; // Real big number with 15 digit precision.
   ossimString tempString;
   char tempChars[SZ];
   char c;

   // Gobble the "(".
   is >> tempString;
   if (!is) return is;
   
   // Get the x.
   is.get(tempChars, SZ, ',');
   if (!is) return is;
   tempChars[SZ-1] = '\0';
   tempString = tempChars;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.x = OSSIM_DBL_NAN;
   }
   else
   {
      pt.x = tempString.toDouble();
   }

   // Eat the ",".
   is.get(c);
   
   // Get the y.
   is >> tempString;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.y = OSSIM_DBL_NAN;
   }
   else
   {
      pt.y = tempString.toDouble();
   }

   // Gobble the trailing ")".
   is >> tempString;

   // Finished
   return is;
}


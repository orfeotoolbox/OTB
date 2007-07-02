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
//  $Id: ossimDpt.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>
#include <iomanip>
#include <sstream>

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimString.h>


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

void ossimDpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
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
   
   const int SZ = 64; // Handle real big number...
   ossimString os;
   char buf[SZ];

   //---
   // X SECTION:
   //---
   
   // Grab data up to the first comma.
   is.get(buf, SZ, ',');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;

   // Get rid of the '(' if there is any.
   std::string::size_type pos = os.find('(');
   if (pos != std::string::npos)
   {
      os.erase(pos, 1);
   }   

   if (os.contains("nan") == false)
   {
      pt.x = os.toFloat64();
   }
   else
   {
      pt.x = OSSIM_DBL_NAN;
   }

   //---
   // Y SECTION:
   //---
   
   // Grab the data up to the ')'
   is.get(buf, SZ, ')');

   if (!is) return is;

   // Copy to ossim string.
   os = buf;

   // Get rid of the ',' if there is any.
   pos = os.find(',');
   if (pos != std::string::npos)
   {
      os.erase(pos, 1);
   }
   
   if (os.contains("nan") == false)
   {
      pt.y = os.toFloat64();
   }
   else
   {
      pt.y = OSSIM_DBL_NAN;
   }

   // Gobble the trailing ")".
   char c = '\0';
   while (c != ')')
   {
      is.get(c);
      if (!is) break;
   }

   // Finished
   return is;
}

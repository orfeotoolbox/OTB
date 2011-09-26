//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
//
// Contains class definitions for ipt.
//
//*******************************************************************
//  $Id: ossimIpt.cpp 20070 2011-09-07 18:48:35Z dburken $

#include <iostream>
#include <sstream>

#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimFpt.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>

//*******************************************************************
// Public constructor:
//*******************************************************************
ossimIpt::ossimIpt(const ossimDpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = ossim::round<int>(pt.x);
      y = ossim::round<int>(pt.y);
   }
}

//*******************************************************************
// Public constructor:
//*******************************************************************
ossimIpt::ossimIpt(const ossimFpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = ossim::round<int>(pt.x);
      y = ossim::round<int>(pt.y);
   }
}

ossimIpt::ossimIpt(const ossimDpt3d &pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = ossim::round<int>(pt.x);
      y = ossim::round<int>(pt.y);
   }
}
//*******************************************************************
// Public method:
//*******************************************************************
const ossimIpt& ossimIpt::operator=(const ossimDpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = ossim::round<int>(pt.x);
      y = ossim::round<int>(pt.y);
   }
   
   return *this;
}

//*******************************************************************
// Public method:
//*******************************************************************
const ossimIpt& ossimIpt::operator=(const ossimFpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = ossim::round<int>(pt.x);
      y = ossim::round<int>(pt.y);
   }
   
   return *this;
}

std::ostream& ossimIpt::print(std::ostream& os) const
{
   os << "( ";

   if (x != OSSIM_INT_NAN)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   
   os << ", ";

   if (y != OSSIM_INT_NAN)
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

std::ostream& operator<<(std::ostream& os, const ossimIpt& pt)
{
   return pt.print(os);
}

ossimString ossimIpt::toString() const
{
   std::ostringstream os;
   os << "(";
   
   if (x != OSSIM_INT_NAN)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   
   os << ",";
   
   if (y != OSSIM_INT_NAN)
   {
      os << y;
   }
   else
   {
      os << "nan";
   }
   
   os << ")";
   
   return ossimString(os.str());
}


void ossimIpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
}

std::istream& operator>>(std::istream& is, ossimIpt &pt)
{
   //---
   // Expected input format:
   // ( 30, -90 )
   //   -x- -y-
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
      pt.x = os.toInt32();
   }
   else
   {
      pt.x = OSSIM_INT_NAN;
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
      pt.y = os.toInt32();
   }
   else
   {
      pt.y = OSSIM_INT_NAN;
   }

   // Gobble the trailing ")".
   char c = 0;
   while (c != ')')
   {
      is.get(c);
      if (!is) break;
   }

   // Finished
   return is;
}

bool ossimIpt::isEqualTo(const ossimIpt& rhs, ossimCompareType /* compareType */)const
{
   return ((x==rhs.x)&&
           (y==rhs.y));
}


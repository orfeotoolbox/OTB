//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Description:
// Contains class definitions for ossimDpt.
// 
//*******************************************************************
//  $Id: ossimDpt.cpp 20204 2011-11-04 15:12:28Z dburken $

#include <iostream>
#include <iomanip>
#include <sstream>

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFpt.h>
#include <ossim/base/ossimGpt.h>

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
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimDpt::ossimDpt(const ossimGpt &pt)
   :
      x(pt.lon), y(pt.lat)
{
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimDpt& ossimDpt::operator=(const ossimFpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = pt.x;
      y = pt.y;
   }
   return *this;
}

//*******************************************************************
// Public Method:
//*******************************************************************
const ossimDpt& ossimDpt::operator=(const ossimIpt& pt)
{
   if(pt.hasNans())
   {
      makeNan();
   }
   else
   {
      x = pt.x;
      y = pt.y;
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

   if (ossim::isnan(x) == false)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   
   os << ", ";

   if (ossim::isnan(y) == false)
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

bool ossimDpt::isEqualTo(const ossimDpt& rhs, ossimCompareType /* compareType */)const
{
   if(rhs.isNan()&&isNan()) return true;
   return (ossim::almostEqual(x, rhs.x)&&
           ossim::almostEqual(y, rhs.y));
}

ossimString ossimDpt::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   os << std::setprecision(precision);

   os << "(";
   if (ossim::isnan(x) == false)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   
   os << ",";
   
   if (ossim::isnan(y) == false)
   {
      os << y;
   }
   else
   {
      os << "nan";
   }
   
   os << ")";
   
   //print(os, precision);
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
      pt.x = ossim::nan();
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
      pt.y = ossim::nan();
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

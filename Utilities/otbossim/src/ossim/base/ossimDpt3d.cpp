//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id: ossimDpt3d.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <sstream>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimString.h>


ossimDpt3d::ossimDpt3d(const ossimDpt &aPt)
   :x(aPt.x),
    y(aPt.y),
    z(0)
{
   if(aPt.isNan())
   {
      makeNan();
   }
}

ossimDpt3d::ossimDpt3d(const ossimIpt &aPt)
   :x(aPt.x),
    y(aPt.y),
    z(0)
{
   if(aPt.isNan())
   {
      makeNan();
   }
}

std::string ossimDpt3d::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   os <<  setprecision(precision);
   
   os << "(";
   if ( ossim::isnan(x) == false)
   {
      os << x;
   }
   else
   {
      os << "nan";
   }
   os << ",";
   if ( ossim::isnan(y) == false )
   {
      os << y;
   }
   else
   {
      os << "nan";
   }
   os << ",";
   if ( ossim::isnan(z) == false )
   {
      os << z;
   }
   else
   {
      os << "nan";
   }
   os << ")";
   
   return os.str();
}

void ossimDpt3d::toPoint(const std::string& s)
{
   // Nan out the column vector for starters.
   x = ossim::nan();
   y = ossim::nan();
   z = ossim::nan();
  
   std::istringstream is(s);

   // Check the stream.
   if (!is) return;

   //---
   // Expected input format:
   // ( 0.0000000, 0.0000000, 0.00000000 )
   //   -----x---- -----y---- -----z----
   //---
   
   const int SZ = 64; // Handle real big number...
   ossimString os;
   char buf[SZ];
   char c = 0;

   //---
   // X SECTION:
   //---
   
   // Grab data up to the first comma.
   is.get(buf, SZ, ',');

   if (!is) return;

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
      x = os.toFloat64();
   }
   else
   {
      x = ossim::nan();
   }

   // Eat the comma that we stopped at.
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }
   
   //---
   // Y SECTION:
   //---
   
   // Grab the data up to the next ','
   is.get(buf, SZ, ',');

   if (!is) return;
   
   // Copy to ossim string.
   os = buf;
   
   if (os.contains("nan") == false)
   {
      y = os.toFloat64();
   }
   else
   {
      y = ossim::nan();
   }
   
   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // Z SECTION:
   //---
   
   // Grab the data up to the ')'
   is.get(buf, SZ, ')');
   
   if (!is) return;
   
   // Copy to ossim string.
   os = buf;
   
   if (os.contains("nan") == false)
   {
      z = os.toFloat64();
   }
   else
   {
      z = ossim::nan();
   }
}

std::ostream& operator<< (std::ostream& out, const ossimDpt3d &rhs)
{
   std::string s = rhs.toString(15);
   out << s;
   return out;
}

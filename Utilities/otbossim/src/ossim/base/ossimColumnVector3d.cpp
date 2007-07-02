//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: This is a 3-D vector without the homogeneous
//              coordinate.
//
//*******************************************************************
//  $Id: ossimColumnVector3d.cpp 9531 2006-09-11 11:18:27Z dburken $

#include <sstream>
#include <ossim/base/ossimColumnVector3d.h>

ossimString ossimColumnVector3d::toString(ossim_uint32 precision) const
{
   std::ostringstream os;
   os << setiosflags(ios::fixed) << setprecision(precision);
   
   os << "( ";
   if (data[0] != OSSIM_DBL_NAN)
   {
      os << data[0];
   }
   else
   {
      os << "nan";
   }
   os << ", ";
   if (data[1] != OSSIM_DBL_NAN)
   {
      os << data[1];
   }
   else
   {
      os << "nan";
   }
   os << ", ";
   if (data[2] != OSSIM_DBL_NAN)
   {
      os << data[2];
   }
   else
   {
      os << "nan";
   }
   os << " )";
   
   return ossimString(os.str());
}

void ossimColumnVector3d::toPoint(const std::string& s)
{
   // Nan out the column vector for starters.
   data[0] = OSSIM_DBL_NAN;
   data[1] = OSSIM_DBL_NAN;
   data[2] = OSSIM_DBL_NAN;
  
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
      data[0] = os.toFloat64();
   }
   else
   {
      data[0] = OSSIM_DBL_NAN;
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
      data[1] = os.toFloat64();
   }
   else
   {
      data[1] = OSSIM_DBL_NAN;
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
      data[2] = os.toFloat64();
   }
   else
   {
      data[2] = OSSIM_DBL_NAN;
   }
}

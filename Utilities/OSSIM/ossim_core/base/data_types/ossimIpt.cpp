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
//  $Id: ossimIpt.cpp,v 1.5 2005/12/16 14:25:11 dburken Exp $

#include <iostream>
#include <sstream>

#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimDpt3d.h>
#include <base/data_types/ossimFpt.h>
#include <base/common/ossimCommon.h>
#include <base/data_types/ossimString.h>

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
      x = irint(pt.x);
      y = irint(pt.y);
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
      x = irint(pt.x);
      y = irint(pt.y);
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
      x = irint(pt.x);
      y = irint(pt.y);
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
      x = irint(pt.x);
      y = irint(pt.y);
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
      x = irint(pt.x);
      y = irint(pt.y);
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
   print(os);
   return ossimString(os.str());
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
      pt.x = OSSIM_INT_NAN;
   }
   else
   {
      pt.x = tempString.toInt32();
   }

   // Eat the ",".
   is.get(c);
   
   // Get the y.
   is >> tempString;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.y = OSSIM_INT_NAN;
   }
   else
   {
      pt.y = tempString.toInt32();
   }

   // Gobble the trailing ")".
   is >> tempString;

   // Finished
   return is;
}

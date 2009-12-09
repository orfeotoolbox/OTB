//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for fpt.
// Used to represent a two dimensional point containing data members x and y.
//*******************************************************************
//  $Id: ossimFpt.h 14789 2009-06-29 16:48:14Z dburken $

#ifndef ossimFpt_HEADER
#define ossimFpt_HEADER
#include <iosfwd>
#include <ossim/base/ossimCommon.h>

// Forward class declarations.
class ossimIpt;
class ossimDpt;


class OSSIMDLLEXPORT ossimFpt
{
public:

   ossimFpt() : x(0), y(0) {}

   ossimFpt(ossim_float32 x, ossim_float32 y)
      : x(x), y(y)
      {}

   ossimFpt(ossim_float64 x, ossim_float64 y)
      : x((ossim_float64)x), y((ossim_float64)y)
      {}
         
   ossimFpt(const ossimFpt& pt) : x(pt.x), y(pt.y) {}

   ossimFpt(const ossimDpt& pt);

   ossimFpt(const ossimIpt& pt);

   const ossimFpt& operator=(const ossimFpt& pt);

   const ossimFpt& operator=(const ossimDpt&);

   const ossimFpt& operator=(const ossimIpt&);

   bool operator==(const ossimFpt& pt) const
      { return ( (x == pt.x) && (y == pt.y) ); } 

   bool operator!=(const ossimFpt& pt) const
      { return ( (x != pt.x) || (y != pt.y) ); }

   void makeNan(){x = ossim::nan(); y=ossim::nan();}
   
   bool hasNans()const
      {
         return (ossim::isnan(x) || ossim::isnan(y));
      }
   
   void print(std::ostream& os) const;
   
   friend std::ostream& operator<<(std::ostream& os, const ossimFpt& pt);

   //***
   // Public data members:
   //***
   ossim_float32 x;
   ossim_float32 y;
};

inline const ossimFpt& ossimFpt::operator=(const ossimFpt& pt)
{
   if (this != &pt)
   {
      x = pt.x;
      y = pt.y;
   }
   
   return *this;
}

#endif

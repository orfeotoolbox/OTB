//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimDpt.
// Used to represent an double point containing an x and y data member.
// 
//*******************************************************************
//  $Id: ossimDpt.h,v 1.11 2005/12/16 14:25:11 dburken Exp $
#ifndef ossimDpt_HEADER
#define ossimDpt_HEADER
#include <iostream>
#include <cmath>

#include <base/common/ossimConstants.h>
#include <base/common/ossimCommon.h>

// Forward class declarations.
class OSSIMDLLEXPORT ossimIpt;
class OSSIMDLLEXPORT ossimFpt;
class OSSIMDLLEXPORT ossimDpt3d;
class OSSIMDLLEXPORT ossimGpt;
class OSSIMDLLEXPORT ossimString;

class OSSIMDLLEXPORT ossimDpt
{
public:

   ossimDpt() : x(0), y(0) {}

   ossimDpt(double anX, double aY) : x(anX), y(aY) {}
         
   ossimDpt(const ossimDpt& pt) : x(pt.x), y(pt.y) {}

   ossimDpt(const ossimFpt& pt);
   
   ossimDpt(const ossimIpt& pt);

   ossimDpt(const ossimDpt3d &pt);

   ossimDpt(const ossimGpt &pt); // assigns lat, lon only

   const ossimDpt& operator=(const ossimDpt&);

   const ossimDpt& operator=(const ossimFpt&);
   
   const ossimDpt& operator=(const ossimIpt&);

   const ossimDpt& operator=(const ossimDpt3d&);

   const ossimDpt& operator=(const ossimGpt&); // assigns lat, lon only

   bool operator==(const ossimDpt& pt) const
      { return ( (x == pt.x) && (y == pt.y) ); } 

   bool operator!=(const ossimDpt& pt) const
      { return ( (x != pt.x) || (y != pt.y) ); }

   void makeNan(){x = OSSIM_DBL_NAN; y=OSSIM_DBL_NAN;}
   
   bool hasNans()const
      {
         return (ossimIsNan(x) || ossimIsNan(y));
      }

   bool isNan()const
      {
         return (ossimIsNan(x) && ossimIsNan(y));
      }
   /*!
    * METHOD: length()
    * Returns the RSS of the components.
    */
   double length() const { return sqrt(x*x + y*y); }
   
   //***
   // OPERATORS: +, -, +=, -=
   // Point add/subtract with other point:
   //***
   ossimDpt operator+(const ossimDpt& p) const
      { return ossimDpt(x+p.x, y+p.y); }
   ossimDpt operator-(const ossimDpt& p) const
      { return ossimDpt(x-p.x, y-p.y); }
   const ossimDpt& operator+=(const ossimDpt& p)
      { x += p.x; y += p.y; return *this; }
   const ossimDpt& operator-=(const ossimDpt& p)
      { x -= p.x; y -= p.y; return *this; }

   //***
   // OPERATORS: *, /
   // Scale point components by scalar:
   //***
   ossimDpt operator*(const double& d) const
      { return ossimDpt(d*x, d*y); }
   ossimDpt operator/(const double& d) const
      { return ossimDpt(x/d, y/d); }

   std::ostream& print(std::ostream& os, ossim_uint32 precision=15) const;
   
   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimDpt& pt);

   /**
    * @param precision Output floating point precision.
    * 
    * @return ossimString representing point.
    *
    * Output format:  ( 30.00000000000000, -90.00000000000000 )
    *                   --------x--------  ---------y--------
    */
   ossimString toString(ossim_uint32 precision=15) const;

   /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( 30.00000000000000, -90.00000000000000 )
    *                     --------x--------  ---------y--------
    * 
    * This method starts by doing a "makeNan" on pt.  So if anything goes
    * wrong with the stream or parsing pt could be all or partially nan.
    *
    * @param is Input stream istream to formatted text.
    * @param pt osimDpt to be initialized from stream.
    * @return istream pass in.
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimDpt& pt);
   
   //***
   // Public data members:
   //***
   union {double x; double samp; double u; double lon;};
   union {double y; double line; double v; double lat;};

};

inline const ossimDpt& ossimDpt::operator=(const ossimDpt& pt)
{
   if (this != &pt)
   {
      x = pt.x;
      y = pt.y;
   }
   
   return *this;
}

#endif /* #ifndef ossimDpt_HEADER */

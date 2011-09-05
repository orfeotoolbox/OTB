//*******************************************************************
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
//  $Id: ossimDpt.h 19793 2011-06-30 13:26:56Z gpotts $
#ifndef ossimDpt_HEADER
#define ossimDpt_HEADER

#include <iosfwd>
#include <string>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>

// Forward class declarations.
class ossimIpt;
class ossimFpt;
class ossimDpt3d;
class ossimGpt;

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
   { return ( ossim::almostEqual(x, pt.x) && ossim::almostEqual(y, pt.y) ); } 

   bool operator!=(const ossimDpt& pt) const
   { return !(*this == pt ); }

   void makeNan(){x = ossim::nan(); y=ossim::nan();}
   
   bool hasNans()const
   {
      return (ossim::isnan(x) || ossim::isnan(y));
   }

   bool isNan()const
   {
      return (ossim::isnan(x) && ossim::isnan(y));
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
    * Initializes this point from string.  This method opens an istream to
    * s and then calls operator>>.
    *
    * Expected format:  ( 30.00000000000000, -90.00000000000000 )
    *                     --------x--------  ---------y--------
    *
    * @param s String to initialize from.
    *
    * @see operator>>
    */
   void toPoint(const std::string& s);
   
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
   bool isEqualTo(const ossimDpt& rhs, ossimCompareType compareType=OSSIM_COMPARE_FULL)const;
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

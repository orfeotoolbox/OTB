//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  
//
// Contains class declaration for ipt.
// Used to represent an interger point containing an x and y data member.
// 
//*******************************************************************
//  $Id: ossimIpt.h 19793 2011-06-30 13:26:56Z gpotts $

#ifndef ossimIpt_HEADER
#define ossimIpt_HEADER
#include <iosfwd>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDpt.h>

// Forward class declarations.
class ossimFpt;
class ossimDpt3d;
class ossimString;

class OSSIMDLLEXPORT ossimIpt
{
public:

   ossimIpt() : x(0), y(0) {}

   ossimIpt(ossim_int32 anX, ossim_int32 aY) : x(anX), y(aY) {}
         
   ossimIpt(const ossimIpt& pt) : x(pt.x), y(pt.y) {}

   ossimIpt(const ossimDpt& pt);

   ossimIpt(const ossimFpt& pt);

   ossimIpt(const ossimDpt3d &pt);

   const ossimIpt& operator=(const ossimIpt& pt);

   const ossimIpt& operator=(const ossimDpt& pt);

   const ossimIpt& operator=(const ossimFpt& pt);

   bool operator==(const ossimIpt& pt) const
   { return ( (x == pt.x) && (y == pt.y) ); } 
   
   bool operator!=(const ossimIpt& pt) const
   { return ( (x != pt.x) || (y != pt.y) ); }
   
   void makeNan(){x = OSSIM_INT_NAN; y=OSSIM_INT_NAN;}
   
   bool hasNans()const
   {
      return ( (x==OSSIM_INT_NAN) || (y==OSSIM_INT_NAN) );
   }
   bool isNan()const
   {
      return ( (x==OSSIM_INT_NAN) && (y==OSSIM_INT_NAN) );
   }
   
   std::ostream& print(std::ostream& os) const;
   
   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimIpt& pt);

   /**
    * @return ossimString representing point.
    *
    * Format:  ( 30, -90 )
    *            -x- -y-
    */
   ossimString toString() const;

   /**
    * Initializes this point from string.  This method opens an istream to
    * s and then calls operator>>.
    *
    * Expected format:  ( 30, -90 )
    *                     -x- -y-
    *
    * @param s String to initialize from.
    *
    * @see operator>>
    */
   void toPoint(const std::string& s);
   
   /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( 30, -90 )
    *                     -x- -y-
    * 
    * This method starts by doing a "makeNan" on pt.  So if anything goes
    * wrong with the stream or parsing pt could be all or partially nan.
    *
    * @param is Input stream istream to formatted text.
    * @param pt osimIpt to be initialized from stream.
    * @return istream pass in.
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimIpt& pt);
   
   bool isEqualTo(const ossimIpt& rhs, ossimCompareType compareType=OSSIM_COMPARE_FULL)const;
   /*!
    * METHOD: length()
    * Returns the RSS of the components.
    */
   double length() const { return sqrt((double)x*x + (double)y*y); }
   
   //***
   // OPERATORS: +, -, +=, -=
   // Point add/subtract with other point:
   //***
   ossimIpt operator+(const ossimIpt& p) const
      { return ossimIpt(x+p.x, y+p.y); }
   ossimIpt operator-(const ossimIpt& p) const
      { return ossimIpt(x-p.x, y-p.y); }
   const ossimIpt& operator+=(const ossimIpt& p)
      { x += p.x; y += p.y; return *this; }
   const ossimIpt& operator-=(const ossimIpt& p)
      { x -= p.x; y -= p.y; return *this; }

   //***
   // OPERATORS: *, /
   // Scale point components by scalar:
   //***
   ossimDpt operator*(const double& d) const
      { return ossimDpt(d*x, d*y); }
   ossimDpt operator/(const double& d) const
      { return ossimDpt(x/d, y/d); }

   //***
   // Public data members:
   //***
   union {ossim_int32 x; ossim_int32 u; ossim_int32 samp;};
   union {ossim_int32 y; ossim_int32 v; ossim_int32 line;};

};

inline const ossimIpt& ossimIpt::operator=(const ossimIpt& pt)
{
   if (this != &pt)
   {
      x = pt.x;
      y = pt.y;
   }
   
   return *this;
}
#endif

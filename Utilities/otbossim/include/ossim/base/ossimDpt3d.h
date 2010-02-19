//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:  
//
// Contains class declaration for dpt3d
// Used to represent a 3d double point containing an x, y and z data member.
//*******************************************************************
//  $Id: ossimDpt3d.h 16347 2010-01-13 23:12:09Z dburken $

#ifndef ossimDpt3d_HEADER
#define ossimDpt3d_HEADER

#include <cmath>
#include <iosfwd>
#include <string>

#include <ossim/base/ossimCommon.h> /* for ossim::isnan */
#include <ossim/base/ossimColumnVector3d.h>

class ossimIpt;
class ossimDpt;

class OSSIMDLLEXPORT ossimDpt3d
{
public:
   friend OSSIMDLLEXPORT std::ostream & operator <<(std::ostream &out,
                                                    const ossimDpt3d &rhs);
   
   ossimDpt3d(const double &aX=0, const double &aY=0, const double &aZ=0)
      :x(aX), y(aY), z(aZ) {}

   ossimDpt3d(const ossimDpt &aPt);
   ossimDpt3d(const ossimIpt &aPt);
   
   ossimDpt3d(const ossimColumnVector3d &pt)
      : x(pt[0]), y(pt[1]), z(pt[2]) {}
   
   bool operator ==(const ossimDpt3d &rhs) const
      {
         return ( (x == rhs.x) &&
                  (y == rhs.y) &&
                  (z == rhs.z));
      }
   bool operator !=(const ossimDpt3d &rhs) const
   {
      return ( (x != rhs.x) ||
               (y != rhs.y) ||
               (z != rhs.z) );
   }
   
   void makeNan(){x = ossim::nan(); y=ossim::nan(); z=ossim::nan();}

   bool hasNans()const
   {
      return (ossim::isnan(x) || ossim::isnan(y) || ossim::isnan(z));
   }
   /*!
    * METHOD: length()
    * Returns the RSS of the components.
    */
   double length() const { return std::sqrt(x*x + y*y + z*z); }
   double length2() const { return x*x + y*y + z*z; }
   
   //***
   // OPERATORS: +, -, +=, -=
   // Point add/subtract with other point:
   //***
   ossimDpt3d operator+(const ossimDpt3d& p) const
      { return ossimDpt3d(x+p.x, y+p.y, z+p.z); }
   ossimDpt3d operator-(const ossimDpt3d& p) const
      { return ossimDpt3d(x-p.x, y-p.y, z-p.z); }
   const ossimDpt3d& operator+=(const ossimDpt3d& p)
      { x += p.x; y += p.y; z += p.z; return *this; }
   const ossimDpt3d& operator-=(const ossimDpt3d& p)
      { x -= p.x; y -= p.y; z -= p.z; return *this; }

   //***
   // OPERATORS: *, /
   // Scale point components by scalar:
   //***
   ossimDpt3d operator*(const double& d) const
      { return ossimDpt3d(d*x, d*y, d*z); }
   ossimDpt3d operator/(const double& d) const
      { return ossimDpt3d(x/d, y/d, z/d); }
  void operator /=(double value)
      {
         x /= value;
         y /= value;
         z /= value;
      }
   void operator *=(double value) 
      {
         x *= value;
         y *= value;
         z *= value;
      }
   double operator *(const ossimDpt3d& src)const
   {
      return (x*src.x + y*src.y + z*src.z);
   }
   inline const ossimDpt3d operator ^ (const ossimDpt3d& rhs) const
   {
      return ossimDpt3d(y*rhs.z-z*rhs.y,
                        z*rhs.x-x*rhs.z ,
                        x*rhs.y-y*rhs.x);
   }

   /**
    * @brief To string method.
    * 
    * @param precision Output floating point precision.
    * 
    * @return std::string representing point.
    *
    * Output format:
    * ( 0.0000000,  0.0000000,  0.00000000 )
    *   -----x----  -----y----  ------z----
    */
   std::string toString(ossim_uint32 precision=15) const;
   
   /**
    * @brief Initializes this point from string.
    *
    * Expected format:
    * 
    * ( 0.0000000,  0.0000000,  0.00000000 )
    *   -----x----  -----y----  ------z----
    *
    * @param s String to initialize from.
    */
   void toPoint(const std::string& s); 
   
   double x;
   double y;
   double z;
};

#endif

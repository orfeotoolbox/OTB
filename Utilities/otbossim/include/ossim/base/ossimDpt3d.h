//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:  
//
// Contains class declaration for dpt3d
// Used to represent a 3d double point containing an x, y and z data member.
//*******************************************************************
//  $Id: ossimDpt3d.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimDpt3d_HEADER
#define ossimDpt3d_HEADER
#include <iostream>
#include <iomanip>
#include <math.h>

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimColumnVector3d.h>

class OSSIMDLLEXPORT ossimDpt3d
{
public:
   friend inline std::ostream & operator << (std::ostream &out, const ossimDpt3d &rhs);
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

   void makeNan(){x = OSSIM_DBL_NAN; y=OSSIM_DBL_NAN; z=OSSIM_DBL_NAN;}

   bool hasNans()const
      {
         return (ossimIsNan(x) || ossimIsNan(y) || ossimIsNan(z));
      }
   /*!
    * METHOD: length()
    * Returns the RSS of the components.
    */
   double length() const { return sqrt(x*x + y*y + z*z); }
   
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
   
   double x;
   double y;
   double z;
};

inline std::ostream &operator << (std::ostream &out, const ossimDpt3d &rhs)
{
   return out << std::setiosflags(std::ios::fixed)
              << std::setprecision(15)
              << (rhs.x==OSSIM_DBL_NAN?ossimString("nan"):ossimString::toString(rhs.x))
              << " "
              << (rhs.y==OSSIM_DBL_NAN?ossimString("nan"):ossimString::toString(rhs.y))
              << " "
              << (rhs.z ==OSSIM_DBL_NAN?ossimString("nan"):ossimString::toString(rhs.z))<< endl;
}
#endif

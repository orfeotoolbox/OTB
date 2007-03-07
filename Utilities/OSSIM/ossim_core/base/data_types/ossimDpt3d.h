//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts  (gpotts@imagelinks.com)
//
// Description:  
//
// Contains class declaration for dpt3d
// Used to represent a 3d double point containing an x, y and z data member.
//*******************************************************************
//  $Id: ossimDpt3d.h,v 1.5 2002/11/25 18:59:54 gpotts Exp $

#ifndef ossimDpt3d_HEADER
#define ossimDpt3d_HEADER
#include <iostream>
#include <iomanip>
#include <math.h>

#include "ossimString.h"
#include "ossimDpt.h"
#include "ossimColumnVector3d.h"

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

//*****************************************************************************
// FILE: ossimEcefVector.h
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// DESCRIPTION:
//   Contains declaration of a 3D vector object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
//   NOTE: There is no associated ossimEcefVector.cc file. All methods are
//         inlined here
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefVector.h,v 1.6 2005/05/13 17:39:02 gpotts Exp $

#ifndef ossimEcefVector_HEADER
#define ossimEcefVector_HEADER

#include <stdio.h>
#include <math.h>
#include <base/common/ossimCommon.h>
#include <base/data_types/ossimColumnVector3d.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimGpt.h>
#include <base/context/ossimNotifyContext.h>

//*****************************************************************************
//  CLASS: ossimEcefVector
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimEcefVector
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimEcefVector()
      : theData(0,0,0) {}
      
   ossimEcefVector(const ossimEcefVector& copy_this)
      : theData (copy_this.theData) {}

   ossimEcefVector(const ossimEcefPoint& from,
                   const ossimEcefPoint& to)
      : theData (to.data() - from.data()) {}
   
   ossimEcefVector(const ossimGpt& from,
                   const ossimGpt& to)
      : theData ((ossimEcefPoint(to) - ossimEcefPoint(from)).data()) {}

   ossimEcefVector(const double& x,
                   const double& y,
                   const double& z)
      : theData(x, y, z) {}

   ossimEcefVector(const ossimColumnVector3d& assign_this)
      : theData(assign_this) {}


   void makeNan()
      {
         theData[0] = OSSIM_DBL_NAN;
         theData[1] = OSSIM_DBL_NAN;
         theData[2] = OSSIM_DBL_NAN;
      }
   
   bool hasNans()const
      {
         return ((theData[0]==OSSIM_DBL_NAN)||
                 (theData[1]==OSSIM_DBL_NAN)||
                 (theData[2]==OSSIM_DBL_NAN));
                           
      }

   bool isNan()const
      {
         return ((theData[0]==OSSIM_DBL_NAN)&&
                 (theData[1]==OSSIM_DBL_NAN)&&
                 (theData[2]==OSSIM_DBL_NAN));
         
      }
   /*!
    * OPERATORS: (all inlined below)
    */
   inline ossimEcefVector        operator- () const; 
   inline ossimEcefVector        operator+ (const ossimEcefVector&) const;
   inline ossimEcefVector        operator- (const ossimEcefVector&) const;
   inline ossimEcefPoint         operator+ (const ossimEcefPoint&)  const;
   inline ossimEcefVector        operator* (const double&)          const;
   inline ossimEcefVector        operator/ (const double&)          const;
   inline bool                   operator==(const ossimEcefVector&) const;
   inline bool                   operator!=(const ossimEcefVector&) const;
   inline const ossimEcefVector& operator= (const ossimEcefVector&);

   /*!
    * Vector-related functions:  (all inlined below)
    */
   inline double          dot    (const ossimEcefVector&) const;
   inline double          angleTo(const ossimEcefVector&) const; // degrees
   inline ossimEcefVector cross  (const ossimEcefVector&) const;
   inline ossimEcefVector unitVector()                    const;
   inline double          magnitude()                     const; // meters
   inline double length() const;
   inline double          normalize();
   
   /*!
    * COMPONENT ACCESS METHODS: 
    */
   double    x() const { return theData[0]; }
   double&   x()       { return theData[0]; }
   double    y() const { return theData[1]; }
   double&   y()       { return theData[1]; }
   double    z() const { return theData[2]; }
   double&   z()       { return theData[2]; }
   double& operator [](int idx){return theData[idx];}
   const double& operator [](int idx)const{return theData[idx];}

   const ossimColumnVector3d& data() const { return theData; }
   ossimColumnVector3d&       data()       { return theData; }

   /*!
    * Debug Dump: 
    */
   void print(ostream& os = ossimNotify(ossimNotifyLevel_INFO)) const
      {	 os << "(ossimEcefVector) " << theData; }

   friend ostream& operator<< (ostream& os , const ossimEcefVector& instance)
      { instance.print(os); return os; }

protected:
   ossimColumnVector3d theData;
};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator-()
//  Reverses direction of vector.
//*****************************************************************************
inline ossimEcefVector ossimEcefVector::operator-() const
{
   return ossimEcefVector(-theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator+(const ossimEcefVector&)
//*****************************************************************************
inline ossimEcefVector
ossimEcefVector::operator+(const ossimEcefVector& v) const
{
   return ossimEcefVector(theData + v.theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator-(const ossimEcefVector&)
//*****************************************************************************
inline ossimEcefVector
ossimEcefVector::operator-(const ossimEcefVector& v) const
{
   return ossimEcefVector(theData - v.theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator+(const ossimEcefPoint&) 
//*****************************************************************************
inline  ossimEcefPoint
ossimEcefVector::operator+(const ossimEcefPoint& p) const
{
   return ossimEcefPoint(theData + p.data());
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator*(const double&)
//*****************************************************************************
inline ossimEcefVector ossimEcefVector::operator*(const double& scalar) const
{
   return ossimEcefVector(theData*scalar);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator/(const double&)
//*****************************************************************************
inline ossimEcefVector ossimEcefVector::operator/(const double& scalar) const
{
   return ossimEcefVector(theData/scalar);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator==(const ossimEcefVector&)
//*****************************************************************************
inline bool ossimEcefVector::operator==(const ossimEcefVector& v) const
{
   return (theData == v.theData);
}
   
//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator!=(const ossimEcefVector&)
//*****************************************************************************
inline bool ossimEcefVector::operator!=(const ossimEcefVector& v) const
{
   return (theData != v.theData);
}
   
//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::operator=
//*****************************************************************************
inline const ossimEcefVector&
ossimEcefVector::operator=(const ossimEcefVector& v)
{
   theData = v.theData;
   return *this;
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::dot()
//  Computes the scalar product.
//*****************************************************************************
inline double ossimEcefVector::dot(const ossimEcefVector& v) const
{
   return theData.dot(v.theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::angleTo()
//  Returns the angle subtended (in DEGREES) between this and arg vector
//*****************************************************************************
inline double ossimEcefVector::angleTo(const ossimEcefVector& v) const
{
   double mag_product = theData.magnitude() * v.theData.magnitude();
   return acosd(theData.dot(v.theData)/mag_product);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::cross()
//  Computes the cross product.
//*****************************************************************************
inline ossimEcefVector ossimEcefVector::cross(const ossimEcefVector& v) const
{
   return ossimEcefVector(theData.cross(v.theData));
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::unitVector()
//  Returns a unit vector parallel to this.
//*****************************************************************************
inline ossimEcefVector ossimEcefVector::unitVector() const
{
   return ossimEcefVector(theData/theData.magnitude());
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::magnitude()
//*****************************************************************************
inline double ossimEcefVector::magnitude() const
{
   return theData.magnitude();
}

inline double ossimEcefVector::length() const
{
   return theData.magnitude();
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefVector::normalize()
//  Normalizes this vector.
//*****************************************************************************
inline double ossimEcefVector::normalize()
{
   double result = theData.magnitude();
   if(result > 1e-15)
   {
      theData /= result;
   }

   return result;
}
#endif



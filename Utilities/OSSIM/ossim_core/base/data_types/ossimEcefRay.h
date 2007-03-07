//*****************************************************************************
// FILE: ossimEcefRay.h
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
//  Class for representing a ray in the earth-centered, earth-fixed (ECEF)
//  coordinate system. A ray is defined as having an origin point and a
//  unit direction vector radiating from the origin.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefRay.h,v 1.10 2004/04/02 22:03:54 gpotts Exp $

#ifndef ossimEcefRay_HEADER
#define ossimEcefRay_HEADER

#include <stdio.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimEcefVector.h>
#include <base/factory/ossimDatumFactory.h>
#include <base/context/ossimNotifyContext.h>

class OSSIMDLLEXPORT ossimGpt;
class OSSIMDLLEXPORT ossimLsrRay;

//*****************************************************************************
//  CLASS: ossimEcefRay
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimEcefRay
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimEcefRay() {};
     
   ossimEcefRay(const ossimEcefRay& copy_this)
      : theOrigin(copy_this.theOrigin), theDirection(copy_this.theDirection) {}
   
   ossimEcefRay(const ossimEcefPoint&  origin,
                const ossimEcefVector& direction)
      : theOrigin(origin), theDirection(direction.unitVector()) {}
   
   ossimEcefRay(const ossimEcefPoint& from,
                const ossimEcefPoint& to);
   
   ossimEcefRay(const ossimGpt& from,
                const ossimGpt& to);

   bool isNan()const
      {
         return theOrigin.isNan()&&theDirection.isNan();
      }
   bool hasNans()const
      {
         return theOrigin.isNan()||theDirection.isNan();
      }
   void makeNan()
      {
         theOrigin.makeNan();
         theDirection.makeNan();
      }
   /*!
    * OPERATORS:
    */
   const ossimEcefRay& operator= (const ossimEcefRay& r); // inline below
   bool                operator==(const ossimEcefRay& r) const; // inline below
   bool                operator!=(const ossimEcefRay& r) const; // inline below
   
   /*!
    * DATA ACCESS METHODS:
    */
   const ossimEcefPoint&  origin()    const     { return theOrigin; }
   const ossimEcefVector& direction() const     { return theDirection; }
   void  setOrigin(const ossimEcefPoint& orig)  { theOrigin = orig; }
   void  setDirection(const ossimEcefVector& d) { theDirection=d.unitVector();}

   /*!
    * Extends the ray by distance t (meters) from the origin to the ECEF
    * point returned.
    */
   ossimEcefPoint extend(const double& t) const; // inline below

   /*!
    * This method computes a ray with the same origin but a new direction
    * corresponding to a reflection from some surface defined by its normal
    * vector (assumed to be a unit vector):
    */
   ossimEcefRay reflectRay(const ossimEcefVector& normal) const;// inline below

   /*!
    * Intersects the ray with the given elevation above the earth ellipsoid.
    */
   ossimEcefPoint intersectAboveEarthEllipsoid
      (const double& heightAboveEllipsoid,
       const ossimDatum* aDatum = ossimDatumFactory::instance()->wgs84()) const;
                                       
   /*!
    * Debug Dump
    */
   void print(ostream& os = ossimNotify(ossimNotifyLevel_INFO)) const; // inline below
   
   friend ostream& operator<< (ostream& os , const ossimEcefRay& instance)
      { instance.print(os); return os; }

private:
   ossimEcefPoint  theOrigin;
   ossimEcefVector theDirection;
}; 

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator=(ossimEcefRay)
//*****************************************************************************
inline const ossimEcefRay& ossimEcefRay::operator=(const ossimEcefRay& r)
{
   theOrigin = r.theOrigin;
   theDirection = r.theDirection;
   return *this;
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator==(ossimEcefRay)
//*****************************************************************************
inline bool ossimEcefRay::operator==(const ossimEcefRay& r) const
{
   return ((theOrigin == r.theOrigin) && (theDirection == r.theDirection));
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator!=(ossimEcefRay)
//*****************************************************************************
inline bool ossimEcefRay::operator!=(const ossimEcefRay& r) const 
{
   return !(*this == r);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::extend(double t)
//  
//   Extends the ray by distance t (meters) from the origin to the ECEF
//   point returned.
//*****************************************************************************
inline ossimEcefPoint ossimEcefRay::extend(const double& t) const
{
   return (theOrigin + theDirection*t);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::reflectRay(normal)
//  
//   This method computes a ray with the same origin but a new direction
//   corresponding to a reflection from some surface defined by its normal
//   vector:
//*****************************************************************************
inline ossimEcefRay
ossimEcefRay::reflectRay(const ossimEcefVector& normal) const
{
   ossimEcefVector new_dir(theDirection - normal*2.0*normal.dot(theDirection));
   return ossimEcefRay(theOrigin, new_dir);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::print(ostream)
//  Dumps contents for debug purposes.
//*****************************************************************************
inline void ossimEcefRay::print(ostream& os) const
{
   os << "(ossimEcefRay)\n"
      << "   theOrigin    = " << theOrigin
      << "\n   theDirection = " << theDirection << ends;
}
   
#endif



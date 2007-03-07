//*****************************************************************************
// FILE: ossimEcefPoint.h
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
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefPoint.h,v 1.10 2005/05/13 17:39:01 gpotts Exp $

#ifndef ossimEcefPoint_HEADER
#define ossimEcefPoint_HEADER
#include <iostream>
#include <stdio.h>
#include <base/data_types/ossimColumnVector3d.h>
#include <base/context/ossimNotifyContext.h>

class ossimGpt;
class ossimEcefVector;
class ossimDpt3d;
//*****************************************************************************
//  CLASS: ossimEcefPoint
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimEcefPoint
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimEcefPoint()
      : theData(0,0,0) {}

   ossimEcefPoint(const ossimEcefPoint& copy_this)
      : theData (copy_this.theData) {}

   ossimEcefPoint(const ossimGpt& convert_this);

   ossimEcefPoint(const double& x,
                  const double& y,
                  const double& z)
      : theData(x, y, z) {}

   ossimEcefPoint(const ossimColumnVector3d& assign_this)
      : theData(assign_this) {}

   ossimEcefPoint(const ossimDpt3d& pt);
   
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
    * OPERATORS:
    */
   ossimEcefVector       operator- (const ossimEcefPoint&)  const;
   ossimEcefPoint        operator+ (const ossimEcefVector&) const;
   ossimEcefPoint        operator- (const ossimEcefVector&) const;
   const ossimEcefPoint& operator= (const ossimEcefPoint&);        // inline
   bool                  operator==(const ossimEcefPoint&)  const; // inline
   bool                  operator!=(const ossimEcefPoint&)  const; // inline
   
   /*!
    * COMPONENT ACCESS METHODS: 
    */
   double    x() const { return theData[0]; }
   double&   x()       { return theData[0]; }
   double    y() const { return theData[1]; }
   double&   y()       { return theData[1]; }
   double    z() const { return theData[2]; }
   double&   z()       { return theData[2]; }
   double&   operator[](int idx){return theData[idx];}
   const double&   operator[](int idx)const{return theData[idx];}
   const ossimColumnVector3d& data() const { return theData; }
   ossimColumnVector3d&       data()       { return theData; }

   double getMagnitude() const
      {
         return theData.magnitude();
      }
   double magnitude()const
   {
      return theData.magnitude();
   }
   double length()const
   {
      return theData.magnitude();
   }
   double normalize()
   {
      double result = magnitude();

      if(result > 1e-15)
      {
         theData[0]/=result;
         theData[1]/=result;
         theData[2]/=result;
      }
      
      return result;
   }
   /*!
    * Debug Dump: 
    */
   void print(std::ostream& os = ossimNotify(ossimNotifyLevel_INFO)) const
      {
         if(isNan())
         {
            os << "(ossimEcefPoint) " << "nan nan nan";
         }
   
         os << "(ossimEcefPoint) " << theData;
      }
   
   friend std::ostream& operator<<(std::ostream& os ,
                                   const ossimEcefPoint& instance);

protected:
   ossimColumnVector3d theData;
};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator=(ossimEcefPoint)
//*****************************************************************************
inline const ossimEcefPoint&
ossimEcefPoint::operator=(const ossimEcefPoint& p)
{
   theData = p.theData;
   return *this;
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator==(ossimEcefPoint)
//*****************************************************************************
inline bool ossimEcefPoint::operator==(const ossimEcefPoint& p) const
{
   return (theData == p.theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator!=(ossimEcefPoint)
//*****************************************************************************
inline bool ossimEcefPoint::operator!=(const ossimEcefPoint& p) const
{
   return (theData != p.theData);
}

#endif



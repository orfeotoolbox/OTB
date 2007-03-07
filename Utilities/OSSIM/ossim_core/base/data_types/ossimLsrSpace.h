//*****************************************************************************
// FILE: ossimLsrSpace.h
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
//  This class maintains the relationship between a local space rectangular
//  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF) system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimLsrSpace.h,v 1.7 2004/04/02 22:03:54 gpotts Exp $

#ifndef ossimLsrSpace_HEADER
#define ossimLsrSpace_HEADER

#include <stdio.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimEcefVector.h>
#include <base/data_types/ossimMatrix3x3.h>
#include <base/context/ossimNotifyContext.h>

class OSSIMDLLEXPORT ossimGpt;

//*****************************************************************************
//  CLASS: ossimLsrSpace
//
//  This class maintains the relationship between a local space rectangular
//  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF) system.
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimLsrSpace
{
public:
   /*!
    * DEFAULT & COPY CONSTRUCTOR: 
    */
   ossimLsrSpace() {}
   
   ossimLsrSpace(const ossimLsrSpace& copy_this)
      :
	 theLsrToEcefRotMatrix(copy_this.theLsrToEcefRotMatrix),
	 theOrigin(copy_this.theOrigin) {}

   /*!
    * CONSTRUCTORS: The following three constructors accept an ECEF origin
    * and two of the three axes direction vectors. Note the use of the dummy
    * "int" argument (not used) to hold the place of the unspecified axes. 
    */
   ossimLsrSpace(const ossimEcefPoint&  origin,
                 const ossimEcefVector& x_dir_ecf_vec,
                 const ossimEcefVector& y_dir_ecf_vec,
                 const int z_not_provided_space_holder);

   ossimLsrSpace(const ossimEcefPoint&  origin,
                 const ossimEcefVector& x_dir_ecf_vec,
                 const int y_not_provided_space_holder,
                 const ossimEcefVector& z_dir_ecf_vec);

   ossimLsrSpace(const ossimEcefPoint&  origin,
                 const int x_not_provided_space_holder,
                 const ossimEcefVector& y_dir_ecf_vec,
                 const ossimEcefVector& z_dir_ecf_vec);

   /*!
    * CONSTRUCTOR: ossimLsrSpace(ossimGpt, y_azimuth)
    * This constructor sets up a local coordinate system centered at the
    * specified groundpoint, with the Z-axis normal to the ellipsoid and the
    * Y-axis rotated clockwise from north by the y_azimuth. This angle defaults
    * to 0, producing an East-North-Up (ENU) system.
    */
   ossimLsrSpace(const ossimGpt& origin,
                 const double&   y_azimuth=0);

   /*!
    * CONSTRUCTOR: ossimLsrSpace(ossimEcefPt, NEWMAT::Matrix)
    * This constructor sets up a local coordinate system centered at the
    * specified ECF, with given LSR-to-ECF rotation.
    */
   ossimLsrSpace(const ossimEcefPoint& origin,
                 const NEWMAT::Matrix& lsrToEcfRot)
      : theLsrToEcefRotMatrix (lsrToEcfRot), theOrigin (origin) {}

   ~ossimLsrSpace() {}

   /*!
    * OPERATORS:
    */
   bool operator == (const ossimLsrSpace&) const;
   
   bool operator != (const ossimLsrSpace& that) const
      { return !(*this == that); }
   
   const ossimLsrSpace& operator = (const ossimLsrSpace& space);

   /*!
    * DATA ACCESS METHODS:
    */
   const ossimEcefPoint&  origin()          const { return theOrigin; }
   const NEWMAT::Matrix&  lsrToEcefRotMatrix() const
      { return theLsrToEcefRotMatrix; }
   NEWMAT::Matrix ecefToLsrRotMatrix() const
      { return theLsrToEcefRotMatrix.t(); }

   /*!
    * METHOD: lsrSpaceErrorMessage()
    * Convenience method accessible to all owners of an ossimLsrSpace for
    * displaying an error message when LSR spaces do not match between
    * objects. All operations between LSR objects must be in a common space.
    */
   static ostream& lsrSpaceErrorMessage(ostream& os=ossimNotify(ossimNotifyLevel_INFO));
   
   /*!
    * Debug Dump: 
    */
   void print(ostream& stream = ossimNotify(ossimNotifyLevel_INFO)) const;
   
   friend ostream& operator<< (ostream& os , const ossimLsrSpace& instance)
      { instance.print(os); return os; }

private:
   NEWMAT::Matrix   theLsrToEcefRotMatrix;
   ossimEcefPoint   theOrigin;
};

#endif



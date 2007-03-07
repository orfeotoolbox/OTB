//*****************************************************************************
// FILE: ossimLsrVector.cc
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
//   Contains implementation of class 
//
// SOFTWARE HISTORY:
//>
//   ddmmm2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Log: ossimLsrVector.cpp,v $
//  Revision 1.2  2004/04/02 22:03:54  gpotts
//  Fixing all occurences of cout and cerr and CLOG to use the ossimNotify interface.
//
//  Revision 1.1  2001/08/13 21:29:18  okramer
//  Initial delivery of ECEF and LSR suite. (okramer@imagelinks.com)
//

#include <base/data_types/ossimLsrVector.h>
#include <base/data_types/ossimEcefVector.h>
#include <base/common/ossimCommon.h>

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrVector(ossimEcefVector, ossimLsrSpace)
//  
//  Establishes an LSR vector given the ECEF equivalent and the LSR space.
//  
//*****************************************************************************
ossimLsrVector::ossimLsrVector(const ossimEcefVector& convert_this,
                               const ossimLsrSpace&   new_space)
   : theLsrSpace (new_space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrVector(ossimLsrVector, ossimLsrSpace)
//  
//  Establishes an LSR vector in a new space given another LSR vector in a
//  different space.
//  
//*****************************************************************************
ossimLsrVector::ossimLsrVector(const ossimLsrVector& convert_this,
                               const ossimLsrSpace&  new_space)
   : theLsrSpace (new_space)
{
   initialize(ossimEcefVector(convert_this));
}

//*****************************************************************************
//  METHOD: ossimLsrVector::dot(ossimLsrVector)
//  
//  Computes the scalar product.
//  
//*****************************************************************************
double ossimLsrVector::dot(const ossimLsrVector& v) const
{
   if (theLsrSpace == v.theLsrSpace)
      return theData.dot(v.data());

   //else error:
   ossimLsrSpace::lsrSpaceErrorMessage();
   return OSSIM_NAN;
}

//*****************************************************************************
//  METHOD: ossimLsrVector::angleTo(ossimLsrVector)
//  
//  Returns the angle subtended (in DEGREES) between this and arg vector
//
//*****************************************************************************
double ossimLsrVector::angleTo(const ossimLsrVector& v) const
{
   if (theLsrSpace == v.theLsrSpace)
   {
      double mag_product = theData.magnitude() * v.theData.magnitude();
      return acosd(theData.dot(v.theData)/mag_product);
   }
   
   //else error:
   ossimLsrSpace::lsrSpaceErrorMessage();
   return OSSIM_NAN;
}

//*****************************************************************************
//  METHOD: ossimLsrVector::cross(ossimLsrVector)
//  
//  Computes the cross product.
//  
//*****************************************************************************
ossimLsrVector ossimLsrVector::cross(const ossimLsrVector& v) const
{
   if (theLsrSpace == v.theLsrSpace)
      return ossimLsrVector(theData.cross(v.data()), theLsrSpace);

   //else error:
   ossimLsrSpace::lsrSpaceErrorMessage();
   return ossimLsrVector(OSSIM_NAN, OSSIM_NAN, OSSIM_NAN, theLsrSpace);
}


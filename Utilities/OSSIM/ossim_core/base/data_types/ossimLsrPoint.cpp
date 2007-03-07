//*****************************************************************************
// FILE: ossimLsrPoint.cc
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
//  $Log: ossimLsrPoint.cpp,v $
//  Revision 1.2  2003/05/02 15:34:17  dburken
//  Fixed build errors on AIX VisualAge compiler.  <dburken@imagelinks.com>
//
//  Revision 1.1  2001/08/13 21:29:18  okramer
//  Initial delivery of ECEF and LSR suite. (okramer@imagelinks.com)
//

#include "ossimLsrPoint.h"
#include "ossimGpt.h"
#include "ossimLsrVector.h"
#include "ossimMatrix3x3.h"

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(ossimLsrPoint, space)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimLsrPoint& convert_this,
                             const ossimLsrSpace& new_space)
   : theLsrSpace(new_space)
{
   initialize(ossimEcefPoint(convert_this));
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(const ossimLsrPoint&  convert_this)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimGpt&  gpt,
                             const ossimLsrSpace& space)
   : theLsrSpace(space)
{
   initialize(ossimEcefPoint(gpt));
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(const ossimLsrPoint&  convert_this)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimEcefPoint& convert_this,
                             const ossimLsrSpace& space)
   : theLsrSpace(space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  PROTECTED METHOD: ossimLsrPoint::initialize(ossimEcefPoint)
//  
//  Convenience method used by several constructors for initializing theData
//  given an ECEF point. Assumes theLsrSpace has been previously initialized.
//  
//*****************************************************************************
void ossimLsrPoint::initialize(ossimEcefPoint ecef_point)
{
   //***
   // Translate to new space given the space's offset origin:
   //***
   ossimColumnVector3d xlated ((ecef_point - theLsrSpace.origin()).data());

   //***
   // Rotate by the inverse (transpose) of the LSR-to-ECEF rot matrix:
   //***
   theData = theLsrSpace.ecefToLsrRotMatrix() * xlated;
}

//*****************************************************************************
//   OPERATOR: ossimLsrPoint::operator-(ossimLsrPoint)
//  
//*****************************************************************************
ossimLsrVector ossimLsrPoint::operator-(const ossimLsrPoint& p) const
{
   if (theLsrSpace == p.theLsrSpace)
      return ossimLsrVector(theData-p.theData, theLsrSpace);

   //else error:
   ossimLsrSpace::lsrSpaceErrorMessage();
   return ossimLsrVector(OSSIM_NAN, OSSIM_NAN, OSSIM_NAN, theLsrSpace);
}

//*****************************************************************************
//  OPERATOR: ossimLsrPoint::operator+(ossimLsrVector)
//  
//*****************************************************************************
ossimLsrPoint ossimLsrPoint::operator+(const ossimLsrVector& v) const
{
   if (theLsrSpace == v.lsrSpace())
      return ossimLsrPoint(theData + v.data(), theLsrSpace);

   //else error:
   theLsrSpace.lsrSpaceErrorMessage();
   return ossimLsrPoint(OSSIM_NAN, OSSIM_NAN, OSSIM_NAN, theLsrSpace);
}


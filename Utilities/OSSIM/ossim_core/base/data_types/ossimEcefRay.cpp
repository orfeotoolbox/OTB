//*****************************************************************************
// FILE: ossimEcefRay.cc
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
//   10Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Log: ossimEcefRay.cpp,v $
//  Revision 1.2  2002/05/31 12:51:14  okramer
//  Added ossimEcefRay::intersectAboveEarthEllipsoid()
//
//  Revision 1.1  2001/08/14 15:01:45  okramer
//  Initial delivery (okramer@imagelinks.com)
//

#include "ossimEcefRay.h"
#include "ossimGpt.h"
#include "base/data_types/datum/ossimDatum.h"
#include "base/data_types/ellipse/ossimEllipsoid.h"

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimEcefRay:exec");
static ossimTrace traceDebug ("ossimEcefRay:debug");

//*****************************************************************************
//  CONSTRUCTOR: ossimEcefRay(ossimEcefPoint, ossimEcefPoint)
//  
//  Constructs a ray originating at "from" and pointing in direction of "to"
//  
//*****************************************************************************
ossimEcefRay::ossimEcefRay(const ossimEcefPoint& from,
                           const ossimEcefPoint& to)
   : theOrigin(from)
{
   ossimEcefVector ecef (to - from);
   ecef.normalize();
   theDirection = ecef;
}
   

//*****************************************************************************
//  CONSTRUCTOR: ossimEcefRay(ossimGpt, ossimGpt)
//  
//  Constructs a ray originating at "from" and pointing in direction of "to"
//  
//*****************************************************************************
ossimEcefRay::ossimEcefRay(const ossimGpt& from,
                           const ossimGpt& to)
   : theOrigin(from)
{
   ossimEcefVector ecef (ossimEcefPoint(to) - ossimEcefPoint(from));
   ecef.normalize();
   theDirection = ecef;
}
   
//*****************************************************************************
//  METHOD: ossimEcefRay::intersectAboveEarthEllipsoid
//  
//  Intersects the ray with the given elevation above the earth ellipsoid.
//  
//*****************************************************************************
ossimEcefPoint ossimEcefRay::intersectAboveEarthEllipsoid
   (const double&     argHeight,
    const ossimDatum* argDatum) const
{
   const ossimDatum* datum = argDatum;
   if (!datum)
      datum = ossimDatumFactory::instance()->wgs84();

   //***
   // Pass the call on to ellipsoid where intersection math is implemented:
   //***
   ossimEcefPoint solution;
   bool intersected = datum->ellipsoid()->nearestIntersection
                      (*this, argHeight, solution);

   if (!intersected)
      solution.makeNan();

   return solution;
}

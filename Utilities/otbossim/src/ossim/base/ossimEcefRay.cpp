//*******************************************************************
//
// License:  See top level LICENSE.txt file.
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
//  $Log$
//  Revision 1.4  2006/11/29 02:01:07  gpotts
//  pdated license
//
//  Revision 1.3  2006/06/13 19:10:51  dburken
//  Global include path change.
//
//  Revision 1.2  2002/05/31 12:51:14  okramer
//  Added ossimEcefRay::intersectAboveEarthEllipsoid()
//
//  Revision 1.1  2001/08/14 15:01:45  okramer
//  Initial delivery (okramer@imagelinks.com)
//

#include <ostream>

#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
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

std::ostream& ossimEcefRay::print(std::ostream& os) const
{
   os << "(ossimEcefRay)\n"
      << "   theOrigin    = " << theOrigin
      << "\n   theDirection = " << theDirection << std::endl;
   return os;
}

std::ostream& operator<<(std::ostream& os ,
                         const ossimEcefRay& instance)
{
   return instance.print(os);
}



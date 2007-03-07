//*****************************************************************************
// FILE: ossimEllipsoid.cc
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
//   Contains implementation of class ossimEllipsoid. The implementation is
//   actually for an OBLATE SPHEROID (x.radius = y.radius) as Earth is
//   considered.
//
// SOFTWARE HISTORY:
//>
//   06Aug2001  Garrett Potts, Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEllipsoid.cpp,v 1.15 2005/05/12 14:52:52 gpotts Exp $

#include <base/data_types/ellipse/ossimEllipsoid.h>

#include <base/data_types/ossimEcefRay.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimEcefVector.h>
#include <base/common/ossimKeywordNames.h>

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimEllipsoid:exec");
static ossimTrace traceDebug ("ossimEllipsoid:debug");


//*****************************************************************************
//  CONSTRUCTOR: ossimEllipsoid #1 (COPY)
//  
//*****************************************************************************
ossimEllipsoid::ossimEllipsoid(const ossimEllipsoid &ellipsoid)
   :
      theName(ellipsoid.theName),
      theCode(ellipsoid.theCode),
      theA(ellipsoid.theA),
      theB(ellipsoid.theB),
      theFlattening(ellipsoid.theFlattening),
      theA_squared(ellipsoid.theA_squared),
      theB_squared(ellipsoid.theB_squared),
      theEccentricitySquared(ellipsoid.theEccentricitySquared)
{
}

//*****************************************************************************
//  CONSTRUCTOR: ossimEllipsoid #2
//  
//*****************************************************************************
ossimEllipsoid::ossimEllipsoid(const ossimString &name,
                               const ossimString &code,
                               const double &a,
                               const double &b)
   :
      theName(name),
      theCode(code),
      theA(a),
      theB(b),
      theA_squared(a*a),
      theB_squared(b*b)
{
   computeFlattening();
   
   theEccentricitySquared = 2*theFlattening - theFlattening*theFlattening;
}

ossimEllipsoid::ossimEllipsoid()
{
   const ossimEllipsoid* ellipse = ossimEllipsoidFactory::instance()->wgs84();
   
   *this = *ellipse;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimEllipsoid #3
//  
//*****************************************************************************
ossimEllipsoid::ossimEllipsoid(const double &a,
                               const double &b)
   :
      theName(""), // initialize to empty
      theCode(""),
      theA(a),
      theB(b),
      theA_squared(a*a),
      theB_squared(b*b)
{
   computeFlattening();

   theEccentricitySquared = 2*theFlattening - theFlattening*theFlattening;
 }

//*****************************************************************************
//  METHOD: ossimEllipsoid::nearestIntersection
//  
//*****************************************************************************
bool ossimEllipsoid::nearestIntersection(const ossimEcefRay &ray,
                                         ossimEcefPoint& rtnPt) const
{
   return nearestIntersection(ray, 0.0, rtnPt);
}

//*****************************************************************************
//  METHOD: ossimEllipsoid::nearestIntersection
//  
//   geographic objects that are derive this class will asssume that
//   the reference datum is wgs84 and that the ray origin is a
//   geocentric coordinate relative to the wgs84 datum.  Will return
//   true if the object was intersected and false otherwise.
//  
//   The nearest intersection will use the ray sphere intersection
//   found in most ray tracers.  We will take a Ray defined by the
//   parametric equation:
//  
//     x = x0 + dxt
//     y = y0 + dyt
//     z = z0 + dzt
//  
//   and intersect this with the equation of a spheroid:
//  
//     x^2/theXRadius^2 + y^2/theYRadius^2 + z^2/theZRadius^2 = 1
//  
//  
//   the intersection is achived by substituting the parametric line
//   into the equation of the sphereroid.  By doing this you should
//   get a quadratic in t and the equation should look like this:
//  
//    a*t^2 + b*t + c = 0
//  
//      let a = dx^2/theXRadius^2 + dy^2/theYRadius^2 + dz^2/theZRadius^2
//      let b = 2*(x0*dx/theXRadius^2 +y0*dy/theYRadius^2 + z0*dz/theZRadius^2
//      let c = x0^2/theXRadius^2 + y0^2/theYRadius^2 + z0^2/theZRadius^2 - 1
//  
//  
//    Now solve the quadratic (-b +- sqrt(b^2 - 4ac) ) / 2a
//  
//    After solving for t, the parameter is applied to the ray to determine
//    the 3D point position in X,Y,Z, passed back in rtnPt. The boolean
//    "true" is returned if an intersection was found.
//
//*****************************************************************************
bool ossimEllipsoid::nearestIntersection(const ossimEcefRay& ray,
                                         const double&       offset,
                                         ossimEcefPoint&     rtnPt) const
{
   static const char MODULE[] = "ossimEllipsoid::nearestIntersection";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", entering...\n";

   
   double A_squared = (theA + offset)*(theA + offset);
   double B_squared = (theB + offset)*(theB + offset);

//    double adjustedOffset = offset/theA;
//    double bOverA = theB/theA;
//    double A_squared = (1.0+adjustedOffset)*(1.0+adjustedOffset);
//    double B_squared = ((bOverA+adjustedOffset)*(bOverA+adjustedOffset));//(theB + offset)*(theB + offset);
   
   //***
   // get the origin and direction of ray:
   //***
   ossimEcefPoint  start = ray.origin();   
   ossimEcefVector direction = ray.direction();
//    start = ossimEcefPoint(start.x()/theA,
//                           start.y()/theA,
//                           start.z()/theA);
   //***
   // Solve the coefficents of the quadratic formula
   //***
   double a = ((direction.x() * direction.x())/A_squared) +
              ((direction.y() * direction.y())/A_squared) +
              ((direction.z() * direction.z())/B_squared);

   double b = 2.0*( ((start.x()*direction.x())/A_squared) +
                    ((start.y()*direction.y())/A_squared) +
                    ((start.z()*direction.z())/B_squared) );

   double c = ((start.x()*start.x())/A_squared) +
              ((start.y()*start.y())/A_squared) +
              ((start.z()*start.z())/B_squared) - 1.0;
   
   //***
   // solve the quadratic
   //***
   double root = b*b - 4*a*c;
   double t;
   if(root < 0.0)
   {
      return false;
   }
   else
   {
      double squareRoot = sqrt(root);
      double t1 = (-b + squareRoot ) / (2.0*a);
      double t2 = (-b - squareRoot ) / (2.0*a);

      //***
      // sort t1 and t2 and take the nearest intersection if they
      // are in front of the ray.
      //***
      if(t2 < t1)
      {
         double temp = t1;
         t1 = t2;
         t2 = temp;
      }     

       if(t1 > 0.0)
          t = t1;
       else
          t = t2;
//      t = t1;
   }

   //***
   // Now apply solved t to ray to extrapolate correct distance to intersection
   //***
   bool rtnval = false;
   if (t >= 0)
   {
      rtnval = true;
      rtnPt  = ray.extend(t); 
//       rtnPt  = ray.extend(t*theA); 
   }
      
   return rtnval; 
}


//*****************************************************************************
//  METHOD: ossimEllipsoid::evaluate(ossimColumnVector3d)
//  
//  Returns neg number if inside, 0 if on, and pos number if outside of
//  ellipsoid.
//  
//*****************************************************************************
double ossimEllipsoid::evaluate(const ossimEcefPoint &location)const
{
   //***
   // get the axis
   //***
   return (((location.x() * location.x())/theA_squared) +
           ((location.y() * location.y())/theA_squared) +
           ((location.z() * location.z())/theB_squared) - 1.0);   
}
 
//*****************************************************************************
//  METHOD: ossimEllipsoid::gradient()  version A
//  
//  Returns vector normal to the ellipsoid at point specified.
//  
//*****************************************************************************
void ossimEllipsoid::gradient(const ossimEcefPoint& location,
                              ossimEcefVector&      result) const
{
   result.x() = (2.0*location.x())/theA_squared;
   result.y() = (2.0*location.y())/theA_squared;
   result.z() = (2.0*location.z())/theB_squared;
}

//*****************************************************************************
//  METHOD: ossimEllipsoid::gradient()  version B
//  
//  Returns vector normal to the ellipsoid at point specified.
//  
//*****************************************************************************
ossimEcefVector
ossimEllipsoid::gradient(const ossimEcefPoint &location)const
{
   ossimEcefVector result;
   gradient(location, result);
   return result;
}
   
   
bool ossimEllipsoid::loadState(const ossimKeywordlist& kwl,
                               const char* prefix)
{
   const char* code = kwl.find(prefix,
                               ossimKeywordNames::ELLIPSE_CODE_KW);
   bool foundCode = false;
   if(code)
   {
      const ossimEllipsoid* ellipse = ossimEllipsoidFactory::instance()->create(ossimString(code));

      if(ellipse)
      {
         foundCode = true;
         *this = *ellipse;
      }
   }
   if(!foundCode)
   {     
      const char* majorAxis = kwl.find(prefix,
                                       ossimKeywordNames::MAJOR_AXIS_KW);
      const char* minorAxis = kwl.find(prefix,
                                       ossimKeywordNames::MAJOR_AXIS_KW);

      theName = "";
      theCode = "";
      if(majorAxis && minorAxis)
      {
         theA = ossimString(majorAxis).toDouble();
         theB = ossimString(minorAxis).toDouble();

         computeFlattening();
         theA_squared = theA*theA;
         theB_squared = theB*theB;
      }
      else
      {
         const ossimEllipsoid* ellipse = ossimEllipsoidFactory::instance()->wgs84();
         
         *this = *ellipse;
      }      
   }

   return true;
}

bool ossimEllipsoid::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   if(theCode != "")
   {
      kwl.add(prefix,
              ossimKeywordNames::ELLIPSE_CODE_KW,
              theCode.c_str(),
              true);

      kwl.add(prefix,
              ossimKeywordNames::ELLIPSE_NAME_KW,
              theName.c_str(),
              true);
   }
   kwl.add(prefix,
           ossimKeywordNames::MAJOR_AXIS_KW,
           theA,
           true);
   
   kwl.add(prefix,
           ossimKeywordNames::MINOR_AXIS_KW,
           theB,
           true);

   return true;
}


//*****************************************************************************
//  METHOD: ossimEllipsoid::geodeticRadius()
//  
//  Computes the "geodetic" radius for a given latitude in degrees
//  
//*****************************************************************************
double ossimEllipsoid::geodeticRadius(const double& lat) const
{
   double sin_lat = sind(lat);
   double sin2_lat = sin_lat*sin_lat;
   
   return (theA_squared/
           sqrt(theA_squared - theA_squared*sin2_lat + theB_squared*sin2_lat));
}

void ossimEllipsoid::latLonHeightToXYZ(double lat, double lon, double height,
                                       double &x, double &y, double &z)const
{
    double sin_latitude = sind(lat);
    double cos_latitude = cosd(lat);
    double N = theA / sqrt( 1.0 - theEccentricitySquared*sin_latitude*sin_latitude);
    x = (N+height)*cos_latitude*cosd(lon);
    y = (N+height)*cos_latitude*sind(lon);
    z = (N*(1-theEccentricitySquared)+height)*sin_latitude;
}

void ossimEllipsoid::XYZToLatLonHeight(double x, double y, double z,
                                       double& lat, double& lon, double& height)const
{
   double d = sqrt(x*x + y*y);

   double phi2 = z / ((1 - theEccentricitySquared) * d);
   double p = 1.0;
   double phi1 = 0.0;
   double N1 = 0.0;
   double height1 = 0.0;
   int iterIdx = 0;
   const int MAX_ITER = 10;
   if (fabs(phi2) > 1e-16 )
   {
      while ( (p > 1e-17) && (iterIdx < MAX_ITER))
      {
         phi1 = phi2;
         N1 = theA / sqrt(1.0 - (theEccentricitySquared * pow(sin(phi1), 2.0)));
         height1 = (d / cos(phi1) - N1);
         phi2 = atan((z / d) * (1.0 + (theEccentricitySquared * N1 * sin(phi1)) / z));
         p = fabs(phi2 - phi1);
         ++iterIdx;
         /* printf("phi: %e   phi2: %e   p: %e  \n", phi1, phi2, p); */
      }                                                                                           
   }                                                                                                   
   else
   {                                                                                              
      phi1 = phi2;                                                                                
      N1 = theA / sqrt(1.0 - (theEccentricitySquared * pow(sin(phi1), 2.0)));                    
      height1 = (d / cos(phi1)) - N1;                                                             
   }                                                                                                   

   /* *Latitude = phi2 * 180/PI; */
   /* *Longitude = atan2(Y, X) * 180/PI; */
   lat = phi2*DEG_PER_RAD; 
   lon = atan2(y, x)*DEG_PER_RAD;                                                                
   height = height1; 

#if 0
    double p = sqrt(x*x + y*y);
    double theta = atan(z*theA/ (p*theB));
    double eDashSquared = (theA*theA - theB*theB)/
                          (theB*theB);

    double sin_theta = sin(theta);
    double cos_theta = cos(theta);

    lat = atan( (z + eDashSquared*theB*sin_theta*sin_theta*sin_theta) /
                     (p - theEccentricitySquared*theA*cos_theta*cos_theta*cos_theta) );
    lon = atan2(y,x);

    double sin_latitude = sin(lat);
    double N = theA / sqrt( 1.0 - theEccentricitySquared*sin_latitude*sin_latitude);

    height = p/cos(lat) - N;
    lat*=DEG_PER_RAD;
    lon*=DEG_PER_RAD;
#endif
}

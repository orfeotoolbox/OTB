//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// This is the class declaration for ossimEllipsoid. Though valid for the
// general class of geometric shape, this implementation contains additional
// methods suited to the Earth ellipsoid.
//
//*******************************************************************
//  $ID$

#ifndef ossimEllipsoid_HEADER
#define ossimEllipsoid_HEADER 1

#include <cmath> /* std::sqrt */

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>
#include <ossim/matrix/newmat.h>

class ossimEcefRay;
class ossimEcefPoint;
class ossimEcefVector;
class ossimMatrix4x4;
class ossimKeywordlist;
class ossimDpt;

/*!****************************************************************************
 *
 * CLASS:  ossimEllipsoid
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimEllipsoid
{
public:

   /*!
    * CONSTRUCTORS...
    */
   ossimEllipsoid(const ossimEllipsoid &ellipsoid);
   ossimEllipsoid(const ossimString &name,
                  const ossimString &code,
                  const double &major_axis,
                  const double &minor_axis,
                  ossim_uint32 epsg_code=0);
   ossimEllipsoid(const double &major_axis,
                  const double &minor_axis);
   ossimEllipsoid();
   
   virtual ~ossimEllipsoid(){};

   /*!
    * ACCESS METHOD...
    */
   const ossimString& name()const{return theName;}
   const ossimString& code()const{return theCode;}

   const double&    a()const{return theA;} // major axis
   const double&    b()const{return theB;} // minor axis

   const double& getA()const{return theA;}
   const double& getB()const{return theB;}
   const double& getFlattening()const{return theFlattening;}
   
   void setA(double a){theA = a;computeFlattening();}
   void setB(double b){theB = b;computeFlattening();}
   void setAB(double a, double b){theA = a; theB = b; computeFlattening();}
   void setEpsgCode(ossim_uint32 code) {theEpsgCode = code;}
   double eccentricitySquared() const { return theEccentricitySquared; }
   
   double flattening()const { return theFlattening; }
   
   double eccentricity()const { return std::sqrt(theEccentricitySquared); }
   
   ossim_uint32 getEpsgCode() const;

   /*!
    * METHOD: nearestIntersection()
    * Returns the point of nearest intersection of the ray with the ellipsoid.
    * The first version performs the intersection at the ellipsoid surface.
    * The second version computes the ray's intersection with a surface at
    * some offset outside (for positive offset) of the ellipsoid (think
    * elevation).
    */
   bool nearestIntersection(const ossimEcefRay&  ray,
                            ossimEcefPoint& rtnPt) const;
   bool nearestIntersection(const ossimEcefRay&  ray,
                            const double&        offset,
                            ossimEcefPoint& rtnPt) const;
   
   /*!
    * METHOD: evaluate()
    * evaluate will evalate the function at location x, y, z (ECEF).
    */
   double   evaluate(const ossimEcefPoint&)const;

   /*!
    * METHOD: gradient()
    * Compute the partials along location x, y, and z and place
    * the result in the result vector.
    */
   void gradient(const ossimEcefPoint& location,
                 ossimEcefVector& result)const;
   ossimEcefVector gradient(const ossimEcefPoint& loc)const;
   
   /*!
    * METHOD: prinRadiiOfCurv()
    * Computes the meridional radius and prime vertical at given point.
    */
   void prinRadiiOfCurv(const ossimEcefPoint& location,
                        double& merRadius,
                        double& primeVert)const;
   
   /*!
    * METHOD: jacobianWrtEcef()
    * Forms Jacobian of partials of geodetic WRT ECF.
    */
   void jacobianWrtEcef(const ossimEcefPoint& location,
                        NEWMAT::Matrix& jMat)const;
   
   /*!
    * METHOD: jacobianWrtGeo()
    * Forms Jacobian of partials of ECF WRT geodetic.
    */
   void jacobianWrtGeo(const ossimEcefPoint& location,
                       NEWMAT::Matrix& jMat)const;
   
   /*!
    * Computes the "geodetic" radius for a given latitude in DEGREES:
    */
   double geodeticRadius(const double& latitude) const;
   
   /*!
    * Computes the "geodetic" radius of curvature of the ellipsoid in the east-west (x) and
    * north-south (y) directions for a given latitude in DEGREES:
    */
   void geodeticRadii(const double& latitude, ossimDpt& radii) const;

   void latLonHeightToXYZ(double lat, double lon, double height,
                          double &x, double &y, double &z)const;
   void XYZToLatLonHeight(double x, double y, double z,
                          double& lat, double& lon, double& height)const;

   //---
   // this is a utility from open scene graph that allows you to create a
   // local space rotational
   // and translation matrix
   //---
   void computeLocalToWorldTransformFromXYZ(double x, double y, double z,
                                            ossimMatrix4x4& localToWorld)const;
   
   bool operator ==(const ossimEllipsoid& rhs)const
   {
      return ( (theName == rhs.theName)&&
               (theCode == rhs.theCode)&&
               (theA    == rhs.theA)&&
               (theB    == rhs.theB)&&
               (theFlattening == rhs.theFlattening));
   }

   bool operator!=(const ossimEllipsoid& rhs) const
   {
      return ( (theName != rhs.theName)||
               (theCode != rhs.theCode)||
               (theA    != rhs.theA)||               
               (theB    != rhs.theB)||
               (theFlattening != rhs.theFlattening));
   }

   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;

   const ossimEllipsoid& operator=(const ossimEllipsoid& copy_me);

   virtual bool isEqualTo(const ossimEllipsoid& rhs,
                          ossimCompareType compareType=OSSIM_COMPARE_FULL)const;

protected:
   void computeFlattening()
   {
      theFlattening = (theA - theB)/theA;
   }
   
   ossimString theName;
   ossimString theCode;
   mutable ossim_uint32 theEpsgCode;
   double    theA;           //semi-major axis in meters
   double    theB;           //semi-minor axis in meters
   double    theFlattening;
   double    theA_squared;
   double    theB_squared;
   double    theEccentricitySquared;

}; // class OSSIMDLLEXPORT ossimEllipsoid

inline bool ossimEllipsoid::isEqualTo(const ossimEllipsoid& rhs,
                                      ossimCompareType /* compareType */)const
{
   return ((theName == rhs.theName)&&
           (theCode == rhs.theCode)&&
           (theEpsgCode ==rhs.theEpsgCode)&&
           ossim::almostEqual(theA, rhs.theA)&&
           ossim::almostEqual(theB, rhs.theB)&&
           ossim::almostEqual(theFlattening, rhs.theFlattening)&&
           ossim::almostEqual(theEccentricitySquared, rhs.theEccentricitySquared));
}

#endif /* #ifndef ossimEllipsoid_HEADER */

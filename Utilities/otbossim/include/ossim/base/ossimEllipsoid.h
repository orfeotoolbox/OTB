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
#define ossimEllipsoid_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIMDLLEXPORT ossimEcefRay;
class OSSIMDLLEXPORT ossimEcefPoint;
class OSSIMDLLEXPORT ossimEcefVector;

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
                  const double &minor_axis);
   ossimEllipsoid(const double &major_axis,
                  const double &minor_axis);
   ossimEllipsoid();
   
   virtual ~ossimEllipsoid(){};

   /*!
    * ACCESS METHOD...
    */
   virtual const ossimString& name()const{return theName;}
   virtual const ossimString& code()const{return theCode;}

   virtual const double&    a()const{return theA;} // major axis
   virtual const double&    b()const{return theB;} // minor axis

   virtual const double& getA()const{return theA;}
   virtual const double& getB()const{return theB;}
   virtual const double& getFlattening()const{return theFlattening;}
   
   virtual void setA(double a){theA = a;computeFlattening();}
   virtual void setB(double b){theB = b;computeFlattening();}
   virtual void setAB(double a, double b){theA = a; theB = b; computeFlattening();}

   virtual double eccentricitySquared() const
   { return theEccentricitySquared; }
   
   virtual double flattening()const
   { return theFlattening; }
   
   virtual double eccentricity()const
   { return sqrt(theEccentricitySquared); }
   
   /*!
    * METHOD: nearestIntersection()
    * Returns the point of nearest intersection of the ray with the ellipsoid.
    * The first version performs the intersection at the ellipsoid surface.
    * The second version computes the ray's intersection with a surface at
    * some offset outside (for positive offset) of the ellipsoid (think
    * elevation).
    */
   virtual bool nearestIntersection(const ossimEcefRay&  ray,
                                    ossimEcefPoint& rtnPt) const;
   virtual bool nearestIntersection(const ossimEcefRay&  ray,
                                    const double&        offset,
                                    ossimEcefPoint& rtnPt) const;

   /*!
    * METHOD: evaluate()
    * evaluate will evalate the function at location x, y, z (ECEF).
    */
   virtual double   evaluate(const ossimEcefPoint&)const;

   /*!
    * METHOD: gradient()
    * Compute the partials along location x, y, and z and place
    * the result in the result vector.
    */
   virtual void gradient(const ossimEcefPoint& location,
                         ossimEcefVector& result)const;
   virtual ossimEcefVector gradient(const ossimEcefPoint& loc)const;

   /*!
    * Computes the "geodetic" radius for a given latitude in DEGREES:
    */
   double geodeticRadius(const double& latitude) const;

   void latLonHeightToXYZ(double lat, double lon, double height,
                          double &x, double &y, double &z)const;
   void XYZToLatLonHeight(double x, double y, double z,
                          double& lat, double& lon, double& height)const;

   // this is a utility from open scene graph that allows you to create a local space rotational
   // and translation matrix
   //
   void computeLocalToWorldTransformFromXYZ(double x, double y, double z,
                                            ossimMatrix4x4& localToWorld)const;
   
   virtual bool operator ==(const ossimEllipsoid& rhs)const
      {
         return ( (theName == rhs.theName)&&
                  (theCode == rhs.theCode)&&
                  (theB    == rhs.theB)&&
                  (theFlattening == rhs.theFlattening));
      }
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   virtual void computeFlattening()
      {
         theFlattening = (theA - theB)/theA;
      }
   
   ossimString theName;
   ossimString theCode;
   double    theA;           //semi-major axis in meters
   double    theB;           //semi-minor axis in meters
   double    theFlattening;
   double    theA_squared;
   double    theB_squared;
   double    theEccentricitySquared;

};

#endif

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
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefRay.h 12769 2008-04-30 17:46:18Z dburken $

#ifndef ossimEcefRay_HEADER
#define ossimEcefRay_HEADER

#include <iosfwd>

#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimNotify.h>

class ossimGpt;
class ossimLsrRay;

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
   std::ostream& print(
      std::ostream& os = ossimNotify(ossimNotifyLevel_INFO))const;
   
   friend std::ostream& operator<<(std::ostream& os ,
                                   const ossimEcefRay& instance);

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

   
#endif



//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimSpaceObliqueMercatorProjection.h 20197 2011-11-03 13:22:02Z dburken $
#ifndef ossimSpaceObliqueMercatorProjection_HEADER
#define ossimSpaceObliqueMercatorProjection_HEADER
#include <ossim/projection/ossimMapProjection.h>

/*!
 * This class implments the formulas for the ellipsoid and
 * circular orbit.  All formulas were directly taken from
 * proj 4.  Proj 4 implementation is from the following
 * manual reference:
 *
 * Map Projections - A Working Manual
 * by John Snyder
 * Space Oblique Mercator p. 221.
 *
 * 
 */
class OSSIMDLLEXPORT ossimSpaceObliqueMercatorProjection : public ossimMapProjection
{
public:
   enum ossimSatelliteType
   {
      SOM_TYPE_LANDSAT_1 = 0,
      SOM_TYPE_LANDSAT_2 = 1,
      SOM_TYPE_LANDSAT_3 = 2,
      SOM_TYPE_LANDSAT_4 = 3, 
      SOM_TYPE_LANDSAT_5 = 4,
      SOM_TYPE_LANDSAT_7 = 5
   };
   
   ossimSpaceObliqueMercatorProjection(ossimSatelliteType type=SOM_TYPE_LANDSAT_7,
                                       double pathNumber=34,
                                       const ossimEllipsoid& ellipsoid = ossimEllipsoid());

   virtual ossimObject* dup()const{return new ossimSpaceObliqueMercatorProjection(*this);}
   virtual ossimDpt forward(const ossimGpt &worldPoint)    const;
   virtual ossimGpt inverse(const ossimDpt &projectedPoint)const;

     /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
  virtual void setParameters(ossimSatelliteType type,
                              double path);
   
   virtual void update();

   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

protected:

   // proj 4 parameters that I bridge to.
   //
   double a2, a4, b, c1, c3; 
   double q, t, u, w, p22, sa, ca, xj, rlm, rlm2;

   // proj4 stuff 
   double lam0;
   double es;
   double e;
   double one_es;
   double rone_es;
   double a;
   
   void seraz0(double lam, double mult);
   
   /*!
    * Is the p variable in Map Projections a Working Manual.
    */
   double thePath;
   ossimSatelliteType theSatelliteType;
   
TYPE_DATA
};
#endif

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Equidistant Cylinder projection code.  
//*******************************************************************
//  $Id: ossimAzimEquDistProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimAzimEquDistProjection_HEADER
#define ossimAzimEquDistProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimAzimEquDistProjection : public ossimMapProjection
{
public:
   ossimAzimEquDistProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                              const ossimGpt& origin = ossimGpt());
   
   ossimAzimEquDistProjection(const ossimEllipsoid& ellipsoid,
                              const ossimGpt& origin,
                              double falseEasting,
                              double falseNorthing);
   
   ossimAzimEquDistProjection(const ossimAzimEquDistProjection& rhs)
      {
         *this = rhs;
      }
   virtual ossimObject *dup()const
   {
      return new ossimAzimEquDistProjection(*this);
   }
   
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   virtual void update();

   double getFalseEasting()const{return Azeq_False_Easting;}
   double getFalseNorthing()const{return Azeq_False_Northing;}

   /*!
    * SetFalseEasting.  The value is in meters.
    * Update is then called so we can pre-compute paramters
    */
   void setFalseEasting(double falseEasting);

   /*!
    * SetFalseNorthing.  The value is in meters.
    * Update is then called so we can pre-compute paramters
    */
   void setFalseNorthing(double falseNorthing);

   /*!
    * Sets both false easting and northing values.  The values are
    * expected to be in meters.
    * Update is then called so we can pre-compute paramters
    */
   void setFalseEastingNorthing(double falseEasting, double falseNorthing);

   /*!
    * Will initialize all parameters to a default values and then call
    * update for pre-computations.
    */
   void setDefaults();

   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:   
   virtual ~ossimAzimEquDistProjection(){}
   
   mutable double abs_Azeq_Origin_Lat;
   mutable double Azeq_a;                  /* Semi-major axis of ellipsoid in meters */
   mutable double Azeq_f;          /* Flattening of ellipsoid */
   mutable double Ra;                /* Spherical Radius */
   mutable double Sin_Azeq_Origin_Lat;
   mutable double Cos_Azeq_Origin_Lat;
   mutable double Azeq_Origin_Lat;               /* Latitude of origin in radians */
   mutable double Azeq_Origin_Long;              /* Longitude of origin in radians */
   mutable double Azeq_False_Northing;           /* False northing in meters */
   mutable double Azeq_False_Easting;            /* False easting in meters */

   mutable double Azeq_Delta_Northing;
   mutable double Azeq_Delta_Easting;
/*!
 * The function Set_Azimuthal_Equidistant_Parameters receives the ellipsoid 
 * parameters and projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise AZEQ_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Origin_Latitude   : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */
  long Set_Azimuthal_Equidistant_Parameters(double a,
                                            double f,
                                            double Origin_Latitude,
                                            double Central_Meridian,
                                            double False_Easting,
                                            double False_Northing);

/*!
 * The function Get_Azimuthal_Equidistant_Parameters returns the current ellipsoid
 * parameters and Azimuthal Equidistant projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */
  void Get_Azimuthal_Equidistant_Parameters(double *a,
                                            double *f,
                                            double *Origin_Latitude,
                                            double *Central_Meridian,
                                            double *False_Easting,
                                            double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Azimuthal_Equidistant converts geodetic (latitude and
 * longitude) coordinates to Azimuthal Equidistant projection (easting and northing)
 * coordinates, according to the current ellipsoid and Azimuthal Equidistant projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise AZEQ_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Azimuthal_Equidistant (double Latitude,
                                                  double Longitude,
                                                  double *Easting,
                                                  double *Northing)const;

/*!
 * The function Convert_Azimuthal_Equidistant_To_Geodetic converts Azimuthal Equidistant 
 * projection (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Azimuthal Equidistant projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise AZEQ_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Azimuthal_Equidistant_To_Geodetic(double Easting,
                                                 double Northing,
                                                 double *Latitude,
                                                 double *Longitude)const;

   
TYPE_DATA
};

#endif

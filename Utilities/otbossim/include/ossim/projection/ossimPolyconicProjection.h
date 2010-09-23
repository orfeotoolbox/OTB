//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Polyconic  projection code.  
//*******************************************************************
//  $Id: ossimPolyconicProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimPolyconicProjection_HEADER
#define ossimPolyconicProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimPolyconicProjection : public ossimMapProjection
{
public:   
   ossimPolyconicProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                            const ossimGpt& origin = ossimGpt());
   ossimPolyconicProjection(const ossimEllipsoid& ellipsoid,
                            const ossimGpt& origin,
                            const double falseEasting,
                            const double falseNorthing);
   ~ossimPolyconicProjection(){}
   virtual ossimObject *dup()const
      {
         return new ossimPolyconicProjection(*this);
      }
   
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   virtual void update();

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

   void setDefaults();

   double getFalseEasting()const{return  Poly_False_Easting;}
   double getFalseNorthing()const{return Poly_False_Northing;}
   
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
private:
   
   mutable double Poly_a;                      /* Semi-major axis of ellipsoid in meters */
   mutable double Poly_f;              /* Flattening of ellipsoid */
   mutable double es2;             /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;              /* es2 * es2 */
   mutable double es6;              /* es4 * es2 */
   mutable double M0;
   mutable double c0;      /* 1 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0 */
   mutable double c1; /* 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + 45.0 * es6 / 1024.0 */
   mutable double c2;             /* 15.0 * es4 / 256.0 + 45.0 * es6 / 1024.0 */
   mutable double c3;               /* 35.0 * es6 / 3072.0 */
   
/* Polyconic projection Parameters */
   mutable double Poly_Origin_Lat;                   /* Latitude of origin in radians     */
   mutable double Poly_Origin_Long;                  /* Longitude of origin in radians    */
   mutable double Poly_False_Northing;               /* False northing in meters          */
   mutable double Poly_False_Easting;                /* False easting in meters           */
   
/* Maximum variance for easting and northing values for WGS 84.
 */
   mutable double Poly_Max_Easting;
   mutable double Poly_Max_Northing;
   mutable double Poly_Min_Easting;
   mutable double Poly_Min_Northing;

/*!
 * The function Set_Polyconic_Parameters receives the ellipsoid parameters and
 * Polyconic projcetion parameters as inputs, and sets the corresponding state
 * variables.If any errors
 * occur, the error code(s) are returned by the function, otherwise 
 * POLY_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid                   (input)
 *    Origin_Latitude   : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */
  long Set_Polyconic_Parameters(double a,
                                double f,
                                double Origin_Latitude,
                                double Central_Meridian,
                                double False_Easting,
                                double False_Northing);



/*!
 * The function Get_Polyconic_Parameters returns the current ellipsoid
 * parameters, and Polyconic projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid                   (output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */
  void Get_Polyconic_Parameters(double *a,
                                double *f,
                                double *Origin_Latitude,
                                double *Central_Meridian,
                                double *False_Easting,
                                double *False_Northing)const;



/*!
 * The function Convert_Geodetic_To_Polyconic converts geodetic (latitude and
 * longitude) coordinates to Polyconic projection easting, and northing
 * coordinates, according to the current ellipsoid and Polyconic projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise POLY_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Polyconic (double Latitude,
                                      double Longitude,
                                      double *Easting,
                                      double *Northing)const; 


/*!
 * The function Convert_Polyconic_To_Geodetic converts Polyconic projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Polyconic projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise POLY_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Polyconic_To_Geodetic(double Easting,
                                     double Northing,
                                     double *Latitude,
                                     double *Longitude)const;

TYPE_DATA
};

#endif

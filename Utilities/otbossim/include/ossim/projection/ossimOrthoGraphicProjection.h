//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans OrthoGraphic  projection code.  
//*******************************************************************
//  $Id: ossimOrthoGraphicProjection.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimOrthoGraphicProjection_HEADER
#define ossimOrthoGraphicProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class ossimOrthoGraphicProjection : public ossimMapProjection
{
public:
   
   ossimOrthoGraphicProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                               const ossimGpt& origin = ossimGpt());
   ossimOrthoGraphicProjection(const ossimEllipsoid& ellipsoid,
                               const ossimGpt& origin,
                               const double falseEasting,
                               const double falseNorthing);
   ~ossimOrthoGraphicProjection(){}
   
   virtual ossimObject *dup()const
      {
         return new ossimOrthoGraphicProjection(*this);
      }
   virtual void getGroundClipPoints(ossimGeoPolygon& gpts)const;
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

   double getFalseEasting()const{return  Orth_False_Easting;}
   double getFalseNorthing()const{return Orth_False_Northing;}

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
   
   //___________________GEOTRANS_______________________
   mutable double Orth_a;                      /* Semi-major axis of ellipsoid in meters */
   mutable double Orth_f;              /* Flattening of ellipsoid */
   mutable double es2;             /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;              /* es2 * es2 */
   mutable double es6;              /* es4 * es2 */
   mutable double Ra;                    /* Spherical Radius */
   
/* Orthographic projection Parameters */
   mutable double Orth_Origin_Lat;                   /* Latitude of origin in radians     */
   mutable double Orth_Origin_Long;                  /* Longitude of origin in radians    */
   mutable double Orth_False_Easting;
   mutable double Orth_False_Northing;
   mutable double Sin_Orth_Origin_Lat;               /* sin(Orth_Origin_Lat) */
   mutable double Cos_Orth_Origin_Lat;               /* cos(Orth_Origin_Lat) */
   
/*!
 * The function Set_Orthographic_Parameters receives the ellipsoid parameters and
 * Orthographic projcetion parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise ORTH_NO_ERROR is returned.
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
  long Set_Orthographic_Parameters(double a,
                                   double f,
                                   double Origin_Latitude,
                                   double Central_Meridian,
                                   double False_Easting,
                                   double False_Northing);



/*!
 * The function Get_Orthographic_Parameters returns the current ellipsoid
 * parameters and Orthographic projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid                   (output)
 *    Origin_Latitude   : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Origin_Longitude  : Longitude in radians at the center of     (output)
 *                          the projection (central meridian)
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output) 
 */
  void Get_Orthographic_Parameters(double *a,
                                   double *f,
                                   double *Origin_Latitude,
                                   double *Origin_Longitude,
                                   double *False_Easting,
                                   double *False_Northing)const;



/*!
 * The function Convert_Geodetic_To_Orthographic converts geodetic (latitude and
 * longitude) coordinates to Orthographic projection easting, and northing
 * coordinates, according to the current ellipsoid and Orthographic projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ORTH_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Orthographic (double Latitude,
                                         double Longitude,
                                         double *Easting,
                                         double *Northing)const;


/*!
 * The function Convert_Orthographic_To_Geodetic converts Orthographic projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Orthographic projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ORTH_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Orthographic_To_Geodetic(double Easting,
                                        double Northing,
                                        double *Latitude,
                                        double *Longitude)const;

TYPE_DATA
};

#endif

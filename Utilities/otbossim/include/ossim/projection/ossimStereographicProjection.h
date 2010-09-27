//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimStereographicProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimStereographicProjection_HEADER
#define ossimStereographicProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimStereographicProjection : public ossimMapProjection
{
public:   
   ossimStereographicProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                            const ossimGpt& origin = ossimGpt());
   ossimStereographicProjection(const ossimEllipsoid& ellipsoid,
                            const ossimGpt& origin,
                            const double falseEasting,
                            const double falseNorthing);
   ~ossimStereographicProjection(){}
   virtual ossimObject *dup()const
      {
         return new ossimStereographicProjection(*this);
      }
   
   virtual void update();
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;

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

   double getFalseEasting()const{return  Stereo_False_Easting;}
   double getFalseNorthing()const{return Stereo_False_Northing;}
   
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

/************************************************************************/
/*                           GLOBAL DECLARATIONS
 *
 */
/* Ellipsoid Parameters, default to WGS 84  */
mutable double Stereo_a;                    /* Semi-major axis of ellipsoid, in meters  */
mutable double Stereo_f;            /* Flattening of ellipsoid  */
mutable double Stereo_Ra;             /* Spherical Radius */
mutable double Two_Stereo_Ra;        /* 2 * Spherical Radius */
mutable long   Stereo_At_Pole;                        /* Flag variable */

/* Stereographic projection Parameters */
mutable double Stereo_Origin_Lat;                 /* Latitude of origin, in radians */
mutable double Stereo_Origin_Long;                /* Longitude of origin, in radians */
mutable double Stereo_False_Easting;              /* False easting, in meters */
mutable double Stereo_False_Northing;             /* False northing, in meters */
mutable double Sin_Stereo_Origin_Lat;             /* sin(Stereo_Origin_Lat) */
mutable double Cos_Stereo_Origin_Lat;             /* cos(Stereo_Origin_Lat) */

/* Maximum variance for easting and northing values for WGS 84. */
mutable double Stereo_Delta_Easting;
mutable double Stereo_Delta_Northing;

/*!
 *  The function Set_Stereographic_Parameters receives the ellipsoid
 *  parameters and Stereograpic projection parameters as inputs, and
 *  sets the corresponding state variables.  If any errors occur, error
 *  code(s) are returned by the function, otherwise STEREO_NO_ERROR is returned.
 *
 *  a                : Semi-major axis of ellipsoid, in meters         (input)
 *  f                : Flattening of ellipsoid					               (input)
 *  Origin_Latitude  : Latitude, in radians, at the center of          (input)
 *                       the projection
 *  Central_Meridian : Longitude, in radians, at the center of         (input)
 *                       the projection
 *  False_Easting    : Easting (X) at center of projection, in meters  (input)
 *  False_Northing   : Northing (Y) at center of projection, in meters (input)
 */
long Set_Stereographic_Parameters (double a,
                                   double f,
                                   double Origin_Latitude,
                                   double Central_Meridian,
                                   double False_Easting,
                                   double False_Northing);

/*!
 * The function Get_Stereographic_Parameters returns the current ellipsoid
 * parameters and Stereographic projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Origin_Latitude   : Latitude, in radians, at the center of    (output)
 *                          the projection
 *    Central_Meridian  : Longitude, in radians, at the center of   (output)
 *                          the projection
 *    False_Easting     : A coordinate value, in meters, assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value, in meters, assigned to the
 *                          origin latitude of the projection       (output) 
 */
void Get_Stereographic_Parameters(double *a,
                                  double *f,
                                  double *Origin_Latitude,
                                  double *Central_Meridian,
                                  double *False_Easting,
                                  double *False_Northing)const;

/*!
 * The function Convert_Geodetic_To_Stereographic converts geodetic
 * coordinates (latitude and longitude) to Stereographic coordinates
 * (easting and northing), according to the current ellipsoid
 * and Stereographic projection parameters. If any errors occur, error
 * code(s) are returned by the function, otherwise STEREO_NO_ERROR is returned.
 *
 *    Latitude   :  Latitude, in radians                      (input)
 *    Longitude  :  Longitude, in radians                     (input)
 *    Easting    :  Easting (X), in meters                    (output)
 *    Northing   :  Northing (Y), in meters                   (output)
 */
long Convert_Geodetic_To_Stereographic (double Latitude,
                                        double Longitude,
                                        double *Easting,
                                        double *Northing)const;

/*!
 * The function Convert_Stereographic_To_Geodetic converts Stereographic projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Stereographic projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise STEREO_NO_ERROR is returned.
 *
 *    Easting           : Easting (X), in meters              (input)
 *    Northing          : Northing (Y), in meters             (input)
 *    Latitude          : Latitude (phi), in radians          (output)
 *    Longitude         : Longitude (lambda), in radians      (output)
 */
long Convert_Stereographic_To_Geodetic(double Easting,
                                       double Northing,
                                       double *Latitude,
                                       double *Longitude)const;

TYPE_DATA
};

#endif

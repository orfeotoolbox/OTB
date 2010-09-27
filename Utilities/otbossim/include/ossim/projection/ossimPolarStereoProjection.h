//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Polar Stereographic  projection code.  
//*******************************************************************
//  $Id: ossimPolarStereoProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimPolarStereoProjection_HEADER
#define ossimPolarStereoProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimPolarStereoProjection : public ossimMapProjection
{
public:
   ossimPolarStereoProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                              const ossimGpt& origin = ossimGpt());
   ossimPolarStereoProjection(const ossimEllipsoid& ellipsoid,
                              const ossimGpt& origin,
                              const double falseEasting,
                              const double falseNorthing);
   ~ossimPolarStereoProjection(){}
   
   virtual ossimObject *dup()const{return new ossimPolarStereoProjection(*this);}
   
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

   double getFalseEasting()const{return  Polar_False_Easting;}
   double getFalseNorthing()const{return Polar_False_Northing;}

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

   mutable double Polar_a;                    /* Semi-major axis of ellipsoid in meters  */
   mutable double Polar_f;            /* Flattening of ellipsoid  */
   mutable double es;            /* Eccentricity of ellipsoid    */
   mutable double es_OVER_2;           /* es / 2.0 */
   mutable double Southern_Hemisphere;                /* Flag variable */
   mutable double mc;                    
   mutable double tc;
   mutable double e4;
   mutable double Polar_a_mc;                 /* Polar_a * mc */
   mutable double two_Polar_a;               /* 2.0 * Polar_a */
   
/* Polar Stereographic projection Parameters */
   mutable double Polar_Origin_Lat;   /* Latitude of origin in radians */
   mutable double Polar_Origin_Long;                /* Longitude of origin in radians */
   mutable double Polar_False_Easting;              /* False easting in meters */
   mutable double Polar_False_Northing;             /* False northing in meters */
   
/* Maximum variance for easting and northing values for WGS 84. */
   mutable double Polar_Delta_Easting;
   mutable double Polar_Delta_Northing;
   
/*!
 *  The function Set_Polar_Stereographic_Parameters receives the ellipsoid
 *  parameters and Polar Stereograpic projection parameters as inputs, and
 *  sets the corresponding state variables.  If any errors occur, error
 *  code(s) are returned by the function, otherwise POLAR_NO_ERROR is returned.
 *
 *  a                : Semi-major axis of ellipsoid, in meters         (input)
 *  f                : Flattening of ellipsoid                         (input)
 *  Latitude_of_True_Scale  : Latitude of true scale, in radians       (input)
 *  Longitude_Down_from_Pole : Longitude down from pole, in radians    (input)
 *  False_Easting    : Easting (X) at center of projection, in meters  (input)
 *  False_Northing   : Northing (Y) at center of projection, in meters (input)
 */
  long Set_Polar_Stereographic_Parameters (double a,
                                           double f,
                                           double Latitude_of_True_Scale,
                                           double Longitude_Down_from_Pole,
                                           double False_Easting,
                                           double False_Northing);


/*!
 * The function Get_Polar_Stereographic_Parameters returns the current
 * ellipsoid parameters and Polar projection parameters.
 *
 *  a                : Semi-major axis of ellipsoid, in meters         (output)
 *  f                : Flattening of ellipsoid                         (output)
 *  Latitude_of_True_Scale  : Latitude of true scale, in radians       (output)
 *  Longitude_Down_from_Pole : Longitude down from pole, in radians    (output)
 *  False_Easting    : Easting (X) at center of projection, in meters  (output)
 *  False_Northing   : Northing (Y) at center of projection, in meters (output)
 */
  void Get_Polar_Stereographic_Parameters (double *a,
                                           double *f,
                                           double *Latitude_of_True_Scale,
                                           double *Longitude_Down_from_Pole,
                                           double *False_Easting,
                                           double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Polar_Stereographic converts geodetic
 * coordinates (latitude and longitude) to Polar Stereographic coordinates
 * (easting and northing), according to the current ellipsoid
 * and Polar Stereographic projection parameters. If any errors occur, error
 * code(s) are returned by the function, otherwise POLAR_NO_ERROR is returned.
 *
 *    Latitude   :  Latitude, in radians                      (input)
 *    Longitude  :  Longitude, in radians                     (input)
 *    Easting    :  Easting (X), in meters                    (output)
 *    Northing   :  Northing (Y), in meters                   (output)
 */
  long Convert_Geodetic_To_Polar_Stereographic (double Latitude,
                                                double Longitude,
                                                double *Easting,
                                                double *Northing)const;



/*!
 *  The function Convert_Polar_Stereographic_To_Geodetic converts Polar
 *  Stereographic coordinates (easting and northing) to geodetic
 *  coordinates (latitude and longitude) according to the current ellipsoid
 *  and Polar Stereographic projection Parameters. If any errors occur, the
 *  code(s) are returned by the function, otherwise POLAR_NO_ERROR
 *  is returned.
 *
 *  Easting          : Easting (X), in meters                   (input)
 *  Northing         : Northing (Y), in meters                  (input)
 *  Latitude         : Latitude, in radians                     (output)
 *  Longitude        : Longitude, in radians                    (output)
 *
 */
  long Convert_Polar_Stereographic_To_Geodetic (double Easting,
                                                double Northing,
                                                double *Latitude,
                                                double *Longitude)const;


TYPE_DATA
};

#endif

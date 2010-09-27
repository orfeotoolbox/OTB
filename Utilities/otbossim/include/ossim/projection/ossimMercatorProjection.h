//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Mercator projection code.  
//*******************************************************************
//  $Id: ossimMercatorProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimMercatorProjection_HEADER
#define ossimMercatorProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimMercatorProjection : public ossimMapProjection
{
public:
   
   ossimMercatorProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                           const ossimGpt& origin = ossimGpt());
   
   ossimMercatorProjection(const ossimEllipsoid& ellipsoid,
                           const ossimGpt& origin,
                           double falseEasting,
                           double falseNorthing,
                           double scaleFactor);
   ~ossimMercatorProjection(){}
   virtual ossimObject *dup()const{return new ossimMercatorProjection(*this);}

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

   /*!
    * sets the scale.
    */
   void setScaleFactor(double scaleFactor);

   /*!
    * Allows one to set all parameters for this projections.  parallels are in
    * degrees and easting northings are in meters.
    * Update is then called so we can pre-compute paramters.
    */
   void setParameters(double falseEasting,
                      double falseNorthing,
                      double scaleFactor);  
   void setDefaults();
  
   double getFalseEasting()const{return Merc_False_Easting;}
   double getFalseNorthing()const{return Merc_False_Northing;}
   double getScaleFactor()const{return Merc_Scale_Factor;}
   
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
   
   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

private:
   //_________________GEOTRANS STUFF____________________________

   mutable double Merc_a;    /* Semi-major axis of ellipsoid in meters */
   mutable double Merc_f;      /* Flattening of ellipsoid */
   mutable double Merc_e; /* Eccentricity of ellipsoid    */
   mutable double Merc_es; /* Eccentricity squared         */
   
/* Mercator projection Parameters */
   mutable double Merc_Origin_Lat;      /* Latitude of origin in radians     */
   mutable double Merc_Origin_Long;     /* Longitude of origin in radians    */
   mutable double Merc_False_Northing;  /* False northing in meters          */
   mutable double Merc_False_Easting;   /* False easting in meters           */
   mutable double Merc_Scale_Factor;    /* Scale factor                      */
   
/* Isometric to geodetic latitude parameters, default to WGS 84 */
   mutable double Merc_ab;
   mutable double Merc_bb;
   mutable double Merc_cb;
   mutable double Merc_db;
   
/* Maximum variance for easting and northing values for WGS 84.
 */
   mutable double Merc_Delta_Easting;
   mutable double Merc_Delta_Northing;
   

   bool theSphericalFlag;
/*!
 * The function Set_Mercator_Parameters receives the ellipsoid parameters and
 * Mercator projcetion parameters as inputs, and sets the corresponding state 
 * variables.  It calculates and returns the scale factor.  If any errors
 * occur, the error code(s) are returned by the function, otherwise 
 * MERC_NO_ERROR is returned.
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
 *    Scale_Factor      : Multiplier which reduces distances in the 
 *                          projection to the actual distance on the
 *                          ellipsoid                               (output)
 */
  long Set_Mercator_Parameters(double a,      
                               double f,
                               double Origin_Latitude,
                               double Central_Meridian,
                               double False_Easting,
                               double False_Northing,
                               double *Scale_Factor);


/*!
 * The function Get_Mercator_Parameters returns the current ellipsoid
 * parameters, Mercator projection parameters, and scale factor.
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
 *    Scale_Factor      : Multiplier which reduces distances in the 
 *                          projection to the actual distance on the
 *                          ellipsoid                               (output)
 */
  void Get_Mercator_Parameters(double *a,
                               double *f,
                               double *Origin_Latitude,
                               double *Central_Meridian,
                               double *False_Easting,
                               double *False_Northing,
                               double *Scale_Factor)const;


/*!
 * The function Convert_Geodetic_To_Mercator converts geodetic (latitude and
 * longitude) coordinates to Mercator projection easting, and northing
 * coordinates, according to the current ellipsoid and Mercator projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MERC_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Mercator (double Latitude,
                                     double Longitude,
                                     double *Easting,
                                     double *Northing)const;


/*!
 * The function Convert_Mercator_To_Geodetic converts Mercator projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Mercator projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MERC_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Mercator_To_Geodetic(double Easting,
                                    double Northing,
                                    double *Latitude,
                                    double *Longitude)const;

TYPE_DATA
};

#endif

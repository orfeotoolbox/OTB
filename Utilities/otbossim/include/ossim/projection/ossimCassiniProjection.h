//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Cassini projection code.  
//*******************************************************************
//  $Id: ossimCassiniProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimCassiniProjection_HEADER
#define ossimCassiniProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimCassiniProjection : public ossimMapProjection
{
public:
   ossimCassiniProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                          const ossimGpt& origin = ossimGpt());
   ossimCassiniProjection(const ossimEllipsoid& ellipsoid,
                          const ossimGpt& origin,
                          const double falseEasting,
                          const double falseNorthing);
   ~ossimCassiniProjection(){}
   virtual ossimObject *dup()const{return new ossimCassiniProjection(*this);}
   
   virtual ossimDpt forward(const ossimGpt &worldPoint)    const;
   virtual ossimGpt inverse(const ossimDpt &projectedPoint)const;
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

   double getFalseEasting()const{return Cass_False_Easting;}
   double getFalseNorthing()const{return Cass_False_Northing;}

   /*!
    * Initialize all pramters to default and calls update for
    * pre-computations.
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
   
private:
   
   //_____________________GETRANS STUFF_____________________
   
   mutable double Cass_a;                      /* Semi-major axis of ellipsoid in meters */
   mutable double Cass_f;              /* Flattening of ellipsoid */
   mutable double es2;             /* Eccentricity (0.08181919084262188000) squared  */
   mutable double es4;               /* es2 * es2 */
   mutable double es6;              /* es4 * es2 */
   mutable double M0;
   mutable double c0;                    /* 1 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0 */
   mutable double c1;                  /* 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + 45.0 * es6 / 1024.0 */
   mutable double c2;               /* 15.0 * es4 / 256.0 + 45.0 * es6 / 1024.0 */
   mutable double c3;               /* 35.0 * es6 / 3072.0 */
   mutable double One_Minus_es2;         /* 1.0 - es2 */
   mutable double a0;                  /* 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0 */
   mutable double a1;               /* 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0 */
   mutable double a2;               /* 151.0 * e3 / 96.0 */
   mutable double a3;               /* 1097.0 * e4 /512.0 */
   
/* Cassini projection Parameters */
   mutable double Cass_Origin_Lat;                   /* Latitude of origin in radians     */
   mutable double Cass_Origin_Long;                  /* Longitude of origin in radians    */
   mutable double Cass_False_Northing;               /* False northing in meters          */
   mutable double Cass_False_Easting;                /* False easting in meters           */
   
 /* Maximum variance for easting and northing values for WGS 84.
  */
   mutable double Cass_Min_Easting;
   mutable double Cass_Max_Easting;
   mutable double Cass_Min_Northing;
   mutable double Cass_Max_Northing;

/*!
 * The function Set_Cassini_Parameters receives the ellipsoid parameters and
 * Cassini projcetion parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise CASS_NO_ERROR is returned.
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
  long Set_Cassini_Parameters(double a,
                              double f,
                              double Origin_Latitude,
                              double Central_Meridian,
                              double False_Easting,
                              double False_Northing);


/*!
 * The function Get_Cassini_Parameters returns the current ellipsoid
 * parameters, and Cassini projection parameters.
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
  void Get_Cassini_Parameters(double *a,
                              double *f,
                              double *Origin_Latitude,
                              double *Central_Meridian,
                              double *False_Easting,
                              double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Cassini converts geodetic (latitude and
 * longitude) coordinates to Cassini projection easting, and northing
 * coordinates, according to the current ellipsoid and Cassini projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CASS_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Cassini (double Latitude,
                                    double Longitude,
                                    double *Easting,
                                    double *Northing)const; 


/*!
 * The function Convert_Cassini_To_Geodetic converts Cassini projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Cassini projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CASS_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Cassini_To_Geodetic(double Easting,
                                   double Northing,
                                   double *Latitude,
                                   double *Longitude)const;

TYPE_DATA
};

#endif

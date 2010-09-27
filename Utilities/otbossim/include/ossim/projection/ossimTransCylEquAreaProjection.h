//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Transverse Cylindrical Equal Area
// projection code.  
//*******************************************************************
//  $Id: ossimTransCylEquAreaProjection.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimTransCylEquAreaProjection_HEADER
#define ossimTransCylEquAreaProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimTransCylEquAreaProjection : public ossimMapProjection
{
public:   
   ossimTransCylEquAreaProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(6378137, 6356752.3142),
                                  const ossimGpt& origin = ossimGpt());
   ossimTransCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                                  const ossimGpt& origin,
                                  double falseEasting,
                                  double falseNorthing,
                                  double scaleFactor);
   
   ~ossimTransCylEquAreaProjection(){}

   virtual ossimObject *dup()const{return new ossimTransCylEquAreaProjection(*this);}

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

   /*
    * Set's scale and then update.
    */
   void setScaleFactor(double scaleFactor);
   /*!
    * Sets both false easting and northing values.  The values are
    * expected to be in meters.
    * Update is then called so we can pre-compute paramters
    */
   void setFalseEastingNorthing(double falseEasting, double falseNorthing);

   void setParameters(double falseEasting,
                      double falseNorthing,
                      double scaleFactor);
   
   void setDefaults();

   double getFalseEasting()const{return  Tcea_False_Easting;}
   double getFalseNorthing()const{return Tcea_False_Northing;}
   double getScaleFactor()const{return Tcea_Scale_Factor;}

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
   
   mutable double Tcea_a;                /* Semi-major axis of ellipsoid in meters */
   mutable double Tcea_f;        /* Flattening of ellipsoid */
   mutable double es2;       /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;        /* es2 * es2 */
   mutable double es6;        /* es4 * es2 */
   mutable double es;             /* sqrt(es2) */
   mutable double M0; 
   mutable double qp;
   mutable double One_MINUS_es2;   /* 1.0 - es2 */
   mutable double One_OVER_2es;    /* 1.0 / (2.0 * es) */
   mutable double a0;     /* es2 / 3.0 + 31.0 * es4 / 180.0 + 517.0 * es6 / 5040.0 */
   mutable double a1;         /*  23.0 * es4 / 360.0 + 251.0 * es6 / 3780.0 */
   mutable double a2;         /* 761.0 * es6 / 45360.0 */
   mutable double b0;            /* 3.0 * e1 / 2.0 - 27.0 * e3 / 32.0 */
   mutable double b1;         /* 21.0 * e2 / 16.0 - 55.0 * e4 / 32.0 */
   mutable double b2;         /* 151.0 * e3 / 96.0 */
   mutable double b3;         /* 1097.0 * e4 / 512.0 */
   mutable double c0;/* 1.0 - es2 / 4.0 - 3.0 * es4 / 64.0 - 5.0 * es6 / 256.0 */
   mutable double c1;/* 3.0 * es2 / 8.0 + 3.0 * es4 / 32.0 + 45.0 * es6 / 1024.0 */
   mutable double c2;         /* 15.0 * es4 / 256.0 + 45.0 * es6 / 1024.0 */
   mutable double c3;         /* 35.0 * es6 / 3072.0 */
   
/* Transverse Cylindrical Equal Area projection Parameters */
   mutable double Tcea_Origin_Lat;             /* Latitude of origin in radians     */
   mutable double Tcea_Origin_Long;            /* Longitude of origin in radians    */
   mutable double Tcea_False_Northing;         /* False northing in meters          */
   mutable double Tcea_False_Easting;          /* False easting in meters           */
   mutable double Tcea_Scale_Factor;           /* Scale factor                      */
   
/* Maximum variance for easting and northing values for WGS 84.
 */
   mutable double Tcea_Min_Easting;
   mutable double Tcea_Max_Easting;
   mutable double Tcea_Min_Northing;
   mutable double Tcea_Max_Northing;
   
/*
 * The function Set_Trans_Cyl_Eq_Area_Parameters receives the ellipsoid parameters and
 * Transverse Cylindrical Equal Area projcetion parameters as inputs, and sets the corresponding
 * state variables.  If any errors occur, the error code(s) are returned by the function, 
 * otherwise TCEA_NO_ERROR is returned.
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
 *    Scale_Factor      : Multiplier which reduces distances in the 
 *                          projection to the actual distance on the
 *                          ellipsoid                               (input)
 */
  long Set_Trans_Cyl_Eq_Area_Parameters(double a,
                                        double f,
                                        double Origin_Latitude,
                                        double Central_Meridian,
                                        double False_Easting,
                                        double False_Northing,
                                        double Scale_Factor);


/*!
 * The function Get_Trans_Cyl_Eq_Area_Parameters returns the current ellipsoid
 * parameters, Transverse Cylindrical Equal Area projection parameters, and scale factor.
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
 *    Scale_Factor      : Multiplier which reduces distances in the
 *                          projection to the actual distance on the
 *                          ellipsoid                               (output)
 */
  void Get_Trans_Cyl_Eq_Area_Parameters(double *a,
                                        double *f,
                                        double *Origin_Latitude,
                                        double *Central_Meridian,
                                        double *False_Easting,
                                        double *False_Northing,
                                        double *Scale_Factor)const;


/*!
 * The function Convert_Geodetic_To_Trans_Cyl_Eq_Area converts geodetic (latitude and
 * longitude) coordinates to Transverse Cylindrical Equal Area projection easting, and northing
 * coordinates, according to the current ellipsoid and Transverse Cylindrical Equal Area projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise TCEA_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Trans_Cyl_Eq_Area (double Latitude,
                                              double Longitude,
                                              double *Easting,
                                              double *Northing)const; 


/*!
 * The function Convert_Trans_Cyl_Eq_Area_To_Geodetic converts Transverse Cylindrical Equal Area 
 * projection easting and northing coordinates to geodetic (latitude and longitude) coordinates.
 * coordinates, according to the current ellipsoid and Transverse Cylindrical Equal Area projection
 * If any errors occur, the error code(s) are returned by the function, otherwise TCEA_NO_ERROR 
 * is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Trans_Cyl_Eq_Area_To_Geodetic(double Easting,
                                             double Northing,
                                             double *Latitude,
                                             double *Longitude)const;

TYPE_DATA
};

#endif

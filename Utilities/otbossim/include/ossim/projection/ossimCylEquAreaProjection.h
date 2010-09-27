//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Cylinder Equal Area  projection code.  
//*******************************************************************
//  $Id: ossimCylEquAreaProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimCylEquAreaProjection_HEADER
#define ossimCylEquAreaProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimCylEquAreaProjection : public ossimMapProjection
{
public: 
   ossimCylEquAreaProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                             const ossimGpt& origin = ossimGpt());
   ossimCylEquAreaProjection(const ossimEllipsoid& ellipsoid,
                             const ossimGpt& origin,
                             double falseEasting,
                             double falseNorthing);
   ~ossimCylEquAreaProjection(){}
   virtual ossimObject *dup()const
      {
         return new ossimCylEquAreaProjection(*this);
      }

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

   double getFalseEasting()const{return  Cyeq_False_Easting;}
   double getFalseNorthing()const{return Cyeq_False_Northing;}

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


/* Ellipsoid Parameters, default to WGS 84 */
   mutable double Cyeq_a;                     /* Semi-major axis of ellipsoid in meters */
   mutable double Cyeq_f;             /* Flattening of ellipsoid */
   mutable double es2;            /* Eccentricity (0.08181919084262188000) squared  */
   mutable double es;                  /* Sqrt(es2) */
   mutable double es4;             /* es2 * es2 */
   mutable double es6;             /* es4 * es2 */
   mutable double k0;
   mutable double Cyeq_a_k0;                  /* Cyeq_a * k0 */
   mutable double two_k0;                           /* 2.0 * k0 */
   mutable double c0;                 /* es2 / 3.0 + 31.0 * es4 / 180.0 + 517.0 * es6 / 5040.0 */
   mutable double c1;              /* 23.0 es4 / 360.0 + 251.0 * es6 / 3780.0 */
   mutable double c2;              /* 761.0 * es6 / 45360.0 */
   
/* Cylindrical Equal Area projection Parameters */
   mutable double Cyeq_Origin_Lat;                  /* Latitude of origin in radians     */
   mutable double Cyeq_Origin_Long;                 /* Longitude of origin in radians    */
   mutable double Cyeq_False_Northing;              /* False northing in meters          */
   mutable double Cyeq_False_Easting;               /* False easting in meters           */
   
/* Maximum variance for easting and northing values for WGS 84.
 */
   mutable double Cyeq_Max_Easting;
   mutable double Cyeq_Min_Easting;
   mutable double Cyeq_Delta_Northing;

/*!
 * The function Set_Cyl_Eq_Area_Parameters receives the ellipsoid parameters and
 * Cylindrical Equal Area projcetion parameters as inputs, and sets the corresponding 
 * state variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise CYEQ_NO_ERROR is returned.
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
  long Set_Cyl_Eq_Area_Parameters(double a,
                                  double f,
                                  double Origin_Latitude,
                                  double Central_Meridian,
                                  double False_Easting,
                                  double False_Northing);


/*!
 * The function Get_Cyl_Eq_Area_Parameters returns the current ellipsoid
 * parameters and Cylindrical Equal Area projection parameters.
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
 */  void Get_Cyl_Eq_Area_Parameters(double *a,
                                  double *f,
                                  double *Origin_Latitude,
                                  double *Central_Meridian,
                                  double *False_Easting,
                                  double *False_Northing)const;



/*!
 * The function Convert_Geodetic_To_Cyl_Eq_Area converts geodetic (latitude and
 * longitude) coordinates to Cylindrical Equal Area projection easting, and northing
 * coordinates, according to the current ellipsoid and Cylindrical Equal Area projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CYEQ_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Cyl_Eq_Area (double Latitude,
                                        double Longitude,
                                        double *Easting,
                                        double *Northing)const; 


/*!
 * The function Convert_Cyl_Eq_Area_To_Geodetic converts Cylindrical Equal Area projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Cylindrical Equal Area projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise CYEQ_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Cyl_Eq_Area_To_Geodetic(double Easting,
                                       double Northing,
                                       double *Latitude,
                                       double *Longitude)const;

/*!
 * RTTI information found in data_types/rtti.h
 */
TYPE_DATA
};

#endif

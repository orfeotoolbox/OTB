//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Eckert4 projection code.  
//*******************************************************************
//  $Id: ossimEckert4Projection.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimEcker4Projection_HEADER
#define ossimEcker4Projection_HEADER
#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimEckert4Projection : public ossimMapProjection
{
public:
    
   ossimEckert4Projection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                          const ossimGpt& origin = ossimGpt());
   ossimEckert4Projection(const ossimEllipsoid& ellipsoid,
                          const ossimGpt& origin,
                          double falseEasting,
                          double falseNorthing);
   ~ossimEckert4Projection(){}
   virtual ossimObject *dup()const
      {
         return new ossimEckert4Projection(*this);
      }
   
   virtual ossimDpt forward(const ossimGpt &worldPoint)     const;
   virtual ossimGpt inverse(const ossimDpt &projectedPoint) const;
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

   double getFalseEasting()const{return  Eck4_False_Easting;}
   double getFalseNorthing()const{return Eck4_False_Northing;}

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
   mutable double Eck4_a;                      /* Semi-major axis of ellipsoid in meters */
   mutable double Eck4_f;              /* Flattening of ellipsoid */
   mutable double es2;             /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;              /* es2 * es2  */
   mutable double es6;              /* es4 * es2  */
   
   mutable double Ra0;                   /* 0.4222382 * Sperical Radius (6371007.1810824) */
   mutable double Ra1;                   /* 1.3265004 * Sperical Radius (6371007.1810824) */
   
/* Eckert4 projection Parameters */
   mutable double Eck4_Origin_Long;                  /* Longitude of origin in radians    */
   mutable double Eck4_False_Easting;
   mutable double Eck4_False_Northing;
   mutable double Eck4_Delta_Northing;
   mutable double Eck4_Max_Easting;
   mutable double Eck4_Min_Easting;
   
/*!
 * The function Set_Eckert4_Parameters receives the ellipsoid parameters and
 * Eckert IV projection parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise ECK4_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (input)
 */
  long Set_Eckert4_Parameters(double a,
                              double f,                           
                              double Central_Meridian,
                              double False_Easting,
                              double False_Northing);


/*!
 * The function Get_Eckert4_Parameters returns the current ellipsoid
 * parameters and EckertIV projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          origin latitude of the projection       (output)
 */
  void Get_Eckert4_Parameters(double *a,
                              double *f,                            
                              double *Central_Meridian,
                              double *False_Easting,
                              double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Eckert4 converts geodetic (latitude and
 * longitude) coordinates to Eckert IV projection easting, and northing
 * coordinates, according to the current ellipsoid and Eckert IV projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK4_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Eckert4 (double Latitude,
                                    double Longitude,
                                    double *Easting,
                                    double *Northing)const;


/*!
 * The function Convert_Eckert4_To_Geodetic converts Eckert IV projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Eckert IV projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise ECK4_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Eckert4_To_Geodetic(double Easting,
                                   double Northing,
                                   double *Latitude,
                                   double *Longitude)const;

/*!
 * RTTI information
 */
TYPE_DATA
};

#endif

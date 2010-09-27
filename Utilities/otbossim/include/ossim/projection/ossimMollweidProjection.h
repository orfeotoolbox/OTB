//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Mollweid projection code.  
//*******************************************************************
//  $Id: ossimMollweidProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimMollweidProojection_HEADER
#define ossimMollweidProojection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimMollweidProjection : public ossimMapProjection
{
public:   
   ossimMollweidProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                           const ossimGpt& origin = ossimGpt());
   ossimMollweidProjection(const ossimEllipsoid& ellipsoid,
                           const ossimGpt& origin,
                           const double falseEasting,
                           const double falseNorthing);
   
   ~ossimMollweidProjection(){}
   
   virtual ossimObject *dup()const{return new ossimMollweidProjection(*this);}

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

   void setCentralMeridian(double centralMeridian);

   double getFalseEasting()const{return Moll_False_Easting;}
   double getFalseNorthing()const{return Moll_False_Northing;}

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

   //__________________GEOTRANS______________________
   mutable double Moll_a;                /* Semi-major axis of ellipsoid in meters */
   mutable double Moll_f;        /* Flattening of ellipsoid */
   mutable double es2;       /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;        /* es2 * es2	*/
   mutable double es6;        /* es4 * es2  */
   mutable double Sqrt2_Ra;        /* sqrt(2.0) * Spherical Radius(6371007.1810824) */
   mutable double Sqrt8_Ra;        /* sqrt(8.0) * Spherical Radius(6371007.1810824) */
   
/* Mollweide projection Parameters */
   mutable double Moll_Origin_Long;            /* Longitude of origin in radians    */
   mutable double Moll_False_Easting;
   mutable double Moll_False_Northing;
   mutable double Moll_Delta_Northing;
   mutable double Moll_Max_Easting;
   mutable double Moll_Min_Easting;

/*!
 * The function Set_Mollweide_Parameters receives the ellipsoid parameters and
 * Mollweide projcetion parameters as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise MOLL_NO_ERROR is returned.
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
  long Set_Mollweide_Parameters(double a,
                                double f,
                                double Central_Meridian,
                                double False_Easting,
                                double False_Northing);


/*!
 * The function Get_Mollweide_Parameters returns the current ellipsoid
 * parameters and Mollweide projection parameters.
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
  void Get_Mollweide_Parameters(double *a,
                                double *f,
                                double *Central_Meridian,
                                double *False_Easting,
                                double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Mollweide converts geodetic (latitude and
 * longitude) coordinates to Mollweide projection easting, and northing
 * coordinates, according to the current ellipsoid and Mollweide projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MOLL_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Mollweide (double Latitude,
                                      double Longitude,
                                      double *Easting,
                                      double *Northing)const;


/*!
 * The function Convert_Mollweide_To_Geodetic converts Mollweide projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Mollweide projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise MOLL_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Mollweide_To_Geodetic(double Easting,
                                     double Northing,
                                     double *Latitude,
                                     double *Longitude)const;
   

TYPE_DATA
};

#endif

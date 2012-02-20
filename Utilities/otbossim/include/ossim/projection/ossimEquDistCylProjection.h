//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Equidistant Cylinder projection code.  
//*******************************************************************
//  $Id: ossimEquDistCylProjection.h 20352 2011-12-12 17:24:52Z dburken $

#ifndef ossimEquDistCylProjection_HEADER
#define ossimEquDistCylProjection_HEADER

#include <ossim/projection/ossimLlxyProjection.h>

class ossimIpt;

class OSSIMDLLEXPORT ossimEquDistCylProjection : public ossimMapProjection//public ossimLlxyProjection
{
public:
   ossimEquDistCylProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                             const ossimGpt& origin = ossimGpt());
   ossimEquDistCylProjection(const ossimEllipsoid& ellipsoid,
                             const ossimGpt& origin,
                             double falseEasting,
                             double falseNorthing);
   
   ossimEquDistCylProjection(const ossimEquDistCylProjection& rhs)
     :ossimMapProjection(rhs)
     //      :ossimLlxyProjection(rhs)
      {
         *this = rhs;
      }
   
   ~ossimEquDistCylProjection(){}
   virtual ossimObject *dup()const
      {
         return new ossimEquDistCylProjection(*this);
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

   double getFalseEasting()const{return Eqcy_False_Easting;}
   double getFalseNorthing()const{return Eqcy_False_Northing;}
   
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const;
   virtual void     worldToLineSample(const ossimGpt &worldPoint,
                                      ossimDpt&       lineSample)const;

   /**
    * @brief Specialized worldToLineSample.
    * 
    * Takes into consideration the image size and crossing the date line.
    * Also makes local points outside of image negative or positive based on
    * there relative closeness to left or right edge.
    *
    * @param worldPoint Ground point to convert to line sample.
    * @param imageSize Size of image in pixels.
    * @param lineSample Initialized by this.
    */
   void worldToLineSample( const ossimGpt& worldPoint,
                           const ossimIpt& imageSize,
                           ossimDpt&       lineSample ) const;
   
   virtual bool isGeographic()const
   {
      return true;
   }
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
   //---------------------GEOTRANS-------------------------------
   mutable double Eqcy_a;                  /* Semi-major axis of ellipsoid in meters */
   mutable double Eqcy_f;          /* Flattening of ellipsoid */
   mutable double es2;         /* Eccentricity (0.08181919084262188000) squared         */
   mutable double es4;          /* es2 * es2	 */
   mutable double es6;          /* es4 * es2  */
   mutable double Ra;                /* Spherical Radius  */
   
/* Equidistant Cylindrical projection Parameters */
   mutable double Eqcy_Std_Parallel;             /* Latitude of standard parallel in radians     */
   mutable double Cos_Eqcy_Std_Parallel;         /* cos(Eqcy_Std_Parallel)  */
   mutable double Eqcy_Origin_Long;              /* Longitude of origin in radians    */
   mutable double Eqcy_False_Easting;
   mutable double Eqcy_False_Northing;
   mutable double Eqcy_Delta_Northing;
   mutable double Eqcy_Max_Easting;
   mutable double Eqcy_Min_Easting;
   mutable double Ra_Cos_Eqcy_Std_Parallel; /* Ra * Cos_Eqcy_Std_Parallel */

/*!
 * The function Set_Equidistant_Cyl_Parameters receives the ellipsoid parameters and
 * projection parameters as inputs, and sets the corresponding state
 * variables.  It also calculates the spherical radius of the sphere having 
 * the same area as the ellipsoid.  If any errors occur, the error code(s) 
 * are returned by the function, otherwise EQCY_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (input)
 *    f                 : Flattening of ellipsoid						        (input)
 *    Std_Parallel      : Latitude in radians at which the          (input)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (input)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (input)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          standard parallel of the projection     (input)
 */
  long Set_Equidistant_Cyl_Parameters(double a,
                                      double f,
                                      double Std_Parallel,
                                      double Central_Meridian,
                                      double False_Easting,
                                      double False_Northing);


/*!
 * The function Get_Equidistant_Cyl_Parameters returns the current ellipsoid
 * parameters and Equidistant Cylindrical projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters   (output)
 *    f                 : Flattening of ellipsoid						        (output)
 *    Std_Parallel      : Latitude in radians at which the          (output)
 *                          point scale factor is 1.0
 *    Central_Meridian  : Longitude in radians at the center of     (output)
 *                          the projection
 *    False_Easting     : A coordinate value in meters assigned to the
 *                          central meridian of the projection.     (output)
 *    False_Northing    : A coordinate value in meters assigned to the
 *                          standard parallel of the projection     (output)
 */
  void Get_Equidistant_Cyl_Parameters(double *a,
                                      double *f,
                                      double *Std_Parallel,
                                      double *Central_Meridian,
                                      double *False_Easting,
                                      double *False_Northing)const;


/*!
 * The function Convert_Geodetic_To_Equidistant_Cyl converts geodetic (latitude and
 * longitude) coordinates to Equidistant Cylindrical projection easting, and northing
 * coordinates, according to the current ellipsoid and Equidistant Cylindrical projection
 * parameters.  If any errors occur, the error code(s) are returned by the
 * function, otherwise EQCY_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi) in radians           (input)
 *    Longitude         : Longitude (lambda) in radians       (input)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */
  long Convert_Geodetic_To_Equidistant_Cyl (double Latitude,
                                            double Longitude,
                                            double *Easting,
                                            double *Northing)const;


/*!
 * The function Convert_Equidistant_Cyl_To_Geodetic converts Equidistant Cylindrical projection
 * easting and northing coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and Equidistant Cylindrical projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise EQCY_NO_ERROR is returned.
 *
 *    Easting           : Easting (X) in meters                  (input)
 *    Northing          : Northing (Y) in meters                 (input)
 *    Latitude          : Latitude (phi) in radians              (output)
 *    Longitude         : Longitude (lambda) in radians          (output)
 */
  long Convert_Equidistant_Cyl_To_Geodetic(double Easting,
                                           double Northing,
                                           double *Latitude,
                                           double *Longitude)const;

TYPE_DATA
};

#endif

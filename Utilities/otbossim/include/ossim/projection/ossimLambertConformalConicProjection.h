//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Lamberts Conformal Conic projection code.  
//*******************************************************************
//  $Id: ossimLambertConformalConicProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimLambertConformalConicProjection_HEADER
#define ossimLambertConformalConicProjection_HEADER

#include <iosfwd>
#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimLambertConformalConicProjection : public ossimMapProjection
{
public:
   ossimLambertConformalConicProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                                        const ossimGpt& origin = ossimGpt());

   /**
    * @param ellipsoid
    * @param origin latitude longitude in decimal degrees.
    * @param stdParallel1 First parallel in DEGREES.
    * @param stdParallel2 Second parallel in DEGREES.
    * @param falseEasting False easting in meters.
    * @param falseNorthing False northing in meters.
    */
   ossimLambertConformalConicProjection(const ossimEllipsoid& ellipsoid,
                                        const ossimGpt& origin,
                                        double stdParallel1,
                                        double stdParallel2,
                                        double falseEasting,
                                        double falseNorthing);
   
   virtual ~ossimLambertConformalConicProjection();
   
   virtual ossimObject *dup()const;
   
   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   virtual void update();
   
   /*!
    * The passed in Argument is in degrees and will convert it to radians
    * for Geotrans. Update is then called so we can pre-compute paramters.
    */
   void setStandardParallel1(double degree);

   /*!
    * The passed in Argument is in degrees and will convert it to radians
    * for Geotrans. Update is then called so we can pre-compute paramters
    */
   void setStandardParallel2(double degree);

   /*!
    * Will allow you to set both parallels. both are passed in degrees.
    */
   void setStandardParallels(double parallel1Degree,
                             double prallel2Degree);

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

   /**
    * @brief Allows one to set all parameters for this projections.
    * Update is then called so we can pre-compute paramters.
    * @param stdParallel1 First parallel in decimal degrees.
    * @param stdParallel2 Second parallel in decimal degrees.
    * @param falseEasting False easting in meters.
    * @param falseNorthing False northing in meters.
    */
   void setParameters(double parallel1,
                      double parallel2,
                      double falseEasting,
                      double falseNorthing);

   /** @returns the first standard parallel in decimal degrees. */
   virtual double getStandardParallel1() const;

   /** @returns the second standard parallel in decimal degrees. */
   virtual double getStandardParallel2() const;
   
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
  
  /**
   * @brief Prints data members to stream.  Returns stream&.
   */
   virtual std::ostream& print(std::ostream& out) const;

   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

private:

   //_______________GEOTRANS____________________
   mutable double Lambert_a;               /* Semi-major axis of ellipsoid, in meters */
   mutable double Lambert_f;       /* Flattening of ellipsoid */
   mutable double es;         /* Eccentricity of ellipsoid */
   mutable double es2;         /* Eccentricity squared */
   mutable double es_OVER_2;        /* Eccentricity / 2.0 */
   mutable double n;                 /* Ratio of angle between meridians */
   mutable double F;                 /* Flattening of ellipsoid */
   mutable double rho0;              /* Height above ellipsoid */
   mutable double Lambert_aF;        /* Lambert_a * F */
   
/* Lambert_Conformal_Conic projection Parameters */
   mutable double Lambert_Std_Parallel_1; /* Lower std. parallel, in radians */
   mutable double Lambert_Std_Parallel_2; /* Upper std. parallel, in radians */
   mutable double Lambert_Origin_Lat;     /* Latitude of origin, in radians */
   mutable double Lambert_Origin_Long;                /* Longitude of origin, in radians */
   mutable double Lambert_False_Northing;             /* False northing, in meters */
   mutable double Lambert_False_Easting;              /* False easting, in meters */

/*!
 * The function Set_Lambert_Parameters receives the ellipsoid parameters and
 * Lambert Conformal Conic projection parameters as inputs, and sets the
 * corresponding state variables.  If any errors occur, the error code(s)
 * are returned by the function, otherwise LAMBERT_NO_ERROR is returned.
 *
 *   a                   : Semi-major axis of ellipsoid, in meters   (input)
 *   f                   : Flattening of ellipsoid				  	       (input)
 *   Origin_Latitude     : Latitude of origin in radians             (input)
 *   Central_Meridian    : Longitude of origin in radians            (input)
 *   Std_Parallel_1      : First standard parallel                   (input)
 *   Std_Parallel_2      : Second standard parallel                  (input)
 *   False_Easting       : False easting in meters                   (input)
 *   False_Northing      : False northing in meters                  (input)
 */
  long Set_Lambert_Parameters(double a,
                              double f,
                              double Origin_Latitude,
                              double Central_Meridian,
                              double Std_Parallel_1,
                              double Std_Parallel_2,
                              double False_Easting,
                              double False_Northing);




/*!
 * The function Get_Lambert_Parameters returns the current ellipsoid
 * parameters and Lambert Conformal Conic projection parameters.
 *
 *   a                   : Semi-major axis of ellipsoid, in meters   (output)
 *   f                   : Flattening of ellipsoid					         (output)
 *   Origin_Latitude     : Latitude of origin, in radians            (output)
 *   Central_Meridian    : Longitude of origin, in radians           (output)
 *   Std_Parallel_1      : First standard parallel                   (output)
 *   Std_Parallel_2      : Second standard parallel                  (output)
 *   False_Easting       : False easting, in meters                  (output)
 *   False_Northing      : False northing, in meters                 (output) 
 */
  void Get_Lambert_Parameters(double *a,
                              double *f,
                              double *Origin_Latitude,
                              double *Central_Meridian,
                              double *Std_Parallel_1,
                              double *Std_Parallel_2,
                              double *False_Easting,
                              double *False_Northing)const;

/*!
 * The function Convert_Geodetic_To_Lambert converts Geodetic (latitude and
 * longitude) coordinates to Lambert Conformal Conic projection (easting
 * and northing) coordinates, according to the current ellipsoid and
 * Lambert Conformal Conic projection parameters.  If any errors occur, the
 * error code(s) are returned by the function, otherwise LAMBERT_NO_ERROR is
 * returned.
 *
 *    Latitude         : Latitude in radians                         (input)
 *    Longitude        : Longitude in radians                        (input)
 *    Easting          : Easting (X) in meters                       (output)
 *    Northing         : Northing (Y) in meters                      (output)
 */
  long Convert_Geodetic_To_Lambert (double Latitude,
                                    double Longitude,
                                    double *Easting,
                                    double *Northing)const;

/*!
 * The function Convert_Lambert_To_Geodetic converts Lambert Conformal
 * Conic projection (easting and northing) coordinates to Geodetic
 * (latitude and longitude) coordinates, according to the current ellipsoid
 * and Lambert Conformal Conic projection parameters.  If any errors occur,
 * the error code(s) are returned by the function, otherwise LAMBERT_NO_ERROR
 * is returned.
 *
 *    Easting          : Easting (X) in meters                       (input)
 *    Northing         : Northing (Y) in meters                      (input)
 *    Latitude         : Latitude in radians                         (output)
 *    Longitude        : Longitude in radians                        (output)
 */
  long Convert_Lambert_To_Geodetic (double Easting,
                                    double Northing,
                                    double *Latitude,
                                    double *Longitude)const;


TYPE_DATA
};

#endif

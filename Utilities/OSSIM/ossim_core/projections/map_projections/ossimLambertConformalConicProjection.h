//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Lamberts Conformal Conic projection code.  
//*******************************************************************
//  $Id: ossimLambertConformalConicProjection.h,v 1.6 2005/09/09 19:23:58 gpotts Exp $

#ifndef ossimLambertConformalConicProjection_HEADER
#define ossimLambertConformalConicProjection_HEADER
#include "ossimMapProjection.h"

class ossimLambertConformalConicProjection : public ossimMapProjection
{
public:
   ossimLambertConformalConicProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                                        const ossimGpt& origin = ossimGpt());
   
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

   /*!
    * Allows one to set all parameters for this projections.  parallels are in
    * degrees and easting northings are in meters.
    * Update is then called so we can pre-compute paramters.
    */
   void setParameters(double parallel1,
                      double parallel2,
                      double falseEasting,
                      double falseNorthing);
   
   virtual double getStandardParallel1() const;
   virtual double getStandardParallel2() const;
   virtual double getFalseEasting()      const;
   virtual double getFalseNorthing()     const;
   
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

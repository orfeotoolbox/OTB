//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Calls Geotrans Transverse Mercator  projection code.  
//*******************************************************************
//  $Id: ossimTransMercatorProjection.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimTransMercatorProjection_HEADER
#define ossimTransMercatorProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimTransMercatorProjection : public ossimMapProjection
{
public:
   ossimTransMercatorProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(6378137, 6356752.3142),
                                const ossimGpt& origin = ossimGpt());
   ossimTransMercatorProjection(const ossimEllipsoid& ellipsoid,
                                const ossimGpt& origin,
                                double falseEasting,
                                double falseNorthing,
                                double scaleFactor);
   ~ossimTransMercatorProjection(){}

   virtual ossimObject *dup()const{return new ossimTransMercatorProjection(*this);}
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

   double getFalseEasting()const{return  TranMerc_False_Easting;}
   double getFalseNorthing()const{return TranMerc_False_Northing;}
   double getScaleFactor()const{return TranMerc_Scale_Factor;}

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

   /*!
    * Prints data members to stream.  Returns stream&.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

protected:

   //_____________GEOTRANS_______________
   
   double TranMerc_a;              /* Semi-major axis of ellipsoid i meters */
   double TranMerc_f;      /* Flattening of ellipsoid  */
   double TranMerc_es; /* Eccentricity (0.08181919084262188000) squared */
   double TranMerc_ebs;   /* Second Eccentricity squared */
   
/* Transverse_Mercator projection Parameters */
   double TranMerc_Origin_Lat;           /* Latitude of origin in radians */
   double TranMerc_Origin_Long;          /* Longitude of origin in radians */
   double TranMerc_False_Northing;       /* False northing in meters */
   double TranMerc_False_Easting;        /* False easting in meters */
   double TranMerc_Scale_Factor;         /* Scale factor  */
   
/* Isometeric to geodetic latitude parameters, default to WGS 84 */
   double TranMerc_ap;
   double TranMerc_bp;
   double TranMerc_cp;
   double TranMerc_dp;
   double TranMerc_ep;
   
/* Maximum variance for easting and northing values for WGS 84. */
   double TranMerc_Delta_Easting;
   double TranMerc_Delta_Northing;
   
/*!
 * The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
 * parameters and Tranverse Mercator projection parameters as inputs, and
 * sets the corresponding state variables. If any errors occur, the error
 * code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
 * returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters    (input)
 *    f                 : Flattening of ellipsoid                    (input)
 *    Origin_Latitude   : Latitude in radians at the origin of the   (input)
 *                         projection
 *    Central_Meridian  : Longitude in radians at the center of the  (input)
 *                         projection
 *    False_Easting     : Easting/X at the center of the projection  (input)
 *    False_Northing    : Northing/Y at the center of the projection (input)
 *    Scale_Factor      : Projection scale factor                    (input) 
 */
  long Set_Transverse_Mercator_Parameters(double a,      
                                          double f,
                                          double Origin_Latitude,
                                          double Central_Meridian,
                                          double False_Easting,
                                          double False_Northing,
                                          double Scale_Factor);


/*!
 * The function Get_Transverse_Mercator_Parameters returns the current
 * ellipsoid and Transverse Mercator projection parameters.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters    (output)
 *    f                 : Flattening of ellipsoid                    (output)
 *    Origin_Latitude   : Latitude in radians at the origin of the   (output)
 *                         projection
 *    Central_Meridian  : Longitude in radians at the center of the  (output)
 *                         projection
 *    False_Easting     : Easting/X at the center of the projection  (output)
 *    False_Northing    : Northing/Y at the center of the projection (output)
 *    Scale_Factor      : Projection scale factor                    (output) 
 */
  void Get_Transverse_Mercator_Parameters(double *a,
                                          double *f,
                                          double *Origin_Latitude,
                                          double *Central_Meridian,
                                          double *False_Easting,
                                          double *False_Northing,
                                          double *Scale_Factor)const;


/*!
 * The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
 * (latitude and longitude) coordinates to Transverse Mercator projection
 * (easting and northing) coordinates, according to the current ellipsoid
 * and Transverse Mercator projection coordinates.  If any errors occur, the
 * error code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
 * returned.
 *
 *    Latitude      : Latitude in radians                         (input)
 *    Longitude     : Longitude in radians                        (input)
 *    Easting       : Easting/X in meters                         (output)
 *    Northing      : Northing/Y in meters                        (output)
 */
  long Convert_Geodetic_To_Transverse_Mercator (double Latitude,
                                                double Longitude,
                                                double *Easting,
                                                double *Northing)const;



/*!
 * The function Convert_Transverse_Mercator_To_Geodetic converts Transverse
 * Mercator projection (easting and northing) coordinates to geodetic
 * (latitude and longitude) coordinates, according to the current ellipsoid
 * and Transverse Mercator projection parameters.  If any errors occur, the
 * error code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
 * returned.
 *
 *    Easting       : Easting/X in meters                         (input)
 *    Northing      : Northing/Y in meters                        (input)
 *    Latitude      : Latitude in radians                         (output)
 *    Longitude     : Longitude in radians                        (output)
 */
  long Convert_Transverse_Mercator_To_Geodetic (double Easting,
                                                double Northing,
                                                double *Latitude,
                                                double *Longitude)const;



TYPE_DATA
};

#endif

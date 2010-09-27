//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Calls Geotrans Utm projection code.  
//*******************************************************************
//  $Id: ossimUtmProjection.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimUtmProjection_HEADER
#define ossimUtmProjection_HEADER
#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimUtmProjection : public ossimMapProjection
{
public:
   ossimUtmProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                      const ossimGpt& origin = ossimGpt());
   
   ossimUtmProjection(const ossimEllipsoid& ellipsoid,
                      const ossimGpt& origin,
                      ossim_int32 zone,
                      char hemisphere);
   ossimUtmProjection(ossim_int32 zone);
   ossimUtmProjection(const ossimUtmProjection& src);
   
   virtual ossimObject* dup()const;

   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   virtual void update();

   /**
    * This will set the utm zone and utm origin base on origin passed in.
    *
    * @note If the origin latitude is not 0.0 it will also set the
    * hemisphere.  If 0.0 hemisphere is unchanged from previous stored value.
    */
   virtual void setOrigin(const ossimGpt& origin);
   
   void setZone(const ossimGpt& ground);
   void setZone(ossim_int32 zone);
   void setHemisphere(const ossimGpt& ground);
   void setHemisphere(char hemisphere);
   static ossim_int32 computeZone(const ossimGpt& gpt);

   /**
    * Return in decimal degrees the zone meridian.
    */
   static double computeZoneMeridian(ossim_int32 zone);

   ossim_int32 getZone()const;

   char getHemisphere()const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Prints data members to stream.  Returns stream&.
    */
   virtual std::ostream& print(std::ostream& out) const;   

   /**
    * @return The false easting.
    */
   virtual double getFalseEasting() const;

   /**
    * @return The false northing.
    */
   virtual double getFalseNorthing() const;

   //! Returns TRUE if principal parameters are within epsilon tolerance.
   virtual bool operator==(const ossimProjection& projection) const;

private:
   /*_____________GEOTRANS_______________*/
   
   /**
    * The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
    * parameters and Tranverse Mercator projection parameters as inputs, and
    * sets the corresponding state variables. If any errors occur, the error
    * code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
    * returned.
    *
    * a                 : Semi-major axis of ellipsoid, in meters    (input)
    * f                 : Flattening of ellipsoid                    (input)
    * Origin_Latitude   : Latitude in radians at the origin of the   (input)
    *                      projection
    * Central_Meridian  : Longitude in radians at the center of the  (input)
    *                      projection
    * False_Easting     : Easting/X at the center of the projection  (input)
    * False_Northing    : Northing/Y at the center of the projection (input)
    * Scale_Factor      : Projection scale factor                    (input) 
    */
   ossim_int32 Set_Transverse_Mercator_Parameters(double a,      
                                                  double f,
                                                  double Origin_Latitude,
                                                  double Central_Meridian,
                                                  double False_Easting,
                                                  double False_Northing,
                                                  double Scale_Factor);
   /**
    * The function Get_Transverse_Mercator_Parameters returns the current
    * ellipsoid and Transverse Mercator projection parameters.
    *
    * a                 : Semi-major axis of ellipsoid, in meters    (output)
    * f                 : Flattening of ellipsoid                    (output)
    * Origin_Latitude   : Latitude in radians at the origin of the   (output)
    *                      projection
    * Central_Meridian  : Longitude in radians at the center of the  (output)
    *                      projection
    * False_Easting     : Easting/X at the center of the projection  (output)
    * False_Northing    : Northing/Y at the center of the projection (output)
    * Scale_Factor      : Projection scale factor                    (output) 
    */
   void Get_Transverse_Mercator_Parameters(double *a,
                                           double *f,
                                           double *Origin_Latitude,
                                           double *Central_Meridian,
                                           double *False_Easting,
                                           double *False_Northing,
                                           double *Scale_Factor)const;
   
   
   /**
    * The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
    * (latitude and longitude) coordinates to Transverse Mercator projection
    * (easting and northing) coordinates, according to the current ellipsoid
    * and Transverse Mercator projection coordinates.  If any errors occur, the
    * error code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
    * returned.
    *
    * Latitude      : Latitude in radians                         (input)
    * Longitude     : Longitude in radians                        (input)
    * Easting       : Easting/X in meters                         (output)
    * Northing      : Northing/Y in meters                        (output)
    */
   ossim_int32 Convert_Geodetic_To_Transverse_Mercator (double Latitude,
                                                        double Longitude,
                                                        double *Easting,
                                                        double *Northing)const;
   
   
   /**
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
   ossim_int32 Convert_Transverse_Mercator_To_Geodetic(double Easting,
                                                       double Northing,
                                                       double *Latitude,
                                                       double *Longitude)const;
   
   double theTranMerc_a;  /* Semi-major axis of ellipsoid i meters */
   double theTranMerc_f;  /* Flattening of ellipsoid  */
   
   /* Eccentricity (0.08181919084262188000) squared */
   double theTranMerc_es;
   
   double theTranMerc_ebs;   /* Second Eccentricity squared */
   
   /* Transverse_Mercator projection Parameters */
   
   double theTranMerc_Origin_Lat;     /* Latitude of origin in radians */
   double theTranMerc_Origin_Long;    /* Longitude of origin in radians */
   double theTranMerc_False_Northing; /* False northing in meters */
   double theTranMerc_False_Easting;  /* False easting in meters */
   double theTranMerc_Scale_Factor;   /* Scale factor  */
   
   /* Isometeric to geodetic latitude parameters, default to WGS 84 */
   double theTranMerc_ap;
   double theTranMerc_bp;
   double theTranMerc_cp;
   double theTranMerc_dp;
   double theTranMerc_ep;
   
   /* Maximum variance for easting and northing values for WGS 84. */
   double theTranMerc_Delta_Easting;
   double theTranMerc_Delta_Northing;  
   
   /** 
    * zone can be from 1 through 60 (0 == NOT SET)
    */
   ossim_int32 theZone;
   
   /**
    * can be N or S.
    */
   char   theHemisphere;
   
TYPE_DATA
};

#endif

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimNewZealandMapGridProjection.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimNewZealandMapGridProjection_HEADER
#define ossimNewZealandMapGridProjection_HEADER

#include <ossim/projection/ossimMapProjection.h>


class OSSIMDLLEXPORT ossimNewZealandMapGridProjection : public ossimMapProjection
{
public:
   ossimNewZealandMapGridProjection();
   ~ossimNewZealandMapGridProjection();
   virtual ossimObject* dup()const
      {
         return new ossimNewZealandMapGridProjection(*this);
      }
   virtual void update();   

   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
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

   double getFalseEasting()const{return  NZMG_False_Easting;}
   double getFalseNorthing()const{return NZMG_False_Northing;}
   
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
protected:

   // these should be set to constant values that never change.  The origin
   // will be fixed and the false easting northings will also be fixed.
   //
   mutable double NZMG_Origin_Lat;             /* Latitude of origin, in radians */
   mutable double NZMG_Origin_Long;            /* Longitude of origin, in radians */
   mutable double NZMG_False_Northing;         /* False northing, in meters */
   mutable double NZMG_False_Easting;          /* False easting, in meters */
   mutable double NZMG_a;
/* Maximum variance for easting and northing values for International. */
   mutable double NZMG_Max_Easting;
   mutable double NZMG_Max_Northing;
   mutable double NZMG_Min_Easting;
   mutable double NZMG_Min_Northing;

/*!
 * The function Convert_Geodetic_To_NZMG converts geodetic (latitude and
 * longitude) coordinates to New Zealand Map Grid projection (easting and northing)
 * coordinates, according to the current ellipsoid and New Zealand Map Grid 
 * projection parameters.  If any errors occur, the error code(s) are returned 
 * by the function, otherwise NZMG_NO_ERROR is returned.
 *
 *    Latitude          : Latitude (phi), in radians           (input)
 *    Longitude         : Longitude (lambda), in radians       (input)
 *    Easting           : Easting (X), in meters               (output)
 *    Northing          : Northing (Y), in meters              (output)
 */
   long Convert_Geodetic_To_NZMG (double Latitude,
                                  double Longitude,
                                  double *Easting,
                                  double *Northing)const;



/*!
 * The function Convert_NZMG_To_Geodetic converts New Zealand Map Grid projection
 * (easting and northing) coordinates to geodetic (latitude and longitude)
 * coordinates, according to the current ellipsoid and New Zealand Map Grid projection
 * coordinates.  If any errors occur, the error code(s) are returned by the
 * function, otherwise NZMG_NO_ERROR is returned.
 *
 *    Easting           : Easting (X), in meters                  (input)
 *    Northing          : Northing (Y), in meters                 (input)
 *    Latitude          : Latitude (phi), in radians              (output)
 *    Longitude         : Longitude (lambda), in radians          (output)
 */
   long Convert_NZMG_To_Geodetic(double Easting,
                                 double Northing,
                                 double *Latitude,
                                 double *Longitude)const;
   
TYPE_DATA   
};
#endif

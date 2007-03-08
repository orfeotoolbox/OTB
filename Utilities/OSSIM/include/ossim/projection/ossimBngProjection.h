//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimBngProjection.h 9968 2006-11-29 14:01:53Z gpotts $
#include <ossim/projection/ossimMapProjection.h>

class OSSIMDLLEXPORT ossimBngProjection : public ossimMapProjection
{
public:
   ossimBngProjection();
      
   virtual ossimObject* dup()const
      {
         return new ossimBngProjection(*this);
      }

   virtual ossimGpt inverse(const ossimDpt &eastingNorthing)const;
   virtual ossimDpt forward(const ossimGpt &latLon)const;
   virtual void update();
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
   
/* Ellipsoid Parameters, must be Airy  */
   double BNG_a;               /* Semi-major axis of ellipsoid, in meters */
   double BNG_f;         /* Flattening of ellipsoid */

/* BNG projection Parameters */
   double BNG_Origin_Lat;  /* Latitude of origin, in radians */
   double BNG_Origin_Long; /* Longitude of origin, in radians */
   double BNG_False_Northing;        /* False northing, in meters */
   double BNG_False_Easting;          /* False easting, in meters */
   double BNG_Scale_Factor;        /* Scale factor                      */
   mutable long string_Broken;

   long Find_Index (char letter, 
                    const char* letter_Array, 
                    long *index)const;
   
   long Round_BNG (double value)const;

   long Make_BNG_String (char ltrnum[4],
                         long Easting, 
                         long Northing,
                         char* BNG,
                         long Precision)const;
   
   long Break_BNG_String (char* BNG,
                          char Letters[3],
                          double* Easting,
                          double* Northing,
                          long* Precision)const;
   
   long Check_Out_Of_Area(char BNG500,
                          char BNG100)const;
   
   long Set_BNG_Parameters(char *Ellipsoid_Code);
   void Get_BNG_Parameters(char *Ellipsoid_Code);
  
/*!
 * The function Convert_Geodetic_To_BNG converts geodetic (latitude and
 * longitude) coordinates to a BNG coordinate string, according to the 
 * current ellipsoid parameters.  If any errors occur, the error code(s)  
 * are returned by the function, otherwise BNG_NO_ERROR is returned.
 * 
 *    Latitude   : Latitude, in radians                    (input)
 *    Longitude  : Longitude, in radians                   (input)
 *    Precision  : Precision level of BNG string           (input)
 *    BNG        : British National Grid coordinate string (output)
 *  
 */
   long Convert_Geodetic_To_BNG (double Latitude,
                                 double Longitude,
                                 long Precision,
                                 char* BNG)const;
   


/*!
 * The function Convert_BNG_To_Geodetic converts a BNG coordinate string 
 * to geodetic (latitude and longitude) coordinates, according to the current
 * ellipsoid parameters. If any errors occur, the error code(s) are returned 
 * by the function, otherwise BNG_NO_ERROR is returned. 
 * 
 *    BNG        : British National Grid coordinate string (input)
 *    Latitude   : Latitude, in radians                    (output)
 *    Longitude  : Longitude, in radians                   (output)
 *  
 */
   long Convert_BNG_To_Geodetic (char *BNG, 
                                 double *Latitude, 
                                 double *Longitude)const;


/*!
 * The function Convert_Transverse_Mercator_To_BNG converts Transverse Mercator
 * (easting and northing) coordinates to a BNG coordinate string, according
 * to the current ellipsoid parameters.  If any errors occur, the error code(s)
 * are returned by the function, otherwise BNG_NO_ERROR is returned.
 *
 *    Easting    : Easting (X), in meters                  (input)
 *    Northing   : Northing (Y), in meters                 (input)
 *    Precision  : Precision level of BNG string           (input)
 *    BNG        : British National Grid coordinate string (output)
 */
   long Convert_Transverse_Mercator_To_BNG(double Easting,
                                           double Northing,            
                                           long Precision,
                                           char *BNG)const;



/*!
 * The function Convert_BNG_To_Transverse_Mercator converts a BNG coordinate string
 * to Transverse Mercator projection (easting and northing) coordinates 
 * according to the current ellipsoid parameters.  If any errors occur, 
 * the error code(s) are returned by the function, otherwise BNG_NO_ERROR 
 * is returned.
 *
 *    BNG        : British National Grid coordinate string (input)
 *    Easting    : Easting (X), in meters                  (output)
 *    Northing   : Northing (Y), in meters                 (output)
 */
   long Convert_BNG_To_Transverse_Mercator(char *BNG,
                                           double *Easting,
                                           double *Northing)const;

TYPE_DATA
};

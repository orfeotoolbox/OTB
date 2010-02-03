#ifndef ossimGeoref_HEADER
#define ossimGeoref_HEADER
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimGpt.h>

class OSSIMDLLEXPORT ossimGeoref
{
public:
   ossimGeoref(const ossimString &georefString,
               long precision);
   ossimGeoref(const ossimGpt &groundPt);
   ossimGeoref();

  /*!   
   *  This function converts Geodetic (latitude and longitude in radians)
   *  coordinates to a GEOREF coordinate string.  Precision specifies the
   *  number of digits in the GEOREF string for latitude and longitude:
   *                                  0 for nearest degree
   *                                  1 for nearest 10 minutes
   *                                  2 for nearest minute
   *                                  3 for nearest tenth of a minute
   *                                  4 for nearest hundredth of a minute
   *                                  5 for nearest thousandth of a minute
   *
   *    Precision : level of precision specified by the user  (input)
   *    ossimGeoref    : GEOREF coordinate string                  (return)
   */    
   ossimString toString(long precision);
   
private:
   ossimGpt thePt;

   // This code belongs to Geotrans
   
  /*!   
   *  This function converts Geodetic (latitude and longitude in radians)
   *  coordinates to a GEOREF coordinate string.  Precision specifies the
   *  number of digits in the GEOREF string for latitude and longitude:
   *                                  0 for nearest degree
   *                                  1 for nearest 10 minutes
   *                                  2 for nearest minute
   *                                  3 for nearest tenth of a minute
   *                                  4 for nearest hundredth of a minute
   *                                  5 for nearest thousandth of a minute
   *
   *    Latitude  : Latitude in radians                       (input)
   *    Longitude : Longitude in radians                      (input)
   *    Precision : level of precision specified by the user  (input)
   *    ossimGeoref    : GEOREF coordinate string                  (output)
   */
  long Convert_Geodetic_To_GEOREF (double Latitude,
                                   double Longitude,
                                   long Precision,
                                   char *georef);


  /*!
   *  This function converts a GEOREF coordinate string to Geodetic (latitude
   *  and longitude in radians) coordinates.
   *
   *    ossimGeoref    : GEOREF coordinate string     (input)
   *    Latitude  : Latitude in radians          (output)
   *    Longitude : Longitude in radians         (output)
   */   
  long Convert_GEOREF_To_Geodetic (char *georef,
                                   double *Latitude, 
                                   double *Longitude);
};

#endif

#ifndef DATUM_H
#define DATUM_H

/***************************************************************************/
/* RSC IDENTIFIER: Datum
 *
 * ABSTRACT
 *
 *    This component provides datum shifts for a large collection of local
 *    datums, WGS72, and WGS84.  A particular datum can be accessed by using its 
 *    standard 5-letter code to find its index in the datum table.  The index 
 *    can then be used to retrieve the name, type, ellipsoid code, and datum 
 *    shift parameters, and to perform shifts to or from that datum.
 *    
 *    By sequentially retrieving all of the datum codes and/or names, a menu
 *    of the available datums can be constructed.  The index values resulting
 *    from selections from this menu can then be used to access the parameters
 *    of the selected datum, or to perform datum shifts involving that datum.
 *
 *    This component supports both 3-parameter local datums, for which only X,
 *    Y, and Z translations relative to WGS 84 have been defined, and 
 *    7-parameter local datums, for which X, Y, and Z rotations, and a scale 
 *    factor, are also defined.  It also includes entries for WGS 84 (with an
 *    index of 0), and WGS 72 (with an index of 1), but no shift parameter 
 *    values are defined for these.
 *
 *    This component provides datum shift functions for both geocentric and
 *    geodetic coordinates.  WGS84 is used as an intermediate state when
 *    shifting from one local datum to another.  When geodetic coordinates are
 *    given Molodensky's method is used, except near the poles where the 3-step
 *    step method is used instead.  Specific algorithms are used for shifting 
 *    between WGS72 and WGS84.
 *
 *    This component depends on two data files, named 3_param.dat and 
 *    7_param.dat, which contain the datum parameter values.  Copies of these
 *    files must be located in the directory specified by the value of the 
 *    environment variable "DATUM_DATA", if defined, or else in the current 
 *    directory whenever a program containing this component is executed. 
 *
 *    Additional datums can be added to these files, either manually or using 
 *    the Create_Datum function.  However, if a large number of datums are 
 *    added, the datum table array sizes in this component will have to be 
 *    increased.
 *
 *    This component depends on two other components: the Ellipsoid component
 *    for access to ellipsoid parameters; and the Geocentric component for 
 *    conversions between geodetic and geocentric coordinates.
 *
 * ERROR HANDLING
 *
 *    This component checks for input file errors and input parameter errors.
 *    If an invalid value is found, the error code is combined with the current
 *    error code using the bitwise or.  This combining allows multiple error
 *    codes to be returned. The possible error codes are:
 *
 *  DATUM_NO_ERROR                  : No errors occurred in function
 *  DATUM_NOT_INITIALIZED_ERROR     : Datum module has not been initialized
 *  DATUM_7PARAM_FILE_OPEN_ERROR    : 7 parameter file opening error
 *  DATUM_7PARAM_FILE_PARSING_ERROR : 7 parameter file structure error
 *  DATUM_7PARAM_OVERFLOW_ERROR     : 7 parameter table overflow
 *  DATUM_3PARAM_FILE_OPEN_ERROR    : 3 parameter file opening error
 *  DATUM_3PARAM_FILE_PARSING_ERROR : 3 parameter file structure error
 *  DATUM_3PARAM_OVERFLOW_ERROR     : 3 parameter table overflow
 *  DATUM_INVALID_INDEX_ERROR       : Index out of valid range (less than one
 *                                      or more than Datum_Count)
 *  DATUM_INVALID_SRC_INDEX_ERROR   : Source datum index invalid
 *  DATUM_INVALID_DEST_INDEX_ERROR  : Destination datum index invalid
 *  DATUM_INVALID_CODE_ERROR        : Datum code not found in table
 *  DATUM_LAT_ERROR                 : Latitude out of valid range (-90 to 90)
 *  DATUM_LON_ERROR                 : Longitude out of valid range (-180 to
 *                                    360)
 *  DATUM_SIGMA_ERROR               : Standard error values must be positive
 *                                    (or -1 if unknown)
 *  DATUM_DOMAIN_ERROR              : Domain of validity not well defined
 *  DATUM_ELLIPSE_ERROR             : Error in ellipsoid module
 *
 *
 * REUSE NOTES
 *
 *    Datum is intended for reuse by any application that needs access to 
 *    datum shift parameters relative to WGS 84.
 *
 *    
 * REFERENCES
 *
 *    Further information on Datum can be found in the Reuse Manual.
 *
 *    Datum originated from :  U.S. Army Topographic Engineering Center (USATEC)
 *                             Geospatial Information Division (GID)
 *                             7701 Telegraph Road
 *                             Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *    Datum has no restrictions.
 *
 * ENVIRONMENT
 *
 *    Datum was tested and certified in the following environments:
 *
 *    1. Solaris 2.5 with GCC 2.8.1
 *    2. MS Windows 95 with MS Visual C++ 6
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *    03/30/97          Original Code
 *    05/28/99          Added user-definable datums (for JMTK)
 *                      Added datum domain of validity checking (for JMTK)
 *                      Added datum shift accuracy calculation (for JMTK) 
 */


/***************************************************************************/
/*
 *                              DEFINES
 */

#define DATUM_NO_ERROR                          0x0000
#define DATUM_NOT_INITIALIZED_ERROR             0x0001
#define DATUM_7PARAM_FILE_OPEN_ERROR            0x0002
#define DATUM_7PARAM_FILE_PARSING_ERROR         0x0004
#define DATUM_7PARAM_OVERFLOW_ERROR             0x0008
#define DATUM_3PARAM_FILE_OPEN_ERROR            0x0010
#define DATUM_3PARAM_FILE_PARSING_ERROR         0x0020
#define DATUM_3PARAM_OVERFLOW_ERROR             0x0040
#define DATUM_INVALID_INDEX_ERROR               0x0080
#define DATUM_INVALID_SRC_INDEX_ERROR           0x0100
#define DATUM_INVALID_DEST_INDEX_ERROR          0x0200
#define DATUM_INVALID_CODE_ERROR                0x0400
#define DATUM_LAT_ERROR                         0x0800
#define DATUM_LON_ERROR                         0x1000
#define DATUM_SIGMA_ERROR                       0x2000
#define DATUM_DOMAIN_ERROR                      0x4000
#define DATUM_ELLIPSE_ERROR                     0x8000


/***************************************************************************/
/*
 *                          GLOBAL DECLARATIONS
 */
typedef enum Datum_Types
{
  Three_Param_Datum,
  Seven_Param_Datum,
  WGS84_Datum,
  WGS72_Datum
} Datum_Type; /* different types of datums */


/***************************************************************************/
/*
 *                            FUNCTION PROTOTYPES
 */

/* ensure proper linkage to c++ programs */
#ifdef __cplusplus 
extern "C" {
#endif

  long Initialize_Datums(void);
/*
 * The function Initialize_Datums creates the datum table from two external
 * files.  If an error occurs, the initialization stops and an error code is
 * returned.  This function must be called before any of the other functions
 * in this component.
 */


  long Create_Datum ( const char *Code,
                      const char *Name,
                      const char *Ellipsoid_Code,
                      double Delta_X,
                      double Delta_Y,
                      double Delta_Z,
                      double Sigma_X,
                      double Sigma_Y,
                      double Sigma_Z,
                      double South_latitude,
                      double North_latitude,
                      double West_longitude,
                      double East_longitude);
/*
 *   Code           : 5-letter new datum code.                      (input)
 *   Name           : Name of the new datum                         (input)
 *   Ellipsoid_Code : 2-letter code for the associated ellipsoid    (input)
 *   Delta_X        : X translation to WGS84 in meters              (input)
 *   Delta_Y        : Y translation to WGS84 in meters              (input)
 *   Delta_Z        : Z translation to WGS84 in meters              (input)
 *   Sigma_X        : Standard error in X in meters                 (input)
 *   Sigma_Y        : Standard error in Y in meters                 (input)
 *   Sigma_Z        : Standard error in Z in meters                 (input)
 *   South_latitude : Southern edge of validity rectangle in radians(input)
 *   North_latitude : Northern edge of validity rectangle in radians(input)
 *   West_longitude : Western edge of validity rectangle in radians (input)
 *   East_longitude : Eastern edge of validity rectangle in radians (input)
 *
 * The function Create_Datum creates a new local (3-parameter) datum with the 
 * specified code, name, shift values, and standard error values.  If the 
 * datum table has not been initialized, the specified code is already in use, 
 * or a new version of the 3-param.dat file cannot be created, an error code 
 * is returned, otherwise DATUM_NO_ERROR is returned.  Note that the indexes 
 * of all datums in the datum table may be changed by this function.
 */


  long Datum_Count ( long *Count );
/*
 *  The function Datum_Count returns the number of Datums in the table
 *  if the table was initialized without error.
 *
 *  Count   : number of datums in the datum table                   (output)
 */


  long Datum_Index ( const char *Code, 
                     long *Index );
/*
 *  The function Datum_Index returns the index of the datum with the 
 *  specified code.
 *
 *  Code    : The datum code being searched for                     (input)
 *  Index   : The index of the datum in the table with the          (output)
 *              specified code
 */


  long Datum_Code ( const long Index,
                    char *Code );
/*
 *  The function Datum_Code returns the 5-letter code of the datum
 *  referenced by index.
 *
 *  Index   : The index of a given datum in the datum table         (input)
 *  Code    : The datum code of the datum referenced by index       (output)
 */


  long Datum_Name ( const long Index,
                    char *Name );
/*
 *  The function Datum_Name returns the name of the datum referenced by
 *  index.
 *
 *  Index   : The index of a given datum in the datum table         (input)
 *  Name    : The datum name of the datum referenced by index       (output)
 */


  long Datum_Ellipsoid_Code ( const long Index,
                              char *Code );
/*
 *  The function Datum_Ellipsoid_Code returns the 2-letter ellipsoid code 
 *  for the ellipsoid associated with the datum referenced by index.
 *
 *  Index   : The index of a given datum in the datum table           (input)
 *  Code    : The ellisoid code for the ellipsoid associated with the (output)
 *               datum referenced by index 
 */


  long Get_Datum_Type ( const long Index,
                        Datum_Type *Type );
/*
 *  The function Get_Datum_Type returns the type of the datum referenced by
 *  index.
 *
 *  Index   : The index of a given datum in the datum table         (input)
 *  Type    : The type of the datum referenced by index             (output)
 */


  long Datum_Seven_Parameters ( const long Index, 
                                double *Delta_X,
                                double *Delta_Y,
                                double *Delta_Z,
                                double *Rx,
                                double *Ry,
                                double *Rz,
                                double *Scale_Factor );
/*
 *   The function Datum_Seven_Parameters returns the seven parameters
 *   for the datum referenced by index.
 *
 *    Index      : The index of a given datum in the datum table   (input)
 *    Delta_X    : X translation in meters                         (output)
 *    Delta_Y    : Y translation in meters                         (output)
 *    Delta_Z    : Z translation in meters                         (output)
 *    Rx         : X rotation in radians                           (output)
 *    Rx         : Y rotation in radians                           (output)
 *    Ry         : Z rotation in radians                           (output)
 *    Scale_Factor : Scale factor                                  (output)
 */


  long Datum_Three_Parameters ( const long Index, 
                                double *Delta_X,
                                double *Delta_Y,
                                double *Delta_Z);
/*
 *   The function Datum_Three_Parameters returns the three parameters 
 *   for the datum referenced by index.
 *
 *    Index      : The index of a given datum in the datum table   (input)
 *    Delta_X    : X translation in meters                         (output)
 *    Delta_Y    : Y translation in meters                         (output)
 *    Delta_Z    : Z translation in meters                         (output)
 */


  long Datum_Errors ( const long Index,
                      double *Sigma_X,
                      double *Sigma_Y,
                      double *Sigma_Z);
/*
 *   The function Datum_Errors returns the standard errors in X,Y, & Z 
 *   for the datum referenced by index.
 *
 *    Index      : The index of a given datum in the datum table   (input)
 *    Sigma_X    : Standard error in X in meters                   (output)
 *    Sigma_Y    : Standard error in Y in meters                   (output)
 *    Sigma_Z    : Standard error in Z in meters                   (output)
 */


  long Datum_Valid_Rectangle ( const long Index,
                               double *South_latitude,
                               double *North_latitude,
                               double *West_longitude,
                               double *East_longitude);
/*
 *   The function Datum_Valid_Rectangle returns the edges of the validity 
 *   rectangle for the datum referenced by index.
 *
 *   Index          : The index of a given datum in the datum table   (input)
 *   South_latitude : Southern edge of validity rectangle in radians  (input)
 *   North_latitude : Northern edge of validity rectangle in radians  (input)
 *   West_longitude : Western edge of validity rectangle in radians   (input)
 *   East_longitude : Eastern edge of validity rectangle in radians   (input)
 */


  long Valid_Datum ( const long Index,
                     double latitude,
                     double longitude,
                     long *result);
/*
 *  This function checks whether or not the specified location is within the 
 *  validity rectangle for the specified datum.  It returns zero if the specified
 *  location is NOT within the validity rectangle, and returns 1 otherwise.
 *
 *   Index     : The index of a given datum in the datum table      (input)
 *   latitude  : Latitude of the location to be checked in radians  (input)
 *   longitude : Longitude of the location to be checked in radians (input)
 *   result    : Indicates whether location is inside (1) or outside (0)
 *               of the validity rectangle of the specified datum   (output)
 */


  long Geocentric_Shift_To_WGS84 (const long Index,
                                  const double X,
                                  const double Y,
                                  const double Z,
                                  double *X_WGS84,
                                  double *Y_WGS84,
                                  double *Z_WGS84);
/*
 *  This function shifts a geocentric coordinate (X, Y, Z in meters) relative
 *  to the datum referenced by index to a geocentric coordinate (X, Y, Z in
 *  meters) relative to WGS84.
 *
 *  Index   : Index of local datum                       (input)
 *  X       : X coordinate relative to the source datum  (input)
 *  Y       : Y coordinate relative to the source datum  (input)
 *  Z       : Z coordinate relative to the source datum  (input)
 *  X_WGS84 : X coordinate relative to WGS84             (output)
 *  Y_WGS84 : Y coordinate relative to WGS84             (output)
 *  Z_WGS84 : Z coordinate relative to WGS84             (output)
 */


  long Geocentric_Shift_From_WGS84 (const double X_WGS84,
                                    const double Y_WGS84,
                                    const double Z_WGS84,
                                    const long Index,
                                    double *X,
                                    double *Y,
                                    double *Z);
/*
 *  This function shifts a geocentric coordinate (X, Y, Z in meters) relative
 *  to WGS84 to a geocentric coordinate (X, Y, Z in meters) relative to the
 *  local datum referenced by index.
 *
 *  X_WGS84 : X coordinate relative to WGS84                 (input)
 *  Y_WGS84 : Y coordinate relative to WGS84                 (input)
 *  Z_WGS84 : Z coordinate relative to WGS84                 (input)
 *  Index   : Index of destination datum                     (input)
 *  X       : X coordinate relative to the destination datum (output)
 *  Y       : Y coordinate relative to the destination datum (output)
 *  Z       : Z coordinate relative to the destination datum (output)
 */


  long Geocentric_Datum_Shift ( const long Index_in,
                                const double X_in,
                                const double Y_in,
                                const double Z_in,
                                const long Index_out,
                                double *X_out,
                                double *Y_out,
                                double *Z_out);
/*
 *  This function shifts a geocentric coordinate (X, Y, Z in meters) relative
 *  to the source datum to geocentric coordinate (X, Y, Z in meters) relative
 *  to the destination datum.
 *
 *  Index_in  : Index of source datum                      (input)
 *  X_in      : X coordinate relative to source datum      (input)
 *  Y_in      : Y coordinate relative to source datum      (input)
 *  Z_in      : Z coordinate relative to source datum      (input)
 *  Index_out : Index of destination datum                 (input)
 *  X_out     : X coordinate relative to destination datum (output)
 *  Y_out     : Y coordinate relative to destination datum (output)
 *  Z_out     : Z coordinate relative to destination datum (output)
 */


  long Geodetic_Shift_To_WGS84 (const long Index,
                                const double Lat_in,
                                const double Lon_in,
                                const double Hgt_in,
                                double *WGS84_Lat,
                                double *WGS84_Lon,
                                double *WGS84_Hgt);
/*
 *  This function shifts geodetic coordinates relative to a given source datum
 *  to geodetic coordinates relative to WGS84.
 *
 *  Index     : Index of source datum                                 (input)
 *  Lat_in    : Latitude in radians relative to source datum          (input)
 *  Lon_in    : Longitude in radians relative to source datum         (input)
 *  Hgt_in    : Height in meters relative to source datum's ellipsoid (input)
 *  WGS84_Lat : Latitude in radians relative to WGS84                 (output)
 *  WGS84_Lon : Longitude in radians relative to WGS84                (output)
 *  WGS84_Hgt : Height in meters relative to WGS84 ellipsoid          (output)
 */


  long Geodetic_Shift_From_WGS84( const double WGS84_Lat,
                                  const double WGS84_Lon,
                                  const double WGS84_Hgt,
                                  const long Index,
                                  double *Lat_out,
                                  double *Lon_out,
                                  double *Hgt_out);
/*
 *  This function shifts geodetic coordinates relative to a WGS84 
 *  to geodetic coordinates relative to a given local datum.
 *                                                                   
 *  WGS84_Lat : Latitude in radians relative to WGS84                      (input)
 *  WGS84_Lon : Longitude in radians relative to WGS84                     (input)
 *  WGS84_Hgt : Height in meters relative to WGS84 ellipsoid               (input)
 *  Index     : Index of destination datum                                 (input)
 *  Lat_in    : Latitude in radians relative to destination datum          (output)
 *  Lon_in    : Longitude in radians relative to destination datum         (output)
 *  Hgt_in    : Height in meters relative to destination datum's ellipsoid (output)
 */


  long Geodetic_Datum_Shift ( const long Index_in,
                              const double Lat_in,
                              const double Lon_in,
                              const double Hgt_in,
                              const long Index_out,
                              double *Lat_out,
                              double *Lon_out,
                              double *Hgt_out);
/*
 *  This function shifts geodetic coordinates (latitude, longitude in radians
 *  and height in meters) relative to the source datum to geodetic coordinates
 *  (latitude, longitude in radians and height in meters) relative to the
 *  destination datum.
 *
 *  Index_in  : Index of source datum                                      (input)
 *  Lat_in    : Latitude in radians relative to source datum               (input)
 *  Lon_in    : Longitude in radians relative to source datum              (input)
 *  Hgt_in    : Height in meters relative to source datum's ellipsoid      (input)
 *  Index_out : Index of destination datum                                 (input)
 *  Lat_out   : Latitude in radians relative to destination datum          (output)
 *  Lon_out   : Longitude in radians relative to destination datum         (output)
 *  Hgt_out   : Height in meters relative to destination datum's ellipsoid (output)
 */


  long Datum_Shift_Error (const long Index_in,
                          const long Index_out,
                          double latitude,
                          double longitude,
                          double *ce90,
                          double *le90,
                          double *se90);
/*
 *  This function returns the 90% horizontal (circular), vertical (linear), and 
 *  spherical errors for a shift from the specified source datum to the 
 *  specified destination datum at the specified location.
 *
 *  Index_in  : Index of source datum                                      (input)
 *  Index_out : Index of destination datum                                 (input)
 *  latitude  : Latitude of point being converted in radians               (input)
 *  longitude : Longitude of point being converted in radians              (input)
 *  ce90      : Combined 90% circular horizontal error in meters           (output)
 *  le90      : Combined 90% linear vertical error in meters               (output)
 *  se90      : Combined 90% spherical error in meters                     (output)
 */


#ifdef __cplusplus 
}
#endif

#endif /* DATUM_H */



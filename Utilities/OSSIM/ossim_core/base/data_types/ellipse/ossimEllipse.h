#ifndef ELLIPSE_H
#define ELLIPSE_H

/****************************************************************************/
/* RSC IDENTIFIER:  Ellipsoid
 *
 * ABSTRACT
 *
 *    The purpose of ELLIPSOID is to provide access to ellipsoid parameters 
 *    for a collection of common ellipsoids.  A particular ellipsoid can be 
 *    accessed by using its standard 2-letter code to find its index in the 
 *    ellipsoid table.  The index can then be used to retrieve the ellipsoid 
 *    name and parameters.
 *
 *    By sequentially retrieving all of the ellipsoid codes and/or names, a 
 *    menu of the available ellipsoids can be constructed.  The index values 
 *    resulting from selections from this menu can then be used to access the 
 *    parameters of the selected ellipsoid.
 *
 *    This component depends on a data file named "ellips.dat", which contains
 *    the ellipsoid parameter values.  A copy of this file must be located in 
 *    the directory specified by the environment variable "ELLIPSOID_DATA", if 
 *    defined, or else in the current directory, whenever a program containing 
 *    this component is executed.
 *
 *    Additional ellipsoids can be added to this file, either manually or using 
 *    the Create_Ellipsoid function.  However, if a large number of ellipsoids 
 *    are added, the ellipsoid table array size in this component will have to 
 *    be increased.
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *  ELLIPSE_NO_ERROR             : No errors occured in function
 *  ELLIPSE_FILE_OPEN_ERROR      : Ellipsoid file opening error
 *  ELLIPSE_INITIALIZE_ERROR     : Ellipsoid database can not initialize
 *  ELLIPSE_TABLE_OVERFLOW_ERROR : Ellipsoid table overflow
 *  ELLIPSE_NOT_INITIALIZED_ERROR: Ellipsoid database not initialized properly
 *  ELLIPSE_INVALID_INDEX_ERROR  : Index is an invalid value
 *  ELLIPSE_INVALID_CODE_ERROR   : Code was not found in database
 *  ELLIPSE_A_ERROR              : Semi-major axis less than or equal to zero
 *  ELLIPSE_B_ERROR              : Semi-minor axis less than or equal to zero
 *  ELLIPSE_A_LESS_B_ERROR       : Semi-major axis less than semi-minor axis
 *
 * REUSE NOTES
 *
 *    Ellipsoid is intended for reuse by any application that requires Earth
 *    approximating ellipsoids.
 *     
 * REFERENCES
 *
 *    Further information on Ellipsoid can be found in the Reuse Manual.
 *
 *    Ellipsoid originated from :  U.S. Army Topographic Engineering Center (USATEC)
 *                                 Geospatial Information Division (GID)
 *                                 7701 Telegraph Road
 *                                 Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *    Ellipsoid has no restrictions.
 *
 * ENVIRONMENT
 *
 *    Ellipsoid was tested and certified in the following environments
 *
 *    1. Solaris 2.5
 *    2. Windows 95 
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *    11-19-95          Original Code
 *    17-Jan-97         Moved local constants out of public interface
 *                      Improved efficiency in algorithms (GEOTRANS)
 *    24-May-99         Added user-defined ellipsoids (GEOTRANS for JMTK)
 *
 */


/***************************************************************************/
/*
 *                               GLOBALS
 */

#define ELLIPSE_NO_ERROR              0x0000
#define ELLIPSE_FILE_OPEN_ERROR       0x0001
#define ELLIPSE_INITIALIZE_ERROR      0x0002
#define ELLIPSE_TABLE_OVERFLOW_ERROR  0x0004
#define ELLIPSE_NOT_INITIALIZED_ERROR 0x0008
#define ELLIPSE_INVALID_INDEX_ERROR   0x0010
#define ELLIPSE_INVALID_CODE_ERROR    0x0020
#define ELLIPSE_A_ERROR               0x0040
#define ELLIPSE_B_ERROR               0x0080
#define ELLIPSE_A_LESS_B_ERROR        0x0100

/***************************************************************************/
/*
 *                          FUNCTION PROTOTYPES
 *                             for ellipse.c
 */

/* ensure proper linkage to c++ programs */
#ifdef __cplusplus 
extern "C" {
#endif

  long Initialize_Ellipsoids ();
/*
 * The function Initialize_Ellipsoids reads ellipsoid data from ellips.dat in
 * the current directory and builds the ellipsoid table from it.  If an error
 * occurs, the error code is returned, otherwise ELLIPSE_NO_ERROR is returned.
 */


  long Create_Ellipsoid (const char* Code,
                         const char* Name,
                         double a,
                         double b);
/*
 *   Code     : 2-letter ellipsoid code.                      (input)
 *   Name     : Name of the new ellipsoid                     (input)
 *   a        : Semi-major axis, in meters, of new ellipsoid  (input)
 *   b        : Semi-minor axis, in meters, of new ellipsoid. (input)
 *
 * The function Create_Ellipsoid creates a new ellipsoid with the specified
 * code, name, and axes.  If the ellipsoid table has not been initialized,
 * the specified code is already in use, or a new version of the ellips.dat 
 * file cannot be created, an error code is returned, otherwise ELLIPSE_NO_ERROR 
 * is returned.  Note that the indexes of all ellipsoids in the ellipsoid
 * table may be changed by this function.
 */


  long Ellipsoid_Count ( long *Count );
/*
 *   Count    : The number of ellipsoids in the ellipsoid table. (output)
 *
 * The function Ellipsoid_Count returns the number of ellipsoids in the
 * ellipsoid table.  If the ellipsoid table has been initialized without error,
 * ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR
 * is returned.
 */


  long Ellipsoid_Index ( const char *Code,
                         long *Index );
/*
 *    Code     : 2-letter ellipsoid code.                      (input)
 *    Index    : Index of the ellipsoid in the ellipsoid table with the 
 *                  specified code                             (output)
 *
 *  The function Ellipsoid_Index returns the index of the ellipsoid in 
 *  the ellipsoid table with the specified code.  If ellipsoid_Code is found, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_CODE_ERROR is 
 *  returned.
 */


  long Ellipsoid_Name ( const long Index, 
                        char *Name );
/*
 *    Index   : Index of a given ellipsoid.in the ellipsoid table with the
 *                 specified index                             (input)
 *    Name    : Name of the ellipsoid referencd by index       (output)
 *
 *  The Function Ellipsoid_Name returns the name of the ellipsoid in 
 *  the ellipsoid table with the specified index.  If index is valid, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR is
 *  returned.
 */


  long Ellipsoid_Code ( const long Index,
                        char *Code );
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    Code     : 2-letter ellipsoid code.                          (output)
 *
 *  The Function Ellipsoid_Code returns the 2-letter code for the 
 *  ellipsoid in the ellipsoid table with the specified index.  If index is 
 *  valid, ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR 
 *  is returned.
 */


  long Ellipsoid_Axes ( const long Index,
                        double *a,
                        double *b );
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    a        : Semi-major axis, in meters, of ellipsoid          (output)
 *    b        : Semi-minor axis, in meters, of ellipsoid.         (output)
 *
 *  The function Ellipsoid_Axes returns the semi-major and semi-minor axes
 *  for the ellipsoid with the specified index.  If index is valid,
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR is 
 *  returned.
 */


  long Ellipsoid_Eccentricity2 ( const long Index,
                                 double *e2 );
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    e2       : Square of eccentricity of ellipsoid               (output)
 *
 *  The function Ellipsoid_Eccentricity2 returns the square of the 
 *  eccentricity for the ellipsoid with the specified index.  If index is 
 *  valid, ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR 
 *  is returned.
 */


  long Ellipsoid_Flattening ( const long Index,
                              double *f );
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    f        : Flattening of ellipsoid.                          (output)
 *
 *  The function Ellipsoid_Flattening returns the flattening of the 
 *  ellipsoid with the specified index.  If index is valid ELLIPSE_NO_ERROR is 
 *  returned, otherwise ELLIPSE_INVALID_INDEX_ERROR is returned.
 */


  long WGS84_Axes ( double *a,
                    double *b );
/*
 *    a      : Semi-major axis, in meters, of ellipsoid       (output)
 *    b      : Semi-minor axis, in meters, of ellipsoid       (output)
 *
 *  The function WGS84_Axes returns the lengths of the semi-major and 
 *  semi-minor axes for the WGS84 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */


  long WGS84_Eccentricity2 ( double *e2 );
/*
 *    e2    : Square of eccentricity of WGS84 ellipsoid      (output)
 *
 *  The function WGS84_Eccentricity2 returns the square of the 
 *  eccentricity for the WGS84 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */


  long WGS84_Flattening ( double *f );
/*
 *  f       : Flattening of WGS84 ellipsoid.                 (output)
 *
 *  The function WGS84_Flattening returns the flattening of the WGS84
 *  ellipsoid.  If the ellipsoid table was initialized successfully, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR 
 *  is returned.
 */

  long WGS72_Axes( double *a,
                   double *b);
/*
 *    a    : Semi-major axis, in meters, of ellipsoid        (output)
 *    b    : Semi-minor axis, in meters, of ellipsoid        (output)
 *
 *  The function WGS72_Axes returns the lengths of the semi-major and 
 *  semi-minor axes for the WGS72 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long WGS72_Eccentricity2 ( double *e2 );
/*
 *    e2     : Square of eccentricity of WGS84 ellipsoid     (output)
 *
 *  The function WGS72_Eccentricity2 returns the square of the 
 *  eccentricity for the WGS72 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long WGS72_Flattening (double *f );
/*
 *    f      : Flattening of WGS72 ellipsoid.                (output)
 *
 *  The function WGS72_Flattening returns the flattening of the WGS72
 *  ellipsoid .  If the ellipsoid table was initialized successfully, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR 
 *  is returned.
 */

#ifdef __cplusplus 
}
#endif

#endif /* ELLIPSE_H */



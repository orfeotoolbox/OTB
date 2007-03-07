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
 *  ELLIPSE_INITIALIZE_ERROR     : Ellipsoid table can not initialize
 *  ELLIPSE_TABLE_OVERFLOW_ERROR : Ellipsoid table overflow
 *  ELLIPSE_NOT_INITIALIZED_ERROR: Ellipsoid table not initialized properly
 *  ELLIPSE_INVALID_INDEX_ERROR  : Index is an invalid value
 *  ELLIPSE_INVALID_CODE_ERROR   : Code was not found in table
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
 *                               INCLUDES
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ossimEllipse.h"

/* 
 *    ctype.h    - standard C character handling library
 *    stdio.h    - standard C input/output library
 *    stdlib.h   - standard C general utilities library
 *    string.h   - standard C string handling library
 *    ellipse.h  - prototype error checking and error codes
 */


/***************************************************************************/
/*
 *                             GLOBAL DATA STRUCTURES
 */

#define MAX_ELLIPSOIDS        32  /* Maximum number of ellipsoids in table */
#define ELLIPSOID_CODE_LENGTH  3  /* Length of ellipsoid code (including null) */
#define ELLIPSOID_NAME_LENGTH 30  /* Max length of ellipsoid name */
#define ELLIPSOID_BUF         90
#define FILENAME_LENGTH      128

const char *WGS84_Ellipsoid_Code = "WE";
const char *WGS72_Ellipsoid_Code = "WD";

typedef struct Ellipsoid_Table_Row
{
  char Name[ELLIPSOID_NAME_LENGTH];
  char Code[ELLIPSOID_CODE_LENGTH];
  double A;
  double B;
  double Recp_F;
} Ellipsoid_Row;

#include "ossimEllipse.hpp" 

static long WGS84_Index = 0;           /* Index of WGS84 in ellipsoid table */
static long WGS72_Index = 0;           /* Index of WGS72 in ellipsoid table */
static long Ellipsoid_Initialized = 0; /* Indicates successful initialization */

/***************************************************************************/
/*                              FUNCTIONS                                  */


void Assign_Ellipsoid_Row (Ellipsoid_Row *destination, 
                           const Ellipsoid_Row *source)
{
   /* Begin Assign_Ellipsoid_Row */
   /*
    *   destination  : The destination of the copy         (output)
    *   source       : The source for the copy             (input)
    *
    * The function Assign_Ellipsoid_Row copies ellipsoid data.
    */

  strcpy(destination->Name, source->Name);
  strcpy(destination->Code, source->Code);
  destination->A = source->A;
  destination->B = source->B;
  destination->Recp_F = source->Recp_F;
} /* End Assign_Ellipsoid_Row */


long Initialize_Ellipsoids () 
{
   /* Begin Initialize_Ellipsoids */
   /*
    * The function Initialize_Ellipsoids reads ellipsoid data from ellips.dat
    * in the current directory and builds the ellipsoid table from it.  If an 
    * error occurs, the error code is returned, otherwise ELLIPSE_NO_ERROR is 
    * returned.
    */
  long error_code = ELLIPSE_NO_ERROR;

  if (error_code)
  {
    Ellipsoid_Initialized = 0;
    Number_of_Ellipsoids = 0;
  }
  else
    Ellipsoid_Initialized = 1;

  /* Store WGS84 Index*/
  if (Ellipsoid_Index(WGS84_Ellipsoid_Code, &WGS84_Index))
    error_code |= ELLIPSE_INITIALIZE_ERROR;

  /* Store WGS72 Index*/
  if (Ellipsoid_Index(WGS72_Ellipsoid_Code, &WGS72_Index))
    error_code |= ELLIPSE_INITIALIZE_ERROR;

  return (error_code);
} /* End of Initialize_Ellipsoids */


long Create_Ellipsoid (const char* Code,
                       const char* Name,
                       double A,
                       double B)
{ /* Begin Create_Ellipsoid */
/*
 *   Code     : 2-letter ellipsoid code.                      (input)
 *   Name     : Name of the new ellipsoid                     (input)
 *   A        : Semi-major axis, in meters, of new ellipsoid  (input)
 *   B        : Semi-minor axis, in meters, of new ellipsoid. (input)
 *
 * The function Create_Ellipsoid creates a new ellipsoid with the specified
 * Code, name, and axes.  If the ellipsoid table has not been initialized,
 * the specified code is already in use, or a new version of the ellips.dat 
 * file cannot be created, an error code is returned, otherwise ELLIPSE_NO_ERROR 
 * is returned.  Note that the indexes of all ellipsoids in the ellipsoid
 * table may be changed by this function.
 */

  long error_code = ELLIPSE_NO_ERROR;
  long index = 0;
  char *PathName;
  char FileName[FILENAME_LENGTH];
  FILE *fp = NULL;                    /* File pointer to file ellips.dat     */

  if (!Ellipsoid_Initialized)
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  else if (!(Number_of_Ellipsoids < MAX_ELLIPSOIDS))
    error_code |= ELLIPSE_TABLE_OVERFLOW_ERROR;
  else
  {
    if (!Ellipsoid_Index(Code, &index))
      error_code |= ELLIPSE_INVALID_CODE_ERROR;
    if (A <= 0.0)
      error_code |= ELLIPSE_A_ERROR;
    if (B <= 0.0)
      error_code |= ELLIPSE_B_ERROR;
    if (A < B)
      error_code |= ELLIPSE_A_LESS_B_ERROR;
    if (!error_code)
    {
      index = Number_of_Ellipsoids;
      strcpy(Ellipsoid_Table[index].Name, Name);
      strcpy(Ellipsoid_Table[index].Code, Code);
      Ellipsoid_Table[index].A = A;
      Ellipsoid_Table[index].B = B;
      Ellipsoid_Table[index].Recp_F = A / (A - B);
      Number_of_Ellipsoids++;

      /*output updated ellipsoid table*/
      PathName = getenv( "ELLIPSOID_DATA" );
      if (PathName != NULL)
      {
        strcpy( FileName, PathName );
        strcat( FileName, "/" );
      }
      else
      {
        strcpy( FileName, "./" );
      }
      strcat( FileName, "ellips.dat" );

      if ((fp = fopen(FileName, "w")) == NULL)
      { /* fatal error */
        return ELLIPSE_FILE_OPEN_ERROR;
      }

      /* write file */
      index = 0;
      while (index < Number_of_Ellipsoids)
      {
        fprintf(fp, "%-29s %-2s %11.3f %12.4f %13.9f \n",
                Ellipsoid_Table[index].Name,
                Ellipsoid_Table[index].Code,
                Ellipsoid_Table[index].A,
                Ellipsoid_Table[index].B,
                Ellipsoid_Table[index].Recp_F);
        index++;
      }
      fclose(fp);

      /* Store WGS84 */
      Ellipsoid_Index(WGS84_Ellipsoid_Code, &WGS84_Index);

      /* Store WGS72 */
      Ellipsoid_Index(WGS72_Ellipsoid_Code, &WGS72_Index);
    }
  }
  return (error_code);
} /* End Create_Ellipsoid */


long Ellipsoid_Count ( long *Count )
{ /* Begin Ellipsoid_Count */
/*
 *   Count    : The number of ellipsoids in the ellipsoid table. (output)
 *
 * The function Ellipsoid_Count returns the number of ellipsoids in the
 * ellipsoid table.  If the ellipsoid table has been initialized without error,
 * ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR
 * is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  if (!Ellipsoid_Initialized)
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  *Count = Number_of_Ellipsoids;
  return (error_code);
} /* End Ellipsoid_Count */


long Ellipsoid_Index ( const char *Code,
                       long *Index )
{ /* Begin Ellipsoid_Index */
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

  char temp_code[3];
  long error_code = ELLIPSE_NO_ERROR;
  long i = 0;                   /* index for ellipsoid table */
  long j = 0;

  *Index = 0;
  if (Ellipsoid_Initialized)
  {
    while (j < ELLIPSOID_CODE_LENGTH)
    {
      temp_code[j] = toupper(Code[j]);
      j++;
    }
    temp_code[ELLIPSOID_CODE_LENGTH - 1] = 0;
    while ((i < Number_of_Ellipsoids)
           && strcmp(temp_code, Ellipsoid_Table[i].Code))
    {
      i++;
    }
    if (strcmp(temp_code, Ellipsoid_Table[i].Code))
      error_code |= ELLIPSE_INVALID_CODE_ERROR;
    else
      *Index = i+1;
  }
  else
  {
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  }
  return (error_code);
} /* End Ellipsoid_Index */


long Ellipsoid_Name ( const long Index,
                      char *Name ) 
{ /* Begin Ellipsoid_Name */
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

  long error_code = ELLIPSE_NO_ERROR;

  strcpy(Name,"");
  if (Ellipsoid_Initialized)
  {
    if ((Index < 1) || (Index > Number_of_Ellipsoids))
      error_code |= ELLIPSE_INVALID_INDEX_ERROR;
    else
      strcpy(Name, Ellipsoid_Table[Index-1].Name);
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End Ellipsoid_Name */


long Ellipsoid_Code ( const long Index,
                      char *Code ) 
{ /* Begin Ellipsoid_Code */
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    Code     : 2-letter ellipsoid code.                          (output)
 *
 *  The Function Ellipsoid_Code returns the 2-letter code for the 
 *  ellipsoid in the ellipsoid table with the specified index.  If index is 
 *  valid, ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR 
 *  is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  strcpy(Code,"");
  if (Ellipsoid_Initialized)
  {
    if ((Index < 1) || (Index > Number_of_Ellipsoids))
      error_code |= ELLIPSE_INVALID_INDEX_ERROR;
    else
      strcpy(Code, Ellipsoid_Table[Index-1].Code);
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End Ellipsoid_Name */


long Ellipsoid_Axes ( const long Index,
                      double *A,
                      double *B )
{ /* Begin Ellipsoid_Axes */
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    a        : Semi-major axis, in meters, of ellipsoid          (output)
 *    B        : Semi-minor axis, in meters, of ellipsoid.         (output)
 *
 *  The function Ellipsoid_Axes returns the semi-major and semi-minor axes
 *  for the ellipsoid with the specified index.  If index is valid,
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR is 
 *  returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  *A = 0;
  *B = 0;
  if (Ellipsoid_Initialized)
  {
    if ((Index < 1) || (Index > Number_of_Ellipsoids))
    {
      error_code |= ELLIPSE_INVALID_INDEX_ERROR;
    }
    else
    {
      *A = Ellipsoid_Table[Index-1].A;
      *B = Ellipsoid_Table[Index-1].B;
    }
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End Ellipsoid_Axes */


long Ellipsoid_Eccentricity2 ( const long Index,
                               double *e2 )
{ /* Begin Ellipsoid_Eccentricity2 */
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    e2       : Square of eccentricity of ellipsoid               (output)
 *
 *  The function Ellipsoid_Eccentricity2 returns the square of the 
 *  eccentricity for the ellipsoid with the specified index.  If index is 
 *  valid, ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_INVALID_INDEX_ERROR 
 *  is returned.
 */
  double a2,b2;
  long error_code = ELLIPSE_NO_ERROR;

  *e2 = 0;
  if (Ellipsoid_Initialized)
  {
    if ((Index < 1) || (Index > Number_of_Ellipsoids))
    {
      error_code |= ELLIPSE_INVALID_INDEX_ERROR;
    }
    else
    {
      a2 = Ellipsoid_Table[Index-1].A * Ellipsoid_Table[Index-1].A;
      b2 = Ellipsoid_Table[Index-1].B * Ellipsoid_Table[Index-1].B;
      *e2 = (a2 - b2) / a2;
    }
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End Ellipsoid_Eccentricity2 */


long Ellipsoid_Flattening ( const long Index,
                            double *f )
{ /* Begin Ellipsoid_Flattening */
/*
 *    Index    : Index of a given ellipsoid in the ellipsoid table (input)
 *    f        : Flattening of ellipsoid.                          (output)
 *
 *  The function Ellipsoid_Flattening returns the flattening of the 
 *  ellipsoid with the specified index.  If index is valid ELLIPSE_NO_ERROR is 
 *  returned, otherwise ELLIPSE_INVALID_INDEX_ERROR is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  *f = 0;
  if (Ellipsoid_Initialized)
  {
    if ((Index < 1) || (Index > Number_of_Ellipsoids))
      error_code |= ELLIPSE_INVALID_INDEX_ERROR;
    else
      *f = 1 / Ellipsoid_Table[Index-1].Recp_F;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End Ellipsoid_Flattening */


long WGS84_Axes ( double *A,
                  double *B)
{ /* Begin WGS84_Axes */
/*
 *    A      : Semi-major axis, in meters, of ellipsoid       (output)
 *    B      : Semi-minor axis, in meters, of ellipsoid       (output)
 *
 *  The function WGS84_Axes returns the lengths of the semi-major and 
 *  semi-minor axes for the WGS84 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  *A = 0;
  *B = 0;
  if (Ellipsoid_Initialized)
  {
    *A = Ellipsoid_Table[WGS84_Index-1].A;
    *B = Ellipsoid_Table[WGS84_Index-1].B;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS84_Axes */


long WGS84_Eccentricity2 ( double *e2 )
{ /* Begin WGS84_Eccentricity2 */
/*
 *    e2    : Square of eccentricity of WGS84 ellipsoid      (output)
 *
 *  The function WGS84_Eccentricity2 returns the square of the 
 *  eccentricity for the WGS84 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;
  double a2, b2;

  *e2 = 0;
  if (Ellipsoid_Initialized)
  {
    a2 = Ellipsoid_Table[WGS84_Index-1].A * Ellipsoid_Table[WGS84_Index-1].A;
    b2 = Ellipsoid_Table[WGS84_Index-1].B * Ellipsoid_Table[WGS84_Index-1].B;
    *e2 = (a2 - b2) / a2;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS84_Eccentricity2 */


long WGS84_Flattening ( double *f )
{ /* Begin WGS84_Flattening */
/*
 *  f       : Flattening of WGS84 ellipsoid.                 (output)
 *
 *  The function WGS84_Flattening returns the flattening of the WGS84
 *  ellipsoid.  If the ellipsoid table was initialized successfully, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR 
 *  is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  *f = 0;
  if (Ellipsoid_Initialized)
  {
    *f = 1 / Ellipsoid_Table[WGS84_Index-1].Recp_F;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS84_Flattening */


long WGS72_Axes( double *A,
                 double *B)
{ /* Begin WGS72_Axes */
/*
 *    A    : Semi-major axis, in meters, of ellipsoid        (output)
 *    B    : Semi-minor axis, in meters, of ellipsoid        (output)
 *
 *  The function WGS72_Axes returns the lengths of the semi-major and 
 *  semi-minor axes for the WGS72 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;

  *A = 0;
  *B = 0;
  if (Ellipsoid_Initialized)
  {
    *A = Ellipsoid_Table[WGS72_Index-1].A;
    *B = Ellipsoid_Table[WGS72_Index-1].B;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS72_Axes */


long WGS72_Eccentricity2 ( double *e2 )
{ /* Begin WGS72_Eccentricity2 */
/*
 *    e2     : Square of eccentricity of WGS84 ellipsoid     (output)
 *
 *  The function WGS72_Eccentricity2 returns the square of the 
 *  eccentricity for the WGS72 ellipsoid.  If the ellipsoid table was 
 *  initialized successfully, ELLIPSE_NO_ERROR is returned, otherwise 
 *  ELLIPSE_NOT_INITIALIZED_ERROR is returned.
 */

  long error_code = ELLIPSE_NO_ERROR;
  double a2, b2;

  *e2 = 0;
  if (Ellipsoid_Initialized)
  {
    a2 = Ellipsoid_Table[WGS72_Index-1].A * Ellipsoid_Table[WGS72_Index-1].A;
    b2 = Ellipsoid_Table[WGS72_Index-1].B * Ellipsoid_Table[WGS72_Index-1].B;
    *e2 = (a2 - b2) / a2;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS72_Eccentricity2 */


long WGS72_Flattening (double *f )
{ /* Begin WGS72_Flattening */
/*
 *    f      : Flattening of WGS72 ellipsoid.                (output)
 *
 *  The function WGS72_Flattening returns the flattening of the WGS72
 *  ellipsoid .  If the ellipsoid table was initialized successfully, 
 *  ELLIPSE_NO_ERROR is returned, otherwise ELLIPSE_NOT_INITIALIZED_ERROR 
 *  is returned.
 */
  long error_code = ELLIPSE_NO_ERROR;

  *f = 0;
  if (Ellipsoid_Initialized)
  {
    *f = 1 / Ellipsoid_Table[WGS72_Index-1].Recp_F;
  }
  else
    error_code |= ELLIPSE_NOT_INITIALIZED_ERROR;
  return (error_code);
} /* End WGS72_Flattening */



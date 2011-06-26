/***************************************************************************/
/* RSC IDENTIFIER:  MGRS
 *
 * ABSTRACT
 *
 *    This component converts between geodetic coordinates (latitude and 
 *    longitude) and Military Grid Reference System (MGRS) coordinates. 
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *          MGRS_NO_ERROR          : No errors occurred in function
 *          MGRS_LAT_ERROR         : Latitude outside of valid range 
 *                                    (-90 to 90 degrees)
 *          MGRS_LON_ERROR         : Longitude outside of valid range
 *                                    (-180 to 360 degrees)
 *          MGRS_STR_ERROR         : An MGRS string error: string too long,
 *                                    too short, or badly formed
 *          MGRS_PRECISION_ERROR   : The precision must be between 0 and 5 
 *                                    inclusive.
 *          MGRS_A_ERROR           : Semi-major axis less than or equal to zero
 *          MGRS_INV_F_ERROR       : Inverse flattening outside of valid range
 *									                  (250 to 350)
 *          MGRS_EASTING_ERROR     : Easting outside of valid range
 *                                    (100,000 to 900,000 meters for UTM)
 *                                    (0 to 4,000,000 meters for UPS)
 *          MGRS_NORTHING_ERROR    : Northing outside of valid range
 *                                    (0 to 10,000,000 meters for UTM)
 *                                    (0 to 4,000,000 meters for UPS)
 *          MGRS_ZONE_ERROR        : Zone outside of valid range (1 to 60)
 *          MGRS_HEMISPHERE_ERROR  : Invalid hemisphere ('N' or 'S')
 *
 * REUSE NOTES
 *
 *    MGRS is intended for reuse by any application that does conversions
 *    between geodetic coordinates and MGRS coordinates.
 *
 * REFERENCES
 *
 *    Further information on MGRS can be found in the Reuse Manual.
 *
 *    MGRS originated from : U.S. Army Topographic Engineering Center
 *                           Geospatial Information Division
 *                           7701 Telegraph Road
 *                           Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *
 * ENVIRONMENT
 *
 *    MGRS was tested and certified in the following environments:
 *
 *    1. Solaris 2.5 with GCC version 2.8.1
 *    2. Windows 95 with MS Visual C++ version 6
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *    16-11-94          Original Code
 *    15-09-99          Reengineered upper layers
 *
 */


/***************************************************************************/
/*
 *                               INCLUDES
 */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ossim/projection/ossimUps.h>
#include <ossim/projection/ossimUtm.h>
#include <ossim/projection/ossimMgrs.h>

/*
 *      ctype.h     - Standard C character handling library
 *      math.h      - Standard C math library
 *      stdio.h     - Standard C input/output library
 *      string.h    - Standard C string handling library
 *      ups.h       - Universal Polar Stereographic (UPS) projection
 *      utm.h       - Universal Transverse Mercator (UTM) projection
 *      mgrs.h      - function prototype error checking
 */


/***************************************************************************/
/*
 *                              GLOBAL DECLARATIONS
 */
#define DEGRAD       0.017453292519943295 /* PI/180                          */
#define R3           0.052359877559829890 /* RADIANS FOR  3 DEGREES          */ 
#define R8           0.139626340159546400 /* RADIANS FOR  8 DEGREES          */
#define R9           0.157079632679489700 /* RADIANS FOR  9 DEGREES          */
#define R21          0.366519142918809200 /* RADIANS FOR  21 DEGREES         */
#define R33          0.575958653158128800 /* RADIANS FOR  33 DEGREES         */
#define R56          0.977384381116824600 /* RADIANS FOR  56 DEGREES         */
#define R64          1.117010721276371000 /* RADIANS FOR  64 DEGREES         */
#define R72          1.256637061435917000 /* RADIANS FOR  72 DEGREES         */
#define R80          1.396263401595464000 /* RADIANS FOR  80 DEGREES         */
#define UPS_SOUTH              3    /* UPS COORDINATE IN SOUTHERN HEMISPHERE */
#define UPS_NORTH              2    /* UPS COORDINATE IN NORTHERN HEMISPHERE */ 
#define UTM                    1    /* UTM COORDINATE                        */ 
#define ALBET                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ" /* ALPHABET       */
#define LETTER_A               0   /* ARRAY INDEX FOR LETTER A               */
#define LETTER_B               1   /* ARRAY INDEX FOR LETTER B               */
#define LETTER_C               2   /* ARRAY INDEX FOR LETTER C               */
#define LETTER_D               3   /* ARRAY INDEX FOR LETTER D               */
#define LETTER_E               4   /* ARRAY INDEX FOR LETTER E               */
#define LETTER_H               7   /* ARRAY INDEX FOR LETTER H               */
#define LETTER_I               8   /* ARRAY INDEX FOR LETTER I               */
#define LETTER_J               9   /* ARRAY INDEX FOR LETTER J               */
#define LETTER_L              11   /* ARRAY INDEX FOR LETTER L               */
#define LETTER_M              12   /* ARRAY INDEX FOR LETTER M               */
#define LETTER_N              13   /* ARRAY INDEX FOR LETTER N               */
#define LETTER_O              14   /* ARRAY INDEX FOR LETTER O               */
#define LETTER_P              15   /* ARRAY INDEX FOR LETTER P               */
#define LETTER_Q              16   /* ARRAY INDEX FOR LETTER Q               */
#define LETTER_R              17   /* ARRAY INDEX FOR LETTER R               */
#define LETTER_S              18   /* ARRAY INDEX FOR LETTER S               */
#define LETTER_U              20   /* ARRAY INDEX FOR LETTER U               */
#define LETTER_V              21   /* ARRAY INDEX FOR LETTER V               */
#define LETTER_W              22   /* ARRAY INDEX FOR LETTER W               */
#define LETTER_X              23   /* ARRAY INDEX FOR LETTER X               */
#define LETTER_Y              24   /* ARRAY INDEX FOR LETTER Y               */
#define LETTER_Z              25   /* ARRAY INDEX FOR LETTER Z               */
#define RND1                  0.1e0  /* ROUND TO NEAREST .1            */
#define RND5                  0.5e0  /* ROUND TO NEAREST .5            */
#define EOLN                   '\0'  /* END OF STRING CHARACTER        */
#define BLANK                   ' '  /* BLANK CHARACTER                */
#define OSSIM_MGRS_LETTERS            3  /* NUMBER OF LETTERS IN MGRS              */
#define NUM_OFFSET             48  /* USED TO CONVERT NUMBERS TO LETTERS     */
#define ONEHT          100000.e0    /* ONE HUNDRED THOUSAND                  */
#define TWOMIL        2000000.e0    /* TWO MILLION                           */
#define EIGHT          800000.e0    /* EIGHT HUNDRED THOUSAND                */
#define ONE3HT        1300000.e0    /* ONE MILLION THREE HUNDRED THOUSAND    */
#define ZERO                   0.e0  /* ZERO                           */
#define TEN                10.e0    /* TEN                                   */
#define TRUE                      1  /* CONSTANT VALUE FOR TRUE VALUE  */
#define FALSE                     0  /* CONSTANT VALUE FOR FALSE VALUE */
#define PI    3.14159265358979323e0  /* PI                             */
#define PI_OVER_2  (PI / 2.0e0)
#define NUM                   "01234567890"                /* NUMBERS        */
#define MAXALBET              25   /* LAST INDEX IN ALPHABET ARRAY(0-25)     */
#define MAXNUMBER             10   /* LAST INDEX IN NUMBER ARRAY(0-9)        */
#define OSSIM_MGRS_ZONE_AND_LETTERS   5  /* NUM. OF CHARS. IN ZONE AND LETTERS     */
#define OSSIM_MGRS_MINIMUM            9  /* MINIMUM NUMBER OF CHARS FOR MGRS       */
#define OSSIM_MGRS_MAXIMUM           15  /* MAXIMUM NUMBER OF CHARS FOR MGRS       */

#define MIN_EASTING  100000
#define MAX_EASTING  900000
#define MIN_NORTHING 0
#define MAX_NORTHING 10000000
#define MAX_PRECISION           5   /* Maximum precision of easting & northing */
#define MIN_UTM_LAT      ( (-80 * PI) / 180.0 ) /* -80 degrees in radians    */
#define MAX_UTM_LAT      ( (84 * PI) / 180.0 )  /* 84 degrees in radians     */

#define MIN_EAST_NORTH 0
#define MAX_EAST_NORTH 4000000

/* Ellipsoid parameters, default to WGS 84 */
double OSSIM_MGRS_a = 6378137.0;    /* Semi-major axis of ellipsoid in meters */
double OSSIM_MGRS_f = 1 / 298.257223563; /* Flattening of ellipsoid           */
double OSSIM_MGRS_recpf = 298.257223563;
char   OSSIM_MGRS_Ellipsoid_Code[3] = {'W','E',0};

const char* OSSIM_CLARKE_1866 = "CC";
const char* OSSIM_CLARKE_1880 = "CD";
const char* OSSIM_BESSEL_1841 = "BR";
/* 
 *    OSSIM_CLARKE_1866 : Ellipsoid code for OSSIM_CLARKE_1866
 *    OSSIM_CLARKE_1880 : Ellipsoid code for OSSIM_CLARKE_1880
 *    OSSIM_BESSEL_1841 : Ellipsoid code for OSSIM_BESSEL_1841
 */


/***************************************************************************/
/*
 *                              FUNCTIONS     
 */


void UTMSET (long izone, 
             long* ltrlow, 
             long* ltrhi, 
             double *fnltr)
{ /* BEGIN UTMSET */
  /*
   *    izone  : Zone number
   *    ltrlow : 2nd letter low number
   *    ltrhi  : 2nd letter high number
   *    fnltr  : False northing
   */
  long iset;      /* Set number (1-6) based on UTM zone number           */
  long igroup;    /* Group number (1-2) based on ellipsoid code and zone */
  iset = 1;
  while (((izone-iset) / 6) * 6 + iset != izone)
  {
    iset = iset +1;
    if (iset > 6)
    {
      return;
    }
  } 
  igroup = 1;
  if (!strcmp(OSSIM_MGRS_Ellipsoid_Code,OSSIM_CLARKE_1866) || !strcmp(OSSIM_MGRS_Ellipsoid_Code, OSSIM_CLARKE_1880) || !strcmp(OSSIM_MGRS_Ellipsoid_Code,OSSIM_BESSEL_1841))
  {
    igroup = 2;
  }
  else if (!strcmp(OSSIM_MGRS_Ellipsoid_Code, OSSIM_CLARKE_1866) && (izone >= 47) && (izone <= 55 ))
  {
    igroup = 1;
  }
  if ((iset == 1) || (iset == 4))
  {
    *ltrlow = LETTER_A;
    *ltrhi = LETTER_H;
  }
  else if ((iset == 2) || (iset == 5))
  {
    *ltrlow = LETTER_J;
    *ltrhi = LETTER_R;
  }
  else if ((iset == 3) || (iset == 6))
  {
    *ltrlow = LETTER_S;
    *ltrhi = LETTER_Z;
  }
  if (igroup == 1)
  {
    *fnltr = ZERO;
    if ((iset % 2) ==  0)
    {
      *fnltr = 1500000.e0;
    }
  }
  else if (igroup == 2)
  {
    *fnltr = 1000000.0e0;
    if ((iset % 2) == 0)
    {
      *fnltr =  500000.e0;
    }
  }
  return;
} /* END OF UTMSET */


void UTMLIM (long* n, 
             double sphi, 
             long izone, 
             double *spsou, 
             double *spnor,
             double *sleast, 
             double *slwest)
{                          /* BEGIN UTMLIM */
  /*
   *    n      : 1st letter number for MGRS
   *    sphi   : Latitude in radians
   *    izone  : Zone number 
   *    spsou  : South latitude limit
   *    spnor  : North latitude limit
   *    sleast : East longitude limit 
   *    slwest : West longitude limit 
   */
  double slcm;     /* Central meridian - Longitude of origin              */
  double temp;     /* Temporary variable                                  */
  long icm;        /* Central meridian                                    */
  long isphi;      /* South latitude limit                                */
  if (*n <= LETTER_A)
  {
    temp = ((sphi + R80) / (R8)) + 2;
    temp = temp + .00000001;
    *n = (long)temp;
    if (*n > LETTER_H)
    {
      *n = *n + 1;
    }
    if (*n > LETTER_N)
    {
      *n = *n + 1;
    }
    if (*n >= LETTER_Y)
    {
      *n = LETTER_X;
    }
    if ((*n  ==  LETTER_M) && (sphi  ==  ZERO ))
    {
      *n = LETTER_N;
    }
    isphi = (*n - 3) * 8 - 80;
  }
  else
  {
    isphi = (*n - 3) * 8 - 80;
    *n = *n - 1;    
  }
  if (*n > LETTER_H)
  {
    isphi = isphi - 8;
  }
  if (*n > LETTER_N)
  {
    isphi = isphi - 8;
  }
  *spsou = (double)(isphi) * DEGRAD;
  *spnor = *spsou + R8;
  if (*n == LETTER_X)
  {
    *spnor = *spsou + 12.e0 * DEGRAD;
  }
  icm = izone * 6 - 183;
  slcm = (double) icm * DEGRAD;
  *sleast = slcm + R3;
  *slwest = slcm - R3;
  if ((izone < 31) || (izone > 37))
  {
    return;
  }
  if (*n < LETTER_V)
  {
    return;
  }
  if ((*n == LETTER_V) && (izone == 31))
  {
    *sleast = R3;
  }
  if ((*n == LETTER_V) && (izone == 32))
  {
    *slwest = R3;
  }
  if (*n < LETTER_X)
  {
    return;
  }
  if (izone == 31)
  {
    *sleast = R9;
  }
  if (izone == 33)
  {
    *slwest = R9;
    *sleast = R21;
  }
  if (izone == 35)
  {
    *slwest = R21;
    *sleast = R33;
  }
  if (izone == 37)
  {
    *slwest = R33;
  }
  return;
} /* END OF UTMLIM */


void UTMOSSIM_MGRS (long izone,
              long* ltrnum,
              double sphi,
              double x,
              double y)
{ /* BEGIN UTMMGRS */
  /*
   *    izone      : Zone number.
   *    ltrnum     : Values of letters in the MGRS coordinate.
   *    sphi       : Latitude in radians.
   *    x          : Easting.
   *    y          : Northing.
   *
   *    UTMMGRS CALLS THE FOLLOWING ROUTINES:
   *
   *    GPTUTM
   *    UTMLIM
   *    UTMSET
   */
  double fnltr;       /* False northing for 3rd letter                     */
  double slcm;        /* Central meridian - longitude of origin            */
  double sleast;      /* Longitude east limit - UTM                        */
  double slwest;      /* Longitude west limit -UTM                         */
  double spnor;       /* MGRS north latitude limits based on 1st letter    */
  double spsou;       /* MGRS south latitude limits based on 1st letter    */
  double xltr;        /* Easting used to derive 2nd letter of MGRS         */
  double yltr;        /* Northing used to derive 3rd letter of MGRS        */
  long ltrlow;        /* 2nd letter range - low number                     */
  long ltrhi;         /* 2nd letter range - high number                    */
  char hemisphere;

  UTMSET(izone, &ltrlow, &ltrhi, &fnltr);
  ltrnum[0] = LETTER_A;
  UTMLIM(&ltrnum[0], sphi, izone, &spsou, &spnor, &sleast, &slwest);
  slcm = (double)(izone * 6 - 183) * DEGRAD;
  /*
    GPTUTM(a, recf, spsou, slcm, &izone, &yltr, &xltr, (long)1);
  */
  Set_UTM_Parameters(OSSIM_MGRS_a,OSSIM_MGRS_f,izone);
  Convert_Geodetic_To_UTM(spsou,slcm,&izone,&hemisphere,&xltr,&yltr);

  yltr = (double)((long)(y + RND5));
  if (((double)((long)(yltr + RND5))) == ((double)((long)(1.e7 + RND5))))
  {
    yltr = (double)((long)(yltr - 1.e0 + RND5)); 
  }
  while (yltr >= TWOMIL)
  {
    yltr = yltr - TWOMIL; 
  }
  yltr = yltr - fnltr;
  if (yltr < ZERO)
  {
    yltr = yltr + TWOMIL;
  }
  ltrnum[2] = (long)((yltr + RND1) / ONEHT); 
  if (ltrnum[2] > LETTER_H)
  {
    ltrnum[2] = ltrnum[2] + 1;
  }
  if (ltrnum[2] > LETTER_N)
  {
    ltrnum[2] = ltrnum[2] + 1;
  }
  xltr = (double)((long)(x));
  if (((ltrnum[0] == LETTER_V) && (izone == 31)) && 
      (((double)((long)(xltr + RND5))) == ((double)((long)(5.e5 + RND5)))))
  {
    xltr = (double)((long)(xltr - 1.e0 + RND5)); /* SUBTRACT 1 METER */
  }
  ltrnum[1] = ltrlow + ((long)((xltr + RND1) / ONEHT) -1); 
  if ((ltrlow == LETTER_J) && (ltrnum[1] > LETTER_N))
  {
    ltrnum[1] = ltrnum[1] + 1;
  }
  return;
} /* END UTMMGRS */


void UPSSET (long n, 
             long* ltrlow, 
             long* ltrhi, 
             double *feltr,
             double *fnltr, 
             long* ltrhy)
{ /* BEGIN UPSSET */
  /*
   *   n      : Value of 1st letter in MGRS coordinate.
   *   ltrlow : Low number for 2nd letter.
   *   ltrhi  : High number for 2nd letter.
   *   feltr  : False easting.
   *   fnltr  : False northing.
   *   ltrhy  : High number for 3rd letter.
   */
  if (n == LETTER_Z) /* EASTERN HEMISPHERE-NORTH ZONE */
  {
    *ltrlow = LETTER_A;
    *ltrhi  = LETTER_J;
    *feltr = TWOMIL;
    *fnltr = ONE3HT;
    *ltrhy = LETTER_P;
  }
  else if (n == LETTER_Y) /* WESTERN HEMISPHERE-NORTH ZONE */
  {
    *ltrlow = LETTER_J;
    *ltrhi  = LETTER_Z;
    *feltr = EIGHT;
    *fnltr = ONE3HT;
    *ltrhy = LETTER_P;
  }
  else if (n == LETTER_B) /* ** EASTERN HEMISPHERE - SOUTH ZONE */
  {
    *ltrlow = LETTER_A;
    *ltrhi  = LETTER_R;
    *feltr = TWOMIL;
    *fnltr = EIGHT;
    *ltrhy = LETTER_Z;
  }
  else if (n == LETTER_A) /* ** WESTERN HEMISPHERE - SOUTH ZONE */
  {
    *ltrlow = LETTER_J;
    *ltrhi  = LETTER_Z;
    *feltr = EIGHT;
    *fnltr = EIGHT;
    *ltrhy = LETTER_Z;
  }
  return;
} /* END OF UPSSET */


void UPS (char* mgrs,
          long* ltrnum,
          double x,
          double y,
          long iarea)
{ /* BEGIN UPS */
  /*
   *    mgrs   : MGRS coordinate.
   *    ltrnum : Values of the letters in the MGRS coordinate.
   *    x      : Easting.
   *    y      : Northing.
   *    iarea  : Set to UPS_NORTH or UPS_SOUTH.
   *
   *    UPS CALLS THE FOLLOWING ROUTINES:
   *
   *    UPSSET
   */
  double feltr=0.0;       /* False easting for 2nd letter                      */
  double fnltr=0.0;       /* False northing for 3rd letter                     */
  double xltr=0.0;        /* Easting used to derive 2nd letter of MGRS         */
  double yltr=0.0;        /* Northing used to derive 3rd letter of MGRS        */
  long ltrlow=0;        /* 2nd letter range - low number                     */
  long ltrhi=0;         /* 2nd letter range - high number                    */
  long ltrhy=0;         /* 3rd letter range - high number (UPS)              */
  if (iarea == UPS_NORTH)
  {
    ltrnum[0] = LETTER_Y;
    if (((double)((long)(x + RND5))) >= TWOMIL)
    {
      ltrnum[0] = LETTER_Z; 
    }
  }
  else if (iarea == UPS_SOUTH)
  {
    ltrnum[0] = LETTER_A;
    if (((double)((long)(x + RND5))) >= TWOMIL)
    {
      ltrnum[0] = LETTER_B;
    }
  }
  UPSSET(ltrnum[0], &ltrlow, &ltrhi, &feltr, &fnltr, &ltrhy);
  mgrs[0] = BLANK;
  mgrs[1] = BLANK;
  yltr = (double)((long)(y + RND5));
  yltr = yltr - fnltr;
  ltrnum[2] = (long)((yltr + RND1) / ONEHT);
  if (ltrnum[2] > LETTER_H)
  {
    ltrnum[2] = ltrnum[2] + 1;
  }
  if (ltrnum[2] > LETTER_N)
  {
    ltrnum[2] = ltrnum[2] + 1;
  }
  xltr = (double)((long)(x + RND5));
  xltr = xltr - feltr;
  ltrnum[1] = ltrlow + ((long)((xltr + RND1) / ONEHT)); 
  if (x < TWOMIL)
  {
    if (ltrnum[1] > LETTER_L)
    {
      ltrnum[1] = ltrnum[1] + 3; 
    }
    if (ltrnum[1] > LETTER_U)
    {
      ltrnum[1] = ltrnum[1] + 2; 
    }
  }
  if (x >= TWOMIL)
  {
    if (ltrnum[1] > LETTER_C)
    {
      ltrnum[1] = ltrnum[1] + 2; 
    }
    if (ltrnum[1] > LETTER_H)
    {
      ltrnum[1] = ltrnum[1] + 1; 
    }
    if (ltrnum[1] > LETTER_L)
    {
      ltrnum[1] = ltrnum[1] + 3; 
    }
  }
  return;
} /* END OF UPS */


void LTR2UPS (long* ltrnum, 
              long ltrlow, 
              long ltrhi, 
              long ltrhy, 
              long* ierr, 
              double *xltr, 
              double *yltr, 
              double fnltr, 
              double feltr, 
              double *x, 
              double *y, 
              double sign)
{ /* BEGIN LTR2UPS */
  /*    
   *    ltrnum : Values of the letters in the MGRS coordinate
   *    ltrlow : Low number
   *    ltrhi  : High number-2nd letter
   *    ltrhy  : High number-3rd letter
   *    ierr   : Error code
   *    xltr   : Easting for 100,000 meter grid square
   *    yltr   : Northing for 100,000 meter grid square
   *    fnltr  : False northing for 3rd letter
   *    feltr  : False easting for 2nd letter
   *    x      : Easting
   *    y      : Northing
   *    sign   : Set to either positive or negative
   */
  if (ltrnum[1] < ltrlow)
  {
    *ierr = TRUE;
    return;
  }
  if (ltrnum[1] > ltrhi)
  {
    *ierr = TRUE;
    return;
  }
  if (ltrnum[2] > ltrhy)
  {
    *ierr = TRUE;
    return;
  }
  if ((ltrnum[1] == LETTER_D) || (ltrnum[1] == LETTER_E) ||
      (ltrnum[1] == LETTER_M) || (ltrnum[1] == LETTER_N) ||
      (ltrnum[1] == LETTER_V) || (ltrnum[1] == LETTER_W))
  {
    *ierr = TRUE;
    return;
  }
  *yltr = (double)ltrnum[2] * ONEHT + fnltr; 
  if (ltrnum[2] > LETTER_I)
  {
    *yltr = *yltr - ONEHT;
  }
  if (ltrnum[2] > LETTER_O)
  {
    *yltr = *yltr - ONEHT;
  }
  *xltr = (double)((ltrnum[1]) - ltrlow) * ONEHT + feltr; 
  if (ltrlow != LETTER_A)
  {
    if (ltrnum[1] > LETTER_L)
    {
      *xltr = *xltr - 3.e0 * ONEHT;
    }
    if (ltrnum[1] > LETTER_U)
    {
      *xltr = *xltr - 2.e0 * ONEHT;
    }
  }
  else if (ltrlow == LETTER_A)
  {
    if (ltrnum[1] > LETTER_C)
    {
      *xltr = *xltr - 2.e0 * ONEHT;
    }
    if (ltrnum[1] > LETTER_I)
    {
      *xltr = *xltr - ONEHT;
    }
    if (ltrnum[1] > LETTER_L)
    {
      *xltr = *xltr - 3.e0 * ONEHT ;
    }
  }
  *x = *xltr;
  *y = *yltr * sign;
  return;
} /* END OF LTR2UPS */


void GRID_UPS (long   *Letters,
               char   *Hemisphere,
               double *Easting,
               double *Northing,
               long   *Error)
{ /* BEGIN GRID_UPS */
  double feltr=0.0;               /* False easting for 2nd letter               */
  double fnltr=0.0;               /* False northing for 3rd letter              */
  double sleast=0.0;              /* Longitude east limit - UTM                 */
  double slwest=0.0;              /* Longitude west limit -UTM                  */
  double spnor=0.0;               /* North latitude limits based on 1st letter  */
  double spsou=0.0;               /* South latitude limits based on 1st letter  */
  double x=0.0;                   /* easting                                    */
  double xltr=0.0;                /* easting for 100,000 meter grid square      */
  double xnum=0.0;                /* easting part of MGRS                       */
  double y=0.0;                   /* northing                                   */
  double yltr=0.0;                /* northing for 100,000 meter grid square     */
  double ynum=0.0;                /* northing part of MGRS                      */
  long izone=0;                 /* Zone number                                */
  long ltrhi=0;                 /* 2nd letter range - high number             */
  long ltrhy=0;                 /* 3rd letter range - high number (UPS)       */
  long ltrlow=0;                /* 2nd letter range - low number              */
  long sign=0;
  double sphi=0.0;
  double slam=0.0;
  if ((Letters[0] == LETTER_Y) || (Letters[0] == LETTER_Z))
  {
    spsou = MAX_UTM_LAT;
    sign = 1;
  }
  else
  {
    spsou = MIN_UTM_LAT;
    sign = -1;
  }
  slam = PI / 2.e0;
  if ((Letters[0] == LETTER_Y) || (Letters[0] == LETTER_A))
  {
    slam = -slam;
  }
  izone = 0;
  sphi = spsou;
  Set_UPS_Parameters(OSSIM_MGRS_a,OSSIM_MGRS_f);
  Convert_Geodetic_To_UPS(sphi,slam,Hemisphere,&x,&y);
  spnor = sphi;
  sleast = slam;
  slwest = slam;
  UPSSET(Letters[0], &ltrlow, &ltrhi, &feltr, &fnltr, &ltrhy);
  LTR2UPS(Letters, ltrlow, ltrhi, ltrhy, Error, &xltr, &yltr, fnltr, feltr, 
          &x, &y, sign);
  xnum = *Easting;
  ynum = *Northing;
  y = (yltr + ynum);
  x = xltr + xnum;
  *Easting = x;
  *Northing = y;
  return;
} /* END OF GRID_UPS */


void LTR2UTM (long* ltrnum, 
              long ltrlow, 
              long ltrhi, 
              long* ierr, 
              double *xltr, 
              double *yltr, 
              double fnltr, 
              double yslow, 
              double ylow) 
{ /* BEGIN LTR2UTM */
  /*    
   *    xltr   : Easting for 100,000 meter grid square.
   *    yltr   : Northing for 100,000 meter grid square.
   *    ierr   : Error code.
   *    ltrnum : Values of the letters in the OSSIM_MGRS coordinate.
   *    ltrlow : Low number.
   *    ltrhi  : High number.
   *    fnltr  : False northing for 3rd letter.
   *    yslow  : Northing scaled down to less than 2 million.
   *    ylow   : Lowest northing of area to nearest 100,000.
   */
  if (ltrnum[1] < ltrlow)
  {
    *ierr = TRUE;
    return;
  }
  if (ltrnum[1] > ltrhi)
  {
    *ierr = TRUE;
    return;
  }
  if (ltrnum[2] > LETTER_V)
  {
    *ierr = TRUE;
    return;
  }
  *yltr = (double)(ltrnum[2]) * ONEHT + fnltr;
  *xltr = (double)((ltrnum[1]) - ltrlow + 1) * ONEHT;
  if ((ltrlow == LETTER_J) && (ltrnum[1] > LETTER_O))
  {
    *xltr = *xltr - ONEHT;
  }
  if (ltrnum[2] > LETTER_O)
  {
    *yltr = *yltr - ONEHT;
  }
  if (ltrnum[2] > LETTER_I)
  {
    *yltr = *yltr - ONEHT; 
  }
  if (((double)((long)(*yltr + RND5))) >= ((double)((long)(TWOMIL + RND5))))
  {
    *yltr = *yltr - TWOMIL;
  }
  *yltr = ((double)((long)(*yltr + RND5)));
  *yltr = *yltr - yslow;
  if (*yltr < ZERO)
  {
    *yltr = *yltr + TWOMIL;
  }
  *yltr = ((double)((long)(ylow + *yltr + RND5)));
  return;
} /* END OF LTR2UTM */


void GRID_UTM (long   *Zone,
               long   *Letters,
               char   *Hemisphere,
               double *Easting,
               double *Northing,
               long   In_Precision,
               long   *Error)
{ /* BEGIN GRID_UTM */
  double fnltr;               /* False northing for 3rd letter              */
  long ltrhi;                 /* 2nd letter range - High number             */
  long ltrlow;                /* 2nd letter range - Low number              */
  long number;                /* Value of ltrnum[0] + 1                     */
/*  double slam;*/
  double slcm;                /* Central meridian                           */
  double sleast;              /* Longitude east limit - UTM                 */
  double slwest;              /* Longitude west limit -UTM                  */
  double sphi;                /* Latitude (needed by UTMLIM)                */
  double spnor;               /* North latitude limits based on 1st letter  */
  double spsou;               /* South latitude limits based on 1st letter  */
  double xltr;                /* Easting for 100,000 meter grid square      */
  double ylow;                /* Lowest northing of area to nearest 100,000 */
  double yltr;                /* Northing for 100,000 meter grid square     */
  double yslow;               /* Northing scaled down to less than 2 million*/
  double Latitude = 0.0;
  double Longitude = 0.0;
  double divisor = 1.0;
  if ((*Zone == 32) && (Letters[0] == LETTER_X))
  {
    *Error = TRUE;
    return;
  }
  if ((*Zone == 34) && (Letters[0] == LETTER_X))
  {
    *Error = TRUE;
    return;
  }
  if ((*Zone == 36) && (Letters[0] == LETTER_X))
  {
    *Error = TRUE;
    return;
  }
  number = Letters[0] + 1;
  sphi = 0.0;
  UTMLIM(&number,sphi,*Zone,&spsou,&spnor,&sleast,&slwest);
  Set_UTM_Parameters(OSSIM_MGRS_a,OSSIM_MGRS_f,*Zone);
  slcm = (double)(*Zone * 6 - 183) * DEGRAD; 
  Convert_Geodetic_To_UTM(spsou,slcm,Zone,Hemisphere,&xltr,&yltr);
  ylow = ((double)((long)((double)((long)(yltr / ONEHT)) * ONEHT)));
  yslow = ylow;
  while (yslow >= TWOMIL)
  {
    yslow = yslow - TWOMIL;
  }
  yslow = ((double)((long)(yslow)));
  UTMSET(*Zone, &ltrlow, &ltrhi, &fnltr);
  LTR2UTM(Letters, ltrlow, ltrhi, Error, &xltr, &yltr, fnltr, yslow, ylow);
  *Easting = xltr + *Easting;
  *Northing = yltr + *Northing;
  /* check that point is within Zone Letter bounds */
  Convert_UTM_To_Geodetic(*Zone,*Hemisphere,*Easting,*Northing,&Latitude,&Longitude);
  divisor = pow (10.0, In_Precision);
  if (((spsou - DEGRAD/divisor) <= Latitude) && (Latitude <= (spnor + DEGRAD/divisor)))
    return;
  else
    *Error = TRUE;
  return;
}/* END OF GRID_UTM */


long Round_OSSIM_MGRS (double value)
/* Round value to nearest integer, using standard engineering rule */
{ /* Round_OSSIM_MGRS */
  double ivalue;
  long ival;
  double fraction = modf (value, &ivalue);
  ival = (long)(ivalue);
  if ((fraction > 0.5) || ((fraction == 0.5) && (ival%2 == 1)))
    ival++;
  return (ival);
} /* Round_OSSIM_MGRS */


long Make_OSSIM_MGRS_String (char* OSSIM_MGRS, 
                       long Zone, 
                       long ltrnum[OSSIM_MGRS_LETTERS], 
                       double Easting, 
                       double Northing,
                       long Precision)
/* Construct an OSSIM_MGRS string from its component parts */
{ /* Make_OSSIM_MGRS_String */
  int i;
  int j;
  int error_code = OSSIM_MGRS_NO_ERROR;
  double divisor;
  long int east;  /* double data type? (drb) */
  long int north; /* double data type? (drb) */
  i = 0;
  if (Zone)
    i = sprintf (OSSIM_MGRS+i,"%2.2ld",Zone);
  for (j=0;j<3;j++)
    OSSIM_MGRS[i++] = ALBET[ltrnum[j]];
  divisor = pow (10.0, (5 - Precision));
  Easting = fmod (Easting, 100000.0);
  if (Easting >= 99999.5)
    Easting = 99999.0;
  east = (long int)(Easting/divisor);
  i += sprintf (OSSIM_MGRS+i, "%*.*ld", (int)Precision, (int)Precision, east);
  Northing = fmod (Northing, 100000.0);
  if (Northing >= 99999.5)
    Northing = 99999.0;
  north = (long int)(Northing/divisor);
  i += sprintf (OSSIM_MGRS+i, "%*.*ld", (int)Precision, (int)Precision, north);
  return (error_code);
} /* Make_OSSIM_MGRS_String */


long Break_OSSIM_MGRS_String (const char* OSSIM_MGRS,
                        long* Zone,
                        long Letters[OSSIM_MGRS_LETTERS],
                        double* Easting,
                        double* Northing,
                        long* Precision)
/* Break down an OSSIM_MGRS string into its component parts */
{ /* Break_OSSIM_MGRS_String */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long i = 0;
  long j;
  long num_digits;
  long num_letters;
  while (OSSIM_MGRS[i] == ' ')
    i++;  /* skip any leading blanks */
  j = i;
  while (isdigit(OSSIM_MGRS[i]))
    i++;
  num_digits = i - j;
  if (num_digits <= 2)
    if (num_digits > 0)
    {
      char zone_string[3];
      /* get zone */
      strncpy (zone_string, OSSIM_MGRS+j, 2);
      zone_string[2] = 0;
      sscanf (zone_string, "%ld", Zone);  
      if ((*Zone < 1) || (*Zone > 60))
        error_code |= OSSIM_MGRS_STRING_ERROR;
    }
    else
      *Zone = 0;
  else
    error_code |= OSSIM_MGRS_STRING_ERROR;
  j = i;
  while (isalpha(OSSIM_MGRS[i]))
    i++;
  num_letters = i - j;
  if (num_letters == 3)
  {
    /* get letters */
    Letters[0] = (toupper(OSSIM_MGRS[j]) - (long)'A');
    if ((Letters[0] == LETTER_I) || (Letters[0] == LETTER_O))
      error_code |= OSSIM_MGRS_STRING_ERROR;
    Letters[1] = (toupper(OSSIM_MGRS[j+1]) - (long)'A');
    if ((Letters[1] == LETTER_I) || (Letters[1] == LETTER_O))
      error_code |= OSSIM_MGRS_STRING_ERROR;
    Letters[2] = (toupper(OSSIM_MGRS[j+2]) - (long)'A');
    if ((Letters[2] == LETTER_I) || (Letters[2] == LETTER_O))
      error_code |= OSSIM_MGRS_STRING_ERROR;
  }
  else
    error_code |= OSSIM_MGRS_STRING_ERROR;
  j = i;
  while (isdigit(OSSIM_MGRS[i]))
    i++;
  num_digits = i - j;
  if ((num_digits <= 10) && (num_digits%2 == 0))
  {
    long n;
    char east_string[6];
    char north_string[6];
    long east;
    long north;
    double multiplier;
    /* get easting & northing */
    n = num_digits/2;
    *Precision = n;
    if (n > 0)
    {
      strncpy (east_string, OSSIM_MGRS+j, n);
      east_string[n] = 0;
      sscanf (east_string, "%ld", &east);
      strncpy (north_string, OSSIM_MGRS+j+n, n);
      north_string[n] = 0;
      sscanf (north_string, "%ld", &north);
      multiplier = pow (10.0, 5 - n);
      *Easting = east * multiplier;
      *Northing = north * multiplier;
    }
    else
    {
      *Easting = 0.0;
      *Northing = 0.0;
    }
  }
  else
    error_code |= OSSIM_MGRS_STRING_ERROR;
  return (error_code);
} /* Break_OSSIM_MGRS_String */


long Set_OSSIM_MGRS_Parameters (double a,
                          double f,
                          const char* Ellipsoid_Code)
/*
 * The function SET_OSSIM_MGRS_PARAMETERS receives the ellipsoid parameters and sets
 * the corresponding state variables. If any errors occur, the error code(s)
 * are returned by the function, otherwise OSSIM_MGRS_NO_ERROR is returned.
 *
 *   a                : Semi-major axis of ellipsoid in meters  (input)
 *   f                : Flattening of ellipsoid                 (input)
 *   Ellipsoid_Code   : 2-letter code for ellipsoid             (input)
 */
{ /* Set_OSSIM_MGRS_Parameters  */

  double inv_f = 1 / f;
  long Error_Code = OSSIM_MGRS_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= OSSIM_MGRS_A_ERROR;
  }
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= OSSIM_MGRS_INV_F_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    OSSIM_MGRS_a = a;
    OSSIM_MGRS_f = f;
    OSSIM_MGRS_recpf = inv_f;
    strcpy (OSSIM_MGRS_Ellipsoid_Code, Ellipsoid_Code);
  }
  return (Error_Code);
}  /* Set_OSSIM_MGRS_Parameters  */



void Get_OSSIM_MGRS_Parameters (double *a,
                          double *f,
                          char* Ellipsoid_Code)
/*
 * The function Get_OSSIM_MGRS_Parameters returns the current ellipsoid
 * parameters.
 *
 *  a                : Semi-major axis of ellipsoid, in meters (output)
 *  f                : Flattening of ellipsoid					       (output)
 *  Ellipsoid_Code   : 2-letter code for ellipsoid             (output)
 */
{ /* Get_OSSIM_MGRS_Parameters */
  *a = OSSIM_MGRS_a;
  *f = OSSIM_MGRS_f;
  strcpy (Ellipsoid_Code, OSSIM_MGRS_Ellipsoid_Code);
  return;
} /* Get_OSSIM_MGRS_Parameters */


long Convert_Geodetic_To_OSSIM_MGRS (double Latitude,
                               double Longitude,
                               long Precision,
                               char* OSSIM_MGRS)
/*
 *    Latitude   : Latitude in radians              (input)
 *    Longitude  : Longitude in radians             (input)
 *    OSSIM_MGRS       : OSSIM_MGRS coordinate string           (output)
 *  
 */
{ /* Convert_Geodetic_To_OSSIM_MGRS */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long zone;
  char hemisphere;
  double easting;
  double northing;
  if ((Latitude < -PI_OVER_2) || (Latitude > PI_OVER_2))
  { /* Latitude out of range */
    error_code |= OSSIM_MGRS_LAT_ERROR;
  }
  if ((Longitude < -PI) || (Longitude > (2*PI)))
  { /* Longitude out of range */
    error_code |= OSSIM_MGRS_LON_ERROR;
  }
  if ((Precision < 0) || (Precision > MAX_PRECISION))
    error_code |= OSSIM_MGRS_PRECISION_ERROR;
  if (!error_code)
  {
    if ((Latitude < MIN_UTM_LAT) || (Latitude > MAX_UTM_LAT))
    {
      Set_UPS_Parameters (OSSIM_MGRS_a, OSSIM_MGRS_f);
      error_code |= Convert_Geodetic_To_UPS (Latitude, Longitude, &hemisphere, &easting, &northing);
      error_code |= Convert_UPS_To_OSSIM_MGRS (hemisphere, easting, northing, Precision, OSSIM_MGRS);
    }
    else
    {
      Set_UTM_Parameters (OSSIM_MGRS_a, OSSIM_MGRS_f, 0);
      error_code |= Convert_Geodetic_To_UTM (Latitude, Longitude, &zone, &hemisphere, &easting, &northing);
      error_code |= Convert_UTM_To_OSSIM_MGRS (zone, hemisphere, easting, northing, Precision, OSSIM_MGRS);
    }
  }
  return (error_code);
} /* Convert_Geodetic_To_OSSIM_MGRS */


long Convert_OSSIM_MGRS_To_Geodetic (const char* OSSIM_MGRS, 
                               double *Latitude, 
                               double *Longitude)
/*
 *    OSSIM_MGRS       : OSSIM_MGRS coordinate string           (output)
 *    Latitude   : Latitude in radians              (input)
 *    Longitude  : Longitude in radians             (input)
 *  
 */
{ /* Convert_OSSIM_MGRS_To_Geodetic */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long Zone;
  long Letters[OSSIM_MGRS_LETTERS];
  char Hemisphere;
  double Easting;
  double Northing;
  long In_Precision;
  error_code = Break_OSSIM_MGRS_String (OSSIM_MGRS, &Zone, Letters, &Easting, &Northing, &In_Precision);
  if (!error_code)
  {
     if (Zone)
     {
        error_code |= Convert_OSSIM_MGRS_To_UTM (OSSIM_MGRS, &Zone, &Hemisphere, &Easting, &Northing);
        Set_UTM_Parameters (OSSIM_MGRS_a, OSSIM_MGRS_f, 0);
        error_code |= Convert_UTM_To_Geodetic (Zone, Hemisphere, Easting, Northing, Latitude, Longitude);
     }
     else
     {
        error_code |= Convert_OSSIM_MGRS_To_UPS (OSSIM_MGRS, &Hemisphere, &Easting, &Northing);
        Set_UPS_Parameters (OSSIM_MGRS_a, OSSIM_MGRS_f);
        error_code |= Convert_UPS_To_Geodetic (Hemisphere, Easting, Northing, Latitude, Longitude);
     }
  }
  return (error_code);
} /* END OF Convert_OSSIM_MGRS_To_Geodetic */



long Convert_UTM_To_OSSIM_MGRS (long Zone,
                          char Hemisphere,
                          double Easting,
                          double Northing,
                          long Precision,
                          char* OSSIM_MGRS)
/*
 * The function Convert_UTM_To_OSSIM_MGRS converts UTM (zone, easting, and
 * northing) coordinates to an OSSIM_MGRS coordinate string, according to the 
 * current ellipsoid parameters.  If any errors occur, the error code(s) 
 * are returned by the  function, otherwise OSSIM_MGRS_NO_ERROR is returned.
 *
 *    Zone       : UTM zone                         (input)
 *    Hemisphere : North or South hemisphere        (input)
 *    Easting    : Easting (X) in meters            (input)
 *    Northing   : Northing (Y) in meters           (input)
 *    Precision  : Precision level of OSSIM_MGRS string   (input)
 *    OSSIM_MGRS       : OSSIM_MGRS coordinate string           (output)
 */
{ /* Convert_UTM_To_OSSIM_MGRS */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long temp_error = OSSIM_MGRS_NO_ERROR;
  long Letters[OSSIM_MGRS_LETTERS]; /* Number location of 3 letters in alphabet   */
  double Latitude;           /* Latitude of UTM point */
  double Longitude;          /* Longitude of UTM point */
  double divisor;
  if ((Zone < 1) || (Zone > 60))
    error_code |= OSSIM_MGRS_ZONE_ERROR;
  if ((Hemisphere != 'S') && (Hemisphere != 'N'))
    error_code |= OSSIM_MGRS_HEMISPHERE_ERROR;
  if ((Easting < MIN_EASTING) || (Easting > MAX_EASTING))
    error_code |= OSSIM_MGRS_EASTING_ERROR;
  if ((Northing < MIN_NORTHING) || (Northing > MAX_NORTHING))
    error_code |= OSSIM_MGRS_NORTHING_ERROR;
  if ((Precision < 0) || (Precision > MAX_PRECISION))
    error_code |= OSSIM_MGRS_PRECISION_ERROR;
  if (!error_code)
  {
    Set_UTM_Parameters (OSSIM_MGRS_a, OSSIM_MGRS_f,0);
    temp_error = Convert_UTM_To_Geodetic (Zone, Hemisphere, Easting, Northing, &Latitude, &Longitude);
	if (temp_error & UTM_NORTHING_ERROR)
		error_code |= OSSIM_MGRS_NORTHING_ERROR;
	else
	{
	  /* Round easting and northing values */
	  divisor = pow (10.0, (5 - Precision));
	  Easting = Round_OSSIM_MGRS (Easting/divisor) * divisor;
	  Northing = Round_OSSIM_MGRS (Northing/divisor) * divisor;
	  UTMOSSIM_MGRS (Zone, Letters, Latitude, Easting, Northing);
	  /* UTM checks - these should be done in UTMOSSIM_MGRS */
	  if ((Zone == 31) && (Letters[0] == LETTER_V))
	    if (Easting > 500000)
		  Easting = 500000;
	  if (Northing > 10000000)
	    Northing = 10000000;
	  Make_OSSIM_MGRS_String (OSSIM_MGRS, Zone, Letters, Easting, Northing, Precision);
	}
  }
  return (error_code);
} /* Convert_UTM_To_OSSIM_MGRS */


long Convert_OSSIM_MGRS_To_UTM (const char   *OSSIM_MGRS,
                          long   *Zone,
                          char   *Hemisphere,
                          double *Easting,
                          double *Northing)
/*
 * The function Convert_OSSIM_MGRS_To_UTM converts an OSSIM_MGRS coordinate string
 * to UTM projection (zone, hemisphere, easting and northing) coordinates 
 * according to the current ellipsoid parameters.  If any errors occur, 
 * the error code(s) are returned by the function, otherwise UTM_NO_ERROR 
 * is returned.
 *
 *    OSSIM_MGRS       : OSSIM_MGRS coordinate string           (input)
 *    Zone       : UTM zone                         (output)
 *    Hemisphere : North or South hemisphere        (output)
 *    Easting    : Easting (X) in meters            (output)
 *    Northing   : Northing (Y) in meters           (output)
 */
{ /* Convert_OSSIM_MGRS_To_UTM */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long Letters[OSSIM_MGRS_LETTERS];
  long In_Precision;
  long Error = 0;
  error_code = Break_OSSIM_MGRS_String (OSSIM_MGRS, Zone, Letters, Easting, Northing, &In_Precision);
  if (!*Zone)
    error_code |= OSSIM_MGRS_STRING_ERROR;
  if (!error_code)
  {
    GRID_UTM (Zone, Letters, Hemisphere, Easting, Northing, In_Precision, &Error);
    if (Error)
      error_code = OSSIM_MGRS_STRING_ERROR;
  }
  return (error_code);
} /* Convert_OSSIM_MGRS_To_UTM */


long Convert_UPS_To_OSSIM_MGRS (char   Hemisphere,
                          double Easting,
                          double Northing,
                          long   Precision,
                          char*  OSSIM_MGRS)
/*
 *  The function Convert_UPS_To_OSSIM_MGRS converts UPS (hemisphere, easting, 
 *  and northing) coordinates to an OSSIM_MGRS coordinate string according to 
 *  the current ellipsoid parameters.  If any errors occur, the error
 *  code(s) are returned by the function, otherwise UPS_NO_ERROR is 
 *  returned.
 *
 *    Hemisphere    : Hemisphere either 'N' or 'S'     (input)
 *    Easting       : Easting/X in meters              (input)
 *    Northing      : Northing/Y in meters             (input)
 *    Precision     : Precision level of OSSIM_MGRS string   (input)
 *    OSSIM_MGRS          : OSSIM_MGRS coordinate string           (output)
 */
{ /* Convert_UPS_To_OSSIM_MGRS */
  double divisor;
  long error_code = OSSIM_MGRS_NO_ERROR;
  long ltrnum[OSSIM_MGRS_LETTERS]; /* Number location of 3 letters in alphabet   */
  if ((Hemisphere != 'N') && (Hemisphere != 'S'))
    error_code |= OSSIM_MGRS_HEMISPHERE_ERROR;
  if ((Easting < MIN_EAST_NORTH) || (Easting > MAX_EAST_NORTH))
    error_code |= OSSIM_MGRS_EASTING_ERROR;
  if ((Northing < MIN_EAST_NORTH) || (Northing > MAX_EAST_NORTH))
    error_code |= OSSIM_MGRS_NORTHING_ERROR;
  if ((Precision < 0) || (Precision > MAX_PRECISION))
    error_code |= OSSIM_MGRS_PRECISION_ERROR;
  if (!error_code)
  {
    divisor = pow (10.0, (5 - Precision));
    Easting = Round_OSSIM_MGRS (Easting/divisor) * divisor;
    Northing = Round_OSSIM_MGRS (Northing/divisor) * divisor;
    if (Hemisphere == 'S')
      UPS (OSSIM_MGRS, ltrnum, Easting, Northing, UPS_SOUTH);
    else
      UPS (OSSIM_MGRS, ltrnum, Easting, Northing, UPS_NORTH);
    Make_OSSIM_MGRS_String (OSSIM_MGRS, 0, ltrnum, Easting, Northing, Precision);
  }
  return (error_code);
} /* Convert_UPS_To_OSSIM_MGRS */


long Convert_OSSIM_MGRS_To_UPS ( const char   *OSSIM_MGRS,
                           char   *Hemisphere,
                           double *Easting,
                           double *Northing)
/*
 *  The function Convert_OSSIM_MGRS_To_UPS converts an OSSIM_MGRS coordinate string
 *  to UPS (hemisphere, easting, and northing) coordinates, according 
 *  to the current ellipsoid parameters. If any errors occur, the error 
 *  code(s) are returned by the function, otherwide UPS_NO_ERROR is returned.
 *
 *    OSSIM_MGRS          : OSSIM_MGRS coordinate string           (input)
 *    Hemisphere    : Hemisphere either 'N' or 'S'     (output)
 *    Easting       : Easting/X in meters              (output)
 *    Northing      : Northing/Y in meters             (output)
 */
{ /* Convert_OSSIM_MGRS_To_UPS */
  long error_code = OSSIM_MGRS_NO_ERROR;
  long Error = 0;
  long Zone;
  long Letters[OSSIM_MGRS_LETTERS];
  long In_Precision;
  error_code = Break_OSSIM_MGRS_String (OSSIM_MGRS, &Zone, Letters, Easting, Northing, &In_Precision);
  if (Zone)
    error_code |= OSSIM_MGRS_STRING_ERROR;
  if (!error_code)
  {
    GRID_UPS (Letters, Hemisphere, Easting, Northing, &Error);
    if (Error)
      error_code = OSSIM_MGRS_STRING_ERROR;
  }
  return (error_code);
} /* Convert_OSSIM_MGRS_To_UPS */



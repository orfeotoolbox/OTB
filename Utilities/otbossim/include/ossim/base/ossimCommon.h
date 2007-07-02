//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// LICENSE: LGPL
//
// Author: Garrett Potts
// Description: Common file for global functions.
//
//*************************************************************************
// $Id: ossimCommon.h 10116 2006-12-14 20:29:59Z dburken $
#ifndef COMMON_H
#define COMMON_H


#include <cmath>
#include <ossim/base/ossimMutex.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRtti.h>

class ossimIpt;

/**
 * Mutex for lock/unlocking.
 */
extern ossimMutex theCriticalSectionMutex;

//***
// Common math functions
//***
inline double  cosd(double X)   {return std::cos(X*RAD_PER_DEG);}
inline double  sind(double X)   {return std::sin(X*RAD_PER_DEG);}
inline double  tand(double X)   {return std::tan(X*RAD_PER_DEG);}
inline double  acosd(double X)  {return DEG_PER_RAD * std::acos(X);}
inline double  asind(double X)  {return DEG_PER_RAD * std::asin(X);}
inline double  atand(double X)  {return DEG_PER_RAD * std::atan(X);}
inline double  atan2d(double Y, double X)
{
   return DEG_PER_RAD * std::atan2(Y,X);
}

// Note: We use n and m as temporaries in this function, so there is no value
// in using const IntType& as we would only need to make a copy anyway...
template <typename IntType>
IntType ossimGcd(IntType n, IntType m)
{
    // Avoid repeated construction
    IntType zero(0);

    // This is abs() - given the existence of broken compilers with Koenig
    // lookup issues and other problems, I code this explicitly. (Remember,
    // IntType may be a user-defined type).
    if (n < zero)
        n = -n;
    if (m < zero)
        m = -m;

    // As n and m are now positive, we can be sure that %= returns a
    // positive value (the standard guarantees this for built-in types,
    // and we require it of user-defined types).
    for(;;) {
      if(m == zero)
        return n;
      n %= m;
      if(n == zero)
        return m;
      m %= n;
    }
}

template<class T> inline T ossimAbs(T val)
{
   return ( (val<0) ? ( val * ( (T)-1) ) : val );
}

template <typename IntType>
IntType ossimLcm(IntType n, IntType m)
{
    // Avoid repeated construction
    IntType zero(0);

    if (n == zero || m == zero)
        return zero;

    n /= gcd(n, m);
    n *= m;

    if (n < zero)
        n = -n;
    return n;
}

inline bool ossimIsNan(double val)
{
   return (val == OSSIM_DBL_NAN);
}

inline bool ossimIsNan(float val)
{
   return (val == OSSIM_FLT_NAN);
}

inline bool ossimIsNan(int val)
{
   return (val == OSSIM_INT_NAN);
}

inline bool ossimIsNan(long val)
{
   return (val == OSSIM_INT_NAN);
}

inline bool ossimIsNan(unsigned long val)
{
   return (val == OSSIM_ULONG_NAN);
}

inline bool ossimIsNan(unsigned int val)
{
   return (val == OSSIM_UINT_NAN);
}

inline double sinc(double x)
{
   x *= M_PI;
   if(x != 0)
      return std::sin(x)/x;
   else
      return 1.0;
}
inline int ossimGetSign(double value)
{
   return (value < 0?-1:1);
}

#ifndef MAX
#  define MAX(x,y) ((x)>(y)?(x):(y))
#  define MIN(x,y) ((x)>(y)?(y):(x))
#endif

#ifndef ABS
#  define ABS(x) ((x)>0?(x):-(x))
#endif

#ifndef SIGN
#  define SIGN(x) ((x)<0?(-1):(1))
#endif

template<class T>
T ossimClamp(const T& value,
             const T& minValue,
             const T& maxValue)
{
   if(value < minValue) return minValue;
   if(value > maxValue) return maxValue;
   return value;
}

template<class T>
T ossimMax(T leftValue, T rightValue)
{
   if(leftValue < rightValue)
   {
      return rightValue;
   }
   return leftValue;
}

template<class T>
T ossimMin(T leftValue, T rightValue)
{
   if(rightValue < leftValue)
   {
      return rightValue;
   }
   return leftValue;
}


template<class Type> inline Type SQ(Type &x) {return (x * x);}

inline double square(double x)
{
   return x*x;
}

inline long square(long x)
{
   return x*x;
}

inline bool equals(double x, double y)
{
   return (ossimAbs(x-y) <= DBL_EPSILON);
}

inline bool equals(float x, float y)
{
   return (ossimAbs(x-y) <= FLT_EPSILON);
}

inline int factorial(int n)
{
   if(n == 0) return 1;
   return (n * factorial(n - 1));
}

// Round a double to an int correctly.
//inline int irint(double d) { return int(floor(d + 0.5)); }
inline int irint(double d)
{
   return  (int) ((d < 0) ? (d-.5):(d+.5));
}

// Round a double to an int correctly.
//inline long irlong(double d) { return long(floor(d + 0.5)); }
inline int irlong(double d)
{
   return  (long) ((d < 0) ? (d-.5):(d+.5));
}

// Round a double to an int correctly.
//inline int irint(double d) { return int(floor(d + 0.5)); }
inline int irint(float d)
{
   return  (int) ((d < 0) ? (d-.5):(d+.5));
}

// Round a double to an int correctly.
//inline long irlong(double d) { return long(floor(d + 0.5)); }
inline int irlong(float d)
{
   return  (long) ((d < 0) ? (d-.5):(d+.5));
}

// Round a float to an int correctly.
//inline int irint(f32 f) { return int(floor(f + 0.5)); }

// Round a double to an int correctly.
//inline long irlong(f32 f) { return long(floor(f + 0.5)); }

// greatest common divisors of two integers
inline int gcd(int m, int n)
{
   if(n == 0) return m;
   return gcd(n, m % n);
}

//***
// Common conversion functions
//***
inline double ft2mtrs(double feet)
{
   return (feet * MTRS_PER_FT);
}

inline double usft2mtrs(double feet)
{
   return (feet * US_METERS_PER_FT);
}

inline double mtrs2ft(double meters)
{
   return (meters / MTRS_PER_FT);
}

inline double mtrs2usft(double meters)
{
   return (meters / US_METERS_PER_FT);
}

OSSIM_DLL ossimByteOrder ossimGetByteOrder();

OSSIM_DLL double ossimGetDefaultMin(ossimScalarType scalarType);
OSSIM_DLL double ossimGetDefaultMax(ossimScalarType scalarType);
OSSIM_DLL double ossimGetDefaultNull(ossimScalarType scalarType);

OSSIM_DLL ossim_uint32 ossimGetScalarSizeInBytes(ossimScalarType scalarType);


OSSIM_DLL void ossimGetDefaultTileSize(ossimIpt& tileSize);

#endif /* #ifndef COMMON_H */

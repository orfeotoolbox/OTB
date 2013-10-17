//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts, with some additions and modifciations by
// Patrick Melody
//
// Description: Common file for utility functions.
//
//*************************************************************************
// $Id: ossimCommon.h 22197 2013-03-12 02:00:55Z dburken $
#ifndef ossimCommon_HEADER
#define ossimCommon_HEADER 1


// XXX nullify these for now, but eventually replace with a #include 
#define ossimREQUIRE(expr) 
#define ossimENSURE(expr)  
#define ossimCHECK(expr) 
#define ossimSTATIC_CHECK(expr,msg)  

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <cfloat>
#include <cmath>
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
class ossimIpt;
class ossimDpt;
class ossimGpt;
namespace NEWMAT
{
   class Matrix;
}

namespace ossim
{
   OSSIM_DLL std::istream& skipws(std::istream& in);
   OSSIM_DLL bool isWhiteSpace(int c);
    template<class T>
/*     inline bool almostEqual(T x, T y, T tolerence = std::numeric_limits<T>::epsilon()) */
/*         // are x and y within tolerance distance of each other? */
/*         { return std::abs(x - y) <= tolerence; } */
    inline bool almostEqual(T x, T y, T tolerence = FLT_EPSILON)
        // are x and y within tolerance distance of each other?
        { return std::fabs(x - y) <= tolerence; }

    template <class T>
    inline bool inInterval(T x, T a, T b)
	// is x in the closed interval [a,b]?
	{ return x >= a && x <= b; }

    template <class T>
    inline bool inOpenInterval(T x, T a, T b)
	// is x in the open interval (a,b)?
	{ return x > a && x < b; }

   
    /**
     * isnan Test for floating point Not A Number (NAN) value.
     * This should be used test for nan.
     * DO NOT USE operator==.  Like "if (height == ossim::nan())"
     *
     * @return true if nan, false if not.
     *
     * @see nan()
     */
#if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__)
    inline bool isnan(const float& v) { return _isnan(v); }
    inline bool isnan(const double& v) { return _isnan(v); }
#elif defined(sun) || defined(__sun)
#    if defined(__SVR4) || defined(__svr4__)
/* Solaris */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const double& v) { return ( ::isnan(v) ); }
#    else
/* SunOS */
    inline bool isnan(const float& v)  { return ( ::isnan(v) ); }
    inline bool isnan(const double& v) { return ( ::isnan(v) ); }
#   endif
#else
    inline bool isnan(const float& v)  { return ( std::isnan(v) ); }
    inline bool isnan(const double& v) { return ( std::isnan(v) ); }
#endif

/* #if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__) */
/*     inline bool isnan(const float& v) { return _isnan(v); } */
/*     inline bool isnan(const double& v) { return _isnan(v); } */
/* #else */
/*     inline bool isnan(const float& v)  { return ( std::isnan(v) ); } */
/*     inline bool isnan(const double& v) { return ( std::isnan(v) ); } */
/* #endif */

    /** @brief Class lets us see bit patterns of floats. */
    class OSSIM_DLL IntFloatBitCoercion
    {
    public:
       union
       { 
          ossim_int64 intname;
          ossim_float64 floatname;
       } bits;
       IntFloatBitCoercion(ossim_int64 x)   { bits.intname = x;   }
       IntFloatBitCoercion(ossim_float64 x) { bits.floatname = x; }
    };
    
    /**
     * @brief Declaration of nan part of nan() declared here for inline
     * ossim::nan().
     */
    extern OSSIM_DLL_DATA(const IntFloatBitCoercion) nanValue;
    
    /**
     * @brief Method to return ieee floating point double precision NAN.
     *
     * @return ieee floating point double precision NAN.
     *
     * @see isnan()
     *
     * @note casts seamlessly to float and long double.
     *
     * @note Use ossim::isnan(v) to test for nan.
     * Like "if (isnan(myDoubleValue)) { doSomething; }"
     * DO NOT USE operator==.  Like "if (myDoubleValue == ossim::nan())"
     */
    inline double nan() { return nanValue.bits.floatname; }

   template <class T>
   inline T abs(const T& value)
   {
      if(value < 0)
      {
         return -value;
      }
      return value;
   }
    template <class S, class T> 
    inline T lerp(S x, T begin, T end)
	// linear interpolation from begin to end by x
	{ return x*(end - begin) + begin; }
    
    template <class T> 
    inline T inverseLerp(T x, T begin, T end)
	// inverse of lerp: if lerp(z,begin,end) = x, then inverseLerp(x,begin,end) = z.
        // when begin=end, inverseLerp is underconstrained, so we define it to be 0.
	{ return begin == end ? (ossim::isnan(x) ? x : T(0)) : (x - begin)/(end - begin); }

    template <class S, class T> 
    T quaderp(S x, T begin, T middle, T end)
	// quadratic interpolation through begin,middle,end by x
	{
	    // newton interpolation
	    const T a1 = S(2)*(middle - begin);
	    const T a2 = S(2)*(end - middle) - a1;
	    return x*((x - S(0.5))*a2 + a1) + begin;
	}

    template <class T> 
    inline T clamp(T x, T a, T b)
	// clamp x to [a, b]
	{
	    ossimREQUIRE(a <= b);  // input must make sense, disallow nans 
	    
            if (ossim::isnan(x)) return x;
            if (x < a) return a;
            if (b < x) return b;
            return x;
	}
    
    template <class T>
    T wrap(T x, T a, T b)
	// wrap x modularly into [a,b)
	{
	    ossimREQUIRE(a <= b);  // input must make sense, disallow nans 
	    
	    if (a == b && !ossim::isnan(x))
		return a;
	    else {
		T z = x < a ? b : a;
		return std::fmod(x - z, b - a) + z;
	    }
	}

    // XXX to Garrett from PJM:
    //     min and max routines. std::min/max do not in fact correctly handle nan.  
    //     this is troublesome, i think my code always was asserting no nans before values got
    //     through std::min/std::max.  i agree with you that if any of the input is nan,
    //     then the result should be nan, but the STL doesn't really consider the possibility
    //     that inputs to min/max have a "strange" ordering to them.  we could overload
    //     std::min/max to do this behavior but that's evil. for all my whining, i think
    //     we should have ossim::max/max that do the right thing wrt nan.  however:
    //     if we "correctly" handle nans like this, does that subtly break any existing code?
    template <class T>
    inline T min(T a, T b)
       // min of a,b; nan if either a or b are nan
    {
       return (a < b) ? a : b;
    }
    
    template <>
    inline ossim_float32 min<ossim_float32>(ossim_float32 a, ossim_float32 b)
    {
       if (ossim::isnan(a)||ossim::isnan(b))
       {
          return ossim::nan();
       }
       else
       {
          return (a < b) ? a : b;
       }
    }
    
    template <>
    inline ossim_float64  min<ossim_float64>(ossim_float64  a, ossim_float64 b)
    {
       if (ossim::isnan(a)||ossim::isnan(b))
       {
          return ossim::nan();
       }
       else
       {
          return (a < b) ? a : b;
       }
    }
    
    template <class T>
    inline T max(T a, T b)
       // max of a,b; nan if either a or b are nan
    {
       return (a < b) ? b : a;
    }
    template <>
    inline ossim_float32 max<ossim_float32>(ossim_float32 a, ossim_float32 b)
    {
       if (ossim::isnan(b))
          return b;
       else
          return (a < b) ? b : a;       
    }
    template <>
    inline ossim_float64 max<ossim_float64>(ossim_float64 a, ossim_float64 b)
    {
       if (ossim::isnan(b))
          return b;
       else
          return (a < b) ? b : a;       
    }
    inline double radiansToDegrees(double x) { return x*DEG_PER_RAD;}
    inline double degreesToRadians(double x) { return x*RAD_PER_DEG;}
    inline double cosd(double x)             { return std::cos(x*RAD_PER_DEG); }
    inline double sind(double x)             { return std::sin(x*RAD_PER_DEG); }
    inline double tand(double x)             { return std::tan(x*RAD_PER_DEG); }
        // trig fncs with parameter in degrees
        
    inline double acosd(double x)            { return DEG_PER_RAD*std::acos(x); }
    inline double asind(double x)            { return DEG_PER_RAD*std::asin(x); }
    inline double atand(double x)            { return DEG_PER_RAD*std::atan(x); }
    inline double atan2d(double y, double x) { return DEG_PER_RAD*std::atan2(y,x); }
        // trig fncs with result in degrees

    template <class IntType>
    IntType gcd(IntType n, IntType m)
        // greatest common divisor of two ints
        // NB: We use n and m as temporaries in this function, so there is no value
        //     in using const IntType& as we would only need to make a copy anyway...
        {
            IntType zero(0);    // Avoid repeated construction
            
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
            for (;;) {
                if (m == zero)
                    return n;
                n %= m;
                if (n == zero)
                    return m;
                m %= n;
            }
        }

    template <>
    inline int gcd<int>(int n, int m)
        // greatest common divisor specialize for int.
        // XXX this is the old gcd, the above code is the old ossimGcd().
        //     i made this a specialization of the template above,
        //     is this really necessary or more efficient, or can we safely delete this specialization?
        //     i don't know why this fnc must be decled inline, otherwise there's a compile error.
        //     the simple test case doesn't have this problem.
        { 
            if (m == 0)
                return n;
            else
                return gcd(m, n % m);  // gcc can optimize tail calls right?
        }

    template <class IntType>
    IntType lcm(IntType n, IntType m)
        // least common multiple
        // NB: We use n and m as temporaries in this function, so there is no value
        //     in using const IntType& as we would only need to make a copy anyway...
        {
            IntType zero(0);    // Avoid repeated construction
            
            if (n == zero || m == zero) {
                return zero;
            } else {
                n /= gcd(n, m);
                n *= m;
                if (n < zero)
                    n = -n;
                return n;
            }
        }
    
    template<class T> 
    inline T square(T x)
        { return x*x; }
        
    // identical to copysign() but usable in templates
    template <class T>
    inline T sgn(T x) 
        // signum function, returns 0, 1, -1, or nan
        {
            const T table[] = {T(0), T(1), T(-1)};
            return table[((x < T(0)) << 1) | (x > T(0))];
        }
    template <>
    inline ossim_float32 sgn<ossim_float32>(ossim_float32 x) 
        // signum function, returns 0, 1, -1, or nan
        {
            const ossim_float32 table[] = {ossim_float32(0), ossim_float32(1), ossim_float32(-1)};
            return ossim::isnan(x) ? x : table[((x < ossim_float32(0)) << 1) | (x > ossim_float32(0))];
        }
    template <>
    inline ossim_float64 sgn(ossim_float64 x) 
        // signum function, returns 0, 1, -1, or nan
        {
            const ossim_float64 table[] = {ossim_float64(0), ossim_float64(1), ossim_float64(-1)};
            return ossim::isnan(x) ? x : table[((x < ossim_float64(0)) << 1) | (x > ossim_float64(0))];
        }

    template <class R, class F>    
    inline R round(F x)
        // correctly round a float, and cast to desired type R
        {
            R result = static_cast<R>((x < F(0)) ? std::ceil(x - F(0.5)) : std::floor(x + F(0.5)));
            
            ossimENSURE(ossim::isnan(x) == ossim::isnan(result));   // if x is nan, R must be a float type
            return result;
            
            // XXX is this better than use of ceil/floor?:    return static_cast<long long>((x < T(0)) ? x - T(0.5) : x + T(0.5));
        }

    inline double ft2mtrs(double feet)      { return (feet * MTRS_PER_FT); }
    inline double usft2mtrs(double feet)    { return (feet * US_METERS_PER_FT); }
    inline double mtrs2ft(double meters)    { return (meters / MTRS_PER_FT); }
    inline double mtrs2usft(double meters)  { return (meters / US_METERS_PER_FT); }
        // Common conversion functions

    template <class T>
    std::pair<T, T> quadraticRoots(T a, T b, T c)
	// evaluates quadradic formula (positive sqrt is first)
	{
            // XXX could suffer from catastrophic cancellation, 
            // see David Goldberg's "What Every Computer Scientist Should Know About Floating-Point Arithmetic"
	    T s = std::sqrt(b*b - T(4)*a*c);
	    T twoA = T(2)*a;
            return std::pair<T, T>((-b + s)/twoA, (-b - s)/twoA); 
	}

    template <class T>
    inline void memClear(T& var, int z = 0)
	// zero out a variable's memory (for a given value of zero)
	{ memset(&var, z, sizeof(T)); }

    template <class T>
    inline void memClear(T* var)
        // prevent user from accidentally passing in a pointer to his struct
        { ossimSTATIC_CHECK(false, YOU_PROBABLY_WANT_TO_MEMCLEAR_WHAT_THE_POINTER_POINTS_TO_NOT_THE_POINTER_ITSELF); }

    OSSIM_DLL ossimByteOrder byteOrder();
        // test endianness of current machine 

    // values for various scalar types
    OSSIM_DLL double defaultMin(ossimScalarType scalarType);
    OSSIM_DLL double defaultMax(ossimScalarType scalarType);
    OSSIM_DLL double defaultNull(ossimScalarType scalarType);
    OSSIM_DLL ossim_uint32 scalarSizeInBytes(ossimScalarType scalarType);
    
    /** @brief @return true if scalar type is signed, false if not. */
    OSSIM_DLL bool isSigned(ossimScalarType scalarType);

    /**
     * @brief Get actual bits per pixel for a given scalar type.
     * 
     * This is bits used.  (OSSIM_USHORT11 = 11)
     * 
     * @returns The actual bits per pixel.  This will return 0 if the
     * input connection is not hooked up or there is an unhandled scalar type.
     */
    OSSIM_DLL ossim_uint32 getActualBitsPerPixel(ossimScalarType scalarType);
    
    /**
     * @brief Get bits per pixel for a given scalar type.
     * 
     * This the total bits per pixel.  (OSSIM_USHORT11 = 16)
     * 
     * @returns The bits per pixel.  This will return 0 if the
     * input connection is not hooked up or there is an unhandled scalar type.
     */
    OSSIM_DLL ossim_uint32 getBitsPerPixel(ossimScalarType scalarType);  
        
    OSSIM_DLL void defaultTileSize(ossimIpt& tileSize);

    OSSIM_DLL std::string convertHtmlSpecialCharactersToNormalCharacter(const std::string& src);

    /** Heading pitch roll extraction from a matrix. */
    OSSIM_DLL bool matrixToHpr( ossim_float64 hpr[3],
                                const NEWMAT::Matrix& rotation );

    /** Heading pitch roll extraction from a matrix. */
    OSSIM_DLL bool matrixToHpr( ossim_float64 hpr[3],
                                const NEWMAT::Matrix& lsrMatrix,
                                const NEWMAT::Matrix& rotationalMatrix);
    
    OSSIM_DLL void lexQuotedTokens(const std::string& str,
                                   ossim_uint32 start,
                                   const char* whitespace,
                                   const char* quotes,
                                   std::vector<std::string>& tokens, bool& unbalancedQuotes);
   /**
    * Will take a vector of ossimDpt and convert to a string list separated by spaces
    * For example: 
    *      (45,34) (12,34)
    */
   
   OSSIM_DLL void toStringList(ossimString& resultStringOfPoints,
                               const std::vector<ossimDpt>& pointList,
                               char separator=' ');
   OSSIM_DLL void toStringList(ossimString& resultStringOfPoints,
                               const std::vector<ossimIpt>& pointList,
                               char separator = ' ');
   OSSIM_DLL void toStringList(ossimString& resultStringOfPoints,
                               const std::vector<ossimGpt>& pointList,
                               char seaprator = ' ');
   
   /**
    * Will take a string list separated by spaces and convert to a vector of ossimDpts.  It will not clear
    * the passed in list but will append all points found.
    */
   OSSIM_DLL void toVector(std::vector<ossimDpt>& result,
                           const ossimString& stringOfPoints);
   OSSIM_DLL void toVector(std::vector<ossimIpt>& result,
                           const ossimString& stringOfPoints);
   OSSIM_DLL void toVector(std::vector<ossimGpt>& result,
                           const ossimString& stringOfPoints);
   
   /**
    * This will output a vector of values inst a string
    *
    *  (value1,...,valueN)
    *
    * Parenthesis are required
    */ 
   template <class T>
   void toSimpleStringList(ossimString& result,
                           const std::vector<T>& valuesList)
   {
      std::ostringstream out;
      
      if(!valuesList.empty())
      {
         ossim_uint32 idx = 0;
         ossim_uint32 size = (ossim_uint32) (valuesList.size()-1);
         for(idx = 0; idx < size; ++idx)
         {
            out << valuesList[idx] << ",";
         }
         out << valuesList[size];
      }
      
      result = "("+out.str()+")";
   }
   
   /**
    * This will output a vector of values inst a string
    *
    *  (value1,...,valueN)
    *
    * Specialize the char for it will output the actual ascii char instead of the numeric value
    *
    * Parenthesis are required
    */ 
   template <>
   OSSIM_DLL void toSimpleStringList(ossimString& result,
                                     const std::vector<ossim_uint8>& valuesList);
   
   /**
    * Generic function to extract a list of values into a vector of string where
    * the string of points is of the form:
    *
    * (value1,value2, ... , )
    *
    * Parenthesis are required
    */ 
   OSSIM_DLL bool extractSimpleValues(std::vector<ossimString>& values,
                                      const ossimString& stringOfPoints);

   
  template <class T>  bool toSimpleVector(std::vector<T>& result, const ossimString& stringOfPoints)
  {
     std::istringstream in(stringOfPoints);
     ossim::skipws(in);
     bool returnValue = true;
     char c = in.get();
     ossimString value = "";
     if(c == '(')
     {
        c = (char)in.get();
        while( (c!=')') && (c!= '\n') && (in.good()) )
        {
           if(c!= ',')
           {
              value += ossimString(c);
           }
           else
           {
              result.push_back(static_cast<T>(value.toDouble()));
              value = "";
           }
           c = in.get();
        }
     }
     if(c!= ')')
     {
        returnValue = false;
     }
     else
     {
        if(!value.empty())
        {
           result.push_back(static_cast<T>(value.toDouble()));
        }
     }
     
     return returnValue;
  }
   
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
  OSSIM_DLL bool toSimpleVector(std::vector<ossim_uint32>& result,
                                const ossimString& stringOfPoints);
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
  OSSIM_DLL bool toSimpleVector(std::vector<ossim_int32>& result,
                                const ossimString& stringOfPoints);
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
   OSSIM_DLL bool toSimpleVector(std::vector<ossim_uint16>& result,
                                 const ossimString& stringOfPoints);
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
  OSSIM_DLL bool toSimpleVector(std::vector<ossim_int16>& result,
                                const ossimString& stringOfPoints);
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
   OSSIM_DLL bool toSimpleVector(std::vector<ossim_uint8>& result,
                                 const ossimString& stringOfPoints);
   /**
    *  Takes input format of the form:
    *  (value1,value2,...,valueN)
    */
   OSSIM_DLL bool toSimpleVector(std::vector<ossim_int8>& result,
                                 const ossimString& stringOfPoints);
   
        // lex str into tokens starting at position start using whitespace  
	//    chars as delimiters and quotes[0] and quotes[1] as the opening
	//    and closing quotation chars (for quoting tokens containing whitespace).
	//    unbalancedQuotes is true iff it aborted when detecting unbalanced quoting.
	// REQUIRE(whitespace != NULL);
	// REQUIRE(quotes != NULL);
	// REQUIRE(tokens != NULL);
	// REQUIRE(unbalancedQuotes != NULL);

   /**
    * @brief Get the number threads to use from ossimPreferences or OpenThreads.
    *
    * Keyword searched for is:  "ossim_threads"
    *
    * If keyword is not found OpenThreads::GetNumberOfProcessors() is used.
    *
    * @return Number of threads. Default=1
    */
   OSSIM_DLL ossim_uint32 getNumberOfThreads();

}

#endif /* #ifndef ossimCommon_HEADER */

//$$ include.h           include files required by various versions of C++

#ifndef INCLUDE_LIB
#define INCLUDE_LIB

#define use_namespace                   // define name spaces
#include <ossim/base/ossimConstants.h>
#define SETUP_C_SUBSCRIPTS              // allow element access via A[i][j]

// Activate just one of the following 3 statements

//#define SimulateExceptions              // use simulated exceptions
#define UseExceptions                   // use C++ exceptions
//#define DisableExceptions               // do not use exceptions


//#define TEMPS_DESTROYED_QUICKLY         // for compilers that delete
					// temporaries too quickly

//#define TEMPS_DESTROYED_QUICKLY_R       // the same thing but applied
					// to return from functions only

//#define DO_FREE_CHECK                   // check news and deletes balance

#define USING_DOUBLE                    // elements of type double
//#define USING_FLOAT                   // elements of type float

#define bool_LIB 0                      // for compatibility with my older libraries

//#define ios_format_flags ios::fmtflags  // for Gnu 3 and Intel for Linux


//#define _STANDARD_                    // using standard library

//#define use_float_h                   // use float.h for precision data


//#define HAS_INT64                     // if unsigned _int64 is recognised
                                        // used by newran03

// comment out next line if Exception causes a problem
#define TypeDefException

//*********************** end of options set by user ********************

#ifdef use_namespace
namespace RBD_COMMON {
#endif


#ifdef USING_FLOAT                      // set precision type to float
typedef float Real;
typedef double long_Real;
#endif

#ifdef USING_DOUBLE                     // set precision type to double
typedef double Real;
//typedef long double long_Real;
typedef double long_Real;
#endif


#ifdef use_namespace
}
#endif


#ifdef use_namespace
namespace RBD_COMMON {}
namespace RBD_LIBRARIES                 // access all my libraries
{
   using namespace RBD_COMMON;
}
#endif


#endif

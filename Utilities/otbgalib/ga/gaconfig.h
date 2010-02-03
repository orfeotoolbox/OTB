// $Header$
/* ----------------------------------------------------------------------------
  config.h
  mbwall 27jun95
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved
  Copyright (c) 1998-2005 Matthew Wall
                          all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_config_h_
#define _ga_config_h_

/* OTB Modifications: This macro has already defined in the OSSIM library.*/
#undef DYN_CAST

/* ----------------------------------------------------------------------------
PREPROCESSOR DIRECTIVES

  Here are the preprocessor directives that the library understands.

  Some of these are already set up for the OSes with which I am familiar.  See
below for the pre-defined sets.  If you come up with a compiler/platform 
configuration that is not listed here, please send it to me so that i can 
incorporate it into the code base.

  GALIB_USE_ANSI_HEADERS

                      Some operating systems/compilers have old-style headers
                      (e.g. <iostream.h>) while others have new-school
		      headers (e.g. <iostream>).  The default is to use
		      ANSI headers.  If you are using an older compiler,
		      make sure this variable is not defined.

  GALIB_USE_NAMED_TEMPLATES
  GALIB_USE_EMPTY_TEMPLATES
  GALIB_USE_COMP_OPERATOR_TEMPLATES

                      Some older compilers do not require strict, ansi template
		      definitions.  And some compilers require slightly 
		      different ways of specifying templates.  And to make it
		      even more complicated, some depend very much on the type
		      of instantiation method that is defined.  Sigh.

  GALIB_USE_STD_NAMESPACE

                      Some platforms need to explicitly use the std namespace
		      when referring to streams and other std components, but
		      others do not.

  GALIB_USE_STREAMS

                      For systems/environments in which streams are not desired
                      and/or required.  If this is not defined, it turns off 
                      errors and all of the read/write routines for the 
                      classes.

  GALIB_USE_RTTI

                      If your compiler supports RTTI, or if you turn on
                      the RTTI abilities of your compiler, then define this
                      macro.  Without RTTI, if improper casts are made,
		      things will die horribly rather than dropping out in an
		      RTTI-induced exception.

  GALIB_USE_PID

                     Define this if the system has a getpid function that
                     returns something sane and useful.

  GALIB_USE_NO_TEMPLATES

                      For compilers that do not do templates.  The only type
                      of genome available when this is defined is binary 
                      string and any derived classes.  list, tree, and array 
		      all use templates.  You can still use the template code,
		      but you will have to hack it yourself to make it work.

  GALIB_USE_BORLAND_INST

                      For compilers that use the Borland instantiation model.
                      These compilers expect all of the template code to be
		      in the header file.  The Cfront model, on the other
                      hand, expects source files with names similar to the
		      header files, but all of the template code does not
		      need to be in the header files.
		    
		      When you define this flag, the source file that 
		      corresponds to the header file is explicitly included
		      at the end of the header file for all headers that
		      contain templates.

  GALIB_USE_AUTO_INST

                      For compilers that do not do automatic instantiation
                      (such as g++ version 2.6.8) you will have to force
                      instantiations.  When this flag is not defined, GAlib
		      forces an instantiation of all of the template classes
		      that it uses (such as real genome and string genome).

  GALIB_HAVE_NOT_ASSERT

                      Some platforms do not have assert.  So for those
		      platforms we define our own simple version of it.



  USE_GALIB_AS_LIB       For windows shared libraries, one must define whether
  USE_GALIB_AS_DLL       static member data are imported or exported.  You 
                         define one or the other of these, but not both.  The
		  	 default is USE_GALIB_AS_LIB (if you define neither).
			
  COMPILE_GALIB_AS_LIB   If you are compiling the dome library, define one of
  COMPILE_GLAIB_AS_DLL   these to indicate the windows exports.  The default
                         is USE_GALIB_AS_LIB (if you define neither).
 
                
   
  GALIB_USE_RAN1      These specify which random number function to use.  Only
  GALIB_USE_RAN2      one of these may be specified.  You may have to tweak 
  GALIB_USE_RAN3      random.h a bit as well (these functions are not defined 
  GALIB_USE_RAND      the same way on each platform).  For best results, use
  GALIB_USE_RANDOM    ran2 or ran3 (performance is slightly slower than most
  GALIB_USE_RAND48    system RNGs, but you'll get better results).

                      If you want to use another random number generator you
                      must hack random.h directly (see the comments in that
                      file).

  GALIB_BITBASE       The built-in type to use for bit conversions.  This 
                      should be set to the type of the largest integer that
                      your system supports.  If you have long long int then
                      use it.  If you don't plan to use more than 16 or 32
                      bits to represent your binary-to-decimal mappings then
                      you can use something smaller (long int for example).
                      If you do not set this, GAlib will automatically use
                      the size of a long int.  The bitbase determines the
		      maximum number of bits you can use to represent a
		      decimal number in the binary-to-decimal genomes.

  GALIB_BITS_IN_WORD  How many bits are in a word?  For many systems, a word is
                      a char and is 8 bits long.

---------------------------------------------------------------------------- */

// This is a bare-bones os-cpu-compiler detection with no dependencies on any
// other header files or macros.  We try to detect everything based on what we
// will get by default from the compilers.  The result of this is three macros
//   GALIB_OS
//   GALIB_CPU
//   GALIB_COMPILER

// determine the operating system
#if defined(__linux__)
#define GALIB_OS "linux"
#elif defined(__sgi)
#define GALIB_OS "irix"
#elif defined(WIN32)
#define GALIB_OS "win32"
#elif defined(sun)
#define GALIB_OS "solaris"
#elif defined(__APPLE__) && defined(__MACH__) && defined(__ppc__)
#define GALIB_OS "macosx"
#elif defined(macintosh)
#define GALIB_OS "macos"
#elif defined(HPUX10)
#define GALIB_OS "hpux10"
#elif defined(HPUX11)
#define GALIB_OS "hpux11"
#elif defined(_AIX) || defined(AIX)
#define GALIB_OS "aix"
#else
#define GALIB_OS "unknown"
#endif

// determine the cpu
#if defined(__INTEL__) || defined(__i386__) || \
    defined(__x86__) || defined(_M_IX86)
#define GALIB_CPU "i386"
#elif defined(__POWERPC__) || defined(__PPC__) || \
      defined(__powerpc__) || defined(__ppc__) || \
      defined(_POWER)
#define GALIB_CPU "ppc"
#elif defined(__m68k__)
#define GALIB_CPU "68k"
#elif defined(__sgi)
#define GALIB_CPU "mips"
#elif defined(sparc) || defined(__sparc__)
#define GALIB_CPU "sparc"
#elif defined(__HP_aCC)
#define GALIB_CPU "hppa"
#else
#define GALIB_CPU "unknown"
#endif

// determine the compiler
#if defined(__GNUG__) || defined(__GNUC__)
#if __GNUC__ == 4
#define GALIB_COMPILER "gcc4"
#elif __GNUC__ == 3
#define GALIB_COMPILER "gcc3"
#elif __GNUC__ == 2
#define GALIB_COMPILER "gcc2"
#else
#define GALIB_COMPILER "gcc"
#endif
#elif defined(__BORLANDC__)
#if __BORLANDC__ < 0x500
#define GALIB_COMPILER "bcc4"
#elif __BORLANDC__ < 0x530
#define GALIB_COMPILER "bcc52"
#elif __BORLANDC__ < 0x540
#define GALIB_COMPILER "bcc53"
#elif __BORLANDC__ < 0x550
#define GALIB_COMPILER "bcc54"
#elif __BORLANDC__ < 0x560
#define GALIB_COMPILER "bcc55"
#else
#define GALIB_COMPILER "bcc"
#endif
#elif defined(__WATCOMC__)
#define GALIB_COMPILER "watcom"
#elif defined(_MIPS_SIM)
#define GALIB_COMPILER "mipscc"
#elif defined(__MWERKS__)
#define GALIB_COMPILER "mwerks"
#elif defined(_MSC_VER)
#if _MSC_VER == 1300
#define GALIB_COMPILER "vcpp7"
#elif _MSC_VER == 1200
#define GALIB_COMPILER "vcpp6"
#else
#define GALIB_COMPILER "vcpp"
#endif
#elif defined(__HP_aCC)
#define GALIB_COMPILER "acc"
#elif defined(__IBMCPP__)
#define GALIB_COMPILER "xlc"
#else
#define GALIB_COMPILER "unknown"
#endif



// ----------------------------------------------------------------------------
// Here are the defines needed for some of the compilers/OSes on which I have
// been able to test the GA library.  You may have to remove and/or modify
// these to get things to work on your system.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Metrowerks' Codewarrior for MacOS, PalmOS, or Win32 (I have not tested CW
// on other platforms yet).
#if defined(__MWERKS__)
#define GALIB_USE_STREAMS
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_AUTO_INST
#if __option(RTTI)
#define GALIB_USE_RTTI
#endif


// ----------------------------------------------------------------------------
// Symantec C++ for mac.  This compiler does not handle templates very well, 
// so if you want to use any of the template components of GAlib then you will
// probably have to do some hacking to get things to work.
#elif defined(__SC__)
#define GALIB_USE_STREAMS
#define GALIB_USE_BORLAND_INST


// ----------------------------------------------------------------------------
// THINK for mac
#elif defined(THINK_C)
#define GALIB_USE_STREAMS
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_COMP_OPERATOR_TEMPLATES


// ----------------------------------------------------------------------------
// borland c++ compiler
//
// You may or may not need the BORLAND_INST flag defined when you use a borland
// compiler.  I did not need it when I compiled using version 4.0, but I did
// need it when I compiled with an earlier version (I think it was 3.x but I
// do not remember for certain).
//   Note that the default random number generator when using a borland (or
// any PC compiler, for that matter) is the basic system's RNG.
// I did this because of the hassles of 16- vs 32-bit DOS/Windows rubbish.  If
// you want a better RNG, you can use the others in GAlib, but you'll have to
// do a bit of checking to make sure it works with your DOS/Windows config.
// All of the RNGs work fine under all of the 32-bit OSes I've tried, but they
// don't do so well in a 16-bit OS.
//  Use the randtest example to check GAlib's RNG after you compile everything.
#elif defined(__BORLANDC__)
//#define GALIB_USE_RAND	// comment this if you're using a 32-bit OS
#define GALIB_USE_RTTI
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_STREAMS
//#define GALIB_USE_PID
#define GALIB_USE_EMPTY_TEMPLATES
//#define GALIB_USE_ANSI_HEADERS
//#define GALIB_USE_STD_NAMESPACE
//#define GALIB_USE_COMP_OPERATOR_TEMPLATES

//#pragma warning (disable : 8027)    // switch statements are not inlined
//#pragma warning (disable : 8004)    // unused variable warnings are lame


// ----------------------------------------------------------------------------
// MicroSoft's Visual C++ programming environment.
//
// this has been test with:
//   vcpp6 (12.00.8804)
//   vcpp6 (12.00.8168)
//   vcpp7 (13.00.9466)
//   vcpp7 (13.10.3077)
//
#elif defined(_MSC_VER)
// visual studio uses the borland model of template instantiation.
#define GALIB_USE_BORLAND_INST
// let visual studio do its own instantations, so by default do not force them.
#define GALIB_USE_AUTO_INST
// use ansi headers with vcpp7.  it is a good idea to use them in vcpp6 as well
// but some vcpp6 apps are built with non-ansi headers, in which case you 
// should build with the non-ansi headers so that you do not cross the streams.
#define GALIB_USE_ANSI_HEADERS
// beware of using streams in an MFC application.  many nasties lurk therein...
#define GALIB_USE_STREAMS
// we default to using std things in the std namespace, but depending on the
// version of vcpp you are using and depending on the libraries with which you
// will use GAlib, you might have to turn this one off.
#define GALIB_USE_STD_NAMESPACE
// use the pid only on winnt and derivatives.  win95/98/ME do not have it.
// this requires unistd.h, which you may or may not have (depending on the 
// way that you installed the compiler).
//#define GALIB_USE_PID
// GAlib behaves much better in unknown conditions when rtti is enabled, but
// you might have to disable it, depending on the linkage and compilation 
// options you need for other components you are using with GAlib.
#if defined(_CPPRTTI)
#define GALIB_USE_RTTI
#else
#undef GALIB_USE_RTTI
#endif

// there are many warnings from vcpp, many of which we can safely ignore.
//#pragma warning (disable : 4244)    // int-to-float warnings
#pragma warning (disable : 4305)    // double-to-float warnings
#pragma warning (disable : 4355)    // allow us to use this in constructors
//#pragma warning (disable : 4250)    // dominated multiple inherits


// ----------------------------------------------------------------------------
// GNU compiler
//
// there are some subtle differences in the way the gcc compiler handles
// templates from one version to another.  the latest versions are much more
// strictly adherant to the c++ standards, although you can relax that with
// the permissive option.  we try to build this library without the use of the
// permissive option.
//
// there are significant changes between the 3.3 and 3.4 releases of gcc.  and
// of course there are major differences between the 2.x and 3.x versions, but
// those affect us mostly with respect to the use of the std libraries.
#elif defined(__GNUG__)
#define GALIB_USE_RTTI
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_STREAMS
#define GALIB_USE_PID
#define GALIB_USE_EMPTY_TEMPLATES
#define GALIB_NEED_INSTANTIATION_PREFIX
#if __GNUC__ > 2
#define GALIB_USE_ANSI_HEADERS
#define GALIB_USE_STD_NAMESPACE
#define GALIB_USE_COMP_OPERATOR_TEMPLATES
#endif


// ----------------------------------------------------------------------------
// irix 5.3 and irix 6.x
#elif defined(__sgi)
#define GALIB_USE_STREAMS
#define GALIB_USE_PID

#include <sgidefs.h>
#if (_MIPS_SIM == _MIPS_SIM_NABI32)
#define GALIB_USE_RTTI
#define GALIB_USE_AUTO_INST
#define GALIB_USE_BORLAND_INST
#elif (_MIPS_SIM == _MIPS_SIM_ABI64)
#define GALIB_USE_RTTI
#elif (_MIPS_SIM == _MIPS_SIM_ABI32)
#define GALIB_USE_AUTO_INST
#endif


// ----------------------------------------------------------------------------
// IBM visual age c++ compiler
#elif defined(__IBMCPP__)
// the -qrtti option turns rtti on and off, but i do not know the
// corresponding preprocessor directive to sense it.
#define GALIB_USE_RTTI
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_AUTO_INST
#define GALIB_USE_ANSI_HEADERS
#define GALIB_USE_STREAMS
#define GALIB_USE_STD_NAMESPACE
#define GALIB_USE_PID
#define GALIB_USE_NAMED_TEMPLATES


// ----------------------------------------------------------------------------
// HP aCC compiler
#elif defined(__HP_aCC)
#define GALIB_USE_RTTI
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_AUTO_INST
//#define GALIB_USE_ANSI_HEADERS
#define GALIB_USE_STREAMS
//#define GALIB_USE_STD_NAMESPACE
#define GALIB_USE_PID

//#pragma disable warning 829 // do not care about string literal conversions


// ----------------------------------------------------------------------------
// This is an unknown/untested platform and/or compiler.  The defaults below 
// might work for you, but then again, they might not.  You may have to adjust
// the values of the macros until everything works properly.  Comment out the
// #error directive to allow things to compile properly.
#else
#error   Unknown/untested compiler/operating system!  Check these settings!

#define GALIB_USE_RTTI
#define GALIB_USE_BORLAND_INST
#define GALIB_USE_AUTO_INST
#define GALIB_USE_ANSI_HEADERS
#define GALIB_USE_STREAMS
#define GALIB_USE_STD_NAMESPACE
#define GALIB_USE_PID
#endif











// deal with assertions.  at some point we might want a proper definition of 
// assert here for platforms that do not have it.
#if defined(GALIB_HAVE_NOT_ASSERT)
#include <stdlib.h>
#define assert(x) \
  { \
    if(! (x)) { \
      fprintf(stderr, "assertion failed at line %d in file %s\n", \
	      __LINE__, __FILE__); \
      exit(1); \
    } \
  }
#else
#include <assert.h>
#endif


// some compilers use one syntax, others use a different syntax.
#if defined(GALIB_NEED_INSTANTIATION_PREFIX)
#define GALIB_INSTANTIATION_PREFIX template class
#else
#define GALIB_INSTANTIATION_PREFIX
#endif


// If no RNG has been selected, use the ran2 generator by default
#if !defined(GALIB_USE_RAND) && \
    !defined(GALIB_USE_RANDOM) && \
    !defined(GALIB_USE_RAND48) && \
    !defined(GALIB_USE_RAN2) && \
    !defined(GALIB_USE_RAN3)
#define GALIB_USE_RAN2
#endif


// This defines how many bits are in a single word on your system.  Most 
// systems have a word length of 8 bits.
#ifndef GALIB_BITS_IN_WORD
#define GALIB_BITS_IN_WORD 8
#endif


// Use this to set the maximum number of bits that can be used in binary-to-
// decimal conversions.  You should make this type the largest integer type 
// that your system supports.
#ifndef GALIB_BITBASE
#define GALIB_BITBASE long int
#endif

// If the system/compiler understands C++ casts, then we use them.  Otherwise
// we default to the C-style casts.  The macros make explicit the fact that we
// are doing casts.

#if defined(GALIB_USE_RTTI)
#define DYN_CAST(type,x) (dynamic_cast<type>(x))
#define CON_CAST(type,x) (const_cast<type>(x))
#define STA_CAST(type,x) (static_cast<type>(x))
#define REI_CAST(type,x) (reinterpret_cast<type>(x))
#else
#define DYN_CAST(type,x) ((type)(x))
#define CON_CAST(type,x) ((type)(x))
#define STA_CAST(type,x) ((type)(x))
#define REI_CAST(type,x) ((type)(x))
#endif


// Windows is brain-dead about how to export things, so we do this to keep the
// code (somewhat) cleaner but still accomodate windows' stupidity.
#if defined(COMPILE_GALIB_AS_DLL)
#define GA_DLLDECL __declspec(dllexport)
#elif defined(USE_GALIB_AS_DLL)
#define GA_DLLDECL __declspec(dllimport)
#else
#define GA_DLLDECL
#endif






/* ----------------------------------------------------------------------------
DEFAULT OPERATORS

  These directives determine which operators will be used by default for each
of the objects in GAlib.
---------------------------------------------------------------------------- */
// scaling schemes
#define USE_LINEAR_SCALING           1
#define USE_SIGMA_TRUNC_SCALING      1
#define USE_POWER_LAW_SCALING        1
#define USE_SHARING                  1

// selection schemes
#define USE_RANK_SELECTOR            1
#define USE_ROULETTE_SELECTOR        1
#define USE_TOURNAMENT_SELECTOR      1
#define USE_DS_SELECTOR              1
#define USE_SRS_SELECTOR             1
#define USE_UNIFORM_SELECTOR         1

// These are the compiled-in defaults for various genomes and GA objects
#define DEFAULT_SCALING              GALinearScaling
#define DEFAULT_SELECTOR             GARouletteWheelSelector
#define DEFAULT_TERMINATOR           TerminateUponGeneration

#define DEFAULT_1DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_1DBINSTR_MUTATOR     FlipMutator
#define DEFAULT_1DBINSTR_COMPARATOR  BitComparator
#define DEFAULT_1DBINSTR_CROSSOVER   OnePointCrossover
#define DEFAULT_2DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_2DBINSTR_MUTATOR     FlipMutator
#define DEFAULT_2DBINSTR_COMPARATOR  BitComparator
#define DEFAULT_2DBINSTR_CROSSOVER   OnePointCrossover
#define DEFAULT_3DBINSTR_INITIALIZER UniformInitializer
#define DEFAULT_3DBINSTR_MUTATOR     FlipMutator
#define DEFAULT_3DBINSTR_COMPARATOR  BitComparator
#define DEFAULT_3DBINSTR_CROSSOVER   OnePointCrossover

#define DEFAULT_BIN2DEC_ENCODER      GABinaryEncode
#define DEFAULT_BIN2DEC_DECODER      GABinaryDecode
#define DEFAULT_BIN2DEC_COMPARATOR   BitComparator

#define DEFAULT_1DARRAY_INITIALIZER  NoInitializer
#define DEFAULT_1DARRAY_MUTATOR      SwapMutator
#define DEFAULT_1DARRAY_COMPARATOR   ElementComparator
#define DEFAULT_1DARRAY_CROSSOVER    OnePointCrossover
#define DEFAULT_2DARRAY_INITIALIZER  NoInitializer
#define DEFAULT_2DARRAY_MUTATOR      SwapMutator
#define DEFAULT_2DARRAY_COMPARATOR   ElementComparator
#define DEFAULT_2DARRAY_CROSSOVER    OnePointCrossover
#define DEFAULT_3DARRAY_INITIALIZER  NoInitializer
#define DEFAULT_3DARRAY_MUTATOR      SwapMutator
#define DEFAULT_3DARRAY_COMPARATOR   ElementComparator
#define DEFAULT_3DARRAY_CROSSOVER    OnePointCrossover

#define DEFAULT_1DARRAY_ALLELE_INITIALIZER  UniformInitializer
#define DEFAULT_1DARRAY_ALLELE_MUTATOR      FlipMutator
#define DEFAULT_1DARRAY_ALLELE_COMPARATOR   ElementComparator
#define DEFAULT_1DARRAY_ALLELE_CROSSOVER    OnePointCrossover
#define DEFAULT_2DARRAY_ALLELE_INITIALIZER  UniformInitializer
#define DEFAULT_2DARRAY_ALLELE_MUTATOR      FlipMutator
#define DEFAULT_2DARRAY_ALLELE_COMPARATOR   ElementComparator
#define DEFAULT_2DARRAY_ALLELE_CROSSOVER    OnePointCrossover
#define DEFAULT_3DARRAY_ALLELE_INITIALIZER  UniformInitializer
#define DEFAULT_3DARRAY_ALLELE_MUTATOR      FlipMutator
#define DEFAULT_3DARRAY_ALLELE_COMPARATOR   ElementComparator
#define DEFAULT_3DARRAY_ALLELE_CROSSOVER    OnePointCrossover

#define DEFAULT_STRING_INITIALIZER   UniformInitializer
#define DEFAULT_STRING_MUTATOR       FlipMutator
#define DEFAULT_STRING_COMPARATOR    ElementComparator
#define DEFAULT_STRING_CROSSOVER     UniformCrossover

#define DEFAULT_REAL_INITIALIZER     UniformInitializer
#define DEFAULT_REAL_MUTATOR         GARealGaussianMutator
#define DEFAULT_REAL_COMPARATOR      ElementComparator
#define DEFAULT_REAL_CROSSOVER       UniformCrossover

#define DEFAULT_TREE_INITIALIZER     NoInitializer
#define DEFAULT_TREE_MUTATOR         SwapSubtreeMutator
#define DEFAULT_TREE_COMPARATOR      TopologyComparator
#define DEFAULT_TREE_CROSSOVER       OnePointCrossover

#define DEFAULT_LIST_INITIALIZER     NoInitializer
#define DEFAULT_LIST_MUTATOR         SwapMutator
#define DEFAULT_LIST_COMPARATOR      NodeComparator
#define DEFAULT_LIST_CROSSOVER       OnePointCrossover

#endif

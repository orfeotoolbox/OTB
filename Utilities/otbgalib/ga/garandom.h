// $Header$
/* ----------------------------------------------------------------------------
  random.h
  mbwall 29jun95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  Initialize the random function by using the current time as the seed.
  We use the pid as well as the time for the random seed in an attempt to get
rid of some of the periodicity from the low bits when using only the time as 
the random seed.
  These routines use the random/srandom routines to generate random numbers.
The documentation says that rand/srand is about 30% faster than random/srandom,
but rand/srand has a significantly smaller period so you'll get less random
results.  For the best results, use a RNG with a larger period (such as ran2).
  This is *not* implemented as a separate RNG class because I wanted to be able
to inline everything as much as possible.  Also, we don't need to switch RNGs
for most purposes - I assume that compiling in the RNG is ok.  In addition, 
many methods make use of RNG calls of a global nature - they should not contain
a RNG, and they may not have access to a RNG other than the global functions.

GARandomInt, GARandomFloat, GARandomDouble
  Return a number selected at random within the bounds low and high, inclusive.
Don't forget the 'inclusive' part!  If you're using this to get the index of
an element in an array, be sure to use the last element of the array, not the
number of elements in the array as the arguement for 'high'.
  If you screw up and pass a value for low that is less than the value for
high, this routine does not complain.  It will merrily return a possibly 
negative value.  We need speed here, so there's no error checking.

GARandomBit
  This is a faster implementation of GARandomInt(0,1).

GAFlipCoin
  Simulate a coin toss.  Use specified probability to bias toss.

GAUnitGaussian
  Returns a number from a Gaussian distribution with mean 0 and stddev of 1

GAGaussianFloat, GAGaussianDouble
  Scaled versions of the gaussian distribution.  You must specify a stddev, 
then these functions scale the distribution to that deviation.  Mean is still 0
---------------------------------------------------------------------------- */
#ifndef _ga_random_h_
#define _ga_random_h_

#include <stdlib.h>
#include <ga/gatypes.h>
#include <ga/gaconfig.h>

// Here we determine which random number generator will be used.  The critical
// parts here are the name of the random number generator (e.g. rand or random)
// the name of the seed routine (e.g. srand or srandom), the maximum value of
// the number returned by the generator (e.g. RAND_MAX or LONG_MAX), and the
// C++ type of the number returned (e.g. int or long int).
//   If you want to use your own random function, this is where to declare it.
// Just set the _GA_RND macros to the appropriate values for your random number
// generator.
//   There are *many* ways to speed things up here if you know specifics about
// the machine on which you're going to be running the code.  For example, if
// you have a floating coprocessor then you might want to use ran3 and convert
// it so that it is entirely floating point (as described in NR in C).  Or if
// you're really daring, substitute some assembly code.  For many bit-flipping
// genetic algorithms the random number generator is the bottleneck, so this
// isn't totally useless musing...

#if defined(GALIB_USE_RAN1) || defined(GALIB_USE_RAN2) || defined(GALIB_USE_RAN3)

#if defined(GALIB_USE_RAN1)

#define _GA_RND             garan1
#define _GA_RND_SEED        gasran1
void gasran1(unsigned int seed=1);
float garan1();

#elif defined(GALIB_USE_RAN2)

#define _GA_RND             garan2
#define _GA_RND_SEED        gasran2
void gasran2(unsigned int seed=1);
float garan2();

#elif defined(GALIB_USE_RAN3)

#define _GA_RND             garan3
#define _GA_RND_SEED        gasran3
void gasran3(unsigned int seed=1);
float garan3();

#endif

inline int GARandomInt(){ return _GA_RND() > 0.5 ? 1 : 0; }
inline int GARandomInt(int low, int high){ 
  float val=STA_CAST(float,high)-STA_CAST(float,low)+(float)1; 
  val*=_GA_RND(); 
  return (STA_CAST(int,val)+low);
}

inline double GARandomDouble(){ return _GA_RND(); }
inline double GARandomDouble(double low, double high){
  double val=high-low; val*=_GA_RND(); return val+low;
}

inline float GARandomFloat(){ return _GA_RND(); }
inline float GARandomFloat(float low, float high){
  float val=high-low; val*=_GA_RND(); return val+low;
}

#elif defined(GALIB_USE_RAND) || defined(GALIB_USE_RANDOM) || defined(GALIB_USE_RAND48)

#error  It is usually a bad idea to use the system randum number generator!
#error  Be sure that your system generator works properly, then comment
#error  these lines to go ahead and do your compilation.

#include <math.h>
#include <limits.h>

#if defined(GALIB_USE_RANDOM) 

#define _GA_RND             random
#define _GA_RND_SEED        srandom
#define _GA_RND_MAX         LONG_MAX

#elif defined(GALIB_USE_RAND48)

#define _GA_RND             lrand48
#define _GA_RND_SEED        srand48
#define _GA_RND_MAX         LONG_MAX

#else

#define _GA_RND             rand
#define _GA_RND_SEED        srand
#define _GA_RND_MAX         RAND_MAX

#endif

inline int GARandomInt(){ return _GA_RND() > _GA_RND_MAX/2 ? 1 : 0; }
inline int GARandomInt(int low, int high){ 
  return low + _GA_RND() % (high-low+1);
}

inline double GARandomDouble(){ 
  double val=_GA_RND(); val/=_GA_RND_MAX; return val;
}
inline double GARandomDouble(double low, double high){
  double val=high-low; val*=_GA_RND(); val/=_GA_RND_MAX; val+=low; return val;
}

inline float GARandomFloat(){
  float val=_GA_RND(); val/=_GA_RND_MAX; return val;
}
inline float GARandomFloat(float low, float high){
  float val=high-low; val*=_GA_RND(); val/=_GA_RND_MAX; val+=low; return val;
}

#endif






unsigned int GAGetRandomSeed();
void GARandomSeed(unsigned int seed=0);
void GAResetRNG(unsigned int seed);
int GARandomBit();
double GAUnitGaussian();

inline GABoolean GAFlipCoin(float p){
  return((p == 1.0) ? gaTrue : (p == 0.0) ? gaFalse :
	 ((GARandomFloat() <= p) ? gaTrue : gaFalse));
}

inline float GAGaussianFloat(float dev){ return (float)GAUnitGaussian()*dev; }
inline double GAGaussianDouble(double dev){ return GAUnitGaussian() * dev; }

const char* GAGetRNG();

#endif

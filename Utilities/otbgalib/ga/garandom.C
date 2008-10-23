// $Header$
/* ----------------------------------------------------------------------------
  random.C
  mbwall 5sep95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Random number stuff for use in GAlib.
---------------------------------------------------------------------------- */
#include <ga/garandom.h>
#include <time.h>
#include <math.h>
#include <string.h>

static void bitseed(unsigned int seed=1);

// If the machine has multiple processes, use the PID to help make the random
// number generator seed more random.
#if defined(GALIB_USE_PID)
#include <unistd.h>
#define _GA_PID * getpid()
#else
#define _GA_PID
#endif

// Return a string indicating which random number generator was compiled-in to
// the library.
const char*
GAGetRNG() {
#if defined(GALIB_USE_RAN1)
  return "RAN1";
#elif defined(GALIB_USE_RAN2)
  return "RAN2";
#elif defined(GALIB_USE_RAN3)
  return "RAN3";
#elif defined(GALIB_USE_RAND)
  return "RAND";
#elif defined(GALIB_USE_RANDOM)
  return "RANDOM";
#elif defined(GALIB_USE_RAND48)
  return "RAND48";
#else
  return "UNKNOWN";
#endif
}

// Seed the random number generator with an appropriate value.  We seed both 
// the random number generator and the random bit generator.  Set the seed only
// if a seed is not specified.  If a seed is specified, then set the seed to 
// the specified value and use it.  We remember the seed so that multiple calls
// to this function with the same seed do not reset the generator.  Subsequent
// calls to this function with a different seed will initialize the generator
// to the new seed.  Multiple calls with a value of 0 do nothing (we do *not*
// re-seed the generator because 0 is the default value and we don't want 
// people to re-seed the generator inadvertantly).
//   Some systems return a long as the return value for time, so we need to be
// sure to get whatever variation from it that we can since our seed is only an
// unsigned int.
static unsigned int seed=0;

unsigned int GAGetRandomSeed() { return seed; }

void 
GARandomSeed(unsigned int s) {
  if(s == 0 && seed == 0) {
    unsigned long int tmp;
    while(seed == 0) {
      tmp = time(NULL) _GA_PID;
      for(unsigned int i=0; i<GALIB_BITS_IN_WORD*sizeof(unsigned int); i++)
	seed += (tmp & (1 << i));
    }
    _GA_RND_SEED (seed); 
    bitseed(seed);
  }
  else if(s != 0 && seed != s) {
    seed = s;
    _GA_RND_SEED (seed); 
    bitseed(seed);
  }
}

// Similar to setting the random seed, but this one sets it as long as the
// specified seed is non-zero.
void
GAResetRNG(unsigned int s) {
  if(s != 0) {
    seed = s;
    _GA_RND_SEED (seed); 
    bitseed(seed);
  }
}






// Return a number from a unit Gaussian distribution.  The mean is 0 and the
// standard deviation is 1.0.
//   First we generate two uniformly random variables inside the complex unit 
// circle.  Then we transform these into Gaussians using the Box-Muller 
// transformation.  This method is described in Numerical Recipes in C 
// ISBN 0-521-43108-5 at http://world.std.com/~nr
//   When we find a number, we also find its twin, so we cache that here so 
// that every other call is a lookup rather than a calculation.  (I think GNU 
// does this in their implementations as well, but I don't remember for 
// certain.)
double
GAUnitGaussian(){
  static GABoolean cached=gaFalse;
  static double cachevalue;
  if(cached == gaTrue){
    cached = gaFalse;
    return cachevalue;
  }

  double rsquare, factor, var1, var2;
  do{
    var1 = 2.0 * GARandomDouble() - 1.0;
    var2 = 2.0 * GARandomDouble() - 1.0;
    rsquare = var1*var1 + var2*var2;
  } while(rsquare >= 1.0 || rsquare == 0.0);

  double val = -2.0 * log(rsquare) / rsquare;
  if(val > 0.0) factor = sqrt(val);
  else           factor = 0.0;	// should not happen, but might due to roundoff

  cachevalue = var1 * factor;
  cached = gaTrue;

  return (var2 * factor);
}







// This is the random bit generator Method II from numerical recipes in C.  The
// seed determines where in the cycle of numbers the generator will start, so
// we don't need full 'long' precision in the argument to the seed function.

#define IB1 1
#define IB2 2
#define IB5 16
#define IB18 131072L
#define MASK (IB1+IB2+IB5)

static unsigned long iseed;

void 
bitseed(unsigned int seed) {
  iseed = seed;
}

int 
GARandomBit() {
  if (iseed & IB18) {
    iseed=((iseed ^ MASK) << 1) | IB1;
    return 1;
  } else {
    iseed <<= 1;
    return 0;
  }
}

#undef MASK
#undef IB18
#undef IB5
#undef IB2
#undef IB1









// The following random number generators are from Numerical Recipes in C.
// I have split them into a seed function and random number function.

// The ran1 pseudo-random number generator.  This one is OK to use as long as
// you don't call it more than about 10^8 times, so for any long GA runs you'd
// better use something with a longer period.

#if defined(GALIB_USE_RAN1)

#define IA 16807L
#define IM 2147483647L
#define AM (1.0/IM)
#define IQ 127773L
#define IR 2836L
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

static long iy=0;
static long iv[NTAB];
static long idum=0;

void
gasran1(unsigned int seed) {
  int j;
  long k;

  idum = seed;
  if (idum == 0) idum=1;
  if (idum < 0) idum = -idum;
  for (j=NTAB+7;j>=0;j--) {
    k=(idum)/IQ;
    idum=IA*(idum-k*IQ)-IR*k;
    if (idum < 0) idum += IM;
    if (j < NTAB) iv[j] = idum;
  }
  iy=iv[0];
}

float garan1() {
  int j;
  long k;
  float temp;

  k=(idum)/IQ;
  idum=IA*(idum-k*IQ)-IR*k;
  if (idum < 0) idum += IM;
  j=iy/NDIV;
  iy=iv[j];
  iv[j] = idum;
  if ((temp=AM*iy) > RNMX) return RNMX;
  else return temp;
}

#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

#endif




// The ran2 pseudo-random number generator.  It has a period of 2 * 10^18 and
// returns a uniform random deviate on the interval (0.0, 1.0) excluding the
// end values.  idum initializes the sequence, so we create a separate seeding
// function to set the seed.  If you reset the seed then you re-initialize the
// sequence.

#if defined(GALIB_USE_RAN2)

#define IM1 2147483563L
#define IM2 2147483399L
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014L
#define IA2 40692L
#define IQ1 53668L
#define IQ2 52774L
#define IR1 12211L
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

static long idum2=123456789;
static long iy=0;
static long iv[NTAB];
static long idum=0;

void 
gasran2(unsigned int seed) {
  int j;
  long k;

  idum = STA_CAST(long,seed);
  if (idum == 0) idum=1;
  if (idum < 0) idum = -idum;
  idum2=(idum);
  for (j=NTAB+7;j>=0;j--) {
    k=(idum)/IQ1;
    idum=IA1*(idum-k*IQ1)-k*IR1;
    if (idum < 0) idum += IM1;
    if (j < NTAB) iv[j] = idum;
  }
  iy=iv[0];
}

float
garan2() {
  int j;
  long k;
  float temp;

  k=(idum)/IQ1;
  idum=IA1*(idum-k*IQ1)-k*IR1;
  if (idum < 0) idum += IM1;
  k=idum2/IQ2;
  idum2=IA2*(idum2-k*IQ2)-k*IR2;
  if (idum2 < 0) idum2 += IM2;
  j=iy/NDIV;
  iy=iv[j]-idum2;
  iv[j] = idum;
  if (iy < 1) iy += IMM1;
  if ((temp=AM*iy) > RNMX) return RNMX;
  else return temp;
}

#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

#endif


#if defined(GALIB_USE_RAN3)

// The ran3 pseudo-random number generator.  It is *not* linear congruential.

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

static int inext,inextp;
static long ma[56];

void 
gasran3(unsigned int seed) {
  long idum = seed;
  long mj,mk;
  int i,ii,k;

  mj=labs(MSEED-labs(idum));
  mj %= MBIG;
  ma[55]=mj;
  mk=1;
  for (i=1;i<=54;i++) {
    ii=(21*i) % 55;
    ma[ii]=mk;
    mk=mj-mk;
    if (mk < MZ) mk += MBIG;
    mj=ma[ii];
  }
  for (k=1;k<=4;k++)
    for (i=1;i<=55;i++) {
      ma[i] -= ma[1+(i+30) % 55];
      if (ma[i] < MZ) ma[i] += MBIG;
    }
  inext=0;
  inextp=31;
}

float 
garan3() {
  long mj;
  int i,ii,k;
  
  if (++inext == 56) inext=1;
  if (++inextp == 56) inextp=1;
  mj=ma[inext]-ma[inextp];
  if (mj < MZ) mj += MBIG;
  ma[inext]=mj;
  return mj*FAC;
}

#undef MBIG
#undef MSEED
#undef MZ
#undef FAC

#endif

// $Header$
/* ----------------------------------------------------------------------------
  bincvt.C
  mbwall 29jun95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Binary-to-decimal converters.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ga/gaconfig.h>
#include <ga/gaerror.h>
#include <ga/gabincvt.h>


// These numbers are machine-specific and are a function of the word length of
// the OS you are running.  The binary string cannot be too long or else we 
// can't represent it.  And we don't do any special multi-word handling, so we
// limit the number of bits that our converters can work on.
#ifndef BITBASE
#define BITBASE long int
#endif

// Define the number of bits based on the builtin type
#define _GA_MAX_BITS          (int)(GALIB_BITS_IN_WORD*sizeof(GALIB_BITBASE))


// These are publicly available, but we don't want to advertise them.  They are
// mostly just for testing purposes.  These are unscaled versions of the 
// converter routines.
int GABinaryEncode(unsigned BITBASE, GABit* bits, unsigned int);
int GABinaryDecode(float&, const GABit* bits, unsigned int);

// If you write your own encoder/decoder you may want to use these to verify
// the parameters.
int GACheckEncoding(float&,unsigned int&,float,float,unsigned BITBASE &);
int GACheckDecoding(unsigned int&);


// This stuff is private.
extern char _gaerrbuf1[];
extern char _gaerrbuf2[];
static int _GAEncodeBase(unsigned int, unsigned BITBASE, GABit *, int, int);





/* ----------------------------------------------------------------------------
  Utilities to check the values for proper sizes.
---------------------------------------------------------------------------- */
int
GACheckDecoding(unsigned int& nbits)
{
  if((int)nbits >= _GA_MAX_BITS){
    sprintf(_gaerrbuf1,"string is %d bits, max is %d", nbits, _GA_MAX_BITS-1);
    GAErr(GA_LOC, "GACheckDecoding", gaErrBinStrTooLong, _gaerrbuf1);
    nbits = _GA_MAX_BITS-1;
    return 1;
  }
  return 0;
}

int
GACheckEncoding(float& val, unsigned int& nbits,
		float minval, float maxval, unsigned BITBASE & nintervals)
{
  int status = 0;
  if((int)nbits >= _GA_MAX_BITS){
    sprintf(_gaerrbuf1,"string is %d bits, max is %d", nbits, _GA_MAX_BITS-1);
    GAErr(GA_LOC, "GACheckEncoding", gaErrBinStrTooLong, _gaerrbuf1);
    nbits = _GA_MAX_BITS-1;
    status = 1;
  }

  nintervals = 1;	      // this type limits the number of bits we can do
  nintervals <<= nbits;
  nintervals--;

  double interval = (maxval-minval)/(double)nintervals;
  double actual = (val-minval)/interval;  // how many intervals we need
  nintervals = (unsigned BITBASE)actual;  // make it an integer
  actual = minval+(double)nintervals*interval; // get value we can represent

  if(actual != val){
    sprintf(_gaerrbuf1,"desired: %f\tactual: %f\tdiscretization: %f",
	    val, actual, interval);
    sprintf(_gaerrbuf2,"  nbits: %d\t\tmin: %f\t\tmax: %f",
	    nbits, minval, maxval);
    GAErr(GA_LOC, "GACheckEncoding", gaErrDataLost, _gaerrbuf1, _gaerrbuf2);
    val = (float)actual;
    status = 1;
  }
  return status;
}



/* ----------------------------------------------------------------------------
  Utility routine to encode bits of a decimal number.  This routine recursively
loops through the decimal value and grabs the remainder (modulo the base) and
sticks the result into each 'bit' (if base is 2 then they're bits, otherwise 
they're the equivalent for whatever base you're doing).
  This will only do unsigned integers.
---------------------------------------------------------------------------- */
static int
_GAEncodeBase(unsigned int base, unsigned BITBASE val, 
	      GABit * binstr, int n, int c)
{
  int status = 0;
  if(c < 0) return 1;		// if this happens we should post an error
				// it means we didn't get a perfect encoding
  unsigned BITBASE modval = val % base;
  binstr[c] = STA_CAST(GABit,modval);  // the case is ok since module is small
  unsigned BITBASE quotient = val / base;
  if(quotient) status = _GAEncodeBase(base, quotient, binstr, n, c-1);
  return status;
}







/* ----------------------------------------------------------------------------
   Convert the string of bits into a decimal value.  This routine does no
scaling - the result that it generates will be a power of two.  You must
specify a number of bits so that we know how many bits to consider.  If you
specify too many bits, then we reset to the max we can handle and do the 
conversion using those bits.
   We return 1 if there was a problem, otherwise 0.
---------------------------------------------------------------------------- */
int
GABinaryDecode(float& result, const GABit* bits, unsigned int nbits)
{
  if(bits == (GABit *)0 || nbits == 0){ result = 0.0; return 1; }
  int status = GACheckDecoding(nbits);

  unsigned BITBASE maxint=1;
  float sum=0.0;
  for(int i=nbits-1; i>(-1); i--){   // most significant bit first
//  for(int i=0; i<nbits; i++){ // least significant bit first
    if(bits[i] != 0) sum += STA_CAST(float, maxint);
    maxint <<= 1;
  }
  result = sum;
  return status;
}

/* ----------------------------------------------------------------------------
   Same as the previous decoder, but here we scale the result based on the
limits that were passed to us.
---------------------------------------------------------------------------- */
int
GABinaryDecode(float& result, const GABit* bits,
	       unsigned int nbits, float minval, float maxval)
{
  if(bits == (GABit *)0 || nbits == 0){ result = 0.0; return 1; }
  int status = GACheckDecoding(nbits);

  unsigned BITBASE maxint=1;
  float sum=0.0;
  for(int i=nbits-1; i>(-1); i--){	// 0th bit is most significant
//  for(int i=0; i<nbits; i++){        // least significant bit first
    if(bits[i]) sum += STA_CAST(float, maxint);
    maxint <<= 1;
  }
  maxint--;
  result = minval + (maxval - minval)*sum/(float)maxint;
  return status;
}


/* ----------------------------------------------------------------------------
  Encode the specified integer into the string of bits.  The caller must make
certain that sufficient space has been allocated for the bits.
  We return 1 if there was a problem, 0 otherwise.
  This encodes the bits with the most significant bit first (to left of all 
other bits).  A higher index in the binstr will yield a less significant bit.
If there aren't enough bits in the string to do the representation, we 
represent all that we can then return.  Then we return 1 to indicate that we
could not represent it perfectly.
---------------------------------------------------------------------------- */
int
GABinaryEncode(unsigned BITBASE d, GABit * binstr, unsigned int nbits)
{
  if(binstr == (GABit *)0 || nbits == 0) return 1;
  memset(binstr, 0, nbits*sizeof(GABit));
  return _GAEncodeBase(2, d, binstr, 0, nbits-1);
}


/* ----------------------------------------------------------------------------
  This uses a threshhold rounding.  First we figure out what our discretization
will be based on the bit-resolution we're given (the number of bits we can use
to represent the interval).  Then we figure out the integer value that maps to
the entire discretized range and use this number to encode the string.  We have
to use a precision higher than that of the value that is to be encoded, so the
float/double is very important.
  If we cannot match the value exactly, we'll undershoot rather than overshoot
the value.
  If we could not get the mapping exactly then we set the value that was
passed to us to the one that we actually could encode and return a nonzero
status to indicate the problem.
  We are limited in the conversion by two numbers - the nintervals (an int of
the maximum size for this system) and the interval (a double for the system).
nintervals must be able to count up to the number of intervals that we may
represent, ie nintervals must be able to represent 2^nbits.  sum and interval
must be able to keep the size of the interval with no loss of data, ie
       (int)((maxval-minval)/interval)
must be equal to 
       ((nintervals=1) << nbits) - 1
with absolutely no roundoff error.  Practically, this means that we cannot do
anything greater than the number of bits needed to represent the mantissa of 
the highest floating precision number on the system.
---------------------------------------------------------------------------- */
int
GABinaryEncode(float& val, GABit * binstr, unsigned int nbits,
	       float minval, float maxval)
{
  if(binstr == (GABit *)NULL || nbits == 0) return 1;
  if(val < minval || maxval < val) return 1;

  unsigned BITBASE nintervals = 1;
  int status = GACheckEncoding(val,nbits,minval,maxval,nintervals);

  memset(binstr, 0, nbits*sizeof(GABit));
  status = (_GAEncodeBase(2, nintervals, binstr, 0, nbits-1) ? 1 : status);
  return status;
}












// Notice that there are many different Gray codings.  This is one of the most
// common Gray codings as described in Numerical Recipes in C.

int 
GAGrayDecode(float& value, const GABit* bits, unsigned int nbits, 
	     float minval, float maxval) 
{
  if(bits == (GABit *)0 || nbits == 0){ value = 0.0; return 1; }
  int status = GACheckDecoding(nbits);

  unsigned BITBASE gray = 0;
  for(unsigned int i=0; i<nbits; i++)
    if(bits[nbits-i-1]) gray |= (1<<i);

  unsigned BITBASE bin = gray;
  for(unsigned int j=1; j<GALIB_BITS_IN_WORD*sizeof(GALIB_BITBASE); j<<=1)
    bin ^= (bin>>j);

  unsigned BITBASE maxint = 1<<nbits;
  maxint--;
  value = minval + (maxval - minval)*(float)bin/(float)maxint;

  return status;
}

// This is not a very efficient way of doing the Gray encoding - we convert to
// binary, then to Gray instead of doing it directly.  We'll optimize later...

int 
GAGrayEncode(float& value, GABit* bits, unsigned int nbits, 
	     float minval, float maxval) 
{
  if(bits == (GABit *)NULL || nbits == 0) return 1;
  if(value < minval || maxval < value) return 1;

  unsigned BITBASE nintervals = 1;
  int status = GACheckEncoding(value,nbits,minval,maxval,nintervals);

  memset(bits, 0, nbits*sizeof(GABit));
  status = (_GAEncodeBase(2, nintervals, bits, 0, nbits-1) ? 1 : status);

  unsigned BITBASE bin = 0;
  for(unsigned int j=0; j<nbits; j++)
    if(bits[nbits-j-1]) bin |= (1<<j);

  unsigned BITBASE gray = bin ^ (bin>>1);
  for(unsigned int i=0; i<nbits; i++)
    bits[nbits-i-1] = (gray & (1<<i)) ? 1 : 0;

  return status;
}

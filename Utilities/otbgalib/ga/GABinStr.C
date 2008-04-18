// $Header$
/* ----------------------------------------------------------------------------
  binstr.C
  mbwall 30jun95
  Copyright (c) 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  Source file for the binary string genome.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ga/gaerror.h>
#include <ga/GABinStr.h>


/* ----------------------------------------------------------------------------
BinaryStringGenome
---------------------------------------------------------------------------- */
// Copy the contents of the bitstream.  We don't care what format it is in - 
// we resize to make sure we have adequate space then we just copy all of the
// data.
//   If the original is actually this, then we don't do anything.  If the 
// original is not the same class as this, then we post an error and return.
void
GABinaryString::copy(const GABinaryString& orig)
{
  if(&orig == this) return;
  resize(orig.sz);
  memcpy(data, orig.data, SZ*sizeof(GABit));
}


// Resize the bitstream to the specified number of bits.  We return the number
// of bits actually allocated.  For now there is no error checking or memory
// management - we assume that we'll always get all of the memory we ask for.
//   If we resize, we copy the previous bits into the new space.  The memory
// will never overlap (new should see to that) so we use memcpy not memmove.
// If we're making more space, we set the contents of the new space to zeros.
int
GABinaryString::resize(unsigned int x)
{
  if(sz == x) return sz;
  if(SZ < x){
    while(SZ < x) SZ += csz;
    if(!data){
      data = new GABit [SZ];
      memset(data, 0, SZ*sizeof(GABit));
    }
    else{
      GABit * tmp = data;
      data = new GABit [SZ];
      memcpy(data, tmp, sz * sizeof(GABit));
      memset(&(data[sz]), 0, (SZ-sz)*sizeof(GABit));
      delete [] tmp;
    }
  }
  return(sz = x);
}

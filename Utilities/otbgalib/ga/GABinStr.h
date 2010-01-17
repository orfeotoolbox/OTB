// $Header$
/* ----------------------------------------------------------------------------
  binstr.h
  mbwall 30jun95
  Copyright (c) 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This header defines the interface for the binary string.  This is a crude
version of a real bitstring object.  We don't do real bitstring in the 
interest of speed and ease of coding this mess up.

 TO DO:
  we can get major improvements to speed by inlining functions and getting rid
of the error checking...  for example, inlining genome and removing the 
conditional makes it go from 7.5 seconds to 3.2 seconds (bm bl cs 1024 c 0.9)
---------------------------------------------------------------------------- */
#ifndef _ga_binstr_h_
#define _ga_binstr_h_

#include <string.h>
#include <ga/gatypes.h>
#include <ga/garandom.h>

#define GA_BINSTR_CHUNKSIZE 32	  // size of the chunks of bits we allocate


class GABinaryString {
public:
  GABinaryString(unsigned int s){
    csz=GA_BINSTR_CHUNKSIZE; sz=0; SZ=0; data=(GABit *)0;
    resize(s);
  }
  GABinaryString(const GABinaryString& orig){
    sz=0; SZ=0; data=(GABit *)0;
    copy(orig);
  }
  virtual ~GABinaryString(){delete [] data;}
  void copy(const GABinaryString&);
  int resize(unsigned int);		// pass desired size, in bits
  int size() const {return sz;}

  short bit(unsigned int a) const {
    return(data[a]);
  }
  short bit(unsigned int a, short val) {	// set/unset the bit
    return(data[a] = (val ? 1 : 0));
  }
  int equal(const GABinaryString & b,
	    unsigned int r, unsigned int x, unsigned int l) const {
    return(memcmp(&(data[r]),&(b.data[x]),l*sizeof(GABit))?0:1);
  }
  void copy(const GABinaryString & orig,
	    unsigned int r, unsigned int x, unsigned int l){
    memcpy(&(data[r]), &(orig.data[x]), l*sizeof(GABit));
  }
  void move(unsigned int r, unsigned int x, unsigned int l){
    memmove(&(data[r]), &(data[x]), l*sizeof(GABit));
  }
  void set(unsigned int a, unsigned int l){
    memset(&(data[a]), 1, l*sizeof(GABit));
  }
  void unset(unsigned int a, unsigned int l){
    memset(&(data[a]), 0, l*sizeof(GABit));
  }
  void randomize(unsigned int a, unsigned int l){
    for(unsigned int i=0; i<l; i++) 
      data[i+a] = (GABit)GARandomBit();
  }
  void randomize(){
    for(unsigned int i=0; i<sz; i++) 
      data[i] = (GABit)GARandomBit();
  }

protected:
  unsigned int sz;		// size of chrom
  unsigned int SZ;		// size of the memory allocated
  unsigned int csz;		// size of chunks we allocate
  GABit *data;			// the data themselves
};

#endif

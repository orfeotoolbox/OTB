// $Header$
/* ----------------------------------------------------------------------------
  binstr1.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This header defines the interface for the 1D binary string genome, including
crossover objects and all the default and built-in operators.
---------------------------------------------------------------------------- */
#ifndef _ga_binstr1_h_
#define _ga_binstr1_h_

#include <ga/GABinStr.h>
#include <ga/GAGenome.h>


/* ----------------------------------------------------------------------------
1DBinaryStringGenome
-------------------------------------------------------------------------------
resize
    These genomes are resizable.  In addition, you can set a resize behaviour
  to specify the bounds in which the resize can occur.

copy
    Copy bits from the specified genome using the location and length 
  that are passed to us.  If the current genome is too short for the 
  entire length, copy whatever we can.  If the original genome is too 
  short for the specified length, copy whatever we can.  If either location
  is out of bounds, return without doing anything.
    We do NOT check for negative values in the locations!
    This routine clips if the copy sizes do not match - it does NOT resize the
  genome to fit the copy.  You'll have to do resizes before you call this
  routine if you want the copy to fit the original.

==, !=
    Are two genomes equal?  Our test for equality is based upon the
  contents of the genome, NOT the behaviour.  So as long as the bitstreams
  match, the genomes are 'equal'.  This means that a resizeable genome
  may be equal to a fixed-length genome.  But a chromsome with 500
  bits allocated is not equal to a genome with 10 bits allocated unless
  both are the same size.
---------------------------------------------------------------------------- */
class GA1DBinaryStringGenome : public GABinaryString, public GAGenome {
public:
  GADefineIdentity("GA1DBinaryStringGenome", GAID::BinaryStringGenome);

  static void UniformInitializer(GAGenome &);
  static void UnsetInitializer(GAGenome &);
  static void SetInitializer(GAGenome &);
  static int FlipMutator(GAGenome &, float);
  static int UniformCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int EvenOddCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int OnePointCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int TwoPointCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static float BitComparator(const GAGenome&, const GAGenome&);

public:
  GA1DBinaryStringGenome(unsigned int x,
			 GAGenome::Evaluator f=(GAGenome::Evaluator)0,
			 void * u=(void *)0);
  GA1DBinaryStringGenome(const GA1DBinaryStringGenome & orig);
  GA1DBinaryStringGenome& operator=(const GAGenome& arg)
    {copy(arg); return *this;}
  GA1DBinaryStringGenome& operator=(const short array []) // no err checks!
    {for(unsigned int i=0; i<sz; i++) gene(i,*(array+i)); return *this;}
  GA1DBinaryStringGenome& operator=(const int array []) // no err checks!
    {for(unsigned int i=0; i<sz; i++) gene(i,*(array+i)); return *this;}
  virtual ~GA1DBinaryStringGenome();
  virtual GAGenome *clone(GAGenome::CloneMethod flag=CONTENTS) const;
  virtual void copy(const GAGenome&);

#ifdef GALIB_USE_STREAMS
  virtual int read(STD_ISTREAM & is);
  virtual int write (STD_OSTREAM & os) const;
#endif

  virtual int equal(const GAGenome & c) const;

  short gene(unsigned int x=0) const {return bit(x);}
  short gene(unsigned int x, short value) 
    {_evaluated = gaFalse; return((bit(x) == value) ? value : bit(x,value));}
  short operator[](unsigned int x) {return gene(x);}
  int length() const {return nx;}
  int length(int x){resize(x); return nx;}
  int resize(int x);
  int resizeBehaviour() const;
  int resizeBehaviour(unsigned int lx, unsigned int ux);
  void copy(const GA1DBinaryStringGenome& orig,
	    unsigned int r, unsigned int x, unsigned int length);
  int equal(const GA1DBinaryStringGenome& orig,
	    unsigned int r, unsigned int x, unsigned int length) const;
  void set(unsigned int x, unsigned int length);
  void unset(unsigned int x, unsigned int length);
  void randomize(unsigned int x, unsigned int length);
  void randomize() { GABinaryString::randomize(); }
  void move(unsigned int destx, unsigned int srcx, unsigned int length);

protected:
  unsigned int nx;		// how long is the data string?
  unsigned int minX;		// what is the lower limit?
  unsigned int maxX;		// what is the upper limit?
};









inline void  
GA1DBinaryStringGenome::copy(const GA1DBinaryStringGenome & orig,
			     unsigned int r, unsigned int x, unsigned int l){
  if(l > 0 && x < orig.nx && r < nx){
    if(x + l > orig.nx) l = orig.nx - x;
    if(r + l > nx) l = nx - r;
    GABinaryString::copy(orig,r,x,l);
  }
  _evaluated = gaFalse;
}
inline void  
GA1DBinaryStringGenome::set(unsigned int x, unsigned int l){
  if(x + l > nx) l = nx - x;
  GABinaryString::set(x, l);
  _evaluated = gaFalse;
}
inline void  
GA1DBinaryStringGenome::unset(unsigned int x, unsigned int l){
  if(x + l > nx) l = nx - x;
  GABinaryString::unset(x, l);
  _evaluated = gaFalse;
}
inline void  
GA1DBinaryStringGenome::randomize(unsigned int x, unsigned int l){
  if(x + l > nx) l = nx - x;
  GABinaryString::randomize(x, l);
  _evaluated = gaFalse;
}
inline void  
GA1DBinaryStringGenome::move(unsigned int x, unsigned int srcx,unsigned int l){
  if(srcx + l > nx) l = nx - srcx;
  if(x + l > nx) l = nx - x;
  GABinaryString::move(x, srcx, l);
  _evaluated = gaFalse;
}

#endif

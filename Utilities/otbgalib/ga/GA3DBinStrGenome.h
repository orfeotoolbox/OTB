// $Header$
/* ----------------------------------------------------------------------------
  binstr3.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This header defines the interface for the 3D binary string genome.
---------------------------------------------------------------------------- */
#ifndef _ga_binstr3_h_
#define _ga_binstr3_h_

#include <ga/GABinStr.h>
#include <ga/GAGenome.h>


/* ----------------------------------------------------------------------------
3DBinaryStringGenome
-------------------------------------------------------------------------------
---------------------------------------------------------------------------- */
class GA3DBinaryStringGenome : public GABinaryString, public GAGenome {
public:
  GADefineIdentity("GA3DBinaryStringGenome", GAID::BinaryStringGenome3D);

  static void UniformInitializer(GAGenome &);
  static void UnsetInitializer(GAGenome &);
  static void SetInitializer(GAGenome &);
  static int FlipMutator(GAGenome &, float);
  static float BitComparator(const GAGenome&, const GAGenome&);
  static int UniformCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int EvenOddCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int OnePointCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);

public:
  GA3DBinaryStringGenome(unsigned int x, unsigned int y, unsigned int z,
			 GAGenome::Evaluator f=(GAGenome::Evaluator)0,
			 void * u=(void *)0);
  GA3DBinaryStringGenome(const GA3DBinaryStringGenome & orig);
  GA3DBinaryStringGenome& operator=(const GAGenome& arg)
    {copy(arg); return *this;}
  GA3DBinaryStringGenome& operator=(const short array []){
    for(unsigned int i=0; i<nx; i++)
      for(unsigned int j=0; j<ny; j++)
	for(unsigned int k=0; k<nz; k++)
	  gene(i,j,k,*(array+k*ny*nx+j*nx+i));
    return *this;
  }
  GA3DBinaryStringGenome& operator=(const int array []){
    for(unsigned int i=0; i<nx; i++)
      for(unsigned int j=0; j<ny; j++)
	for(unsigned int k=0; k<nz; k++)
	  gene(i,j,k,*(array+k*ny*nx+j*nx+i));
    return *this;
  }
  virtual ~GA3DBinaryStringGenome();
  virtual GAGenome *clone(GAGenome::CloneMethod flag=CONTENTS) const;
  virtual void copy(const GAGenome & chrom);

#ifdef GALIB_USE_STREAMS
  virtual int read(STD_ISTREAM &);
  virtual int write (STD_OSTREAM &) const;
#endif

  virtual int equal(const GAGenome & c) const;

// specific to this class
  short gene(unsigned int x, unsigned int y, unsigned int z) const {
    return bit(z*ny*nx + y*nx + x);
  }
  short gene(unsigned int x, unsigned int y, unsigned int z, short value) {
    _evaluated = gaFalse;
    return((bit(z*ny*nx+y*nx+x) == value) ? value : bit(z*ny*nx+y*nx+x,value));
  }
  int width() const {return nx;}
  int width(int w){resize(w, ny, nz); return nx;}
  int height() const {return ny;}
  int height(int h){resize(nx, h, nz); return ny;}
  int depth() const {return nz;}
  int depth(int d){resize(nx, ny, d); return nz;}
  int resize(int x, int y, int z);
  int resizeBehaviour(Dimension which) const;
  int resizeBehaviour(Dimension which, 
		      unsigned int low, unsigned int upper);
  int resizeBehaviour(unsigned int lowerX, unsigned int upperX, 
		      unsigned int lowerY, unsigned int upperY, 
		      unsigned int lowerZ, unsigned int upperZ){
    return(resizeBehaviour(WIDTH,  lowerX, upperX) * 
	   resizeBehaviour(HEIGHT, lowerY, upperY) * 
	   resizeBehaviour(DEPTH,  lowerZ, upperZ));
  }
  void copy(const GA3DBinaryStringGenome&, 
	    unsigned int, unsigned int, unsigned int,
	    unsigned int, unsigned int, unsigned int,
	    unsigned int, unsigned int, unsigned int);
  int equal(const GA3DBinaryStringGenome&, 
	    unsigned int, unsigned int, unsigned int,
	    unsigned int, unsigned int, unsigned int,
	    unsigned int, unsigned int, unsigned int) const;
  void set(unsigned int, unsigned int, unsigned int,
	   unsigned int, unsigned int, unsigned int);
  void unset(unsigned int, unsigned int, unsigned int, 
	     unsigned int, unsigned int, unsigned int);
  void move(unsigned int, unsigned int, unsigned int,
	    unsigned int, unsigned int, unsigned int, 
	    unsigned int, unsigned int, unsigned int);
  void randomize(unsigned int, unsigned int, unsigned int,
		 unsigned int, unsigned int, unsigned int);
  void randomize() { GABinaryString::randomize(); }

protected:
  unsigned int nx, ny, nz, minX, minY, minZ, maxX, maxY, maxZ;
};

#endif


// $Header$
/* ----------------------------------------------------------------------------
  bin2dec.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This header defines the interface for the binary-to-decimal genome and the 
phenotype objects used for the binary-to-decimal mappings in the 
binary-to-decimal genomes.
   The binary-to-decimal phenotype uses reference counting so that you only
need one instance of the underlying bit-to-float mapping.  Whenever you create
a new phenotype it gets its own core.  You can use the 'link' member to make
a phenotype refer to another's core.  This is what the genomes do when they 
clone themselves so that you don't end up with multiple phenotypes running 
around.  The core will persist until the last phenotype is destroyed (last one
out, please turn off the lights).
   When you clone a phenotype, you get a competeley new one with its own core.

 TO DO:
  binary to integer phenotype?

 TO DO:
*** Eventually we may want to cache the values in the phenotype mapping, but 
    for now we'll generate them on the fly.
*** Need to write a read method that can interpret binary/decimal input.
---------------------------------------------------------------------------- */
#ifndef _ga_bin2dec_h_
#define _ga_bin2dec_h_

#include <ga/GA1DBinStrGenome.h>
#include <ga/gabincvt.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

class GABin2DecPhenotypeCore {
public:
  GABin2DecPhenotypeCore();
  GABin2DecPhenotypeCore(const GABin2DecPhenotypeCore&);
  virtual ~GABin2DecPhenotypeCore();
  GABin2DecPhenotypeCore& operator=(const GABin2DecPhenotypeCore&);

  unsigned int cnt;		// how many references to us?
  unsigned int csz;		// how big are the chunks we allocate?
  unsigned int n, N;		// how many phenotypes do we have? (real,alloc)
  unsigned short *nbits;	// number of bits that max/min get mapped into
  unsigned short *oset;		// offset of the nth gene
  float *minval, *maxval;	// min, max value of phenotype elem
  unsigned int sz;		// total number of bits required
};

class GABin2DecPhenotype {
public:
  GABin2DecPhenotype() : core(new GABin2DecPhenotypeCore) {}
  GABin2DecPhenotype(const GABin2DecPhenotype& p) :
  core(new GABin2DecPhenotypeCore(*(p.core))) {}
  virtual ~GABin2DecPhenotype(){core->cnt -= 1; if(core->cnt==0){delete core;}}
  GABin2DecPhenotype & operator=(const GABin2DecPhenotype& p){
    if(&p != this) *core = *(p.core);
    return *this;
  } 
  GABin2DecPhenotype * clone() const {return new GABin2DecPhenotype(*this);}
  void link(GABin2DecPhenotype& p){
    core->cnt -= 1;
    if(core->cnt == 0) delete core;
    core=p.core; core->cnt += 1;
  }
  void add(unsigned int nbits, float min, float max);
  void remove(unsigned int which);
  unsigned int size() const {return core->sz;}
  unsigned int nPhenotypes() const {return core->n;}
  float min(int which) const {return core->minval[which];}
  float max(int which) const {return core->maxval[which];}
  int length(int which) const {return core->nbits[which];}
  int offset(int which) const {return core->oset[which];}
  int equal(const GABin2DecPhenotype&) const;

protected:
  GABin2DecPhenotypeCore *core;
};

inline int 
operator==(const GABin2DecPhenotype& a, const GABin2DecPhenotype& b) {
  return a.equal(b);
}

inline int 
operator!=(const GABin2DecPhenotype& a, const GABin2DecPhenotype& b) {
  return (a.equal(b) ? 0 : 1);
}





/* ----------------------------------------------------------------------------
   The phenotype does reference counting, so we can make a copy of it for our
own use and we don't have to worry about extra overhead.
---------------------------------------------------------------------------- */
class GABin2DecGenome : public GA1DBinaryStringGenome {
public:
  GADefineIdentity("GABin2DecGenome", GAID::Bin2DecGenome);

public:
  GABin2DecGenome(const GABin2DecPhenotype & p,
		  GAGenome::Evaluator f=(GAGenome::Evaluator)0,
		  void * u=(void *)0) : 
  GA1DBinaryStringGenome(p.size(), f, u), ptype(new GABin2DecPhenotype(p)){
    comparator(DEFAULT_BIN2DEC_COMPARATOR);
    encoder(DEFAULT_BIN2DEC_ENCODER);
    decoder(DEFAULT_BIN2DEC_DECODER);
  }
  GABin2DecGenome(const GABin2DecGenome & orig) :
  GA1DBinaryStringGenome(orig.sz) {ptype=(GABin2DecPhenotype *)0; copy(orig);}
  GABin2DecGenome& operator=(const GAGenome& arg) {copy(arg); return *this;}
  virtual ~GABin2DecGenome(){delete ptype;}
  virtual GAGenome *clone(GAGenome::CloneMethod flag=CONTENTS) const ;
  virtual void copy(const GAGenome &);

#ifdef GALIB_USE_STREAMS
  virtual int read (STD_ISTREAM &);
  virtual int write (STD_OSTREAM &) const;
#endif

  virtual int equal(const GAGenome &) const;
  virtual int notequal(const GAGenome &) const;

  const GABin2DecPhenotype & phenotypes(const GABin2DecPhenotype & p);
  const GABin2DecPhenotype & phenotypes() const {return *ptype;}
  int nPhenotypes() const {return ptype->nPhenotypes();}
  float phenotype(unsigned int n, float val);
  float phenotype(unsigned int n) const;

  void encoder(GABinaryEncoder e){encode = e; _evaluated = gaFalse;}
  void decoder(GABinaryDecoder d){decode = d; _evaluated = gaFalse;}

protected:
  GABin2DecPhenotype * ptype;
  GABinaryEncoder encode;	// function we use to encode the bits
  GABinaryDecoder decode;	// function we use to decode the bits
};


#endif

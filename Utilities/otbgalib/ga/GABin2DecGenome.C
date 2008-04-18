// $Header$
/* ----------------------------------------------------------------------------
  bin2dec.C
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the binary-to-decimal genome.
  This is the phenotype for converting binary strings to decimal values.  There
are limits to the size of the numbers you can use (ie you're limited to the
number of bits that can represent a float - see the converters file for more 
information).
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ga/gaerror.h>
#include <ga/GABin2DecGenome.h>


/* ----------------------------------------------------------------------------
   Phenotype class definitions
---------------------------------------------------------------------------- */

#define GA_B2D_CHUNKSIZE 20

GABin2DecPhenotypeCore::GABin2DecPhenotypeCore() : 
csz(GA_B2D_CHUNKSIZE), n(0), N(0), sz(0) {
  nbits = oset = 0;
  minval = maxval = 0;
  cnt = 1;
}

GABin2DecPhenotypeCore::
GABin2DecPhenotypeCore(const GABin2DecPhenotypeCore& p) :
csz(p.csz), n(p.n), N(p.N), sz(p.sz) {
  nbits = new unsigned short[N];
  oset = new unsigned short[N];
  minval = new float[N];
  maxval = new float[N];

  memcpy(nbits, p.nbits, n*sizeof(unsigned short));
  memcpy(oset, p.oset, n*sizeof(unsigned short));
  memcpy(minval, p.minval, n*sizeof(float));
  memcpy(maxval, p.maxval, n*sizeof(float));

  cnt = 1;
}

GABin2DecPhenotypeCore::~GABin2DecPhenotypeCore(){
  if(cnt > 0) 
    GAErr(GA_LOC, "GABin2DecPhenotypeCore", "destructor", gaErrRefsRemain);
  delete [] nbits;
  delete [] oset;
  delete [] minval;
  delete [] maxval;
}

GABin2DecPhenotypeCore& 
GABin2DecPhenotypeCore::operator=(const GABin2DecPhenotypeCore& p){
  if(&p == this) return *this;

  delete [] nbits;
  delete [] oset;
  delete [] minval;
  delete [] maxval;

  n = p.n;
  sz = p.sz;
  N = p.N;
  csz = p.csz;
 
  nbits = new unsigned short[N];
  oset = new unsigned short[N];
  minval = new float[N];
  maxval = new float[N];

  memcpy(nbits, p.nbits, n*sizeof(unsigned short));
  memcpy(oset, p.oset, n*sizeof(unsigned short));
  memcpy(minval, p.minval, n*sizeof(float));
  memcpy(maxval, p.maxval, n*sizeof(float));

  return *this;
}









// Add another phenotype to this phenotype object.  If needed, we allocate more
// space, otherwise just tag the new on then end.  We allocate space in chunks
// so we don't spend too much time doing memory allocation stuff.
void
GABin2DecPhenotype::add(unsigned int nb, float min, float max){
  if(core->n + 1 > core->N){
    core->N += core->csz;

    unsigned short *nbtmp = core->nbits;
    core->nbits = new unsigned short[core->N];
    memcpy(core->nbits, nbtmp, core->n*sizeof(unsigned short));
    delete [] nbtmp;

    unsigned short *ostmp = core->oset;
    core->oset = new unsigned short[core->N];
    memcpy(core->oset, ostmp, core->n*sizeof(unsigned short));
    delete [] ostmp;

    float *mintmp = core->minval;
    core->minval = new float[core->N];
    memcpy(core->minval, mintmp, core->n*sizeof(float));
    delete [] mintmp;

    float *maxtmp = core->maxval;
    core->maxval = new float[core->N];
    memcpy(core->maxval, maxtmp, core->n*sizeof(float));
    delete [] maxtmp;
  }
  core->nbits[core->n] = nb;
  if(core->n > 0)
    core->oset[core->n] = core->oset[core->n-1] + core->nbits[core->n-1];
  else
    core->oset[core->n] = 0;
  core->minval[core->n] = min;
  core->maxval[core->n] = max;
  core->n++;
  core->sz += nb;
}

void
GABin2DecPhenotype::remove(unsigned int x){
  if(x >= core->n) return;
  memmove(&(core->nbits[x]), &(core->nbits[x+1]),
	  (core->n - x - 1)*sizeof(unsigned short));
  memmove(&(core->oset[x]), &(core->oset[x+1]),
	  (core->n - x - 1)*sizeof(unsigned short));
  memmove(&(core->minval[x]), &(core->minval[x+1]),
	  (core->n - x - 1)*sizeof(float));
  memmove(&(core->maxval[x]), &(core->maxval[x+1]),
	  (core->n - x - 1)*sizeof(float));
  core->n -= 1;
}


int
GABin2DecPhenotype::equal(const GABin2DecPhenotype & b) const {
  if(core->sz != b.core->sz || core->n != b.core->n) return gaFalse;
  if(memcmp(core->nbits,b.core->nbits,core->n*sizeof(unsigned short))!=0 ||
     memcmp(core->oset, b.core->oset, core->n*sizeof(unsigned short))!=0 ||
     memcmp(core->minval, b.core->minval, core->n*sizeof(float)) != 0 ||
     memcmp(core->maxval, b.core->maxval, core->n*sizeof(float)))
    return gaFalse;
  return(gaTrue);
}









/* ----------------------------------------------------------------------------
   Genome class definition
---------------------------------------------------------------------------- */
void
GABin2DecGenome::copy(const GAGenome & orig)
{
  if(&orig == this) return;
  const GABin2DecGenome* c = DYN_CAST(const GABin2DecGenome*, &orig);
  if(c) {
    GA1DBinaryStringGenome::copy(*c);
    encode = c->encode;
    decode = c->decode;
    if(ptype) *ptype = *(c->ptype);
    else       ptype = new GABin2DecPhenotype(*(c->ptype));
  }
}

// We shouldn't have to worry about our superclass's data members for the
// attributes part here, but there is no 'copy attributes' function, so we
// end up doing it.  bummer.
GAGenome *
GABin2DecGenome::clone(GAGenome::CloneMethod flag) const {
  GABin2DecGenome *cpy = new GABin2DecGenome(*ptype);
  if(flag == CONTENTS){ cpy->copy(*this); }
  else{
    cpy->GAGenome::copy(*this);
    cpy->maxX = maxX; cpy->minX = minX;
    *(cpy->ptype) = *ptype;
    cpy->encode = encode;
    cpy->decode = decode;
  }
  return cpy;
}


// The phenotype does reference counting, so its ok to keep our own copy of 
// the phenotype.  So all we have to do here is copy the one that is passed
// to us, then modify the bit string to accomodate the new mapping.
const GABin2DecPhenotype &
GABin2DecGenome::phenotypes(const GABin2DecPhenotype & p)
{
  *ptype = p;
  GA1DBinaryStringGenome::resize(p.size());
  return *ptype;
}


// We access the data string directly here.  This could be dangerous (if the
// bitstream ever changes on us it will affect the way this method sees the
// data string).
//   Eventually we may need to cache the decimal values in an array of floats,
// but for now we call the converter routine every time each phenotype is 
// requested.
float
GABin2DecGenome::phenotype(unsigned int n) const
{
  if(n >= ptype->nPhenotypes()){
    GAErr(GA_LOC, className(), "phenotype", gaErrBadPhenotypeID);
    return(0.0);
  }
  float val=0.0;
  decode(val,
	 &(data[ptype->offset(n)]), ptype->length(n), 
	 ptype->min(n), ptype->max(n));
  return val;
}


// Set the bits of the binary string based on the decimal value that is passed
// to us.  Notice that the number you pass may or may not be set properly.  It
// depends on the resolution defined in the phenotype.  If you didn't define 
// enough resolution, then there may be no way to represent the number.
//   We round off to the closest representable value, then return the number 
// that we actually entered (the rounded value).
// *** this is dangerous!  we're accessing the superclass' data representation
// directly, so if the representation changes to a bit stream, this will break.
//   If someone tries to set the phenotype beyond the bounds, we post an error
// then set the bits to the closer bound.
float
GABin2DecGenome::phenotype(unsigned int n, float val)
{
  if(n >= ptype->nPhenotypes()){
    GAErr(GA_LOC, className(), "phenotype", gaErrBadPhenotypeID);
    return val;
  }
  if(val < ptype->min(n) || val > ptype->max(n)){
    GAErr(GA_LOC, className(), "phenotype", gaErrBadPhenotypeValue);
    val = ((val < ptype->min(n)) ? ptype->min(n) : ptype->max(n));
  }
  encode(val,
	 &(data[ptype->offset(n)]), ptype->length(n),
	 ptype->min(n), ptype->max(n));
  return val;
}


#ifdef GALIB_USE_STREAMS
// Read the incoming data as a list of phenotype values.  It would be nice to
// do this either as binary or decimal read, but oh well...  not much need.
int
GABin2DecGenome::read(STD_ISTREAM & is)
{
  float value;
  for(unsigned int i=0; i<phenotypes().nPhenotypes(); i++){
    is >> value;
    if(is.fail() || is.eof()) return 1;
    phenotype(i, value);
  }
  return 0;
}


int
GABin2DecGenome::write(STD_OSTREAM & os) const 
{
  for(unsigned int i=0; i<phenotypes().nPhenotypes(); i++)
    os << phenotype(i) << " ";
  return 0;
}
#endif


// For two bin2dec genomes to be equal they must have the same bits AND the
// same phenotypes.
int 
GABin2DecGenome::equal(const GAGenome& g) const {
  GABin2DecGenome & b = (GABin2DecGenome&)g;
  return((GA1DBinaryStringGenome::equal(b) && *ptype == *(b.ptype)) ?
	 gaTrue : gaFalse);
}


int 
GABin2DecGenome::notequal(const GAGenome& g) const {
  GABin2DecGenome & b = (GABin2DecGenome&)g;
  return((GA1DBinaryStringGenome::notequal(b) || *ptype != *(b.ptype)) ?
	 gaTrue : gaFalse);
}

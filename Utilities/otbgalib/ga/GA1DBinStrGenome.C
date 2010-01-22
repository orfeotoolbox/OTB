// $Header$
/* ----------------------------------------------------------------------------
  binstr1.C
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 1D binary string genome.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ga/gaerror.h>
#include <ga/garandom.h>
#include <ga/GA1DBinStrGenome.h>
#include <ga/GAMask.h>

#define SWAP(a,b) {unsigned int tmp=a; a=b; b=tmp;}

/* ----------------------------------------------------------------------------
   Genome class definition
---------------------------------------------------------------------------- */
// Set all the initial values to NULL or zero, then allocate the space we'll
// need (using the resize method).  We do NOT call the initialize method at
// this point - initialization must be done explicitly by the user of the
// genome (eg when the population is created or reset).  If we called the
// initializer routine here then we could end up with multiple initializations
// and/or calls to dummy initializers (for example when the genome is 
// created with a dummy initializer and the initializer is assigned later on).
GA1DBinaryStringGenome::
GA1DBinaryStringGenome(unsigned int len, 
		       GAGenome::Evaluator f, void * u) :
GABinaryString(len),
GAGenome(DEFAULT_1DBINSTR_INITIALIZER, 
	 DEFAULT_1DBINSTR_MUTATOR,
	 DEFAULT_1DBINSTR_COMPARATOR) {
  evaluator(f);
  userData(u);
  crossover(DEFAULT_1DBINSTR_CROSSOVER); // assign the default sexual crossover
  nx=minX=maxX=0;
  resize(len);
}


// This is the copy initializer.  We set everything to the default values, then
// copy the original.  The BinaryStringGenome creator takes care of zeroing
// the data and sz members.
GA1DBinaryStringGenome::
GA1DBinaryStringGenome(const GA1DBinaryStringGenome& orig) :
GABinaryString(orig.GABinaryString::size()),
GAGenome() {
  nx=minX=maxX=0;
  GA1DBinaryStringGenome::copy(orig);
}

GA1DBinaryStringGenome::~GA1DBinaryStringGenome() {
}


// The clone member creates a duplicate (exact or just attributes, depending
// on the flag).  The caller is responsible for freeing the memory that is 
// allocated by this method.
GAGenome*
GA1DBinaryStringGenome::clone(GAGenome::CloneMethod flag) const {
  GA1DBinaryStringGenome *cpy = new GA1DBinaryStringGenome(nx);
  if(flag == CONTENTS){
    cpy->copy(*this);
  }
  else{
    cpy->GAGenome::copy(*this);
    cpy->maxX = maxX; cpy->minX = minX;
  }
  return cpy;
}


// This is the class-specific copy method.  It will get called by the super
// class since the superclass operator= is set up to call ccopy (and that is
// what we define here - a virtual function).  We should check to be sure that
// both genomes are the same class and same dimension.  This function tries
// to be smart about they way it copies.  If we already have data, then we do
// a memcpy of the one we're supposed to copy.  If we don't or we're not the 
// same size as the one we're supposed to copy, then we adjust ourselves.
//   The BinaryStringGenome takes care of the resize in its copy method.
// It also copies the bitstring for us.
void
GA1DBinaryStringGenome::copy(const GAGenome & orig)
{
  if(&orig == this) return;
  const GA1DBinaryStringGenome* c = 
    DYN_CAST(const GA1DBinaryStringGenome*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GABinaryString::copy(*c);
    nx = c->nx; minX = c->minX; maxX = c->maxX;
  }
}


//   Resize the genome.  If someone specifies ANY_RESIZE then we pick a random
// size within the behaviour limits that have been set.  If limits have been
// set and someone passes us something outside the limits, we resize to the
// closest bound.  If the genome is fixed size (ie min limit equals max limit)
// then we resize to the specified value and move the min/max to match it.
int
GA1DBinaryStringGenome::resize(int l)
{
  if(l == STA_CAST(int, nx)) return nx;

  if(l == GAGenome::ANY_SIZE)
    l = GARandomInt(minX, maxX);
  else if(l < 0)
    return nx;			// do nothing
  else if(minX == maxX)
    minX=maxX=l;
  else{
    if(l < STA_CAST(int, minX)) l=minX;
    if(l > STA_CAST(int, maxX)) l=maxX;
  }

  GABinaryString::resize(l);
  if(l > STA_CAST(int, nx))
    for(int i=nx; i<l; i++)
      bit(i, GARandomBit());
  nx = l;
  _evaluated = gaFalse;
  return sz;
}


#ifdef GALIB_USE_STREAMS
// We read data from a stream as a series of 1's and 0's.  We want a continuous
// stream (ie no spaces) but if there are spaces then we can handle them.  We
// ignore all whitespace.  We ignore anything that is not a digit.  If it is a
// zero then we set to zero.  Anything else is a 1.
int
GA1DBinaryStringGenome::read(STD_ISTREAM & is)
{
  static char c;
  unsigned int i=0;

  while(!is.fail() && !is.eof() && i<nx) {
    is >> c;
    if(isdigit(c)) gene(i++, ((c == '0') ? 0 : 1));
  }

  _evaluated = gaFalse;

  if(is.eof() && i < nx){
    GAErr(GA_LOC, className(), "read", gaErrUnexpectedEOF);
    is.clear(STD_IOS_BADBIT | is.rdstate());
    return 1;
  }

  return 0;
}


// When we write the data to a stream we do it without any spaces.  Also, there
// is no newline at the end of the stream of digits.
int
GA1DBinaryStringGenome::write(STD_OSTREAM & os) const
{
  for(unsigned int i=0; i<nx; i++)
    os << gene(i);
  return 0;
}
#endif


//   Set the resize behaviour of the genome.  A genome can be fixed
// length, resizeable with a max and min limit, or resizeable with no limits
// (other than an implicit one that we use internally).
//   A value of 0 means no resize, a value less than zero mean unlimited 
// resize, and a positive value means resize with that value as the limit.
//   We return the upper limit of the genome's size.
int
GA1DBinaryStringGenome::
resizeBehaviour(unsigned int lower, unsigned int upper)
{
  if(upper < lower){
    GAErr(GA_LOC, className(), "resizeBehaviour", gaErrBadResizeBehaviour);
    return resizeBehaviour();
  }
  minX = lower; maxX = upper;
  if(nx > upper) resize(upper);
  if(nx < lower) resize(lower);
  return resizeBehaviour();
}

int 
GA1DBinaryStringGenome::resizeBehaviour() const {
  int val = maxX;
  if(maxX == minX) val = FIXED_SIZE;
  return val;
}

int 
GA1DBinaryStringGenome::
equal(const GA1DBinaryStringGenome& c,
      unsigned int dest, unsigned int src, unsigned int len) const {
  return GABinaryString::equal(c,dest,src,len);
}

int 
GA1DBinaryStringGenome::equal(const GAGenome & c) const {
  if(this == &c) return 1;
  const GA1DBinaryStringGenome* b = DYN_CAST(const GA1DBinaryStringGenome*,&c);
  int eq = 0;
  if(b) {
    eq = ((nx != b->nx) ? 0 : GABinaryString::equal(*b,0,0,nx));
  }
  return eq;
}










/* ----------------------------------------------------------------------------
   Operators
---------------------------------------------------------------------------- */
//   Set the bits of the genome to random values.  We use the library's
// random bit function so we don't have to worry about machine-specific stuff.
//   We also do a resize so the genome can resize itself (randomly) if it
// is a resizeable genome.
void 
GA1DBinaryStringGenome::UniformInitializer(GAGenome & c)
{
  GA1DBinaryStringGenome &child=DYN_CAST(GA1DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  for(int i=child.length()-1; i>=0; i--)
    child.gene(i, GARandomBit()); // initial values are all random
}


//   Unset all of the bits in the genome.
void 
GA1DBinaryStringGenome::UnsetInitializer(GAGenome & c)
{
  GA1DBinaryStringGenome &child=DYN_CAST(GA1DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  child.unset(0, child.length());
}


//   Set all of the bits in the genome.
void 
GA1DBinaryStringGenome::SetInitializer(GAGenome & c)
{
  GA1DBinaryStringGenome &child=DYN_CAST(GA1DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  child.set(0, child.length());
}


// This function gets called a lot (especially for the simple ga) so it must
// be as streamlined as possible.  If the mutation probability is small, then
// we must call random on each bit in the string.  Otherwise, can can simply
// mutate a known number of bits (based on the mutation rate).  We don't check
// to see how many bits we flip, nor do we keep track of which ones got flipped
// so this can result in an actual mutation that is lower than that specified,
// but the bigger the genome and the smaller the mutation probability, the
// better the chance that it will match the desired mutation rate.
//   If nMut is greater than 1, then we round up, so a mutation of 2.2 would
// be 3 mutations, and 2.9 would be 3 as well.  nMut of 3 would be 3 mutations.
int 
GA1DBinaryStringGenome::FlipMutator(GAGenome & c, float pmut)
{
  GA1DBinaryStringGenome &child=DYN_CAST(GA1DBinaryStringGenome &, c);
  register int n, i;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float, child.length());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.length()-1; i>=0; i--){
      if(GAFlipCoin(pmut)){
	child.gene(i, ((child.gene(i) == 0) ? 1 : 0));
	nMut++;
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++){
      i = GARandomInt(0, child.length()-1); // the index of the bit to flip
      child.gene(i, ((child.gene(i) == 0) ? 1 : 0));
    }
  }
  return(STA_CAST(int, nMut));
}


// Return a number from 0 to 1 to indicate how similar two genomes are.  For 
// the binary strings we compare bits.  We count the number of bits that are
// the same then divide by the number of bits.  If the genomes are different
// length then we return a -1 to indicate that we cannot calculate the 
// similarity.
//   Normal hamming distance makes use of population information - this is not
// a hamming measure!  This is a similarity measure of two individuals, not
// two individuals relative to the rest of the population.  This comparison is
// independent of the population!  (you can do Hamming measure in the scaling
// object)
float
GA1DBinaryStringGenome::BitComparator(const GAGenome& a, const GAGenome& b){
  const GA1DBinaryStringGenome &sis=DYN_CAST(const GA1DBinaryStringGenome&, a);
  const GA1DBinaryStringGenome &bro=DYN_CAST(const GA1DBinaryStringGenome&, b);
  if(sis.length() != bro.length()) return -1;
  if(sis.length() == 0) return 0;
  float count = 0.0;
  for(int i=sis.length()-1; i>=0; i--)
    count += ((sis.gene(i) == bro.gene(i)) ? 0 : 1);
  return count/sis.length();
}














// Randomly take bits from each parent.  For each bit we flip a coin to see if
// that bit should come from the mother or the father.  This operator can be 
// used on genomes of different lengths, but the crossover is truncated to the
// shorter of the parents and child.
int
GA1DBinaryStringGenome::
UniformCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA1DBinaryStringGenome &mom=
    DYN_CAST(const GA1DBinaryStringGenome &, p1);
  const GA1DBinaryStringGenome &dad=
    DYN_CAST(const GA1DBinaryStringGenome &, p2);

  int n=0;
  int i;

  if(c1 && c2){
    GA1DBinaryStringGenome &sis=DYN_CAST(GA1DBinaryStringGenome &, *c1);
    GA1DBinaryStringGenome &bro=DYN_CAST(GA1DBinaryStringGenome &, *c2);

    if(sis.length() == bro.length() &&
       mom.length() == dad.length() &&
       sis.length() == mom.length()){
      for(i=sis.length()-1; i>=0; i--){
	if(GARandomBit()){
	  sis.gene(i, mom.gene(i));
	  bro.gene(i, dad.gene(i));
	}
	else{
	  sis.gene(i, dad.gene(i));
	  bro.gene(i, mom.gene(i));
	}
      }
    }
    else{
      GAMask mask;
      int start;
      int max = (sis.length() > bro.length()) ? sis.length() : bro.length();
      int min = (mom.length() < dad.length()) ? mom.length() : dad.length();
      mask.size(max);
      for(i=0; i<max; i++)
	mask[i] = GARandomBit();
      start = (sis.length() < min) ? sis.length()-1 : min-1;
      for(i=start; i>=0; i--)
	sis.gene(i, (mask[i] ? mom.gene(i) : dad.gene(i)));
      start = (bro.length() < min) ? bro.length()-1 : min-1;
      for(i=start; i>=0; i--)
	bro.gene(i, (mask[i] ? dad.gene(i) : mom.gene(i)));
    }
    n = 2;
  }
  else if(c1 || c2){
    GA1DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA1DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA1DBinaryStringGenome&, *c2));

    if(mom.length() == dad.length() && sis.length() == mom.length()){
      for(i=sis.length()-1; i>=0; i--)
	sis.gene(i, (GARandomBit() ? mom.gene(i) : dad.gene(i)));
    }
    else{
      int min = (mom.length() < dad.length()) ? mom.length() : dad.length();
      min = (sis.length() < min) ? sis.length() : min;
      for(i=min-1; i>=0; i--)
	sis.gene(i, (GARandomBit() ? mom.gene(i) : dad.gene(i)));
    }
    n = 1;
  }

  return n;
}





// Pick a point in the parents then grab alternating chunks for each child.
//   A word about crossover site mapping.  If a genome has width 10, the
// cross site can assume a value of 0 to 10, inclusive.  A site of 0 means
// that all of the material comes from the father.  A site of 10 means that
// all of the material comes from the mother.  A site of 3 means that bits
// 0-2 come from the mother and bits 3-9 come from the father.
int
GA1DBinaryStringGenome::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2,
		  GAGenome* c1, GAGenome* c2){
  const GA1DBinaryStringGenome &mom=
    DYN_CAST(const GA1DBinaryStringGenome &, p1);
  const GA1DBinaryStringGenome &dad=
    DYN_CAST(const GA1DBinaryStringGenome &, p2);

  int n=0;
  unsigned int momsite, momlen;
  unsigned int dadsite, dadlen;

  if(c1 && c2){
    GA1DBinaryStringGenome &sis=DYN_CAST(GA1DBinaryStringGenome &, *c1);
    GA1DBinaryStringGenome &bro=DYN_CAST(GA1DBinaryStringGenome &, *c2);

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || 
	 sis.length() != bro.length() ||
	 sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return n;
      }
      momsite = dadsite = GARandomInt(0, mom.length());
      momlen = dadlen = mom.length() - momsite;
    }
    else if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameBehavReqd);
      return n;
    }
    else{
      momsite = GARandomInt(0, mom.length());
      dadsite = GARandomInt(0, dad.length());
      momlen = mom.length() - momsite;
      dadlen = dad.length() - dadsite;
      sis.resize(momsite+dadlen);
      bro.resize(dadsite+momlen);
    }

    sis.copy(mom, 0, 0, momsite);
    sis.copy(dad, momsite, dadsite, dadlen);
    bro.copy(dad, 0, 0, dadsite);
    bro.copy(mom, dadsite, momsite, momlen);
  
    n = 2;
  }
  else if(c1 || c2){
    GA1DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA1DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA1DBinaryStringGenome&, *c2));

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return n;
      }
      momsite = dadsite = GARandomInt(0, mom.length());
      momlen = dadlen = mom.length() - momsite;
    }
    else{
      momsite = GARandomInt(0, mom.length());
      dadsite = GARandomInt(0, dad.length());
      momlen = mom.length() - momsite;
      dadlen = dad.length() - dadsite;
      sis.resize(momsite+dadlen);
    }

    if(GARandomBit()){
      sis.copy(mom, 0, 0, momsite);
      sis.copy(dad, momsite, dadsite, dadlen);
    }
    else{
      sis.copy(dad, 0, 0, dadsite);
      sis.copy(mom, dadsite, momsite, momlen);
    }

    n = 1;
  }

  return n;
}







// Two point crossover for one dimension binary strings.  The first part is
// taken from the mother, the second from the father, and the third from the
// mother.  If the child is resizable then we resize before copying the parts.
//   The rules for doing resizable crossover apply here as well as for the
// single point crossover (see comments for 1Pt for details).
int
GA1DBinaryStringGenome::
TwoPointCrossover(const GAGenome& p1, const GAGenome& p2,
		  GAGenome* c1, GAGenome* c2){
  const GA1DBinaryStringGenome &mom=
    DYN_CAST(const GA1DBinaryStringGenome &, p1);
  const GA1DBinaryStringGenome &dad=
    DYN_CAST(const GA1DBinaryStringGenome &, p2);

  int n=0;
  unsigned int momsite[2], momlen[2];
  unsigned int dadsite[2], dadlen[2];

  if(c1 && c2){
    GA1DBinaryStringGenome &sis=DYN_CAST(GA1DBinaryStringGenome &, *c1);
    GA1DBinaryStringGenome &bro=DYN_CAST(GA1DBinaryStringGenome &, *c2);

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || 
	 sis.length() != bro.length() ||
	 sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "two-point cross", gaErrSameLengthReqd);
	return n;
      }
      momsite[0] = GARandomInt(0, mom.length());
      momsite[1] = GARandomInt(0, mom.length());
      if(momsite[0] > momsite[1]) SWAP(momsite[0], momsite[1]);
      momlen[0] = momsite[1] - momsite[0];
      momlen[1] = mom.length() - momsite[1];

      dadsite[0] = momsite[0];
      dadsite[1] = momsite[1];
      dadlen[0] = momlen[0];
      dadlen[1] = momlen[1];
    }
    else if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "two-point cross", gaErrSameBehavReqd);
      return n;
    }
    else{
      momsite[0] = GARandomInt(0, mom.length());
      momsite[1] = GARandomInt(0, mom.length());
      if(momsite[0] > momsite[1]) SWAP(momsite[0], momsite[1]);
      momlen[0] = momsite[1] - momsite[0];
      momlen[1] = mom.length() - momsite[1];
      
      dadsite[0] = GARandomInt(0, dad.length());
      dadsite[1] = GARandomInt(0, dad.length());
      if(dadsite[0] > dadsite[1]) SWAP(dadsite[0], dadsite[1]);
      dadlen[0] = dadsite[1] - dadsite[0];
      dadlen[1] = dad.length() - dadsite[1];

      sis.resize(momsite[0]+dadlen[0]+momlen[1]);
      bro.resize(dadsite[0]+momlen[0]+dadlen[1]);
    }

    sis.copy(mom, 0, 0, momsite[0]);
    sis.copy(dad, momsite[0], dadsite[0], dadlen[0]);
    sis.copy(mom, momsite[0]+dadlen[0], momsite[1], momlen[1]);
    bro.copy(dad, 0, 0, dadsite[0]);
    bro.copy(mom, dadsite[0], momsite[0], momlen[0]);
    bro.copy(dad, dadsite[0]+momlen[0], dadsite[1], dadlen[1]);

    n = 2;
  }
  else if(c1 || c2){
    GA1DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA1DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA1DBinaryStringGenome&, *c2));

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "two-point cross", gaErrSameLengthReqd);
	return n;
      }
      momsite[0] = GARandomInt(0, mom.length());
      momsite[1] = GARandomInt(0, mom.length());
      if(momsite[0] > momsite[1]) SWAP(momsite[0], momsite[1]);
      momlen[0] = momsite[1] - momsite[0];
      momlen[1] = mom.length() - momsite[1];

      dadsite[0] = momsite[0];
      dadsite[1] = momsite[1];
      dadlen[0] = momlen[0];
      dadlen[1] = momlen[1];
    }
    else{
      momsite[0] = GARandomInt(0, mom.length());
      momsite[1] = GARandomInt(0, mom.length());
      if(momsite[0] > momsite[1]) SWAP(momsite[0], momsite[1]);
      momlen[0] = momsite[1] - momsite[0];
      momlen[1] = mom.length() - momsite[1];
      
      dadsite[0] = GARandomInt(0, dad.length());
      dadsite[1] = GARandomInt(0, dad.length());
      if(dadsite[0] > dadsite[1]) SWAP(dadsite[0], dadsite[1]);
      dadlen[0] = dadsite[1] - dadsite[0];
      dadlen[1] = dad.length() - dadsite[1];

      sis.resize(momsite[0]+dadlen[0]+momlen[1]);
    }

    if(GARandomBit()){
      sis.copy(mom, 0, 0, momsite[0]);
      sis.copy(dad, momsite[0], dadsite[0], dadlen[0]);
      sis.copy(mom, momsite[0]+dadlen[0], momsite[1], momlen[1]);
    }
    else{
      sis.copy(dad, 0, 0, dadsite[0]);
      sis.copy(mom, dadsite[0], momsite[0], momlen[0]);
      sis.copy(dad, dadsite[0]+momlen[0], dadsite[1], dadlen[1]);
    }

    n = 1;
  }

  return n;
}





//   Even and odd crossovers take alternating bits from the mother and father.
// For even crossover, we take every even bit from the mother and every odd bit
// from the father (the first bit is the 0th bit, so it is even).  Odd
// crossover is just the opposite.  
int
GA1DBinaryStringGenome::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2, 
		 GAGenome* c1, GAGenome* c2){
  const GA1DBinaryStringGenome &mom=
    DYN_CAST(const GA1DBinaryStringGenome &, p1);
  const GA1DBinaryStringGenome &dad=
    DYN_CAST(const GA1DBinaryStringGenome &, p2);

  int n=0;
  int i;

  if(c1 && c2){
    GA1DBinaryStringGenome &sis=DYN_CAST(GA1DBinaryStringGenome &, *c1);
    GA1DBinaryStringGenome &bro=DYN_CAST(GA1DBinaryStringGenome &, *c2);

    if(sis.length() == bro.length() &&
       mom.length() == dad.length() &&
       sis.length() == mom.length()){
      for(i=sis.length()-1; i>=1; i-=2){
	sis.gene(i, mom.gene(i));
	bro.gene(i, dad.gene(i));
	sis.gene(i-1, dad.gene(i-1));
	bro.gene(i-1, mom.gene(i-1));
      }
      if(i==0){
	sis.gene(0, mom.gene(0));
	bro.gene(0, dad.gene(0));
      }
    }
    else{
      int start;
      int min = (mom.length() < dad.length()) ? mom.length() : dad.length();
      start = (sis.length() < min) ? sis.length()-1 : min-1;
      for(i=start; i>=0; i--)
	sis.gene(i, ((i%2 == 0) ? mom.gene(i) : dad.gene(i)));
      start = (bro.length() < min) ? bro.length()-1 : min-1;
      for(i=start; i>=0; i--)
	bro.gene(i, ((i%2 == 0) ? dad.gene(i) : mom.gene(i)));
    }

    n = 2;
  }
  else if(c1 || c2){
    GA1DBinaryStringGenome &sis = (c1 ?
				   DYN_CAST(GA1DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA1DBinaryStringGenome&, *c2));

    if(mom.length() == dad.length() && sis.length() == mom.length()){
      for(i=sis.length()-1; i>=1; i-=2){
	sis.gene(i, mom.gene(i));
	sis.gene(i-1, dad.gene(i-1));
      }
      if(i==0){
	sis.gene(0, mom.gene(0));
      }
    }
    else{
      int min = (mom.length() < dad.length()) ? mom.length() : dad.length();
      min = (sis.length() < min) ? sis.length()-1 : min-1;
      for(i=min; i>=0; i--)
	sis.gene(i, ((i%2 == 0) ? mom.gene(i) : dad.gene(i)));
    }

    n = 1;
  }

  return n;
}

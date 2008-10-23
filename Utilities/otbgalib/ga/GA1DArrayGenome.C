// $Header$
/* ----------------------------------------------------------------------------
  array1.C
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 1D array genome.
---------------------------------------------------------------------------- */
#ifndef _ga_array1_C_
#define _ga_array1_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ga/garandom.h>
#include <ga/GA1DArrayGenome.h>
#include <ga/GAMask.h>

template <class T> int 
GA1DArrayIsHole(const GA1DArrayGenome<T>&, const GA1DArrayGenome<T>&,
		int, int, int);


/* ----------------------------------------------------------------------------
1DArrayGenome
---------------------------------------------------------------------------- */
template <class T> const char *
GA1DArrayGenome<T>::className() const {return "GA1DArrayGenome";}
template <class T> int
GA1DArrayGenome<T>::classID() const {return GAID::ArrayGenome;}

// Set all the initial values to NULL or zero, then allocate the space we'll
// need (using the resize method).  We do NOT call the initialize method at
// this point - initialization must be done explicitly by the user of the
// genome (eg when the population is created or reset).  If we called the
// initializer routine here then we could end up with multiple initializations
// and/or calls to dummy initializers (for example when the genome is 
// created with a dummy initializer and the initializer is assigned later on).
// Besides, we default to the no-initialization initializer by calling the
// default genome constructor.
template <class T> 
GA1DArrayGenome<T>::
GA1DArrayGenome(unsigned int length, GAGenome::Evaluator f, void * u) :
GAArray<T>(length),
GAGenome(DEFAULT_1DARRAY_INITIALIZER, 
	 DEFAULT_1DARRAY_MUTATOR,
	 DEFAULT_1DARRAY_COMPARATOR) {
  evaluator(f);
  userData(u);
  nx=minX=maxX=length;
  crossover(DEFAULT_1DARRAY_CROSSOVER);
}


// This is the copy initializer.  We set everything to the default values, then
// copy the original.  The Array creator takes care of zeroing the data.
template <class T> 
GA1DArrayGenome<T>::
GA1DArrayGenome(const GA1DArrayGenome<T> & orig) : 
GAArray<T>(orig.sz), GAGenome() {
  GA1DArrayGenome<T>::copy(orig);
}


// Delete whatever we own.
template <class T>
GA1DArrayGenome<T>::~GA1DArrayGenome() { }


// This is the class-specific copy method.  It will get called by the super
// class since the superclass operator= is set up to call ccopy (and that is
// what we define here - a virtual function).  We should check to be sure that
// both genomes are the same class and same dimension.  This function tries
// to be smart about they way it copies.  If we already have data, then we do
// a memcpy of the one we're supposed to copy.  If we don't or we're not the 
// same size as the one we're supposed to copy, then we adjust ourselves.
//   The Array takes care of the resize in its copy method.
template <class T> void
GA1DArrayGenome<T>::copy(const GAGenome & orig){
  if(&orig == this) return;
  const GA1DArrayGenome<T>* c = DYN_CAST(const GA1DArrayGenome<T>*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GAArray<T>::copy(*c);
    nx = c->nx; minX = c->minX; maxX = c->maxX;
  }
}


template <class T> GAGenome *
GA1DArrayGenome<T>::clone(GAGenome::CloneMethod flag) const {
  GA1DArrayGenome<T> *cpy = new GA1DArrayGenome<T>(nx);
  if(flag == CONTENTS){ 
    cpy->copy(*this);
  }
  else{
    cpy->GAGenome::copy(*this);
    cpy->maxX = maxX; cpy->minX = minX;
  }
  return cpy;
}


//   Resize the genome.
//   A negative value for the length means that we should randomly set the
// length of the genome (if the resize behaviour is resizeable).  If
// someone tries to randomly set the length and the resize behaviour is fixed
// length, then we don't do anything.
//   We pay attention to the values of minX and maxX - they determine what kind
// of resizing we are allowed to do.  If a resize is requested with a length
// less than the min length specified by the behaviour, we set the minimum 
// to the length.  If the length is longer than the max length specified by
// the behaviour, we set the max value to the length.
//   We return the total size (in bits) of the genome after resize. 
//   We don't do anything to the new contents!
template <class T> int
GA1DArrayGenome<T>::resize(int len)
{
  if(len == STA_CAST(int,nx)) return nx;

  if(len == GAGenome::ANY_SIZE)
    len = GARandomInt(minX, maxX);
  else if(len < 0)
    return nx;			// do nothing
  else if(minX == maxX)
    minX=maxX=len;
  else{
    if(len < STA_CAST(int,minX)) len=minX;
    if(len > STA_CAST(int,maxX)) len=maxX;
  }

  nx = GAArray<T>::size(len);
  _evaluated = gaFalse;
  return this->sz;
}


#ifdef GALIB_USE_STREAMS
// We don't define this one apriori.  Do it in a specialization.
template <class T> int
GA1DArrayGenome<T>::read(STD_ISTREAM &) {
  GAErr(GA_LOC, className(), "read", gaErrOpUndef);
  return 1;
}


// When we write the data to a stream we do it with spaces between elements.
// Also, there is no newline at the end of the stream of digits.
template <class T> int
GA1DArrayGenome<T>::write(STD_OSTREAM & os) const {
  for(unsigned int i=0; i<nx; i++)
    os << gene(i) << " ";
  return 0;
}
#endif


//   Set the resize behaviour of the genome.  A genome can be fixed
// length, resizeable with a max and min limit, or resizeable with no limits
// (other than an implicit one that we use internally).
//   A value of 0 means no resize, a value less than zero mean unlimited 
// resize, and a positive value means resize with that value as the limit.
template <class T> int
GA1DArrayGenome<T>::
resizeBehaviour(unsigned int lower, unsigned int upper)
{
  if(upper < lower){
    GAErr(GA_LOC, className(), "resizeBehaviour", gaErrBadResizeBehaviour);
    return resizeBehaviour();
  }
  minX = lower; maxX = upper;
  if(nx > upper) GA1DArrayGenome<T>::resize(upper);
  if(nx < lower) GA1DArrayGenome<T>::resize(lower);
  return resizeBehaviour();
}

template <class T> int
GA1DArrayGenome<T>::resizeBehaviour() const {
  int val = maxX;
  if(maxX == minX) val = FIXED_SIZE;
  return val;
}

template <class T> int 
GA1DArrayGenome<T>::equal(const GAGenome & c) const {
  const GA1DArrayGenome<T> & b = DYN_CAST(const GA1DArrayGenome<T> &, c);
  return((this == &c) ? 1 : ((nx != b.nx) ? 0 : GAArray<T>::equal(b,0,0,nx)));
}









/* ----------------------------------------------------------------------------
1DArrayAlleleGenome

  These genomes contain an allele set.  When we create a new genome, it owns
its own, independent allele set.  If we clone a new genome, the new one gets a
link to our allele set (so we don't end up with zillions of allele sets).  Same
is true for the copy constructor.
  The array may have a single allele set or an array of allele sets, depending
on which creator was called.  Either way, the allele set cannot be changed 
once the array is created.
---------------------------------------------------------------------------- */
template <class T> const char *
GA1DArrayAlleleGenome<T>::className() const {return "GA1DArrayAlleleGenome";}
template <class T> int
GA1DArrayAlleleGenome<T>::classID() const {return GAID::ArrayAlleleGenome;}

template <class T> 
GA1DArrayAlleleGenome<T>::
GA1DArrayAlleleGenome(unsigned int length, const GAAlleleSet<T> & s,
		      GAGenome::Evaluator f, void * u) :
GA1DArrayGenome<T>(length, f, u){
  naset = 1;
  aset = new GAAlleleSet<T>[1];
  aset[0] = s;

  initializer(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_INITIALIZER);
  mutator(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_MUTATOR);
  comparator(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_COMPARATOR);
  crossover(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_CROSSOVER);
}

template <class T> 
GA1DArrayAlleleGenome<T>::
GA1DArrayAlleleGenome(const GAAlleleSetArray<T> & sa,
		      GAGenome::Evaluator f, void * u) :
GA1DArrayGenome<T>(sa.size(), f, u) {
  naset = sa.size();
  aset = new GAAlleleSet<T>[naset];
  for(int i=0; i<naset; i++)
    aset[i] = sa.set(i);

  initializer(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_INITIALIZER);
  mutator(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_MUTATOR);
  comparator(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_COMPARATOR);
  crossover(GA1DArrayAlleleGenome<T>::DEFAULT_1DARRAY_ALLELE_CROSSOVER);
}


// The copy constructor creates a new genome whose allele set refers to the
// original's allele set.
template <class T> 
GA1DArrayAlleleGenome<T>::
GA1DArrayAlleleGenome(const GA1DArrayAlleleGenome<T>& orig) : 
GA1DArrayGenome<T>(orig.sz) {
  naset = 0;
  aset = (GAAlleleSet<T>*)0;
  GA1DArrayAlleleGenome<T>::copy(orig);
}


// Delete the allele set
template <class T>
GA1DArrayAlleleGenome<T>::~GA1DArrayAlleleGenome(){
  delete [] aset;
}


// This implementation of clone does not make use of the contents/attributes
// capability because this whole interface isn't quite right yet...  Just 
// clone the entire thing, contents and all.
template <class T> GAGenome *
GA1DArrayAlleleGenome<T>::clone(GAGenome::CloneMethod) const {
  return new GA1DArrayAlleleGenome<T>(*this);
}


template <class T> void 
GA1DArrayAlleleGenome<T>::copy(const GAGenome& orig){
  if(&orig == this) return;
  const GA1DArrayAlleleGenome<T> * c = 
    DYN_CAST(const GA1DArrayAlleleGenome<T>*, &orig);
  if(c) {
    GA1DArrayGenome<T>::copy(*c);
    if(naset != c->naset){
      delete [] aset;
      naset = c->naset;
      aset = new GAAlleleSet<T>[naset];
    }
    for(int i=0; i<naset; i++)
      aset[i].link(c->aset[i]);
  }
}


// If we resize to a larger length then we need to set the contents to a valid
// value (ie one of our alleles).
template <class T> int
GA1DArrayAlleleGenome<T>::resize(int len){
  unsigned int oldx = this->nx;
  GA1DArrayGenome<T>::resize(len);
  if(this->nx > oldx){
    for(unsigned int i=oldx; i<this->nx; i++)
      this->a[i] = aset[i % naset].allele();
  }
  return len;
}



// Define these so they can easily be specialized as needed.
#ifdef GALIB_USE_STREAMS
template <class T> int
GA1DArrayAlleleGenome<T>::read(STD_ISTREAM& is){
  return GA1DArrayGenome<T>::read(is);
}

template <class T> int
GA1DArrayAlleleGenome<T>::write(STD_OSTREAM& os) const {
  return GA1DArrayGenome<T>::write(os);
}
#endif

template <class T> int
GA1DArrayAlleleGenome<T>::equal(const GAGenome & c) const {
  return GA1DArrayGenome<T>::equal(c);
}











/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
// The random initializer sets the elements of the array based on the alleles
// set.  We choose randomly the allele for each element.
template <class ARRAY_TYPE> void 
GA1DArrayAlleleGenome<ARRAY_TYPE>::UniformInitializer(GAGenome & c)
{
  GA1DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA1DArrayAlleleGenome<ARRAY_TYPE> &, c);
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  for(int i=child.length()-1; i>=0; i--)
    child.gene(i, child.alleleset(i).allele());
}


// Random initializer for order-based genome.  Loop through the genome
// and assign each element the next allele in the allele set.  Once each
// element has been initialized, scramble the contents by swapping elements.
// This assumes that there is only one allele set for the array.
template <class ARRAY_TYPE> void 
GA1DArrayAlleleGenome<ARRAY_TYPE>::OrderedInitializer(GAGenome & c)
{
  GA1DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA1DArrayAlleleGenome<ARRAY_TYPE> &, c);
  child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can
  int length = child.length()-1;
  int n=0;
  int i;
  for(i=length; i>=0; i--){
    child.gene(i, child.alleleset().allele(n++));
    if(n >= child.alleleset().size()) n = 0;
  }
  for(i=length; i>=0; i--)
    child.swap(i, GARandomInt(0, length));
}


// Randomly pick elements in the array then set the element to any of the 
// alleles in the allele set for this genome.  This will work for any number
// of allele sets for a given array.
template <class ARRAY_TYPE> int 
GA1DArrayAlleleGenome<ARRAY_TYPE>::FlipMutator(GAGenome & c, float pmut)
{
  GA1DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA1DArrayAlleleGenome<ARRAY_TYPE> &, c);
  register int n, i;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.length());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.length()-1; i>=0; i--){
      if(GAFlipCoin(pmut)){
	child.gene(i, child.alleleset(i).allele());
	nMut++;
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++){
      i = GARandomInt(0, child.length()-1);
      child.gene(i, child.alleleset(i).allele());
    }
  }
  return(STA_CAST(int,nMut));
}


// Randomly swap elements in the array.
template <class ARRAY_TYPE> int 
GA1DArrayGenome<ARRAY_TYPE>::SwapMutator(GAGenome & c, float pmut)
{
  GA1DArrayGenome<ARRAY_TYPE> &child=DYN_CAST(GA1DArrayGenome<ARRAY_TYPE>&, c);
  register int n, i;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.length());
  int length = child.length()-1;
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=length; i>=0; i--){
      if(GAFlipCoin(pmut)){
	child.swap(i, GARandomInt(0, length));
	nMut++;
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++)
      child.swap(GARandomInt(0, length), GARandomInt(0, length));
  }
  return(STA_CAST(int,nMut));
}


// The comparator is supposed to return a number that indicates how similar
// two genomes are, so here we just compare elements and return a number that
// indicates how many elements match.  If they are different lengths then we
// return -1 to indicate that we could not calculate the differences.
//   This assumes that there is an operator == defined for the object in the
// elements of the array.
template <class ARRAY_TYPE> float
GA1DArrayGenome<ARRAY_TYPE>::
ElementComparator(const GAGenome& a, const GAGenome& b)
{
  const GA1DArrayGenome<ARRAY_TYPE>& sis=
    DYN_CAST(const GA1DArrayGenome<ARRAY_TYPE>&, a);
  const GA1DArrayGenome<ARRAY_TYPE>& bro=
    DYN_CAST(const GA1DArrayGenome<ARRAY_TYPE>&, b);

  if(sis.length() != bro.length()) return -1;
  if(sis.length() == 0) return 0;
  float count = 0.0;
  for(int i=sis.length()-1; i>=0; i--)
    count += ((sis.gene(i) == bro.gene(i)) ? 0 : 1);
  return count/sis.length();
}














#define SWAP(a,b) {unsigned int tmp=a; a=b; b=tmp;}

// Randomly take bits from each parent.  For each bit we flip a coin to see if
// that bit should come from the mother or the father.  If strings are 
// different lengths then we need to use the mask to get things right.
template <class T> int
GA1DArrayGenome<T>::
UniformCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int n=0;
  int i;

  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

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
    GA1DArrayGenome<T> &sis = (c1 ? 
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) : 
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

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





// Single point crossover for 1D array genomes.  Pick a single point then
// copy genetic material from each parent.  We must allow for resizable genomes
// so be sure to check the behaviours before we do the crossovers.  If resizing
// is allowed then the children will change depending on where the site is
// located.  It is also possible to have a mixture of resize behaviours, but
// we won't worry about that at this point.  If this happens we just say that
// we cannot handle that and post an error message.
template <class T> int
GA1DArrayGenome<T>::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2, 
		  GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  unsigned int momsite, momlen;
  unsigned int dadsite, dadlen;

  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || 
	 sis.length() != bro.length() ||
	 sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      momsite = dadsite = GARandomInt(0, mom.length());
      momlen = dadlen = mom.length() - momsite;
    }
    else if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameBehavReqd);
      return nc;
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
  
    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ? 
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) : 
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
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

    nc = 1;
  }

  return nc;
}











// Two point crossover for the 1D array genome.  Similar to the single point
// crossover, but here we pick two points then grab the sections based upon 
// those two points.
//   When we pick the points, it doesn't matter where they fall (one is not
// dependent upon the other).  Make sure we get the lesser one into the first
// position of our site array.
template <class T> int
GA1DArrayGenome<T>::
TwoPointCrossover(const GAGenome& p1, const GAGenome& p2, 
		  GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  unsigned int momsite[2], momlen[2];
  unsigned int dadsite[2], dadlen[2];

  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || 
	 sis.length() != bro.length() ||
	 sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "two-point cross", gaErrSameLengthReqd);
	return nc;
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
      return nc;
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

    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ?
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) :
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

    if(sis.resizeBehaviour() == GAGenome::FIXED_SIZE){
      if(mom.length() != dad.length() || sis.length() != mom.length()){
	GAErr(GA_LOC, mom.className(), "two-point cross", gaErrSameLengthReqd);
	return nc;
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

    nc = 1;
  }

  return nc;
}







// Even and odd crossover for the array works just like it does for the 
// binary strings.  For even crossover we take the 0th element and every other
// one after that from the mother.  The 1st and every other come from the
// father.  For odd crossover, we do just the opposite.
template <class T> int
GA1DArrayGenome<T>::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2, 
		 GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  int i;

  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);
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

    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ? 
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) : 
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));
    
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

    nc = 1;
  }

  return nc;
}





// Partial match crossover for the 1D array genome.  This uses the partial
// matching algorithm described in Goldberg's book.
//   Parents and children must be the same size for this crossover to work.  If
// they are not, we post an error message.
//   We make sure that b will be greater than a.
template <class T> int
GA1DArrayGenome<T>::
PartialMatchCrossover(const GAGenome& p1, const GAGenome& p2, 
		      GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  int a = GARandomInt(0, mom.length());
  int b = GARandomInt(0, dad.length());
  if(b < a) SWAP(a,b);
  int i,j,index;

  if(mom.length() != dad.length()){
    GAErr(GA_LOC, mom.className(), "parial match cross", gaErrBadParentLength);
    return nc;
  }
  
  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

    sis.GAArray<T>::copy(mom);
    for(i=a, index=a; i<b; i++, index++){
      for(j=0; j<sis.length()-1 && sis.gene(j) != dad.gene(index); j++);
      sis.swap(i,j);
    }
    bro.GAArray<T>::copy(dad);
    for(i=a, index=a; i<b; i++, index++){
      for(j=0; j<bro.length()-1 && bro.gene(j) != mom.gene(index); j++);
      bro.swap(i,j);
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ? 
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) : 
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

    const GA1DArrayGenome<T> *parent1, *parent2;
    if(GARandomBit()) { parent1 = &mom; parent2 = &dad; }
    else              { parent1 = &dad; parent2 = &mom; }

    sis.GAArray<T>::copy(*parent1);
    for(i=a, index=a; i<b; i++, index++){
      for(j=0; j<sis.length()-1 && sis.gene(j) != parent2->gene(index); j++);
      sis.swap(i,j);
    }

    nc = 1;
  }

  return nc;
}





// This function determines whether or not an indexed position is a hole that
// we can substitute into.  It does a linear search to find the holes (yuk).
template <class T> int
GA1DArrayIsHole(const GA1DArrayGenome<T> &c, const GA1DArrayGenome<T> &dad,
		int index, int a, int b){
  for(int i=a; i<b; i++)
    if(c.gene(index) == dad.gene(i)) return 1;
  return 0;
}


// Order crossover for the 1D array genome.  This uses the order crossover
// described in Goldberg's book.
//   Parents and children must be the same length.
//   We make sure that b will be greater than a.
//   This implementation isn't terribly smart.  For example, I do a linear
// search rather than caching and doing binary search or smarter hash tables.
//   First we copy the mother into the sister.  Then move the 'holes' into the
// crossover section and maintain the ordering of the non-hole elements.  
// Finally, put the 'holes' in the proper order within the crossover section.
// After we have done the sister, we do the brother.
template <class T> int
GA1DArrayGenome<T>::
OrderCrossover(const GAGenome& p1, const GAGenome& p2,
	       GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  int a = GARandomInt(0, mom.length());
  int b = GARandomInt(0, mom.length());
  if(b < a) SWAP(a,b);
  int i,j, index;

  if(mom.length() != dad.length()){
    GAErr(GA_LOC, mom.className(), "order cross", gaErrBadParentLength);
    return nc;
  }
  
  if(c1 && c2){
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

// Copy the parent
    sis.GAArray<T>::copy(mom);

// Move all the 'holes' into the crossover section
    for(i=0, index=b; i<sis.size(); i++, index++){
      if(index >= sis.size()) index=0;
      if(GA1DArrayIsHole(sis,dad,index,a,b)) break;
    }
    
    for(; i<sis.size()-b+a; i++, index++){
      if(index >= sis.size()) index=0;
      j=index;
      do{
	j++;
	if(j >= sis.size()) j=0;
      } while(GA1DArrayIsHole(sis,dad,j,a,b));
      sis.swap(index,j);
    }

// Now put the 'holes' in the proper order within the crossover section.
    for(i=a; i<b; i++){
      if(sis.gene(i) != dad.gene(i)){
	for(j=i+1; j<b; j++)
	  if(sis.gene(j) == dad.gene(i)) sis.swap(i,j);
      }
    }

// Now do the other child
    bro.GAArray<T>::copy(dad);

// Move all the 'holes' into the crossover section
    for(i=0, index=b; i<bro.size(); i++, index++){
      if(index >= bro.size()) index=0;
      if(GA1DArrayIsHole(bro,mom,index,a,b)) break;
    }

    for(; i<bro.size()-b+a; i++, index++){
      if(index >= bro.size()) index=0;
      j=index;
      do{
	j++;
	if(j >= bro.size()) j=0;
      } while(GA1DArrayIsHole(bro,mom,j,a,b));
      bro.swap(index,j);
    }
    
// Now put the 'holes' in the proper order within the crossover section.
    for(i=a; i<b; i++){
      if(bro.gene(i) != mom.gene(i)){
	for(j=i+1; j<b; j++)
	  if(bro.gene(j) == mom.gene(i)) bro.swap(i,j);
      }
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ? 
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) : 
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

    const GA1DArrayGenome<T> *parent1, *parent2;
    if(GARandomBit()) { parent1 = &mom; parent2 = &dad; }
    else              { parent1 = &dad; parent2 = &mom; }

    sis.GAArray<T>::copy(*parent1);
    for(i=0, index=b; i<sis.size(); i++, index++){
      if(index >= sis.size()) index=0;
      if(GA1DArrayIsHole(sis,*parent2,index,a,b)) break;
    }
    for(; i<sis.size()-b+a; i++, index++){
      if(index >= sis.size()) index=0;
      j=index;
      do{
	j++;
	if(j >= sis.size()) j=0;
      } while(GA1DArrayIsHole(sis,*parent2,j,a,b));
      sis.swap(index,j);
    }
    for(i=a; i<b; i++){
      if(sis.gene(i) != parent2->gene(i)){
	for(j=i+1; j<b; j++)
	  if(sis.gene(j) == parent2->gene(i)) sis.swap(i,j);
      }
    }

    nc = 1;
  }

  return nc;
}








// Cycle crossover for the 1D array genome.  This is implemented as described 
// in goldberg's book.  The first is picked from mom, then cycle using dad.
// Finally, fill in the gaps with the elements from dad.
//   We allocate space for a temporary array in this routine.  It never frees
// the memory that it uses, so you might want to re-think this if you're really
// memory-constrained (similar to what we do with the uniform crossover when
// the children are resizeable).
//  Allocate space for an array of flags.  We use this to keep track of whether
// the child's contents came from the mother or the father.  We don't free the
// space here, but it is not a memory leak.
//   The first step is to cycle through mom & dad to get the cyclic part of 
// the crossover.  Then fill in the rest of the sis with dad's contents that 
// we didn't use in the cycle.  Finally, do the same thing for the other child.
//   Notice that this implementation makes serious use of the operator= for the
// objects in the array.  It also requires the operator != and == comparators.
template <class T> int
GA1DArrayGenome<T>::
CycleCrossover(const GAGenome& p1, const GAGenome& p2,
	       GAGenome* c1, GAGenome* c2){
  const GA1DArrayGenome<T> &mom=DYN_CAST(const GA1DArrayGenome<T> &, p1);
  const GA1DArrayGenome<T> &dad=DYN_CAST(const GA1DArrayGenome<T> &, p2);

  int nc=0;
  int i, current=0;

  if(mom.length() != dad.length()){
    GAErr(GA_LOC, mom.className(), "cycle cross", gaErrBadParentLength);
    return nc;
  }

  if(c1 && c2){
    GAMask mask;
    GA1DArrayGenome<T> &sis=DYN_CAST(GA1DArrayGenome<T> &, *c1);
    GA1DArrayGenome<T> &bro=DYN_CAST(GA1DArrayGenome<T> &, *c2);

    mask.size(sis.length());
    mask.clear();

    sis.gene(0, mom.gene(0));
    mask[0] = 1;
    while(dad.gene(current) != mom.gene(0)){
      for(i=0; i<sis.size(); i++){
	if(mom.gene(i) == dad.gene(current)){
	  sis.gene(i, mom.gene(i));
	  mask[i] = 1;
	  current = i;
	  break;
	}
      }
    }

    for(i=0; i<sis.size(); i++)
      if(mask[i] == 0) sis.gene(i, dad.gene(i));

    mask.clear();

    bro.gene(0, dad.gene(0));
    mask[0] = 1;
    while(mom.gene(current) != dad.gene(0)){
      for(i=0; i<bro.size(); i++){
	if(dad.gene(i) == mom.gene(current)){
	  bro.gene(i, dad.gene(i));
	  mask[i] = 1;
	  current = i;
	  break;
	}
      }
    }

    for(i=0; i<bro.size(); i++)
      if(mask[i] == 0) bro.gene(i, mom.gene(i));

    nc = 2;
  }
  else if(c1 || c2){
    GA1DArrayGenome<T> &sis = (c1 ?
			       DYN_CAST(GA1DArrayGenome<T> &, *c1) :
			       DYN_CAST(GA1DArrayGenome<T> &, *c2));

    const GA1DArrayGenome<T> *parent1, *parent2;
    if(GARandomBit()) { parent1 = &mom; parent2 = &dad; }
    else              { parent1 = &dad; parent2 = &mom; }

    GAMask mask;
    mask.size(sis.length());
    mask.clear();
    
    sis.gene(0, parent1->gene(0));
    mask[0] = 1;
    while(parent2->gene(current) != parent1->gene(0)){
      for(i=0; i<sis.size(); i++){
	if(parent1->gene(i) == parent2->gene(current)){
	  sis.gene(i, parent1->gene(i));
	  mask[i] = 1;
	  current = i;
	  break;
	}
      }
    }
    for(i=0; i<sis.size(); i++)
      if(mask[i] == 0) sis.gene(i, parent2->gene(i));

    nc = 1;
  }

  return nc;
}

#undef SWAP

#endif

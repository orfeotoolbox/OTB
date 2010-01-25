// $Header$
/* ----------------------------------------------------------------------------
  array1.h
  mbwall 25feb95
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved

 DESCRIPTION:
  This header defines the interface for the 1D array genome.
  You can use ANY kind of object in this genome.  But notice that it is
really easy to optimize this for some of the simpler types.  I'll try to do 
that for common instantiations (float, char).
  The objects in the array must have the following operators defined:
  =  ==  != 
  >> must be defined if you use the default read methods

 TO DO:
*** If you want speed, specialize the comparison routines and copy routines
    so that you can use memcpy, memmove, memcmp rather than looping through
    each element.
*** make the object defined for simple types, if you want to use complex types
    then specialize to do member copy rather than bit copy (that way simple 
    users won't sacrifice speed, and complex users will get more complexity)
---------------------------------------------------------------------------- */
#ifndef _ga_array1_h_
#define _ga_array1_h_

#include <ga/GAArray.h>
#include <ga/GAGenome.h>
#include <ga/GAAllele.h>


/* ----------------------------------------------------------------------------
1DArrayGenome
---------------------------------------------------------------------------- */
template <class T>
class GA1DArrayGenome : public GAArray<T>, public GAGenome {
public:
  GADeclareIdentity();

  static int SwapMutator(GAGenome&, float);
  static float ElementComparator(const GAGenome&, const GAGenome&);
  static int UniformCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int EvenOddCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int OnePointCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int TwoPointCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int PartialMatchCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int OrderCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);
  static int CycleCrossover(const GAGenome&, const GAGenome&, 
			      GAGenome*, GAGenome*);

public:
  GA1DArrayGenome(unsigned int x,
		  GAGenome::Evaluator f=(GAGenome::Evaluator)0,
		  void * u=(void *)0);
  GA1DArrayGenome(const GA1DArrayGenome<T> & orig);
  GA1DArrayGenome<T>& operator=(const GAGenome& orig)
    {copy(orig); return *this;}
  GA1DArrayGenome<T>& operator=(const T array []) // no err checks!
    {for(unsigned int i=0; i<this->sz; i++) gene(i, *(array+i)); return *this;}
  virtual ~GA1DArrayGenome();
  virtual GAGenome *clone(GAGenome::CloneMethod flag=CONTENTS) const;
  virtual void copy(const GAGenome &);

#ifdef GALIB_USE_STREAMS
  virtual int read(STD_ISTREAM & is);
  virtual int write (STD_OSTREAM & os) const;
#endif

  virtual int equal(const GAGenome & c) const ;

  const T & gene(unsigned int x=0) const {return this->a[x];}
  T & gene(unsigned int x, const T & value){
    if(this->a[x] != value){ this->a[x] = value; _evaluated = gaFalse;}
    return this->a[x];
  }
  int length() const {return nx;}
  int length(int x){resize(x); return nx;}
  virtual int resize(int x);
  int resizeBehaviour() const ;
  int resizeBehaviour(unsigned int lx, unsigned int ux);
  void copy(const GA1DArrayGenome<T> & orig,
	    unsigned int r, unsigned int x, unsigned int l){
    if(l > 0 && x < orig.nx && r < nx){
      if(x + l > orig.nx) l = orig.nx - x;
      if(r + l > nx) l = nx - r;
      GAArray<T>::copy(orig,r,x,l);
    }
    _evaluated = gaFalse;
  }
  void swap(unsigned int i, unsigned int j)
    {GAArray<T>::swap(i,j); _evaluated = gaFalse; }


protected:
  unsigned int nx;		// how long is the data string?
  unsigned int minX;		// what is the lower limit?
  unsigned int maxX;		// what is the upper limit? 

private:
  GA1DArrayGenome() : GAArray<T>(0){}
};




/* ----------------------------------------------------------------------------
1DArrayAlleleGenome
-------------------------------------------------------------------------------
  We don't do any error checking on the assignment to const array of type T, so
the array may contain elements that are not in the allele set.
  When we clone, we link the new allele set to our own so that we don't make 
unnecessary copies.  If someone sets a new allele set on the genome, then we
make a complete new copy of the new one and break any link to a previous one.
  It is OK to resize these genomes, so we don't have to protect the resize.
  If this is an order-based genome then resizing should be done when the allele
set is changed, but there is nothing implicit in the object that tells us that
this is an order-based genome, so that's up to the user to take care of.  If
you're really concerned about catching this type of error, derive a class from
this class that does order-based protection.
  I have defined all of the genome virtual functions here to make it easier to
do specializations (you can specialize this class instead if its superclass).
  We define our own resize so that we can set to allele values on resize to a
bigger length.
---------------------------------------------------------------------------- */
template <class T>
class GA1DArrayAlleleGenome : public GA1DArrayGenome<T> {
public:
  GADeclareIdentity();

  static void UniformInitializer(GAGenome&);
  static void OrderedInitializer(GAGenome&);
  static int FlipMutator(GAGenome&, float);

public:
  GA1DArrayAlleleGenome(unsigned int x,
			const GAAlleleSet<T> & a,
			GAGenome::Evaluator f=(GAGenome::Evaluator)0,
			void * u=(void *)0);
  GA1DArrayAlleleGenome(const GAAlleleSetArray<T> & a,
			GAGenome::Evaluator f=(GAGenome::Evaluator)0,
			void * u=(void *)0);
  GA1DArrayAlleleGenome(const GA1DArrayAlleleGenome<T>&);
  GA1DArrayAlleleGenome<T>& operator=(const GAGenome& arr)
    {copy(arr); return *this;}
  GA1DArrayAlleleGenome<T>& operator=(const T array []) // no err checks!
    {GA1DArrayGenome<T>::operator=(array); return *this;}
  virtual ~GA1DArrayAlleleGenome();
  virtual GAGenome *clone(GAGenome::CloneMethod flag=GAGenome::CONTENTS) const;
  virtual void copy(const GAGenome &);

#ifdef GALIB_USE_STREAMS
  virtual int read(STD_ISTREAM & is);
  virtual int write (STD_OSTREAM & os) const;
#endif

  virtual int equal(const GAGenome & c) const ;
  virtual int resize(int x);

  const GAAlleleSet<T>& alleleset(unsigned int i=0) const 
    {return aset[i%naset];}

protected:
  int naset;
  GAAlleleSet<T> * aset;		// the allele set(s) for this genome
};




#ifdef GALIB_USE_BORLAND_INST
#include <ga/GA1DArrayGenome.C>
#endif

#endif


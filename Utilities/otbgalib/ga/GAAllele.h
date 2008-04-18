// $Header$
/* ----------------------------------------------------------------------------
  allele.h
  mbwall 21mar95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
   Here we define a class of alleles.  An allele is a possible value for a gene
and an allele set is a list of possible values (I use 'set' because it doesn't
imply the specific implementation of the container class).
---------------------------------------------------------------------------- */
#ifndef _ga_allele_h_
#define _ga_allele_h_

#include <ga/gaconfig.h>
#include <ga/garandom.h>
#include <ga/std_stream.h>

class GAAllele {
public:
  enum Type {ENUMERATED=1, BOUNDED=2, DISCRETIZED};
  enum BoundType {NONE, INCLUSIVE, EXCLUSIVE};
};


/* ----------------------------------------------------------------------------
  This object contains a set of alleles for use with a similarly typed genome.
  This object can be used with any type that has operator= defined for it.  If
you use the remove member then you must have operator== defined for it.
  This should be implemented as a derivative of the Array class?  But I don't
want that overhead at this point.  Also, behaviour is not the same.
  The allele set uses the envelope/message structure.  The core allele object
is a reference-counted structure that contains all of the guts of an allele
set.  The outer shell, the allele set itself, is what users actually see.  It
defines the interface.  With this setup you can create a single allele set then
each genome does not have to make its own copy.  And we don't have to worry
about an allele set going out of scope then leaving genomes hanging (a problem
if we just used a pointer to a single alleleset with no reference counting).
  You can link an allele set to another so that they share the same core.  Use
the 'link' member function (this is typically used within the GAlib to reduce 
the number of actual alleleset instances when cloning populations of genomes).
  There is no way to 'resize' an allele set.  You must add to it or remove
elements from it.
  The base template class assumes that the objects in the allele set are 
complex, i.e. it is not OK to do a bit-copy of each object.  We should do 
specializations for non-complex objects (or perhaps a separate class that does
bit-copies rather than logical-copies?)
  When you clone an allele set, the new one has its own core.
  Why didn't I do this as a couple of objects (enumerated set, bounded set, 
discretized set, etc)?  I wanted to be able to have an array that uses sets of
many different types.  I suppose the allele set should be completely 
polymorphic like the rest of the GAlib objects, but for now we'll do it as
a single object with multiple personalities (and a state).
  There is no error checking.  You should check the type before you try to
call any of the member functions.  In particular, if you try to get the
bounds on an enumerated set of one element, it will break.

*** should the assignment operator check to be sure that no allele is 
    duplicated, or is it OK to have duplicate alleles in a set?  For now we
    allow duplicates (via either the add or assignemnt ops).
---------------------------------------------------------------------------- */
template <class T>
class GAAlleleSetCore {
public:
  GAAlleleSetCore();
  GAAlleleSetCore(unsigned int n, const T array []);
  GAAlleleSetCore(const T& lower, const T& upper, 
		  GAAllele::BoundType lb=GAAllele::INCLUSIVE, 
		  GAAllele::BoundType ub=GAAllele::INCLUSIVE);
  GAAlleleSetCore(const T& lower, const T& upper, const T& increment, 
		  GAAllele::BoundType lb=GAAllele::INCLUSIVE, 
		  GAAllele::BoundType ub=GAAllele::INCLUSIVE);
  GAAlleleSetCore(const GAAlleleSetCore<T>&);
  virtual ~GAAlleleSetCore();
  GAAlleleSetCore<T>& operator=(const GAAlleleSetCore<T>&);

  GAAllele::Type type;		// is this an ennumerated or bounded set?
  GAAllele::BoundType lowerb, upperb; // what kind of limit is the bound?
  unsigned int cnt;		// how many objects are using us?
  unsigned int csz;		// how big are the chunks to allocate?
  unsigned int sz;		// number we have
  unsigned int SZ;		// how many have we allocated?
  T * a;
};


#if defined(GALIB_USE_COMP_OPERATOR_TEMPLATES)
template <class T> int operator==(const T &, const T &);
template <class T> int operator!=(const T &, const T &);
#endif


template <class T>
class GAAlleleSet {
public:
  GAAlleleSet() : core(0) {}
  GAAlleleSet(unsigned int n, const T a[]) : 
  core(new GAAlleleSetCore<T>(n,a)) {}
  GAAlleleSet(const T& lower, const T& upper, 
	      GAAllele::BoundType lb=GAAllele::INCLUSIVE, 
	      GAAllele::BoundType ub=GAAllele::INCLUSIVE) :
  core(new GAAlleleSetCore<T>(lower, upper, lb, ub)) {}
  GAAlleleSet(const T& lower, const T& upper, const T& increment,
	      GAAllele::BoundType lb=GAAllele::INCLUSIVE, 
	      GAAllele::BoundType ub=GAAllele::INCLUSIVE) :
  core(new GAAlleleSetCore<T>(lower, upper, increment, lb, ub)) {}
  GAAlleleSet(const GAAlleleSet<T>& set) : 
  core(new GAAlleleSetCore<T>(*(set.core))) {}
  virtual ~GAAlleleSet(){
    if(core != 0){
      core->cnt -= 1;
      if(core->cnt==0) delete core;
    }
  }
  GAAlleleSet<T> & operator=(const GAAlleleSet<T>& set){
    if(this == &set) return *this;
    if(core) *core = *set.core;
    else core = new GAAlleleSetCore<T>(*(set.core));
    return *this;
  } 
  GAAlleleSet<T> * clone() const {return new GAAlleleSet<T>(*this);}
  void link(GAAlleleSet<T>& set);
  void unlink();
  int size() const {return core->sz;}    // only meaninful for enumerated sets
  int add(const T&);		         // only for enumerated sets
  int remove(const T&);			 // only for enumerated sets
  int remove(unsigned int);		 // only for enumerated sets
  T allele() const;		         // ok for any type of set
  T allele(unsigned int i) const;        // only for enumerated sets
  T lower() const {return core->a[0];}   // only for bounded sets
  T upper() const {return core->a[1];}
  T inc() const {return core->a[2];}
  GAAllele::BoundType lowerBoundType() const {return core->lowerb;}
  GAAllele::BoundType upperBoundType() const {return core->upperb;}
  GAAllele::Type type() const {return core->type;}

#ifdef GALIB_USE_STREAMS
  int read(STD_ISTREAM &);
  int write(STD_OSTREAM & os) const;
#endif

#if defined(THINK_C)
  friend operator==(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
  friend operator!=(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
#elif defined(GALIB_USE_EMPTY_TEMPLATES)
  friend int operator==<>(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
  friend int operator!=<>(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
#elif defined(GALIB_USE_NAMED_TEMPLATES)
  friend int operator==<T>(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
  friend int operator!=<T>(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
#else
  friend int operator==(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
  friend int operator!=(const GAAlleleSet<T> &, const GAAlleleSet<T> &);
#endif

protected:
  GAAlleleSetCore<T> *core;
};


template <class T> 
class GAAlleleSetArray {
public:
  GAAlleleSetArray();
  GAAlleleSetArray(const GAAlleleSet<T>& s);
  GAAlleleSetArray(const GAAlleleSetArray<T>&);
  virtual ~GAAlleleSetArray();
  GAAlleleSetArray<T>& operator=(const GAAlleleSetArray<T>&);

  int size() const {return sz;}
  const GAAlleleSet<T>& set(unsigned int i) const {return *(aset[i]);}
  int add(const GAAlleleSet<T>& s);
  int add(unsigned int n, const T a[]);
  int add(const T& lower, const T& upper,
	  GAAllele::BoundType lb=GAAllele::INCLUSIVE,
	  GAAllele::BoundType ub=GAAllele::INCLUSIVE);
  int add(const T& lower, const T& upper, const T& increment,
	  GAAllele::BoundType lb=GAAllele::INCLUSIVE, 
	  GAAllele::BoundType ub=GAAllele::INCLUSIVE);
  int remove(unsigned int);

protected:
  unsigned int csz;
  unsigned int sz;
  unsigned int SZ;
  GAAlleleSet<T> **aset;
};


#ifdef GALIB_USE_STREAMS
template <class T> STD_OSTREAM & 
operator<< (STD_OSTREAM & os, const GAAlleleSet<T> & arg)
{ arg.write(os); return os; }
template <class T> STD_ISTREAM & 
operator>> (STD_ISTREAM & is, GAAlleleSet<T> & arg)
{ arg.read(is); return is; }
#endif



#ifdef GALIB_USE_BORLAND_INST
#include <ga/GAAllele.C>
#endif

#endif

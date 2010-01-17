// $Header$
/* ----------------------------------------------------------------------------
  allele.C
  mbwall 21mar95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
   Definition for the allele set class and its core container.  
---------------------------------------------------------------------------- */
#ifndef _ga_allele_C_
#define _ga_allele_C_

#include <string.h>
#include <ga/GAAllele.h>
#include <ga/gaerror.h>

#define GA_ALLELE_CHUNK 10

template <class T>
GAAlleleSetCore<T>::GAAlleleSetCore() : 
type(GAAllele::ENUMERATED), csz(GA_ALLELE_CHUNK), sz(0), SZ(0), a(0) {
  lowerb = GAAllele::NONE;
  upperb = GAAllele::NONE;

  cnt = 1;
}

template <class T>
GAAlleleSetCore<T>::GAAlleleSetCore(unsigned int n, const T array []) : 
type(GAAllele::ENUMERATED), csz(GA_ALLELE_CHUNK), sz(n), SZ(GA_ALLELE_CHUNK) {
  while(SZ < sz) SZ += csz;
  a = new T [SZ];
//  memcpy(a, array, n*sizeof(T));
  for(unsigned int i=0; i<sz; i++)
    a[i] = *(array+i);
  lowerb = GAAllele::NONE;
  upperb = GAAllele::NONE;

  cnt = 1;
}

template <class T>
GAAlleleSetCore<T>::
GAAlleleSetCore(const T& lower, const T& upper, 
		GAAllele::BoundType lb, GAAllele::BoundType ub) : 
type(GAAllele::BOUNDED), csz(GA_ALLELE_CHUNK), sz(2), SZ(2), a(new T[2]) {
  a[0] = lower;
  a[1] = upper;
  lowerb = lb;
  upperb = ub;

  cnt = 1;
}

template <class T>
GAAlleleSetCore<T>::
GAAlleleSetCore(const T& lower, const T& upper, const T& increment,
		GAAllele::BoundType lb, GAAllele::BoundType ub) : 
type(GAAllele::DISCRETIZED), csz(GA_ALLELE_CHUNK), sz(3), SZ(3), a(new T[3]) {
  a[0] = lower;
  a[1] = upper;
  a[2] = increment;
  lowerb = lb;
  upperb = ub;

  cnt = 1;
}

// We do not copy the original's reference count!
template <class T>
GAAlleleSetCore<T>::GAAlleleSetCore(const GAAlleleSetCore<T> & orig) :
csz(orig.csz), sz(orig.sz), SZ(orig.SZ), a(new T[orig.SZ]) {
//  memcpy(a, orig.a, sz*sizeof(T));
  for(unsigned int i=0; i<sz; i++)
    a[i] = orig.a[i];
  lowerb = orig.lowerb;
  upperb = orig.upperb;
  type = orig.type;

  cnt = 1;
}

// This just trashes everything.  This shouldn't get called before the count
// reaches zero, so if it does, we post an error.
template <class T>
GAAlleleSetCore<T>::~GAAlleleSetCore(){
  if(cnt > 0) GAErr(GA_LOC, "GAAlleleSetCore", "destructor", gaErrRefsRemain);
  delete [] a;
}

// Copying the contents of another allele set core does NOT change the current
// count of the allele set core!
template <class T> GAAlleleSetCore<T> & 
GAAlleleSetCore<T>::operator=(const GAAlleleSetCore<T> & orig){
  if(this == &orig) return *this;

  if(SZ < orig.sz){
    while(SZ < orig.sz) SZ += csz;
    delete [] a;
    a = new T [SZ];
  }

//  memcpy(a, orig.a, orig.sz*sizeof(T));
  for(unsigned int i=0; i<orig.sz; i++)
    a[i] = orig.a[i];

  sz = orig.sz;
  lowerb = orig.lowerb;
  upperb = orig.upperb;
  type = orig.type;

  return *this;
}

















// When we link to another allele set, we point our core to that one.  Be sure
// that we have a core.  If not, just point.  If so, trash as needed.
template <class T> void 
GAAlleleSet<T>::link(GAAlleleSet<T>& set){
  if(&set != this){
    if(core != 0){
      core->cnt -= 1;
      if(core->cnt == 0) delete core;
    }
    core=set.core; core->cnt += 1;
  }
}

template <class T> void
GAAlleleSet<T>::unlink(){
  if(core == 0) return;		// nothing to unlink
  if(core->cnt > 1){
    core->cnt -= 1;
    core = new GAAlleleSetCore<T>(*core);
  }
}



// If everthing goes OK, return 0.  If there's an error, we return -1.  I 
// really wish there were enough compilers doing exceptions to use them...
template <class T> int
GAAlleleSet<T>::add(const T & alle){
  if(core == 0) core = new GAAlleleSetCore<T>;
  if(core->type != GAAllele::ENUMERATED) return 1;
  if(core->sz >= core->SZ){
    core->SZ += core->csz;
    T * tmp = core->a;
    core->a = new T [core->SZ];
    for(unsigned int i=0; i<core->sz; i++)
      core->a[i] = tmp[i];
//      memcpy(core->a, tmp, core->sz*sizeof(T));
    delete [] tmp;
  }
  core->a[core->sz] = alle;
  core->sz += 1;
  return 0;
}

template <class T> int
GAAlleleSet<T>::remove(const T & allele){
  if(core == 0) core = new GAAlleleSetCore<T>;
  if(core->type != GAAllele::ENUMERATED) return 1;
  for(unsigned int i=0; i<core->sz; i++){
    if(core->a[i] == allele){
      for(unsigned int j=i; j<core->sz-1; j++)
	core->a[j] = core->a[j+1];
//	memmove(&(core->a[i]), &(core->a[i+1]), (core->sz-i-1)*sizeof(T));
      core->sz -= 1;
      i = core->sz;		// break out of the loop
    }
  }
  return 0;
}

template <class T> int
GAAlleleSet<T>::remove(unsigned int x){
  for(unsigned int j=x; j<core->sz-1; j++)
    core->a[j] = core->a[j+1];
//  memmove(&(core->a[i]), &(core->a[i+1]), (core->sz-i-1)*sizeof(T));
  core->sz -= 1;
  return 0;
}


// When returning an allele from the set, we have to know what type we are.
// The allele that we return depends on the type.  If we're an enumerated set
// then just pick randomly from the list of alleles.  If we're a bounded set
// then pick randomly from the bounds, and respect the bound types.  If we're 
// a discretized set then we do much as we would for the bounded set, but we 
// respect the discretization.
//   Be sure to specialize this member function (see the real genome for an
// example of how to do this)
template <class T> T
GAAlleleSet<T>::allele() const {
  if(core->type == GAAllele::ENUMERATED)
    return core->a[GARandomInt(0, core->sz-1)];
  else if(core->type == GAAllele::DISCRETIZED){
    GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)", gaErrOpUndef);
    return core->a[0];
  }
  else{
    GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)", gaErrOpUndef);
    return core->a[0];
  }
}


// This works only for enumerated sets.  If someone tries to use this on a
// non-enumerated set then we post an error message.  No bounds checking on 
// the value that was passed to us, but we do modulo it so that we'll never
// break.  Also, this means you can wrap an allele set around an array that
// is significantly larger than the allele set that defines its contents.
template <class T> T
GAAlleleSet<T>::allele(unsigned int i) const {
  if(core->type == GAAllele::ENUMERATED)
    return core->a[i % core->sz];
  else if(core->type == GAAllele::DISCRETIZED){
    GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)", gaErrOpUndef);
    return core->a[0];
  }
  else{
    GAErr(GA_LOC, "GAAlleleSet", "allele(unsigned int)", gaErrNoAlleleIndex);
    return core->a[0];
  }
}



#ifdef GALIB_USE_STREAMS
template <class T> int
GAAlleleSet<T>::read(STD_ISTREAM&){
  GAErr(GA_LOC, "GAAlleleSet", "read", gaErrOpUndef);
  return 1;
}
template <class T> int 
GAAlleleSet<T>::write(STD_OSTREAM &) const {
  GAErr(GA_LOC, "GAAlleleSet", "write", gaErrOpUndef);
  return 1;
}

/*
// ** won't work with NULL cores!!!!!!!

// The default read assumes input in the format that we write out.
//   We should do more checks on the values that we read in.
template <class T> int
GAAlleleSet<T>::read(STD_ISTREAM& is){
  char buf[32];
  int n;
  if(is.fail() || is.eof()) return 1;
  is.width(sizeof(buf));	// don't allow to overflow buffer
  is >> buf;
  if(strcmp(buf, "ENUMERATED") == 0){
    core->type = GAAllele::ENUMERATED;
    is >> n;
    if(is.fail() || is.eof()) return 1;
    core->sz = n;
    if(core->sz >= core->SZ){
      while(core->sz >= core->SZ) core->SZ += core->csz;
      delete [] core->a;
      core->a = new T [core->SZ];
      for(int i=0; i<core->sz; i++){
	is >> core->a[i];
	if(is.fail() || is.eof()) return 1;
      }
    }
  }
  else if(strcmp(buf, "BOUNDED") == 0){
    core->type = GAAllele::BOUNDED;
    delete [] core->a;
    core->SZ = 2;
    core->sz = 2;
    core->a = new T [core->SZ];
    is >> buf;
    if(strcmp(buf, "INCL") == 0) core->lowerb = GAAllele::INCLUSIVE;
    else core->lowerb = GAAllele::EXCLUSIVE;
    is >> buf;
    if(strcmp(buf, "INCL") == 0) core->upperb = GAAllele::INCLUSIVE;
    else core->upperb = GAAllele::EXCLUSIVE;
    is >> core->a[0] >> core->a[1];
    if(is.fail() || is.eof()) return 1;
  }
  else if(strcmp(buf, "DISCRETIZED") == 0){
    core->type = GAAllele::DISCRETIZED;
    delete [] core->a;
    core->SZ = 3;
    core->sz = 3;
    core->a = new T [core->SZ];
    is >> buf;
    if(strcmp(buf, "INCL") == 0) core->lowerb = GAAllele::INCLUSIVE;
    else core->lowerb = GAAllele::EXCLUSIVE;
    is >> buf;
    if(strcmp(buf, "INCL") == 0) core->upperb = GAAllele::INCLUSIVE;
    else core->upperb = GAAllele::EXCLUSIVE;
    is >> core->a[0] >> core->a[1];
    if(is.fail() || is.eof()) return 1;
  }
  else {
    is.clear(STD_IOS_BADBIT | is.rdstate());
    GAErr(GA_LOC, "GAAlleleSet", "read",
	  "unrecognized allele set type.", 
	  "Expected ENUMERATED, BOUNDED, or DISCRETIZED");
    return 1;
  }
  return 0;
}

// The default write method prints out the type of the allele set followed by
// the contents.  We use a single space as our separator.
template <class T> int 
GAAlleleSet<T>::write(STD_OSTREAM & os) const {
  switch(core->type){
  case GAAllele::ENUMERATED:
    os << "ENUMERATED ";
    os << core->sz << "\n";
    for(int i=0; i<core->sz; i++)
      os << core->a[i] << " ";
    os << "\n";
    break;

  case GAAllele::BOUNDED:
    os << "BOUNDED ";
    os << ((core->lowerb == GAAllele::INCLUSIVE) ? "INCL" : "EXCL") << " ";
    os << ((core->upperb == GAAllele::INCLUSIVE) ? "INCL" : "EXCL") << "\n";
    os << core->a[0] << " " << core->a[1] << "\n";
    break;

  case GAAllele::DISCRETIZED:
    os << "DISCRETIZED ";
    os << ((core->lowerb == GAAllele::INCLUSIVE) ? "INCL" : "EXCL") << " ";
    os << ((core->upperb == GAAllele::INCLUSIVE) ? "INCL" : "EXCL") << "\n";
    os << core->a[0] << " " << core->a[1] << " " << core->a[2] << "\n";
    break;

  default:
    break;
  }
  return 0;
}
*/
#endif

// could do these with a memcmp if the type is simple...
template <class T> int 
operator==(const GAAlleleSet<T> & a, const GAAlleleSet<T> & b) {
  if(a.core == b.core) return 1;
  if(a.core == 0 || b.core == 0) return 0;
  if(a.core->sz != b.core->sz) return 0;
// return((memcmp(a.core, b.core, a.core->sz * sizeof(T)) == 0) ? 1 : 0);
  unsigned int i;
  for(i=0; i<a.core->sz && a.core->a[i] == b.core->a[i]; i++);
  return((i == a.core->sz) ? 1 : 0);
}

template <class T> int 
operator!=(const GAAlleleSet<T> & a, const GAAlleleSet<T> & b) {
  if(a.core == b.core) return 0;
  if(a.core == 0 || b.core == 0) return 1;
  if(a.core->sz != b.core->sz) return 1;
// return((memcmp(a.core, b.core, a.core->sz * sizeof(T)) == 0) ? 0 : 1);
  unsigned int i;
  for(i=0; i<a.core->sz && a.core->a[i] == b.core->a[i]; i++);
  return((i == a.core->sz) ? 0 : 1);
}














template <class T>
GAAlleleSetArray<T>::GAAlleleSetArray() :
csz(GA_ALLELE_CHUNK), sz(0), SZ(0), aset(0) {}

template <class T>
GAAlleleSetArray<T>::GAAlleleSetArray(const GAAlleleSet<T>& s) :
csz(GA_ALLELE_CHUNK), sz(1), SZ(GA_ALLELE_CHUNK), 
aset(new GAAlleleSet<T> * [GA_ALLELE_CHUNK]) {
  aset[0] = new GAAlleleSet<T>(s);
}

template <class T>
GAAlleleSetArray<T>::GAAlleleSetArray(const GAAlleleSetArray<T>& orig) :
csz(orig.csz), sz(orig.sz), SZ(orig.SZ), 
aset(new GAAlleleSet<T> * [orig.SZ]) {
  for(unsigned int i=0; i<sz; i++)
    aset[i] = new GAAlleleSet<T>(orig.set(i));
}

template <class T>
GAAlleleSetArray<T>::~GAAlleleSetArray(){
  for(unsigned int i=0; i<sz; i++)
    delete aset[i];
  delete [] aset;
}

template <class T> GAAlleleSetArray<T>&
GAAlleleSetArray<T>::operator=(const GAAlleleSetArray<T>& orig){
  if(this == &orig) return *this;
  unsigned int i;
  for(i=0; i<sz; i++)
    delete aset[i];

  if(SZ < orig.sz){
    while(SZ < orig.sz) SZ += csz;
    delete [] aset;
    aset = new GAAlleleSet<T> * [SZ];
  }
  for(i=0; i<orig.sz; i++)
    aset[i] = new GAAlleleSet<T>(orig.set(i));

  sz = orig.sz;

  return *this;
}

// Return 0 if everything goes OK, otherwise return -1
template <class T> int
GAAlleleSetArray<T>::add(const GAAlleleSet<T>& s){
  if(sz+1 > SZ){
    SZ += csz;
    GAAlleleSet<T>** tmp = aset;
    aset = new GAAlleleSet<T> * [SZ];
    memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T>*));
    delete [] tmp;
  }
  aset[sz] = new GAAlleleSet<T>(s);
  sz++;
  return 0;
}

template <class T> int
GAAlleleSetArray<T>::add(unsigned int n, const T a[]){
  if(sz+1 > SZ){
    SZ += csz;
    GAAlleleSet<T>** tmp = aset;
    aset = new GAAlleleSet<T> * [SZ];
    memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T>*));
    delete [] tmp;
  }
  aset[sz] = new GAAlleleSet<T>(n, a);
  sz++;
  return 0;
}

template <class T> int
GAAlleleSetArray<T>::add(const T& lower, const T& upper,
			 GAAllele::BoundType lb, GAAllele::BoundType ub){
  if(sz+1 > SZ){
    SZ += csz;
    GAAlleleSet<T>** tmp = aset;
    aset = new GAAlleleSet<T> * [SZ];
    memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T>*));
    delete [] tmp;
  }
  aset[sz] = new GAAlleleSet<T>(lower,upper,lb,ub);
  sz++;
  return 0;
}

template <class T> int
GAAlleleSetArray<T>::add(const T& lower, const T& upper, const T& increment,
			 GAAllele::BoundType lb, GAAllele::BoundType ub){
  if(sz+1 > SZ){
    SZ += csz;
    GAAlleleSet<T>** tmp = aset;
    aset = new GAAlleleSet<T> * [SZ];
    memcpy(aset, tmp, sz * sizeof(GAAlleleSet<T>*));
    delete [] tmp;
  }
  aset[sz] = new GAAlleleSet<T>(lower,upper,increment,lb,ub);
  sz++;
  return 0;
}

template <class T> int
GAAlleleSetArray<T>::remove(unsigned int n){
  if(n >= sz) return 1;
  delete aset[n];
  for(unsigned int i=n; i<sz-1; i++)
    aset[i] = aset[i+1];
  sz--;
  return 0;
}

#endif

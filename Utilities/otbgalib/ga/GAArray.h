// $Header$
/* ----------------------------------------------------------------------------
  arraytmpl.h
  mbwall 22apr95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the array base class.  Be careful what you put into one of these
arrays!  This class can be used only on objects that have:

  a default constructor (takes no arguments)
  operator=
  operator==
  operator!=

In other words, use only primitive objects in this array (e.g. int, float,
pointers, etc)

The constructor will try to initialize to zero, but only if the type is right.

We don't do any over-allocation, so resizing can be expensive.
No error checking on the copy, so don't walk over end of array!

TODO:
  should do specialization for simple types that does memcpy rather than loop
---------------------------------------------------------------------------- */
#ifndef _ga_arraytmpl_h_
#define _ga_arraytmpl_h_

template <class T>
class GAArray {
public:
  GAArray(unsigned int s) : sz(s), a(sz ? new T[sz] : 0)
    {for(unsigned int i=0; i<sz; i++) a[i] = (T)0; }
  GAArray(const GAArray<T> & orig){sz=0; a=(T *)0; copy(orig);}
  GAArray<T> & operator=(const GAArray<T> & orig){copy(orig); return *this;}
  GAArray<T> & operator=(const T array []) // no err checks!
    {for(unsigned int i=0; i<sz; i++) a[i] = *(array+i); return *this;}
  virtual ~GAArray(){delete [] a;}

  GAArray<T> * clone(){return new GAArray<T>(*this);}
  operator const T * () const {return a;}
  operator T * () {return a;}
  const T & operator[](unsigned int i) const {return a[i];}
  T & operator[](unsigned int i) {return a[i];}
  void copy(const GAArray<T> & orig){
    size(orig.sz);
    for(unsigned int i=0; i<sz; i++)
      a[i] = orig.a[i];
  }
  void copy(const GAArray<T> & orig, unsigned int dest,
	    unsigned int src, unsigned int length){
    for(unsigned int i=0; i<length; i++) a[dest+i] = orig.a[src+i];
  }
  void move(unsigned int dest, unsigned int src, unsigned int length){
    if(src > dest)
      for(unsigned int i=0; i<length; i++) a[dest+i] = a[src+i];
    else if(src < dest)
      for(unsigned int i=length-1; i!=0; i--) a[dest+i] = a[src+i];
  }
  void swap(unsigned int i, unsigned int j){T tmp=a[j]; a[j]=a[i]; a[i]=tmp;}
  int size() const {return sz;}
  int size(unsigned int n){
    if(n == sz) return sz;
    T * tmp = (n ? new T[n] : 0);
    for(int i=((n < sz) ? n-1 : sz-1); i>=0; i--) tmp[i] = a[i];
    delete [] a;
    a = tmp;
    return sz=n;
  }
  int equal(const GAArray<T> & b,
	    unsigned int dest, unsigned int src, unsigned int length) const {
    for(unsigned int i=0; i<length; i++)
      if(a[dest+i] != b.a[src+i]) return 0;
    return 1;
  }

protected:
  unsigned int sz;		// number of elements
  T * a;			// the contents of the array
};


template <class T> int 
operator==(const GAArray<T> & a, const GAArray<T> & b){
  if(a.size() != b.size()) return 0;
  return a.equal(b,0,0,a.sz);
}
template <class T> int 
operator!=(const GAArray<T> & a, const GAArray<T> & b){
  if(a.size() != b.size()) return 1;
  return a.equal(b,0,0,a.sz) ? 0 : 1;
}

#endif

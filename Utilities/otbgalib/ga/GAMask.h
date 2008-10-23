// $Header$
/* ----------------------------------------------------------------------------
  binstr1.h
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved
---------------------------------------------------------------------------- */
#ifndef _ga_mask_h_
#define _ga_mask_h_

#include <string.h>

#define GA_MASK_TYPE char

class GAMask {
public:
  GAMask() { _n=0; _mask=(GA_MASK_TYPE*)0; }
  GAMask(const GAMask& m) { _n=0; _mask=(GA_MASK_TYPE*)0; copy(m); }
  GAMask& operator=(const GAMask& m) { copy(m); return *this; }
  ~GAMask() { delete [] _mask; }
  void copy(const GAMask& m) {
    size(m.size());
    memcpy(_mask, m._mask, _n*sizeof(GA_MASK_TYPE));
  }

  void clear() { memset(_mask, 0, _n*sizeof(GA_MASK_TYPE)); }
  int size() const {return _n;}
  int size(unsigned int s){
    if(s > _n){
      _n = s;
      delete [] _mask;
      _mask = new GA_MASK_TYPE [_n];
    }
    return _n;
  }
  GA_MASK_TYPE mask(unsigned int i) const { return _mask[i]; }
  GA_MASK_TYPE & mask(unsigned int i) { return _mask[i]; }
  GA_MASK_TYPE operator[] (unsigned int i) const { return _mask[i]; }
  GA_MASK_TYPE & operator[] (unsigned int i) { return _mask[i]; }

protected:
  GA_MASK_TYPE * _mask;
  unsigned int _n;
};

#endif

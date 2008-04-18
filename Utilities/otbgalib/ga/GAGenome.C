// $Header$
/* ----------------------------------------------------------------------------
  genome.C
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Definitions for genome base class.  See the header file for complete
documentation for deriving new classes.  Comments here are implementation-
specific details about base class member functions.
---------------------------------------------------------------------------- */
#include <ga/GAGenome.h>

//   These are the default genome operators.
// None does anything - they just post an error message to let you know that no
// method has been defined.  These are for the base class (which has no 
// function by itself). 
void 
GAGenome::NoInitializer(GAGenome & c){ 
  GAErr(GA_LOC, c.className(), "initializer", gaErrOpUndef); 
}
int 
GAGenome::NoMutator(GAGenome & c, float){ 
  GAErr(GA_LOC, c.className(), "mutator", gaErrOpUndef); return 0; 
}
float 
GAGenome::NoComparator(const GAGenome& c, const GAGenome&){ 
  GAErr(GA_LOC, c.className(), "comparator", gaErrOpUndef); return -1.0; 
}



GAGenome::
GAGenome(Initializer i, Mutator m, Comparator c){
  if(i==0) i=NoInitializer;
  if(m==0) m=NoMutator;
  if(c==0) c=NoComparator;
  _score=_fitness=0.0; _evaluated=gaFalse; _neval=0;
  ga=0; ud=0; eval=0; evd=0;
  init=i; mutr=m; cmp=c;
  sexcross = 0;
  asexcross = 0;
}

GAGenome::GAGenome(const GAGenome & orig){
  evd=0; _neval=0;
  GAGenome::copy(orig);
}

GAGenome::~GAGenome(){
  delete evd;
}

GAGenome*
GAGenome::clone(CloneMethod) const {
  GAErr(GA_LOC, className(), "clone", gaErrOpUndef);
  return new GAGenome(*this);
}

// The eval count is not copied from the other genome - that would inflate the
// count.
void
GAGenome::copy(const GAGenome & orig){
  if(&orig == this) return;
  _score=orig._score; _fitness=orig._fitness; _evaluated=orig._evaluated;
  ga=orig.ga; ud=orig.ud; eval=orig.eval;
  init=orig.init; mutr=orig.mutr; cmp=orig.cmp;
  sexcross=orig.sexcross; asexcross=orig.asexcross;
  _neval = 0;

  if(orig.evd){
    if(evd) evd->copy(*orig.evd);
    else evd = orig.evd->clone();
  }				// don't delete if c doesn't have one
}


float 
GAGenome::evaluate(GABoolean flag) const {
  if(_evaluated == gaFalse || flag == gaTrue){
    GAGenome *This = (GAGenome*)this;
    if(eval){ This->_neval++; This->_score = (*eval)(*This); }
    This->_evaluated = gaTrue;
  }
  return _score; 
}

// $Header$
/* ----------------------------------------------------------------------------
  scaling.C
  mbwall 10aug94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Class definitions for the scaling objects.  These objects translate (scale)
the raw objective scores of a population into 'fitness' scores that are used to
determine which genomes are fit for mating/selection.
  In all of these routines we do a check to be sure that we have enough space
to do our evaluations.  We don't need to do this so long as the population 
objects that call us do the test themselves.  I'll leave the redundancy for
now.
---------------------------------------------------------------------------- */
#include <math.h>
#include <ga/gaerror.h>
#include <ga/GAScaling.h>
#include <ga/GAGenome.h>
#include <ga/GAPopulation.h>
#include <ga/GABaseGA.h>


float gaDefLinearScalingMultiplier   = 1.2;
float gaDefSigmaTruncationMultiplier = 2.0;
float gaDefPowerScalingFactor        = 1.0005;
float gaDefSharingCutoff             = 1.0;




/* ----------------------------------------------------------------------------
NoScaling
---------------------------------------------------------------------------- */
// Assign the fitness scores to be the same as the objective scores for all of
// the individuals in the population.
void 
GANoScaling::evaluate(const GAPopulation& p) {
  for(int i=0; i<p.size(); i++)
    p.individual(i).fitness(p.individual(i).score());
}



#if USE_LINEAR_SCALING == 1
/* ----------------------------------------------------------------------------
LinearScaling
---------------------------------------------------------------------------- */
// Scale the objective scores in the population.  We assume that the raw
// evaluation has already taken place (the calculation of the objective scores
// of the genomes in the population).  We must use a precision higher than that
// of the genome scores so that we don't lose any information.

void 
GALinearScaling::evaluate(const GAPopulation & p) {
// Here we calculate the slope and intercept using the multiplier and objective
// score ranges...

  double pmin = p.min();
  double pmax = p.max();
  double pave = p.ave();

  double delta, a, b;
  if(pave == pmax){	// no scaling - population is all the same
    a = 1.0; 
    b = 0.0;
  }
  else if(pmin > ((double)c * pave - pmax)/((double)c - 1.0)){
    delta = pmax - pave;
    a = ((double)c - 1.0) * pave / delta;
    b = pave * (pmax - (double)c * pave) / delta;
  }
  else{				// stretch to make min be 0
    delta = pave - pmin;
    a = pave / delta;
    b = -pmin * pave / delta;
  }

// and now we calculate the scaled scaled values.  Negative scores are not
// allowed with this kind of scaling, so check for negative values.  If we get
// a negative value, dump an error message then set all of the scores to 0.

  for(int i=0; i<p.size(); i++){
    double f = p.individual(i).score();
    if(f < 0.0){
      GAErr(GA_LOC, className(), "evaluate", gaErrNegFitness);
      for(int ii=0; ii<p.size(); ii++)
	p.individual(ii).fitness(0.0);
      return;
    }
    f = f * a + b;
    if(f < 0) f = 0.0;	// truncate if necessary (only due to roundoff error)
    p.individual(i).fitness((float)f);       // might lose information here!
  }
}


// Set the multiplier for this selection type.  The fmultiplier must be greater
// than 1.0 or else we'll get a divide by zero error in our scaling operations.
float
GALinearScaling::multiplier(float fm) {
  if(fm <= 1.0){
    GAErr(GA_LOC, className(), "multiplier", gaErrBadLinearScalingMult);
    return c;
  }
  return c = fm;
}

#endif



#if USE_SIGMA_TRUNC_SCALING == 1
/* ----------------------------------------------------------------------------
SigmaTruncationScaling
---------------------------------------------------------------------------- */
// This is an implementation of the sigma truncation scaling method descibed in
// goldberg p 124.  If the scaled fitness is less than zero, we arbitrarily set
// it to zero (thus the truncation part of 'sigma truncation').
void 
GASigmaTruncationScaling::evaluate(const GAPopulation & p) {
  for(int i=0; i<p.size(); i++){
    double f = (double)(p.individual(i).score()) - (double)(p.ave());
    f += (double)c * (double)(p.dev());
    if(f < 0) f = 0.0;
    p.individual(i).fitness((float)f);       // might lose information here!
  }
}


// Set the multiplier for this selection type.  It should be greater than or
// equal to zero.
float
GASigmaTruncationScaling::multiplier(float fm) {
  if(fm < 0.0){
    GAErr(GA_LOC, className(), "multiplier", gaErrBadSigmaTruncationMult);
    return c;
  }
  return c = fm;
}

#endif



#if USE_POWER_LAW_SCALING == 1
/* ----------------------------------------------------------------------------
PowerLawScaling
---------------------------------------------------------------------------- */
// This is an implementation of the most basic form of power scaling, where the
// fitness is a function of the objective score raised to some power.  Negative
// objective scores are not allowed.  If we get one, we post an error and set
// all of the fitness scores to zero.
void 
GAPowerLawScaling::evaluate(const GAPopulation & p) {
  for(int i=0; i<p.size(); i++){
    double f = p.individual(i).score();
    if(f < 0.0){
      GAErr(GA_LOC, className(), "evaluate", gaErrPowerNegFitness);
      for(int ii=0; ii<p.size(); ii++)
	p.individual(ii).fitness(0.0);
      return;
    }
    f = pow(f,(double)k);
    p.individual(i).fitness((float)f);       // might lose information here!
  }
}
#endif



#if USE_SHARING == 1
/* ----------------------------------------------------------------------------
Sharing
---------------------------------------------------------------------------- */
// This is an implementation of speciation using the sharing method described
// by goldberg in his book.  This requires a user-defined distance function in
// order to work.  The distance function returns a value between
// 0 and 1 inclusive to tell us how similar two genomes are to each other.
// A value of 0 means that the two genomes are identical to each other, a
// value of 1 means they are completely different.
//   A single genome is identical to itself, so d(i,i) is 0.
//   If alpha is 1 then we don't use pow().
//   If we have a comparator to use, use it.  If not, use the comparator of
// each genome.
//   We can cut in half the number of calls to the sharing function by keeping
// one half of the ixj matrix.  This is because d(i,j) is the same as d(j,i).
// We cache the distances in an upper right triangular matrix stored as a 
// series of floats.
//   If the population is maximizing then we derate by dividing.  If the 
// population is minimizing then we derate by multiplying.  First we check to 
// see if there is a GA using the population.  If there is, we use its min/max
// flag to determine whether or not we should be minimizing or maximizing.  If
// there is not GA with the population, then we use the population's sort order
// as the basis for whether to minimize or maximize.
// *** This could be done with n*n/2 instead of n*n, to reduce storage, but we
// can't reduce computation any more...
// *** probably should use the diversity built-in to the population...
void 
GASharing::evaluate(const GAPopulation& p) {
  if(p.size() > (int)N){
    delete [] d;
    N = p.size();
    d = new float[N*N];
  }
  int n = p.size();

  int i, j;
  if(df) {
    for(i=0; i<n; i++){		// calculate and cache the distances
      d[i*n+i] = 0.0;		// each genome is same as itself
      for(j=i+1; j<n; j++)
	d[i*n+j] = d[j*n+i] = (*df)(p.individual(i), p.individual(j));
    }
  }
  else {
    for(i=0; i<n; i++){		// calculate and cache the distances
      d[i*n+i] = 0.0;		// each genome is same as itself
      for(j=i+1; j<n; j++)
	d[i*n+j] = d[j*n+i] = p.individual(i).compare(p.individual(j));
    }
  }

  int mm;
  if(_minmax == 0) {
    if(p.geneticAlgorithm())
      mm = p.geneticAlgorithm()->minimaxi();
    else
      mm = ((p.order() == GAPopulation::HIGH_IS_BEST) ? 
	    GAGeneticAlgorithm::MAXIMIZE : GAGeneticAlgorithm::MINIMIZE);
  }
  else {
    mm = _minmax;
  }

  for(i=0; i<n; i++){		// now derate the fitness of each genome
    double sum = 0.0;
    for(j=0; j<n; j++) {
      if(d[i*n+j] < _sigma) {
	if(_alpha == 1)
	  sum += ((d[i*n+j] >= _sigma) ? 0.0 : 1.0 - d[i*n+j]/_sigma);
	else
	  sum += ((d[i*n+j]>=_sigma) ? 0.0 : 1.0-pow(d[i*n+j]/_sigma,_alpha));
      }
    }
    double f;
    if(mm == GAGeneticAlgorithm::MINIMIZE)
      f = p.individual(i).score() * sum;
    else
      f = p.individual(i).score() / sum;
    p.individual(i).fitness((float)f);       // might lose information here!
  }
}

void 
GASharing::copy(const GAScalingScheme & arg){
  if(&arg == this) return;

  GAScalingScheme::copy(arg);
  const GASharing& s = DYN_CAST(const GASharing&, arg);
  delete [] d;
  _minmax = s._minmax;
  _sigma = s._sigma;
  _alpha = s._alpha;
  df = s.df;
  N = s.N;
  d = new float[N*N];
  memcpy(d, s.d, N*N*sizeof(float));
}


// The cutoff for triangular sharing must always be greater than 0
float
GASharing::sigma(float c) {
  if(c <= 0.0){
    GAErr(GA_LOC, className(), "sigma", gaErrBadSharingCutoff);
    return _sigma;
  }
  return _sigma = c;
}

int
GASharing::minimaxi(int i) { 
  if(i == GAGeneticAlgorithm::MAXIMIZE)      
    _minmax = GAGeneticAlgorithm::MAXIMIZE;
  else if(i == GAGeneticAlgorithm::MINIMIZE) 
    _minmax = GAGeneticAlgorithm::MINIMIZE;
  else                       
    _minmax = 0;
  return _minmax;
}

#endif

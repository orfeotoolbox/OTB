// $Header$
/* ----------------------------------------------------------------------------
  gasteadystate.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

  Header file for the steady-state genetic algorithm class.
---------------------------------------------------------------------------- */
#ifndef _ga_gasteadystate_h_
#define _ga_gasteadystate_h_

#include <ga/GABaseGA.h>

class GASteadyStateGA : public GAGeneticAlgorithm {
public:
  GADefineIdentity("GASteadyStateGA", GAID::SteadyStateGA);

  static GAParameterList& registerDefaultParameters(GAParameterList&);

public:
  GASteadyStateGA(const GAGenome&);
  GASteadyStateGA(const GAPopulation&);
  GASteadyStateGA(const GASteadyStateGA&);
  GASteadyStateGA& operator=(const GASteadyStateGA&);
  virtual ~GASteadyStateGA();
  virtual void copy(const GAGeneticAlgorithm&);

  virtual void initialize(unsigned int seed=0);
  virtual void step();
  GASteadyStateGA & operator++() { step(); return *this; }

  virtual int setptr(const char* name, const void* value);
  virtual int get(const char* name, void* value) const;

  virtual int minimaxi() const {return minmax;}
  virtual int minimaxi(int m);

  virtual const GAPopulation& population() const {return *pop;}
  virtual const GAPopulation& population(const GAPopulation&);
  virtual int populationSize() const {return pop->size();}
  virtual int populationSize(unsigned int n);
  virtual GAScalingScheme& scaling() const {return pop->scaling();}
  virtual GAScalingScheme& scaling(const GAScalingScheme & s)
    { /* tmpPop->scaling(s); */ return GAGeneticAlgorithm::scaling(s); }
  virtual GASelectionScheme& selector() const {return pop->selector(); }
  virtual GASelectionScheme& selector(const GASelectionScheme& s)
    { /* tmpPop->selector(s); */ return GAGeneticAlgorithm::selector(s); }
  virtual void objectiveFunction(GAGenome::Evaluator f);
  virtual void objectiveData(const GAEvalData& v);

  float pReplacement() const { return pRepl; }
  float pReplacement(float p);
  int nReplacement() const { return nRepl; }
  int nReplacement(unsigned int n);

protected:
  GAPopulation *tmpPop;		// temporary population for replacements
  float pRepl;			// percentage of population to replace each gen
  unsigned int nRepl;		// how many of each population to replace
  short which;			// 0 if prepl, 1 if nrepl
};



#ifdef GALIB_USE_STREAMS
inline STD_OSTREAM & operator<< (STD_OSTREAM & os, GASteadyStateGA & arg)
{ arg.write(os); return(os); }
inline STD_ISTREAM & operator>> (STD_ISTREAM & is, GASteadyStateGA & arg)
{ arg.read(is); return(is); }
#endif

#endif

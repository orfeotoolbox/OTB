/* ----------------------------------------------------------------------------
  gademe.h
  mbwall jan96
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved

  Header for the deme (parallel population) genetic algorithm class.
  This genetic algorithm lets you specify a number of individuals to migrate
from one population to another at the end of each generation.  You can specify
how many populations to maintain.  Each population evolves using a steady-state
genetic algorithm.  At the end of each generation, the specified number of 
individuals migrate from one population to the next (we use the loop migration
topology in this implementation).
  You can modify the migration method by deriving a new class from this one and
redefine the migration method.  If you want to use a different kind of genetic
algorithm for each population then you'll have to modify the mechanics of the
step method.
---------------------------------------------------------------------------- */
#ifndef _ga_gademe_h_
#define _ga_gademe_h_

#include <ga/GABaseGA.h>

class GADemeGA : public GAGeneticAlgorithm {
public:
  GADefineIdentity("GADemeGA", GAID::DemeGA);

  enum {ALL=(-1)};
  static GAParameterList& registerDefaultParameters(GAParameterList&);

public:
  GADemeGA(const GAGenome&);
  GADemeGA(const GAPopulation&);
  GADemeGA(const GADemeGA&);
  GADemeGA& operator=(const GADemeGA&);
  virtual ~GADemeGA();
  virtual void copy(const GAGeneticAlgorithm&);


  virtual void initialize(unsigned int seed=0);
  virtual void step();
  virtual void migrate();	// new for this derived class
  GADemeGA & operator++() { step(); return *this; }

  virtual int setptr(const char* name, const void* value);
  virtual int get(const char* name, void* value) const;

  virtual int minimaxi() const {return minmax;}
  virtual int minimaxi(int m);

  virtual const GAPopulation& population() const {return *pop;}
  virtual const GAPopulation& population(const GAPopulation& p)
    { GAGeneticAlgorithm::population(p); return population(ALL,p); }
  virtual int populationSize() const {return pop->size();}
  virtual int populationSize(unsigned int n) 
    { GAGeneticAlgorithm::populationSize(n); return populationSize(ALL,n); }
  virtual GAScalingScheme& scaling() const {return pop->scaling();}
  virtual GAScalingScheme& scaling(const GAScalingScheme & s)
    { GAGeneticAlgorithm::scaling(s); return scaling(ALL,s);}
  virtual GASelectionScheme& selector() const {return pop->selector(); }
  virtual GASelectionScheme& selector(const GASelectionScheme& s)
    { GAGeneticAlgorithm::selector(s); return selector(ALL,s);}
  virtual void objectiveFunction(GAGenome::Evaluator f)
    { GAGeneticAlgorithm::objectiveFunction(f); objectiveFunction(ALL,f); }
  virtual void objectiveData(const GAEvalData& v)
    { GAGeneticAlgorithm::objectiveData(v); objectiveData(ALL,v); }

  const GAPopulation& population(unsigned int i) const {return *deme[i];}
  const GAPopulation& population(int i, const GAPopulation&);
  int populationSize(unsigned int i) const {return deme[i]->size();}
  int populationSize(int i, unsigned int n);
  int nReplacement(unsigned int i) const {return nrepl[i];}
  int nReplacement(int i, unsigned int n);
  int nMigration() const {return nmig;}
  int nMigration(unsigned int i);
  int nPopulations() const {return npop;}
  int nPopulations(unsigned int i);

  GAScalingScheme& scaling(unsigned int i) const {return deme[i]->scaling();}
  GAScalingScheme& scaling(int i, const GAScalingScheme & s);
  GASelectionScheme& selector(unsigned int i)const{return deme[i]->selector();}
  GASelectionScheme& selector(int i, const GASelectionScheme& s);
  void objectiveFunction(int i, GAGenome::Evaluator f);
  void objectiveData(int i, const GAEvalData&);

  const GAStatistics& statistics() const {return stats;}
  const GAStatistics& statistics(unsigned int i) const {return pstats[i];}

protected:
  unsigned int npop;		// how many populations do we have?
  int *nrepl;			// how many to replace each generation
  GAPopulation** deme;		// array of populations that we'll use
  GAPopulation* tmppop;		// temp pop for doing the evolutions
  GAStatistics* pstats;		// statistics for each population
  unsigned int nmig;		// number to migrate from each population
};

#ifdef GALIB_USE_STREAMS
inline STD_OSTREAM & operator<< (STD_OSTREAM & os, GADemeGA & arg)
{arg.write(os); return(os);}
inline STD_ISTREAM & operator>> (STD_ISTREAM & is, GADemeGA & arg)
{arg.read(is); return(is);}
#endif

#endif

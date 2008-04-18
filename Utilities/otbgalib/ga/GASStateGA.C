// $Header$
/* ----------------------------------------------------------------------------
  gasteadystate.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

   Souce file for the steady-state genetic algorithm object.
---------------------------------------------------------------------------- */
#include <ga/GASStateGA.h>
#include <ga/garandom.h>

//#define GA_DEBUG

#define USE_PREPL 0
#define USE_NREPL 1

GAParameterList&
GASteadyStateGA::registerDefaultParameters(GAParameterList& p) {
  GAGeneticAlgorithm::registerDefaultParameters(p);

  int ival = 1;
  p.add(gaNnReplacement, gaSNnReplacement, GAParameter::INT, &ival);
  p.add(gaNpReplacement, gaSNpReplacement, GAParameter::FLOAT, &gaDefPRepl);

  p.set(gaNscoreFrequency, gaDefScoreFrequency2);

  return p;
}

GASteadyStateGA::GASteadyStateGA(const GAGenome& c) : GAGeneticAlgorithm(c) {
  pRepl = gaDefPRepl;
  params.add(gaNpReplacement, gaSNpReplacement, GAParameter::FLOAT, &pRepl);

  float n = ((pRepl*(float)pop->size() < 1) ? 1 : pRepl*(float)pop->size());
  tmpPop = new GAPopulation(pop->individual(0), (unsigned int)n);

  nRepl = tmpPop->size();
  params.add(gaNnReplacement, gaSNnReplacement, GAParameter::INT, &nRepl);
  stats.scoreFrequency(gaDefScoreFrequency2);
  params.set(gaNscoreFrequency, gaDefScoreFrequency2);

  which = USE_PREPL;
}
GASteadyStateGA::GASteadyStateGA(const GAPopulation& p): GAGeneticAlgorithm(p){
  pRepl = gaDefPRepl;
  params.add(gaNpReplacement, gaSNpReplacement, GAParameter::FLOAT, &pRepl);

  float n = ((pRepl*(float)pop->size() < 1) ? 1 : pRepl*(float)pop->size());
  tmpPop = new GAPopulation(pop->individual(0), (unsigned int)n);

  nRepl = tmpPop->size();
  params.add(gaNnReplacement, gaSNnReplacement, GAParameter::INT, &nRepl);
  stats.scoreFrequency(gaDefScoreFrequency2);
  params.set(gaNscoreFrequency, gaDefScoreFrequency2);

  which = USE_PREPL;
}
GASteadyStateGA::GASteadyStateGA(const GASteadyStateGA& ga) : 
GAGeneticAlgorithm(ga) {
  tmpPop = (GAPopulation *)0;
  copy(ga);
}
GASteadyStateGA::~GASteadyStateGA(){
  delete tmpPop;
}
GASteadyStateGA&
GASteadyStateGA::operator=(const GASteadyStateGA& ga){
  if(&ga != this) copy(ga); 
  return *this;
}
void
GASteadyStateGA::copy(const GAGeneticAlgorithm & g){
  GAGeneticAlgorithm::copy(g);
  const GASteadyStateGA& ga = DYN_CAST(const GASteadyStateGA&, g);
  
  pRepl = ga.pRepl;
  nRepl = ga.nRepl;
  
  if(tmpPop) tmpPop->copy(*(ga.tmpPop));
  else tmpPop = ga.tmpPop->clone();
  tmpPop->geneticAlgorithm(*this);
  
  which = ga.which;
}


int
GASteadyStateGA::setptr(const char* name, const void* value){
  int status = GAGeneticAlgorithm::setptr(name, value);

  if(strcmp(name, gaNpReplacement) == 0 ||
	  strcmp(name, gaSNpReplacement) == 0){
#ifdef GA_DEBUG
  cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name << "' to '" << *((float*)value) << "'\n";
#endif
    pReplacement(*((float*)value));
    status = 0;
  }
  else if(strcmp(name, gaNnReplacement) == 0 ||
	  strcmp(name, gaSNnReplacement) == 0){
#ifdef GA_DEBUG
  cerr << "GAGeneticAlgorithm::setptr\n  setting '" << name << "' to '" << *((int*)value) << "'\n";
#endif
    nReplacement(*((int*)value));
    status = 0;
  }
  return status;
}


int
GASteadyStateGA::get(const char* name, void* value) const {
  int status = GAGeneticAlgorithm::get(name, value);

  if(strcmp(name, gaNpReplacement) == 0 ||
	  strcmp(name, gaSNpReplacement) == 0){
    *((float*)value) = pRepl;
    status = 0;
  }
  else if(strcmp(name, gaNnReplacement) == 0 || 
	  strcmp(name, gaSNnReplacement) == 0){
    *((int*)value) = nRepl;
    status = 0;
  }
  return status;
}


void 
GASteadyStateGA::objectiveFunction(GAGenome::Evaluator f){
  GAGeneticAlgorithm::objectiveFunction(f);
  for(int i=0; i<tmpPop->size(); i++)
    tmpPop->individual(i).evaluator(f);
}

void
GASteadyStateGA::objectiveData(const GAEvalData& v){
  GAGeneticAlgorithm::objectiveData(v);
  for(int i=0; i<tmpPop->size(); i++)
    tmpPop->individual(i).evalData(v);
}

const GAPopulation&
GASteadyStateGA::population(const GAPopulation& p) {
  if(p.size() < 1) {
    GAErr(GA_LOC, className(), "population", gaErrNoIndividuals);
    return *pop;
  }

  GAGeneticAlgorithm::population(p);
  delete tmpPop;

  if(which == USE_PREPL){
    float n = pRepl * pop->size();
    if(n < 1) n = 1.0;
    nRepl = (unsigned int)n;
    params.set(gaNnReplacement, nRepl);
  }
  else{
    if(nRepl > (unsigned int)(pop->size())) nRepl = pop->size();
    if(nRepl < 1) nRepl = 1;
  }
  tmpPop = new GAPopulation(pop->individual(0), nRepl);
  tmpPop->geneticAlgorithm(*this);

  return *pop;
}

int
GASteadyStateGA::populationSize(unsigned int value){
  GAGeneticAlgorithm::populationSize(value);
  if(which == USE_PREPL){
    float n = ((pRepl*(float)pop->size() < 1) ? 1 : pRepl*(float)pop->size());
    nRepl = (unsigned int)n;
    params.set(gaNnReplacement, (unsigned int)nRepl);
    tmpPop->size(nRepl);
  }
  else {			// if we're using nrepl, be sure in valid range
    if(nRepl > value) {		// clip to new population size
      nRepl = value;
      tmpPop->size(nRepl);
    }
  }
  return value;
}


// If we get a value of 0 for either of these, this means to use the other
// measure instead.
float
GASteadyStateGA::pReplacement(float value){
  if(value == pRepl) return pRepl;
  if(value <= 0 || value > 1){
    GAErr(GA_LOC, className(), "pReplacement", gaErrBadPRepl);
    params.set(gaNpReplacement, pRepl);	// force it back
    return pRepl;
  }

  params.set(gaNpReplacement, (double)value);
  pRepl = value;

  float n = ((value*(float)pop->size() < 1) ? 1 : value*(float)pop->size());
  nRepl = (unsigned int)n;
  params.set(gaNnReplacement, (unsigned int)nRepl);
  
  which = USE_PREPL;

  tmpPop->size(nRepl);

  return pRepl;
}

int
GASteadyStateGA::nReplacement(unsigned int value){
  if(value == nRepl) return nRepl;
  if(value == 0 || value > (unsigned int)(pop->size())){
    GAErr(GA_LOC, className(), "nReplacement", gaErrBadNRepl);
    params.set(gaNnReplacement, nRepl);	// force it back
    return nRepl;
  }

  params.set(gaNnReplacement, (unsigned int)value);
  nRepl = value;
    
  pRepl = (float)nRepl / (float)pop->size();
  params.set(gaNpReplacement, (double)pRepl);
    
  which = USE_NREPL;

  tmpPop->size(nRepl);

  return nRepl;
}

int 
GASteadyStateGA::minimaxi(int m) { 
  GAGeneticAlgorithm::minimaxi(m);
  if(m == MINIMIZE)
    tmpPop->order(GAPopulation::LOW_IS_BEST);
  else
    tmpPop->order(GAPopulation::HIGH_IS_BEST);
  return minmax;
}







// For initialization we set the random seed, check for stupid errors, init the
// population, reset the statistics, and that's it.
//   If we don't get a seed then we set it ourselves.  If we do get one, then
// we use it as the random seed.
void
GASteadyStateGA::initialize(unsigned int seed)
{
  GARandomSeed(seed);

  pop->initialize();
  pop->evaluate(gaTrue);

  stats.reset(*pop);

  if(!scross) 
    GAErr(GA_LOC, className(), "initialize", gaErrNoSexualMating);
}


//   Evolve a new generation of genomes.  A steady-state GA has no 'old'
// and 'new' populations - we pick from the current population and replace its
// members with the new ones we create.  We replace the worst members of the
// preceeding population.  If a genome in the tmp population is worse than
// one in the main population, the genome in the main population will be
// replaced regardless of its better score.
void
GASteadyStateGA::step()
{
  int i, mut, c1, c2;
  GAGenome *mom, *dad;          // tmp holders for selected genomes

// Generate the individuals in the temporary population from individuals in 
// the main population.

  for(i=0; i<tmpPop->size()-1; i+=2){	// takes care of odd population
    mom = &(pop->select());  
    dad = &(pop->select());
    stats.numsel += 2;		// keep track of number of selections

    c1 = c2 = 0;
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad, &tmpPop->individual(i), 
				&tmpPop->individual(i+1));
      c1 = c2 = 1;
    }
    else{
      tmpPop->individual( i ).copy(*mom);
      tmpPop->individual(i+1).copy(*dad);
    }
    stats.nummut += (mut = tmpPop->individual( i ).mutate(pMutation()));
    if(mut > 0) c1 = 1;
    stats.nummut += (mut = tmpPop->individual(i+1).mutate(pMutation()));
    if(mut > 0) c2 = 1;

    stats.numeval += c1 + c2;
  }
  if(tmpPop->size() % 2 != 0){	// do the remaining population member
    mom = &(pop->select());  
    dad = &(pop->select());
    stats.numsel += 2;		// keep track of number of selections

    c1 = 0;
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad,
				&tmpPop->individual(i), (GAGenome*)0);
      c1 = 1;
    }
    else{
      if(GARandomBit())
	tmpPop->individual( i ).copy(*mom);
      else
	tmpPop->individual( i ).copy(*dad);
    }
    stats.nummut += (mut = tmpPop->individual( i ).mutate(pMutation()));
    if(mut > 0) c1 = 1;

    stats.numeval += c1;
  }

// Replace the worst genomes in the main population with all of the individuals
// we just created.  Notice that we invoke the population's add member with a
// genome pointer rather than reference.  This way we don't force a clone of
// the genome - we just let the population take over.  Then we take it back by
// doing a remove then a replace in the tmp population.

  for(i=0; i<tmpPop->size(); i++)
    pop->add(&tmpPop->individual(i));
  pop->evaluate();		// get info about current pop for next time
  pop->scale();			// remind the population to do its scaling

// the individuals in tmpPop are all owned by pop, but tmpPop does not know 
// that.  so we use replace to take the individuals from the pop and stick 
// them back into tmpPop
  for(i=0; i<tmpPop->size(); i++)
    tmpPop->replace(pop->remove(GAPopulation::WORST, GAPopulation::SCALED), i);

  stats.numrep += tmpPop->size();

  stats.update(*pop);		// update the statistics by one generation
}

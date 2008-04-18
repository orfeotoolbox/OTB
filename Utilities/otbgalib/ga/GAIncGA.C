// $Header$
/* ----------------------------------------------------------------------------
  gainc.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

  Source file for the incremental genetic algorithm object.
---------------------------------------------------------------------------- */
#include <ga/GAIncGA.h>
#include <ga/garandom.h>


GAParameterList&
GAIncrementalGA::registerDefaultParameters(GAParameterList& p) {
  GAGeneticAlgorithm::registerDefaultParameters(p);

  p.add(gaNnOffspring, gaSNnOffspring,
	GAParameter::INT, &gaDefNumOff);

  p.set(gaNscoreFrequency, &gaDefScoreFrequency2);

  return p;
}

GAIncrementalGA::GAIncrementalGA(const GAGenome& c) : GAGeneticAlgorithm(c) {
  child1 = pop->individual(0).clone(GAGenome::ATTRIBUTES);
  child2 = pop->individual(0).clone(GAGenome::ATTRIBUTES);
  child1->geneticAlgorithm(*this);
  child2->geneticAlgorithm(*this);

  rs = WORST;
  rf = 0;

  noffspr = gaDefNumOff;
  params.add(gaNnOffspring, gaSNnOffspring, GAParameter::INT, &noffspr);

  stats.scoreFrequency(gaDefScoreFrequency2);
  params.set(gaNscoreFrequency, &gaDefScoreFrequency2);
}
GAIncrementalGA::GAIncrementalGA(const GAPopulation& p): GAGeneticAlgorithm(p){
  child1 = pop->individual(0).clone(GAGenome::ATTRIBUTES);
  child2 = pop->individual(0).clone(GAGenome::ATTRIBUTES);
  child1->geneticAlgorithm(*this);
  child2->geneticAlgorithm(*this);

  rs = WORST;
  rf = 0;

  noffspr = gaDefNumOff;
  params.add(gaNnOffspring, gaSNnOffspring, GAParameter::INT, &noffspr);

  stats.scoreFrequency(gaDefScoreFrequency2);
  params.set(gaNscoreFrequency, &gaDefScoreFrequency2);
}
GAIncrementalGA::GAIncrementalGA(const GAIncrementalGA& ga) :
GAGeneticAlgorithm(ga){
  child1 = (GAGenome *)0;
  child2 = (GAGenome *)0;
  copy(ga);
}
GAIncrementalGA::~GAIncrementalGA(){
  delete child1; 
  delete child2;
}
GAIncrementalGA&
GAIncrementalGA::operator=(const GAIncrementalGA& ga){
  if(&ga != this) copy(ga); 
  return *this;
}
void 
GAIncrementalGA::copy(const GAGeneticAlgorithm & g){
  GAGeneticAlgorithm::copy(g);
  const GAIncrementalGA& ga = DYN_CAST(const GAIncrementalGA&, g);
  
  rs = ga.rs;
  rf = ga.rf;
  noffspr = ga.noffspr;

  if(child1) child1->copy(*ga.child1);
  else child1 = ga.child1->clone();
  if(child2) child2->copy(*ga.child2);
  else child2 = ga.child2->clone();
  child1->geneticAlgorithm(*this);
  child2->geneticAlgorithm(*this);
}


int
GAIncrementalGA::setptr(const char* name, const void* value){
  int status = GAGeneticAlgorithm::setptr(name, value);

  if(strcmp(name, gaNnOffspring) == 0 || 
     strcmp(name, gaSNnOffspring) == 0){
    nOffspring(*((int*)value));
    status = 0;
  }
  return status;
}

int
GAIncrementalGA::get(const char* name, void* value) const {
  int status = GAGeneticAlgorithm::get(name, value);

  if(strcmp(name, gaNnOffspring) == 0 || 
     strcmp(name, gaSNnOffspring) == 0){
    *((int*)value) = noffspr;
    status = 0;
  }
  return status;
}


void 
GAIncrementalGA::objectiveFunction(GAGenome::Evaluator f){
  GAGeneticAlgorithm::objectiveFunction(f);
  child1->evaluator(f);
  child2->evaluator(f);
}

void 
GAIncrementalGA::objectiveData(const GAEvalData& v){
  GAGeneticAlgorithm::objectiveData(v);
  child1->evalData(v);
  child2->evalData(v);
}


GAIncrementalGA::ReplacementScheme
GAIncrementalGA::
replacement(GAIncrementalGA::ReplacementScheme n, 
	    GAIncrementalGA::ReplacementFunction f){
  switch(n){
  case BEST:
  case WORST:
  case RANDOM:
  case PARENT:
    rs = n;
    break;
  case CUSTOM:
    if(f){ rs = n; rf = f; }
    else GAErr(GA_LOC, className(), "replacement", gaErrNeedRS);
    break;
  default:
    GAErr(GA_LOC, className(), "replacement", gaErrBadRS);
    break;
  }
  return rs;
}

int
GAIncrementalGA::nOffspring(unsigned int value){
  if(value != 1 && value != 2){
    GAErr(GA_LOC, className(), "numCrossStrategy", gaErrBadCS);
    noffspr = 1;
  }
  else{
    noffspr = value;
  }
  params.set(gaNnOffspring, value);
  return noffspr;
}



// Do some basic stupidity checks then initialize the population.  We must 
// also initialize our temporary genomes, but we don't have to evaluate
// them.  Finally, reset the statistics.
void
GAIncrementalGA::initialize(unsigned int seed)
{
  GARandomSeed(seed);

  pop->initialize();
  pop->evaluate(gaTrue);

  stats.reset(*pop);

  if(!scross) GAErr(GA_LOC, className(), "initialize", gaErrNoSexualMating);
}


//   Evolve a new generation of genomes.  A steady-state GA has no 'old'
// and 'new' populations - we pick from the current population and replace its
// members with the new ones we create.  We generate either one or two children
// each 'generation'.  The replacement strategy is set by the GA.
void
GAIncrementalGA::step()
{
  int mut, c1, c2;
  GAGenome *mom, *dad;          // tmp holders for selected genomes

  mom = &(pop->select()); 
  dad = &(pop->select()); 
  stats.numsel += 2;		// keep track of the number of selections

  if(noffspr == 1){
    c1 = 0;
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad, child1, (GAGenome*)0);
      c1 = 1;
    }
    else{
      if(GARandomBit())
	child1->copy(*mom);
      else
	child1->copy(*dad);
    }
    stats.nummut += (mut = child1->mutate(pMutation()));
    if(mut > 0) c1 = 1;

    stats.numeval += c1;

    if(rs == PARENT)
      child1 = pop->replace(child1, mom);
    else if(rs == CUSTOM)
      child1 = pop->replace(child1, &(rf(*child1, *pop)));
    else
      child1 = pop->replace(child1, rs);

    stats.numrep += 1;
  }
  else{
    c1 = c2 = 0;
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad, child1, child2);
      c1 = c2 = 1;
    }
    else{
      child1->copy(*mom);
      child2->copy(*dad);
    }
    stats.nummut += (mut = child1->mutate(pMutation()));
    if(mut > 0) c1 = 1;
    stats.nummut += (mut = child2->mutate(pMutation()));
    if(mut > 0) c2 = 1;

    stats.numeval += c1 + c2;

    if(rs == PARENT){
      child1 = pop->replace(child1, mom);
      if(mom == dad)		// this is a possibility, if so do worst
	child2 = pop->replace(child2, GAPopulation::WORST);
      else
	child2 = pop->replace(child2, dad);
    }
    else if(rs == CUSTOM){
      child1 = pop->replace(child1, &(rf(*child1, *pop)));
      child2 = pop->replace(child2, &(rf(*child2, *pop)));
    }
    else{
      child1 = pop->replace(child1, rs);
      child2 = pop->replace(child2, rs);
    }

    stats.numrep += 2;
  }

  pop->evaluate(gaTrue);        // allow pop-based evaluators to do their thing
  stats.update(*pop);		// update the statistics for this generation
}

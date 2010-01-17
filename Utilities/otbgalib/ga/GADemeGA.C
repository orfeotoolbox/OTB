/* ----------------------------------------------------------------------------
  gademe.C
  mbwall 28jul94
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved

   Souce file for the deme-based genetic algorithm object.
---------------------------------------------------------------------------- */
#include <ga/garandom.h>
#include <ga/GADemeGA.h>


GAParameterList&
GADemeGA::registerDefaultParameters(GAParameterList& p) {
  GAGeneticAlgorithm::registerDefaultParameters(p);

  p.add(gaNnPopulations, gaSNnPopulations, GAParameter::INT, &gaDefNPop);
  p.add(gaNnMigration, gaSNnMigration, GAParameter::INT, &gaDefNMig);

  return p;
}

GADemeGA::GADemeGA(const GAGenome& c) : GAGeneticAlgorithm(c) {
  npop = gaDefNPop;
  params.add(gaNnPopulations, gaSNnPopulations, GAParameter::INT, &npop);
  nmig = gaDefNMig;
  params.add(gaNnMigration, gaSNnMigration, GAParameter::INT, &nmig);

  unsigned int nr = pop->size()/2;
  nrepl = new int [npop];
  deme = new GAPopulation* [npop];
  pstats = new GAStatistics [npop];
  tmppop = new GAPopulation(c, nr);

  for(unsigned int i=0; i<npop; i++) {
    nrepl[i] = nr;
    deme[i] = new GAPopulation(*pop);
  }
}
GADemeGA::GADemeGA(const GAPopulation& p) : GAGeneticAlgorithm(p) {
  if(p.size() < 1) {
    GAErr(GA_LOC, className(), "GADemeGA(GAPopulation&)", gaErrNoIndividuals);
    pop = 0; nrepl = 0; tmppop = 0; pstats = 0;
  }
  else {
    npop = gaDefNPop;
    params.add(gaNnPopulations, gaSNnPopulations, GAParameter::INT, &npop);
    nmig = gaDefNMig;
    params.add(gaNnMigration, gaSNnMigration, GAParameter::INT, &nmig);
    unsigned int nr = pop->size()/2;

    nrepl = new int [npop];
    deme = new GAPopulation* [npop];
    pstats = new GAStatistics [npop];
    tmppop = new GAPopulation(p.individual(0), nr);
    
    for(unsigned int i=0; i<npop; i++) {
      nrepl[i] = nr;
      deme[i] = new GAPopulation(p);
    }
  }
}
GADemeGA::GADemeGA(const GADemeGA& orig) : GAGeneticAlgorithm(orig) {
  deme = 0; nrepl = 0; tmppop = 0; pstats = 0;
  copy(orig);
}
GADemeGA::~GADemeGA(){
  for(unsigned int i=0; i<npop; i++)
    delete deme[i];
  delete [] deme;
  delete [] nrepl;
  delete [] pstats;
  delete tmppop;
}
GADemeGA&
GADemeGA::operator=(const GADemeGA& orig){
  if(&orig != this) copy(orig); 
  return *this;
}
void
GADemeGA::copy(const GAGeneticAlgorithm& g){
  GAGeneticAlgorithm::copy(g);
  const GADemeGA& ga = DYN_CAST(const GADemeGA&,g);

  unsigned int i;
  for(i=0; i<npop; i++)
    delete deme[i];
  delete [] deme;
  delete [] nrepl;
  delete [] pstats;

  nmig = ga.nmig;
  npop = ga.npop;
  nrepl = new int [npop];
  deme = new GAPopulation* [npop];

  memcpy(nrepl, ga.nrepl, npop * sizeof(int));
  for(i=0; i<npop; i++) 
    deme[i]->copy(*(ga.deme[i]));

  tmppop->copy(*(ga.tmppop));
  
  pstats = new GAStatistics[npop];
  for(i=0; i<npop; i++)
    pstats[i] = ga.pstats[i];
}


// We make sure that the replacement pop is always at least one individual.  If
// the percentage replacement is specified then we use that to determine the
// size of the tmp pop.  Otherwise we use the absolute number of individuals.
// If we're using absolute number then we don't have to resize the tmp pop.
int
GADemeGA::setptr(const char* name, const void* value){
  int status = GAGeneticAlgorithm::setptr(name, value);

  if(strcmp(name, gaNnPopulations) == 0 ||
	  strcmp(name, gaSNnPopulations) == 0){
    nPopulations(*((int*)value));
    status = 0;
  }
  else if(strcmp(name, gaNnMigration) == 0 ||
	  strcmp(name, gaSNnMigration) == 0){
    nMigration(*((int*)value));
    status = 0;
  }

  return status;
}


int
GADemeGA::get(const char* name, void* value) const {
  int status = GAGeneticAlgorithm::get(name, value);

  if(strcmp(name, gaNnPopulations) == 0 || 
	  strcmp(name, gaSNnPopulations) == 0){
    *((int*)value) = npop;
    status = 0;
  }
  else if(strcmp(name, gaNnMigration) == 0 || 
	  strcmp(name, gaSNnMigration) == 0){
    *((int*)value) = nmig;
    status = 0;
  }

  return status;
}


void 
GADemeGA::objectiveFunction(int i, GAGenome::Evaluator f){
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      for(int jj=0; jj<deme[ii]->size(); jj++)
	deme[ii]->individual(jj).evaluator(f);
  else
    for(int jj=0; jj<deme[i]->size(); jj++)
      deme[i]->individual(jj).evaluator(f);
}

void
GADemeGA::objectiveData(int i, const GAEvalData& v){
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      for(int jj=0; jj<deme[ii]->size(); jj++)
	deme[ii]->individual(jj).evalData(v);
  else
    for(int jj=0; jj<deme[i]->size(); jj++)
      deme[i]->individual(jj).evalData(v);
}

const GAPopulation&
GADemeGA::population(int i, const GAPopulation& p) {
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      *deme[ii] = p;
  else
    *deme[i] = p;
  return *deme[((i==ALL) ? 0 : i)];
}


int
GADemeGA::populationSize(int i, unsigned int value){
  if(value < 1){
    GAErr(GA_LOC, className(), "populationSize", gaErrBadPopSize);
    value = 1;
  }
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      deme[ii]->size(value);
  else
    deme[i]->size(value);
  return value;
}

int
GADemeGA::nReplacement(int i, unsigned int value){
  if(i == ALL) {
    for(unsigned int ii=0; ii<npop; ii++){
      if(value > (unsigned int)deme[ii]->size()) 
	GAErr(GA_LOC, className(), "nReplacement", gaErrBadPRepl);
      else {
	params.set(gaNnReplacement, (unsigned int)value);
	nrepl[ii] = value;
      }
    }
  }
  else {
    if(value > (unsigned int)deme[i]->size())
      GAErr(GA_LOC, className(), "nReplacement", gaErrBadNRepl);
    else
      nrepl[i] = value;
  }
  if((unsigned int)(tmppop->size()) < value) {
    tmppop->size(value);
  }
  return value;
}

GAScalingScheme& 
GADemeGA::scaling(int i, const GAScalingScheme & s){
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      deme[ii]->scaling(s);
  else
    deme[i]->scaling(s);
  return deme[((i==ALL) ? 0 : i)]->scaling();
}

GASelectionScheme&
GADemeGA::selector(int i, const GASelectionScheme& s){
  if(i == ALL)
    for(unsigned int ii=0; ii<npop; ii++)
      deme[ii]->selector(s);
  else
    deme[i]->selector(s);
  return deme[((i==ALL) ? 0 : i)]->selector();
}

int 
GADemeGA::nMigration(unsigned int n) {
  params.set(gaNnMigration, (unsigned int)n);
  return nmig = n;
}

// change the number of populations.  try affect the evolution as little as
// possible in the process, so set things to sane values where we can.
int
GADemeGA::nPopulations(unsigned int n) {
  if(n < 1 || n == npop) return npop;
  if(n < npop) {
    for(unsigned int i=n; i<npop; i++)
      delete deme[i];
    GAPopulation** ptmp = deme;
    deme = new GAPopulation* [n];
    memcpy(deme, ptmp, n * sizeof(GAPopulation*));
    delete [] ptmp;

    GAStatistics* stmp = pstats;
    pstats = new GAStatistics[n];
    for(unsigned int j=0; j<n; j++)
      pstats[j] = stmp[j];
    delete [] stmp;

    int* rtmp = nrepl;
    nrepl = new int[n];
    memcpy(nrepl, rtmp, n * sizeof(int));
    delete [] rtmp;

    npop = n;
  }
  else {
    GAPopulation** ptmp = deme;
    deme = new GAPopulation* [n];
    memcpy(deme, ptmp, npop * sizeof(GAPopulation*));
    delete [] ptmp;
    for(unsigned int i=npop; i<n; i++)
      deme[i] = new GAPopulation(*deme[GARandomInt(0,npop-1)]);

    GAStatistics* stmp = pstats;
    pstats = new GAStatistics[n];
    for(unsigned int j=0; j<npop; j++)
      pstats[j] = stmp[j];
    delete [] stmp;

    int* rtmp = nrepl;
    nrepl = new int[n];
    memcpy(nrepl, rtmp, npop * sizeof(int));
    for(unsigned int k=npop; k<n; k++)
      nrepl[k] = nrepl[0];    

    npop = n;
  }
  params.set(gaNnPopulations, (unsigned int)n);
  pop->size(npop);
  return npop;
}

int
GADemeGA::minimaxi(int m) { 
  if(m == MINIMIZE){
    tmppop->order(GAPopulation::LOW_IS_BEST);
    for(unsigned int i=0; i<npop; i++)
      deme[i]->order(GAPopulation::LOW_IS_BEST);
  }
  else{
    tmppop->order(GAPopulation::HIGH_IS_BEST);
    for(unsigned int i=0; i<npop; i++)
      deme[i]->order(GAPopulation::HIGH_IS_BEST);
  }
  return GAGeneticAlgorithm::minimaxi(m);
}










// We only use the sexual mating, so only check for that one.  Initialize each
// of the popultions and set stats appropriately.
void
GADemeGA::initialize(unsigned int seed) {
  GARandomSeed(seed);

  for(unsigned int i=0; i<npop; i++) {
    deme[i]->initialize();
    deme[i]->evaluate(gaTrue);
    pstats[i].reset(*deme[i]);
    pop->individual(i).copy(deme[i]->best());
  }
  pop->touch();
  stats.reset(*pop);

  if(!scross) GAErr(GA_LOC, className(), "initialize", gaErrNoSexualMating);
}


// To evolve the genetic algorithm, we loop through all of our populations and
// evolve each one of them.  Then allow the migrator to do its thing.  Assumes
// that the tmp pop is at least as big as the largest nrepl that we'll use.  
// The master population maintains the best n individuals from each of the
// populations, and it is based on those that we keep the statistics for the
// entire genetic algorithm run.
void
GADemeGA::step() {
  int i, mut, c1, c2;
  GAGenome *mom, *dad;
  float pc;

  if(!scross) pc = 0.0;
  else        pc = pCrossover();

  for(unsigned int ii=0; ii<npop; ii++) {
    for(i=0; i<nrepl[ii]-1; i+=2){	// takes care of odd population
      mom = &(deme[ii]->select()); 
      dad = &(deme[ii]->select());
      pstats[ii].numsel += 2;
      c1 = c2 = 0;
      if(GAFlipCoin(pc)){
	pstats[ii].numcro += (*scross)(*mom, *dad, &tmppop->individual(i), 
				       &tmppop->individual(i+1));
	c1 = c2 = 1;
      }
      else{
	tmppop->individual( i ).copy(*mom);
	tmppop->individual(i+1).copy(*dad);
      }
      pstats[ii].nummut += (mut=tmppop->individual( i ).mutate(pMutation()));
      if(mut > 0) c1 = 1;
      pstats[ii].nummut += (mut=tmppop->individual(i+1).mutate(pMutation()));
      if(mut > 0) c2 = 1;
      pstats[ii].numeval += c1 + c2;
    }
    if(nrepl[ii] % 2 != 0){	// do the remaining population member
      mom = &(deme[ii]->select()); 
      dad = &(deme[ii]->select()); 
      pstats[ii].numsel += 2;
      c1 = 0;
      if(GAFlipCoin(pc)){
	pstats[ii].numcro += 
	  (*scross)(*mom, *dad, &tmppop->individual(i), (GAGenome*)0);
	c1 = 1;
      }
      else{
	if(GARandomBit()) tmppop->individual(i).copy(*mom);
	else              tmppop->individual(i).copy(*dad);
      }
      pstats[ii].nummut += (mut=tmppop->individual(i).mutate(pMutation()));
      if(mut > 0) c1 = 1;
      pstats[ii].numeval += c1;
    }

    for(i=0; i<nrepl[ii]; i++)
      deme[ii]->add(&tmppop->individual(i));
    deme[ii]->evaluate();
    deme[ii]->scale();
    for(i=0; i<nrepl[ii]; i++)
      tmppop->replace(deme[ii]->remove(GAPopulation::WORST,
				       GAPopulation::SCALED), i);
    
    pstats[ii].numrep += nrepl[ii];
  }

  migrate();

  for(unsigned int jj=0; jj<npop; jj++) {
    deme[jj]->evaluate();
    pstats[jj].update(*deme[jj]);
  }

  stats.numsel = stats.numcro = stats.nummut = stats.numrep = stats.numeval=0;
  for(unsigned int kk=0; kk<npop; kk++) {
    pop->individual(kk).copy(deme[kk]->best());
    stats.numsel += pstats[kk].numsel;
    stats.numcro += pstats[kk].numcro;
    stats.nummut += pstats[kk].nummut;
    stats.numrep += pstats[kk].numrep;
    stats.numeval += pstats[kk].numeval;
  }

  pop->touch();
  stats.update(*pop);
  for(unsigned int ll=0; ll<npop; ll++)
    stats.numpeval += pstats[ll].numpeval;
}

// This implementation uses an island model for parallel populations in which
// each population migrates a certain number of individuals to its nearest
// neighbor (I've heard of this referred to as the 'stepping-stone' model).
//   In this implementation we migrate the best individuals only.  This assumes
// that all populations have at least nmig individuals (we don't do any checks
// for conflicts).
void
GADemeGA::migrate() {
  GAGenome **ind;
  ind = new GAGenome* [nmig];
  unsigned int j;

  for(j=0; j<nmig; j++) 
    ind[j] = &(deme[0]->individual(j));

  for(unsigned int i=1; i<npop; i++) {
    for(j=0; j<nmig; j++) 
      ind[j] = deme[i]->replace(ind[j], j);
  }

  for(j=0; j<nmig; j++) 
    deme[0]->replace(ind[j], j);

  delete [] ind;
}



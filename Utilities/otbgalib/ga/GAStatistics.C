// $Header$
/* ----------------------------------------------------------------------------
  statistics.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology 
                     all rights reserved

 DESCRIPTION:
  Definition of the statistics object.
---------------------------------------------------------------------------- */
#include <string.h>
#include <ga/gaerror.h>
#include <ga/GAStatistics.h>



// Default settings and their names.
int  gaDefNumBestGenomes   = 1;
int  gaDefScoreFrequency1  = 1;
int  gaDefScoreFrequency2  = 100;
int  gaDefFlushFrequency   = 0;
char gaDefScoreFilename[]  = "generations.dat";


GAStatistics::GAStatistics() {
  curgen = 0;
  numsel = numcro = nummut = numrep = numeval = numpeval = 0;
  maxever = minever = 0.0;
  on = offmax = offmin = 0.0;
  aveInit = maxInit = minInit = devInit = 0.0;
  divInit = -1.0;
  aveCur = maxCur = minCur = devCur = 0.0;
  divCur = -1.0;

  scoreFreq = gaDefScoreFrequency1;
  dodiv = gaFalse;		// default is do not calculate diversity

  nconv=0; Nconv = 10;
  cscore = new float[Nconv]; memset(cscore, 0, Nconv*sizeof(float));

  nscrs=0; Nscrs = gaDefFlushFrequency;
  gen = new int[Nscrs]; memset(gen, 0, Nscrs*sizeof(int));
  aveScore = new float[Nscrs]; memset(aveScore, 0, Nscrs*sizeof(float));
  maxScore = new float[Nscrs]; memset(maxScore, 0, Nscrs*sizeof(float));
  minScore = new float[Nscrs]; memset(minScore, 0, Nscrs*sizeof(float));
  devScore = new float[Nscrs]; memset(devScore, 0, Nscrs*sizeof(float));
  divScore = new float[Nscrs]; memset(divScore, 0, Nscrs*sizeof(float));
  scorefile = new char[strlen(gaDefScoreFilename)+1];
  strcpy(scorefile, gaDefScoreFilename);
  which = Maximum;

  boa = (GAPopulation *)0;
}
GAStatistics::GAStatistics(const GAStatistics & orig){
  cscore=(float *)0;
  gen=(int *)0;
  aveScore=(float *)0;
  maxScore=(float *)0;
  minScore=(float *)0;
  devScore=(float *)0;
  divScore=(float *)0;
  scorefile=(char *)0;
  boa=(GAPopulation *)0;
  copy(orig);
}
GAStatistics::~GAStatistics(){
  delete [] cscore;
  delete [] gen;
  delete [] aveScore;
  delete [] maxScore;
  delete [] minScore;
  delete [] devScore;
  delete [] divScore;
  delete [] scorefile;
  delete boa;
}
void 
GAStatistics::copy(const GAStatistics & orig){
  if(&orig == this) return;

  curgen = orig.curgen;
  numsel = orig.numsel;
  numcro = orig.numcro;
  nummut = orig.nummut;
  numrep = orig.numrep;
  numeval = orig.numeval;
  numpeval = orig.numpeval;
  maxever = orig.maxever;
  minever = orig.minever;
  on = orig.on;
  offmax = orig.offmax;
  offmin = orig.offmin;
  aveInit = orig.aveInit;
  maxInit = orig.maxInit;
  minInit = orig.minInit;
  devInit = orig.devInit;
  divInit = orig.divInit;
  aveCur = orig.aveCur;
  maxCur = orig.maxCur;
  minCur = orig.minCur;
  devCur = orig.devCur;
  divCur = orig.divCur;

  scoreFreq = orig.scoreFreq;
  dodiv = orig.dodiv;

  nconv=orig.nconv; Nconv=orig.Nconv;
  delete [] cscore;
  cscore = new float [Nconv]; memcpy(cscore, orig.cscore, Nconv*sizeof(float));

  nscrs=orig.nscrs; Nscrs=orig.Nscrs;
  delete [] gen;
  gen = new int [Nscrs];
  memcpy(gen, orig.gen, Nscrs*sizeof(int));
  delete [] aveScore;
  aveScore = new float [Nscrs];
  memcpy(aveScore, orig.aveScore, Nscrs*sizeof(float));
  delete [] maxScore;
  maxScore = new float [Nscrs];
  memcpy(maxScore, orig.maxScore, Nscrs*sizeof(float));
  delete [] minScore;
  minScore = new float [Nscrs];
  memcpy(minScore, orig.minScore, Nscrs*sizeof(float));
  delete [] devScore;
  devScore = new float [Nscrs];
  memcpy(devScore, orig.devScore, Nscrs*sizeof(float));
  delete [] divScore;
  divScore = new float [Nscrs];
  memcpy(divScore, orig.divScore, Nscrs*sizeof(float));

  delete [] scorefile;
  if(orig.scorefile){
    scorefile = new char [strlen(orig.scorefile)+1];
    strcpy(scorefile, orig.scorefile);
  }
  else scorefile = (char*)0;

  which = orig.which;

  delete boa;
  if(orig.boa) boa = orig.boa->clone();
}


// Update the genomes in the 'best of all' population to reflect any 
// changes made to the current population.  We just grab the genomes with
// the highest scores from the current population, and if they are higher than
// those of the genomes in the boa population, they get copied.  Note that
// the bigger the boa array, the bigger your running performance hit because
// we have to look through all of the boa to figure out which are better than
// those in the population.  The fastest way to use the boa is to keep only 
// one genome in the boa population.  A flag of 'True' will reset the boa
// population so that it is filled with the best of the current population.
//   Unfortunately it could take a long time to update the boa array using the
// copy method.  We'd like to simply keep pointers to the best genomes, but
// the genomes change from generation to generation, so we can't depend on
// that.
//   Notice that keeping boa is useful even for overlapping populations.  The
// boa keeps individuals that are different from each other - the overlapping
// population may not.  However, keeping boa is most useful for populations
// with little overlap.
//   When we check to see if a potentially better member is already in our
// best-of-all population, we use the operator== comparator not the genome
// comparator to do the comparison.
void
GAStatistics::
updateBestIndividual(const GAPopulation & pop, GABoolean flag){
  if(boa == (GAPopulation *)0 || boa->size() == 0) return; // do nothing
  if(pop.order() != boa->order()) boa->order(pop.order());

  if(flag == gaTrue){		// reset the BOA array
    int j=0;
    for(int i=0; i<boa->size(); i++){
      boa->best(i).copy(pop.best(j));
      if(j < pop.size()-1) j++;
    }
    return;
  }

  if(boa->size() == 1){		// there's only one boa so replace it with bop
    if(boa->order() == GAPopulation::HIGH_IS_BEST &&
       pop.best().score() > boa->best().score())
      boa->best().copy(pop.best());
    if(boa->order() == GAPopulation::LOW_IS_BEST &&
       pop.best().score() < boa->best().score())
      boa->best().copy(pop.best());
  }
  else{
    int i=0, j, k;
    if(boa->order() == GAPopulation::HIGH_IS_BEST) {
      while(i < pop.size() && pop.best(i).score() > boa->worst().score()){
	for(k=0;
	    pop.best(i).score() < boa->best(k).score() && k < boa->size();
	    k++);
	for(j=k; j<boa->size(); j++){
	  if(pop.best(i) == boa->best(j)) break;
	  if(pop.best(i).score() > boa->best(j).score()){
	    boa->worst().copy(pop.best(i));        // replace worst individual
	    boa->sort(gaTrue, GAPopulation::RAW);  // re-sort the population
	    break;
	  }
	}
	i++;
      }
    }
    if(boa->order() == GAPopulation::LOW_IS_BEST) {
      while(i < pop.size() && pop.best(i).score() < boa->worst().score()){
	for(k=0;
	    pop.best(i).score() > boa->best(k).score() && k < boa->size();
	    k++);
	for(j=k; j<boa->size(); j++){
	  if(pop.best(i) == boa->best(j)) break;
	  if(pop.best(i).score() < boa->best(j).score()){
	    boa->worst().copy(pop.best(i));        // replace worst individual
	    boa->sort(gaTrue, GAPopulation::RAW);  // re-sort the population
	    break;
	  }
	}
	i++;
      }
    }
  }
  return;
}


// Use this method to update the statistics to account for the current
// population.  This routine increments the generation counter and assumes that
// the population that gets passed is the current population.
//   If we are supposed to flush the scores, then we dump them to the specified
// file.  If no flushing frequency has been specified then we don't record.
void
GAStatistics::update(const GAPopulation & pop){
  ++curgen;			// must do this first so no divide-by-zero
  if(scoreFreq > 0 && (curgen % scoreFreq == 0)) setScore(pop);
  if(Nscrs > 0 && nscrs >= Nscrs) flushScores();
  maxever = (pop.max() > maxever) ? pop.max() : maxever;
  minever = (pop.min() < minever) ? pop.min() : minever;
  float tmpval;
  tmpval = (on*(curgen-1) + pop.ave()) / curgen;
  on = tmpval;
  tmpval = (offmax*(curgen-1) + pop.max()) / curgen;
  offmax = tmpval;
  tmpval = (offmin*(curgen-1) + pop.min()) / curgen;
  offmin = tmpval;
  setConvergence((pop.order() == GAPopulation::HIGH_IS_BEST) ?
		 pop.max() : pop.min());
  updateBestIndividual(pop);
  numpeval = pop.nevals();
}


// Reset the GA's statistics based on the population.  To do this right you
// should initialize the population before you pass it to this routine.  If you
// don't, the stats will be based on a non-initialized population.
void
GAStatistics::reset(const GAPopulation & pop){
  curgen = 0;
  numsel = numcro = nummut = numrep = numeval = numpeval = 0;

  memset(gen, 0, Nscrs*sizeof(int));
  memset(aveScore, 0, Nscrs*sizeof(float));
  memset(maxScore, 0, Nscrs*sizeof(float));
  memset(minScore, 0, Nscrs*sizeof(float));
  memset(devScore, 0, Nscrs*sizeof(float));
  memset(divScore, 0, Nscrs*sizeof(float));
  nscrs = 0;
  setScore(pop);
  if(Nscrs > 0) flushScores();

  memset(cscore, 0, Nconv*sizeof(float));
  nconv = 0;			// should set to -1 then call setConv
  cscore[0] = 
    ((pop.order() == GAPopulation::HIGH_IS_BEST) ? pop.max() : pop.min());
//  cscore[0] = pop.max();
//  setConvergence(maxScore[0]);

  updateBestIndividual(pop, gaTrue);
  aveCur = aveInit = pop.ave();
  maxCur = maxInit = maxever = pop.max();
  minCur = minInit = minever = pop.min();
  devCur = devInit = pop.dev();
  divCur = divInit = ((dodiv == gaTrue) ? pop.div() : (float)-1.0);

  on = pop.ave();
  offmax = pop.max();
  offmin = pop.min();
  numpeval = pop.nevals();
  for(int i=0; i<pop.size(); i++)
    numeval += pop.individual(i).nevals();
}

void
GAStatistics::flushScores(){
  if(nscrs == 0) return;
  writeScores();
  memset(gen, 0, Nscrs*sizeof(int));
  memset(aveScore, 0, Nscrs*sizeof(float));
  memset(maxScore, 0, Nscrs*sizeof(float));
  memset(minScore, 0, Nscrs*sizeof(float));
  memset(devScore, 0, Nscrs*sizeof(float));
  memset(divScore, 0, Nscrs*sizeof(float));  
  nscrs = 0;
}


// Set the score info to the appropriate values.  Update the score count.
void
GAStatistics::setScore(const GAPopulation& pop){ 
  aveCur = pop.ave();
  maxCur = pop.max();
  minCur = pop.min();
  devCur = pop.dev();
  divCur = ((dodiv == gaTrue) ? pop.div() : (float)-1.0);

  if(Nscrs == 0) return;
  gen[nscrs] = curgen;
  aveScore[nscrs] = aveCur;
  maxScore[nscrs] = maxCur;
  minScore[nscrs] = minCur;
  devScore[nscrs] = devCur;
  divScore[nscrs] = divCur;
  nscrs++;
}


// For recording the convergence we have to keep a running list of the past N
// best scores.  We just keep looping around and around the array of past 
// scores.  nconv keeps track of which one is the current one.  The current 
// item is thus nconv%Nconv.  The oldest is nconv%Nconv+1 or 0.
void
GAStatistics::setConvergence(float s){
  nconv++;
  cscore[nconv%Nconv] = s;
}


// When a new number of gens to conv is specified, keep all the data that we
// can in the transfer.  Make sure we do it in the right order!  Then just
// continue on as before.
//   If someone passes us a zero then we set to 1.
int
GAStatistics::nConvergence(unsigned int n){
  if(n == 0) n = 1;
  float *tmp = cscore;
  cscore = new float[n];
  if(Nconv < n){
    if(nconv < Nconv){
      memcpy(cscore, tmp, (nconv+1) * sizeof(float));
    }
    else{
      memcpy(&(cscore[Nconv-(nconv%Nconv)-1]), tmp,
	     ((nconv%Nconv)+1) * sizeof(float));
      memcpy(cscore, &(tmp[(nconv%Nconv)+1]),
	     (Nconv-(nconv%Nconv)-1) * sizeof(float));
    }
  }
  else{
    if(nconv < n){
      memcpy(cscore, tmp, (nconv+1) * sizeof(float));
    }
    else{
      if((nconv % Nconv) + 1 < n){
	memcpy(&(cscore[n-(nconv%Nconv)-1]), tmp,
	       ((nconv%Nconv)+1) * sizeof(float));
	memcpy(cscore, &(tmp[Nconv-(1+n-(nconv%Nconv))]), sizeof(float));
      }
      else{
	memcpy(cscore, &(tmp[1+(nconv%Nconv)-n]), n * sizeof(float));
      }
    }
  }

  Nconv = n;
  delete [] tmp;
  return Nconv;
}


int 
GAStatistics::nBestGenomes(const GAGenome& genome, unsigned int n){
  if(n == 0){
    delete boa;
    boa = (GAPopulation*)0;
  }
  else if(boa == (GAPopulation*)0){
    boa = new GAPopulation(genome, n);
  }
  else {
    boa->size(n);
  }
  return n;
}

const GAGenome & 
GAStatistics::bestIndividual(unsigned int n) const {
  if(boa == 0 || (int)n >= boa->size()){
    GAErr(GA_LOC, "GAStatistics", "bestIndividual", gaErrBadPopIndex);
    n = 0;
  }
  return boa->best(n);		// this will crash if no boa
}

// Adjust the scores buffers to match the specified amount.  If someone
// specifies zero then we don't keep the scores, so set all to NULL.
int 
GAStatistics::flushFrequency(unsigned int freq){
  if(freq == 0){
    if(nscrs > 0) flushScores();
    resizeScores(freq);
  }
  else if(freq > Nscrs){
    resizeScores(freq);
  }
  else if(freq < Nscrs){
    if(nscrs > freq) flushScores();
    resizeScores(freq);
  }
  Nscrs = freq;
  return freq;
}


// Resize the scores vectors to the specified amount.  Copy any scores that
// exist.
void
GAStatistics::resizeScores(unsigned int n){
  int *tmpi;
  float *tmpf;

  if(n == 0){
    delete [] gen;
    gen = (int*)0;
    delete [] aveScore;
    aveScore = (float*)0;
    delete [] maxScore;
    maxScore = (float*)0;
    delete [] minScore;
    minScore = (float*)0;
    delete [] devScore;
    devScore = (float*)0;
    delete [] divScore;
    divScore = (float*)0;

    nscrs = n;
  }
  else{
    tmpi = gen;
    gen = new int [n];
    memcpy(gen, tmpi, (n < Nscrs ? n : Nscrs)*sizeof(int));
    delete [] tmpi;

    tmpf = aveScore;
    aveScore = new float [n];
    memcpy(aveScore, tmpf, (n < Nscrs ? n : Nscrs)*sizeof(float));
    delete [] tmpf;

    tmpf = maxScore;
    maxScore = new float [n];
    memcpy(maxScore, tmpf, (n < Nscrs ? n : Nscrs)*sizeof(float));
    delete [] tmpf;

    tmpf = minScore;
    minScore = new float [n];
    memcpy(minScore, tmpf, (n < Nscrs ? n : Nscrs)*sizeof(float));
    delete [] tmpf;

    tmpf = devScore;
    devScore = new float [n];
    memcpy(devScore, tmpf, (n < Nscrs ? n : Nscrs)*sizeof(float));
    delete [] tmpf;

    tmpf = divScore;
    divScore = new float [n];
    memcpy(divScore, tmpf, (n < Nscrs ? n : Nscrs)*sizeof(float));
    delete [] tmpf;

    if(nscrs > n) nscrs = n;
  }
  Nscrs = n;
}


// Write the current scores to file.  If this is the first chunk (ie gen[0] 
// is 0) then we create a new file.  Otherwise we append to an existing file.
// We give no notice that we're overwriting the existing file!!
void
GAStatistics::writeScores(){
  if(!scorefile) return;
#ifdef GALIB_USE_STREAMS
  STD_OFSTREAM outfile(scorefile, ((gen[0] == 0) ?
				   (STD_IOS_OUT | STD_IOS_TRUNC) :
				   (STD_IOS_OUT | STD_IOS_APP)));
// should be done this way, but SGI systems (and others?) don't do it right...
//  if(! outfile.is_open()){
  if(outfile.fail()){
    GAErr(GA_LOC, "GAStatistics", "writeScores", gaErrWriteError, scorefile);
    return;
  }
  scores(outfile, which);
  outfile.close();
#endif
}


#ifdef GALIB_USE_STREAMS
int 
GAStatistics::write(const char* filename) const {
  STD_OFSTREAM outfile(filename, (STD_IOS_OUT | STD_IOS_TRUNC));
// should be done this way, but SGI systems (and others?) don't do it right...
//  if(! outfile.is_open()){
  if(outfile.fail()){
    GAErr(GA_LOC, "GAStatistics", "write", gaErrWriteError, filename);
    return 1;
  }
  write(outfile);
  outfile.close();
  return 0;
}

int
GAStatistics::write(STD_OSTREAM & os) const {
  os << curgen << "\t# current generation\n";
  os << convergence() << "\t# current convergence\n";
  os << numsel << "\t# number of selections since initialization\n";
  os << numcro << "\t# number of crossovers since initialization\n";
  os << nummut << "\t# number of mutations since initialization\n";
  os << numrep << "\t# number of replacements since initialization\n";
  os << numeval << "\t# number of genome evaluations since initialization\n";
  os << numpeval << "\t# number of population evaluations since initialization\n";
  os << maxever << "\t# maximum score since initialization\n";
  os << minever << "\t# minimum score since initialization\n";
  os << on << "\t# average of all scores ('on-line' performance)\n";
  os << offmax << "\t# average of maximum scores ('off-line' performance)\n";
  os << offmin << "\t# average of minimum scores ('off-line' performance)\n";
  os << "\n";
  os << aveInit << "\t# mean score in initial population\n";
  os << maxInit << "\t# maximum score in initial population\n";
  os << minInit << "\t# minimum score in initial population\n";
  os << devInit << "\t# standard deviation of initial population\n";
  os <<divInit<<"\t# diversity of initial population (0=identical,-1=unset)\n";
  os << "\n";
  os << aveCur << "\t# mean score in current population\n";
  os << maxCur << "\t# maximum score in current population\n";
  os << minCur << "\t# minimum score in current population\n";
  os << devCur << "\t# standard deviation of current population\n";
  os << divCur<<"\t# diversity of current population (0=identical,-1=unset)\n";
  os << "\n";
  os << Nconv << "\t# how far back to look for convergence\n";
  os << scoreFreq << "\t# how often to record scores\n";
  os << Nscrs << "\t# how often to write scores to file\n";
  os << scorefile << "\t# name of file to which scores are written\n";
  return 0;
}


// You can specify the data that you want to dump out when you call this
// routine, or you can just let it use the selection from the object.  If you
// specify a data set, that will be used rather than the 'which' in the object.
int 
GAStatistics::scores(const char* filename, int w){
  STD_OFSTREAM outfile(filename, (STD_IOS_OUT | STD_IOS_TRUNC));
// should be done this way, but SGI systems (and others?) don't do it right...
//  if(! outfile.is_open()){
  if(outfile.fail()){
    GAErr(GA_LOC, "GAStatistics", "scores", gaErrWriteError, filename);
    return 1;
  }
  scores(outfile, w);
  outfile.close();
  return 0;
}


int
GAStatistics::scores(STD_OSTREAM & os, int w){
  if(w == NoScores) w = which;

  for(unsigned int i=0; i<nscrs; i++){
    os << gen[i];
    if(w & Mean) os << "\t" << aveScore[i];
    if(w & Maximum)  os << "\t" << maxScore[i];
    if(w & Minimum)  os << "\t" << minScore[i];
    if(w & Deviation)  os << "\t" << devScore[i];
    if(w & Diversity)  os << "\t" << divScore[i];
    os << "\n";
  }
  return 0;
}
#endif

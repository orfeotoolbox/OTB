/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from libgalib. See GALIBCopyright.txt
    for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(galibTests24);
}

/* ----------------------------------------------------------------------------
  ex24.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to restricted mating using a custom genetic algorithm
and custom selection scheme.  The restricted mating in the genetic algorithm
tries to pick individuals that are similar (based upon their comparator).  The
selector chooses only the upper half of the population (so it is very selective
and tends to drive the convergence faster than roulette wheel selection, for
example).
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define endl STD_ENDL
#define ofstream STD_OFSTREAM

#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

#define MIN_VALUE -2
#define MAX_VALUE 2
#define INC       0.005

#define THRESHOLD 0.5

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

float Objective(GAGenome &);
float Comparator(const GAGenome&, const GAGenome&);





// Use any ID above 200 for the object's ID number.  The selection scheme
// takes care of keeping the population up-to-date - we only have to worry
// about the selection method itself.  This selector picks randomly, but only
// from the upper half of the population (based on the scaled fitness scores).
class FinickySelector : public GASelectionScheme {
public:
  GADefineIdentity("FinickySelector", 273);
  FinickySelector(int w=GASelectionScheme::SCALED) : GASelectionScheme(w) { }
  FinickySelector(const FinickySelector& orig) { copy(orig); }
  FinickySelector& operator=(const FinickySelector& orig)
    { if(&orig != this) copy(orig); return *this; }
  virtual ~FinickySelector() { }
  virtual GASelectionScheme* clone() const { return new FinickySelector; }
  virtual GAGenome& select() const;
};

GAGenome&
FinickySelector::select() const {
  return pop->best(GARandomInt(0, pop->size()/2), GAPopulation::SCALED);
}




// This is the genetic algorithm that does the restricted mating.  It is
// similar to the steady state genetic algorithm, but we modify the selection
// part of the step method to do the restricted mating.

class RestrictedMatingGA : public GASteadyStateGA {
public:
  GADefineIdentity("RestrictedMatingGA", 288);
  RestrictedMatingGA(const GAGenome& g) : GASteadyStateGA(g) {}
  virtual ~RestrictedMatingGA() {}
  virtual void step();
  RestrictedMatingGA & operator++() { step(); return *this; }
};

// This step method is similar to that of the regular steady-state genetic
// algorithm, but here we select only individuals that are similar to each
// other (based on what their comparators tell us).

void
RestrictedMatingGA::step()
{
  int i;
  GAGenome *mom, *dad;

// Generate the individuals in the temporary population from individuals in
// the main population.  We do a restrictive selection in which we only let
// individuals that are dissimilar mate.  We do a little count just to make
// sure that we don't loop forever (just accept whatever we selected if we
// can't find what we'd like).

  for(i=0; i<tmpPop->size()-1; i+=2){	// takes care of odd population
    mom = &(pop->select());
    int k=0;
    do { k++; dad = &(pop->select()); }
    while(mom->compare(*dad) < THRESHOLD && k<pop->size());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad, &tmpPop->individual(i),
				&tmpPop->individual(i+1));
    }
    else{
      tmpPop->individual( i ).copy(*mom);
      tmpPop->individual(i+1).copy(*dad);
    }
    stats.nummut += tmpPop->individual( i ).mutate(pMutation());
    stats.nummut += tmpPop->individual(i+1).mutate(pMutation());
  }
  if(tmpPop->size() % 2 != 0){	// do the remaining population member
    mom = &(pop->select());
    dad = &(pop->select());
    int k=0;
    do { k++; dad = &(pop->select()); }
    while(mom->compare(*dad) < THRESHOLD && k<pop->size());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover())){
      stats.numcro += (*scross)(*mom, *dad,
				&tmpPop->individual(i), (GAGenome*)0);
    }
    else{
      if(GARandomBit()) tmpPop->individual( i ).copy(*mom);
      else tmpPop->individual( i ).copy(*dad);
    }
    stats.nummut += tmpPop->individual( i ).mutate(pMutation());
  }

// Now stick the new individuals into the population, force an evaluation,
// force a scaling, then remove the worst individuals so that we keep a
// constant population size.

  for(i=0; i<tmpPop->size(); i++)
    pop->add(tmpPop->individual(i));

  pop->evaluate();		// get info about current pop for next time
  pop->scale();			// remind the population to do its scaling

  for(i=0; i<tmpPop->size(); i++)
    pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);

  stats.update(*pop);		// update the statistics by one generation
}








int
galibTests24(int argc, char * argv[])
{
  cout << "Example 24\n\n";
  cout << "This example illustrates how to derive your own genetic\n";
  cout << "algorithm.  This genetic algorithm does restricted mating and\n";
  cout << "uses a selector slightly more finicky than a uniform random\n";
  cout << "selector.  The objective function is a simple sinusoidal.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      seed = atoi(argv[ii+1]);
    }
  }

  const char *filename1 = argv[1];//"settings.txt";
  const char *filename2 = argv[2];//"population.dat";
  const char *filename3 = argv[3];//"sinusoid.dat";

  GARealAlleleSet alleles(MIN_VALUE, MAX_VALUE);
  GARealGenome genome(1, alleles, Objective);
  GASharing scale(Comparator);
  FinickySelector select;

  RestrictedMatingGA ga(genome);
  ga.selector(select);		// use our selector instead of default
  ga.scaling(scale);		// set the scaling method to our sharing
  ga.populationSize(50);	// how many individuals in the population
  ga.nGenerations(200);		// number of generations to evolve
  ga.pMutation(0.001);		// likelihood of mutating new offspring
  ga.pCrossover(0.9);		// likelihood of crossing over parents
  ga.scoreFilename("bog.dat");	// name of file for scores
  ga.scoreFrequency(1);		// keep the scores of every generation
  ga.flushFrequency(50);	// specify how often to write the score to disk
  ga.selectScores(GAStatistics::AllScores);
  ga.parameters(filename1/*"settings.txt"*/); // read parameters from settings file

  ga.parameters(argc, argv, gaTrue); // parse commands, complain if bogus args
  ga.evolve(seed);

  ofstream outfile;

// dump the final population to file

  cout << "printing population to file 'population.dat'..." << endl;
  outfile.open(filename2/*"population.dat"*/, (STD_IOS_OUT | STD_IOS_TRUNC));
  for(int i=0; i<ga.population().size(); i++){
    genome = ga.population().individual(i);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

// dump the function to file so you can plot the population on it

  cout << "printing function to file 'sinusoid.dat'..." << endl;
  outfile.open(filename3/*"sinusoid.dat"*/, (STD_IOS_OUT | STD_IOS_TRUNC));
  for(float x=MIN_VALUE; x<=MAX_VALUE; x+=INC){
    outfile << genome.gene(0,x) << "\t" << genome.score() << "\n";
  }
  outfile.close();

  return 0;
}




// This objective function returns the sin of the value in the genome.
float
Objective(GAGenome& g){
  GARealGenome& genome = (GARealGenome &)g;
  return 1 + sin(genome.gene(0)*2*M_PI);
}

// The comparator returns a number in the interval [0,1] where 0 means that
// the two genomes are identical (zero diversity) and 1 means they are
// completely different (maximum diversity).
float
Comparator(const GAGenome& g1, const GAGenome& g2) {
  GARealGenome& a = (GARealGenome &)g1;
  GARealGenome& b = (GARealGenome &)g2;
  return exp( -(a.gene(0) - b.gene(0)) * (a.gene(0) - b.gene(0)) / 100.0);
}

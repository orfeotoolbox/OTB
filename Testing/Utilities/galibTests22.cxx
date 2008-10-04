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
REGISTER_TEST(galibTests22);
}

/* ----------------------------------------------------------------------------
  ex22.C
  mbwall 5jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to derive your own genetic algorithm class.  This one
does a modified form of speciation that is useful for fitness-scaled speciation
with overlapping populations (Goldberg's speciation is designed for use with 
non-overlapping populations.  
   The steady-state genetic algorithm built-in to GAlib is actually capable of
doing this already, but this example illustrates how you can modify a genetic
algorithm to do your own thing.  For example, instead of using the "single
child crossover" you could use your own crossover algorithm instead.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define endl STD_ENDL
#define ofstream STD_OFSTREAM


// force instantiations for compilers that do not do auto instantiation
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GA1DArrayGenome.C>
GALIB_INSTANTIATION_PREFIX GA1DArrayGenome<float>;
#endif


#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define OBJECTIVE Objective1
#define MIN_VALUE -100
#define MAX_VALUE 100

//#define OBJECTIVE Objective2
//#define MIN_VALUE -50
//#define MAX_VALUE 50

float Objective1(GAGenome&);
float Objective2(GAGenome&);
int   Mutator(GAGenome&, float);
void  Initializer(GAGenome&);
float Comparator(const GAGenome&, const GAGenome&);
int   Crossover(const GAGenome&, const GAGenome&, GAGenome*);



// Here we define our own genetic algorithm class.  This class is almost the
// same as the steady-state genetic algorithm, but we modify the step method
// (the one that does all the work) so that we do a slightly modified 
// replacement.  We're only going to do a two-parents-make-one-child mating,
// so we define our own crossover and use it rather than the standard one in
// GAlib.

typedef int (*SingleChildCrossover)(const GAGenome&,const GAGenome&,GAGenome*);

class SharedOverlapGA : public GASteadyStateGA {
public:
  GADefineIdentity("SharedOverlapGA", 200);
  SharedOverlapGA(const GAGenome& g) : GASteadyStateGA(g) {}
  virtual ~SharedOverlapGA() {}
  virtual void step();
  SharedOverlapGA & operator++() { step(); return *this; }
  void crossover(SingleChildCrossover func) {crossFunction = func;}
protected:
  SingleChildCrossover crossFunction;
};

// This step method is similar to that of the regular steady-state genetic
// algorithm, but here we generate only one child in a crossover, and we
// do a slightly different type of replacement.  Here we generate the new
// individuals, insert them into the population, force a scaling to occur, 
// then remove the worst individuals.  This is all done based on the scaled
// (fitness) scores, not the raw (objective) scores.

void
SharedOverlapGA::step()
{ 
  int i;
  GAGenome *mom, *dad;

  for(i=0; i<tmpPop->size(); i++){	// takes care of odd population
    mom = &(pop->select());  
    dad = &(pop->select());
    stats.numsel += 2;		// keep track of number of selections
    if(GAFlipCoin(pCrossover()))
      stats.numcro += crossFunction(*mom, *dad, &tmpPop->individual(i));
    else if(GARandomBit()) 
      tmpPop->individual(i).copy(*mom);
    else
      tmpPop->individual(i).copy(*dad);
    stats.nummut += tmpPop->individual(i).mutate(pMutation());
  }

  for(i=0; i<tmpPop->size(); i++)
    pop->add(tmpPop->individual(i));

  pop->evaluate();		// get info about current pop for next time
  pop->scale();			// remind the population to do its scaling

  for(i=0; i<tmpPop->size(); i++)
    pop->destroy(GAPopulation::WORST, GAPopulation::SCALED);

  stats.update(*pop);		// update the statistics by one generation
}






int
galibTests22(int argc, char * argv[])
{
  cout << "Example 22\n\n";
  cout << "This example shows how to derive your own genetic algorithm\n";
  cout << "class.  Here we use a custom, single-child crossover and a\n";
  cout << "modified replacement strategy with overlapping populations.\n\n";
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

  ofstream outfile;
  const char *file = argv[1];//"sinusoid.dat";
  const char *ifile = argv[2];//"pop.initial.dat";
  const char *ffile = argv[3];//"pop.final.dat";
  int i;

  GA1DArrayGenome<float> genome(1, OBJECTIVE);
  genome.initializer(::Initializer);
  genome.mutator(::Mutator);
  genome.comparator(::Comparator);

  GASharing share(Comparator);

  SharedOverlapGA ga(genome);
  ga.crossover(Crossover);
  ga.scaling(share);
  ga.populationSize(100);
  ga.pReplacement(0.25);
  ga.nGenerations(500);
  ga.pMutation(0.01);
  ga.pCrossover(1.0);
  ga.scoreFilename("bog.dat");	// name of file for scores
  ga.scoreFrequency(10);	// keep the scores of every 10th generation
  ga.flushFrequency(100);	// specify how often to write the score to disk
  ga.selectScores(GAStatistics::AllScores);
  ga.parameters(argc, argv, gaTrue); // parse commands, complain if bogus args

  cout << "initializing...\n"; cout.flush();
  ga.initialize(seed);

// dump the initial population to file

  outfile.open(ifile, (STD_IOS_OUT | STD_IOS_TRUNC));
  for(i=0; i<ga.population().size(); i++){
    genome = ga.population().individual(i);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

// Evolve until the termination function says we're finished.  Print out a
// little status indicator periodically to let us know what's going on.  After
// the evolution we flush any remaining scores to file.

  cout << "evolving"; cout.flush();
  while(!ga.done()){
    ga.step();
    if(ga.generation() % 50 == 0){
      cout << ".";
      cout.flush();
    }
  }
  cout << "\n\n";
  ga.flushScores();

// dump the final population to file

  outfile.open(ffile, (STD_IOS_OUT | STD_IOS_TRUNC));
  for(i=0; i<ga.population().size(); i++){
    genome = ga.population().individual(i);
    outfile << genome.gene(0) << "\t" << genome.score() << "\n";
  }
  outfile.close();

// dump the function to file

  cout << "dumping the function to file..." << endl;
  outfile.open(file, (STD_IOS_OUT | STD_IOS_TRUNC));
  if(outfile.fail()){
    cerr << "Cannot open " << file << " for output.\n";
    exit(1);
  }
  for(float x=MIN_VALUE; x<=MAX_VALUE; x+=1.0)
    outfile << genome.gene(0,x) << "\t" << genome.score() << "\n";
  outfile << "\n";
  outfile.close();

  cout << "initial population is in '" << ifile << "'\n";
  cout << "final population is in '" << ffile << "'\n";
  cout << "the function is in '" << file << "'\n";
  cout << "parameters were:\n\n" << ga.parameters() << "\n";

  return 0;
}
 





// Here are two different objective functions.  Function 1 has multiple peaks
// with significant difference between peak heights - it is a modulated
// sinusoid.  Function 2 has less difference between peaks - it is an 
// approximation of a square plateau using a sum of sinusoids.
float
Objective1(GAGenome& g)
{
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  float v = genome.gene(0);
  float y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
  if(v < MIN_VALUE || v > MAX_VALUE) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}

float
Objective2(GAGenome& g)
{
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  float v = genome.gene(0) / 100.0;
  float y = 0.5 + 0.6 * sin(M_PI*v) + 0.2 * sin(3*M_PI*v) + 0.1 * sin(5*M_PI*v)
    + 0.02 * sin(7*M_PI*v) + 0.01 * sin(7*M_PI*v);
  if(v < -0.23 || v > 1.23) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}

void
Initializer(GAGenome& g)
{
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  genome.gene(0, GARandomFloat(-100.0, 100.0));
}

int
Mutator(GAGenome& g, float pmut)
{
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  int nmut = 0;
  if(GAFlipCoin(pmut)){
    genome.gene(0, genome.gene(0) + 
		GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    nmut = 1;
  }
  return nmut;
}

int
Crossover(const GAGenome& g1, const GAGenome& g2, GAGenome* c1)
{
  GA1DArrayGenome<float>& mom = (GA1DArrayGenome<float>&)g1;
  GA1DArrayGenome<float>& dad = (GA1DArrayGenome<float>&)g2;
  GA1DArrayGenome<float>& child = (GA1DArrayGenome<float>&)*c1;

  float distance = 0.0, midpoint = 0.0;

  midpoint = (mom.gene(0) + dad.gene(0)) / 2;
  distance = fabs(mom.gene(0) - dad.gene(0));
  child.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

  return 1;
}


// You can change the factor to control how tightly the distance function 
// considers the spacing of two genomes.  Higher numbers will give you a 
// tighter clustering at function peaks.

#define FACTOR 800

float
Comparator(const GAGenome& g1, const GAGenome& g2) 
{
  GA1DArrayGenome<float>& a = (GA1DArrayGenome<float>&)g1;
  GA1DArrayGenome<float>& b = (GA1DArrayGenome<float>&)g2;

  float val= exp( - (a.gene(0)-b.gene(0)) * (a.gene(0)-b.gene(0)) / FACTOR);
  if(1-val < 0 || 1-val > 1) cerr << "val: " << val << "\n";
  return 1-val;
}

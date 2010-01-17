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
REGISTER_TEST(galibTests27);
}

/* ----------------------------------------------------------------------------
  ex27.C
  mbwall 24mar96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

  The code for this example is adapted from an original implementation
  by Thomas Grueninger (from Uni Stuttgart, visiting scholar at MIT)

 DESCRIPTION:
   This example shows how to use the Deterministic Crowding genetic algorithm.
You can specify one of 4 different functions (they are described in more detail
later in this file).
   (This code was originally used with a 3D OpenGL-based program to illustrate
the differences in convergence between various speciation methods.  Believe me,
it looks much better running in real-time in 3D, but, alas, there is not yet
any standard 3D cross-platform API, so you get this instead.)
---------------------------------------------------------------------------- */
#include <math.h>
#include <ga/GASStateGA.h>
#include <ga/GAList.h>
#include <ga/GA1DArrayGenome.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define endl STD_ENDL

// force instantiations for compilers that do not do auto instantiation
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GAList.C>
#include <ga/GA1DArrayGenome.C>
GALIB_INSTANTIATION_PREFIX GAList<int>;
GALIB_INSTANTIATION_PREFIX GA1DArrayGenome<float>;
#endif





// This is the class definition for the deterministic crowding genetic
// algorithm.  It is based upon the steady-state genetic algorithm, but we
// modify the replacement so that it does deterministic crowding as described
// by Goldberg (not in his book) and his students.
class DCrowdingGA : public GASteadyStateGA {
public:
  GADefineIdentity("DeterministicCrowdingGA", 241);
  DCrowdingGA(const GAGenome& g) : GASteadyStateGA(g) {}
  virtual ~DCrowdingGA() {}
  virtual void step();
  DCrowdingGA & operator++() { step(); return *this; }
};

void
DCrowdingGA::step() {
  int i,*ip;
  float d1,d2;
  GAGenome *mom, *dad;
  GAList<int> IndPool;

  while (IndPool.head()) IndPool.destroy();

  for (i=0; i<pop->size(); i++)
    IndPool.insert(i);

  do {
    //select mom
    IndPool.warp(GARandomInt(0,IndPool.size()-1));
    ip=IndPool.remove();
    mom = &pop->individual(*ip);
    delete ip;
    //select dad
    IndPool.warp(GARandomInt(0,IndPool.size()-1));
    ip=IndPool.remove();
    dad = &pop->individual(*ip);
    delete ip;
    //create child
    stats.numsel += 2;
    stats.numcro += (*(mom->sexual()))(*mom, *dad, &tmpPop->individual(0), 0);
    stats.nummut += tmpPop->individual(0).mutate(pMutation());
    stats.numeval += 1;
    //replace closest parent
    d1 = tmpPop->individual(0).compare(*mom);
    d2 = tmpPop->individual(0).compare(*dad);
    if (d1 < d2) {
      if (minmax == MINIMIZE) {
	if (tmpPop->individual(0).score() < mom->score()) {
	  mom->copy(tmpPop->individual(0));
	  stats.numrep += 1;
	}
      }
      else {
	if (tmpPop->individual(0).score() > mom->score()) {
	  mom->copy(tmpPop->individual(0));
	  stats.numrep += 1;
	}
      }
    }
    else {
      if (minmax == MINIMIZE) {
	if (tmpPop->individual(0).score() < dad->score()) {
	  dad->copy(tmpPop->individual(0));
	  stats.numrep += 1;
	}
      }
      else {
	if (tmpPop->individual(0).score() > dad->score()) {
	  dad->copy(tmpPop->individual(0));
	  stats.numrep += 1;
	}
      }
    }
  } while (IndPool.size()>1);

  pop->evaluate(gaTrue);
  stats.update(*pop);
}






// Set up the various 2-dimensional, real number functions that we will use.
typedef float (*Function)(float, float);

float Function1(float, float);
float Function2(float, float);
float Function3(float, float);
float Function4(float, float);
float ai[25],bi[25];

static int which = 0;
static Function obj[] = { Function1, Function2, Function3, Function4 };
static float minx[] = {-6, -60, -500, -10 };
static float maxx[] = { 6,  60,  500, 10 };
static float miny[] = {-6, -60, -500, -10 };
static float maxy[] = { 6,  60,  500, 10 };



// These are the declarations for our genome operators (we do not use the
// defaults from GAlib for this example).
float Objective(GAGenome&);
int   Mutator(GAGenome&, float);
void  Initializer(GAGenome&);
int   Crossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
float Comparator(const GAGenome&, const GAGenome&);


int
galibTests27(int argc, char* argv[]) {
  cout << "Example 27\n\n";
  cout << "Deterministic crowding demonstration program.\n\n";
  cout << "In addition to the standard GAlib command-line arguments,\n";
  cout << "you can specify one of the four following functions:\n";
  cout << "   0 - modified Himmelblau's function\n";
  cout << "   1 - Foxholes (25)\n";
  cout << "   2 - Schwefel's nasty (1 glob. Max bei (420.96/420.96)\n";
  cout << "   3 - Mexican Hat (optimum at 0,0)\n";
  cout << endl;

  int i;

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  for(i=1; i<argc; i++) {
    if(strcmp(argv[i++],"seed") == 0)
      GARandomSeed((unsigned int)atoi(argv[i]));
  }

  for (i=0; i<25; i++) {
    ai[i] = 16 * ((i % 5) -2);
    bi[i] = 16 * ((i / 5) -2);
  }

  GA1DArrayGenome<float> genome(2, Objective);
  genome.initializer(::Initializer);
  genome.mutator(::Mutator);
  genome.comparator(::Comparator);
  genome.crossover(::Crossover);

  DCrowdingGA ga(genome);
  ga.maximize();
  ga.populationSize(100);
  ga.nGenerations(100);
  ga.pMutation(0.05);
  ga.pCrossover(1.0);
  ga.selectScores(GAStatistics::AllScores);
  ga.parameters(argc, argv, gaFalse);

  for (i=1; i<argc; i++){
    if(strcmp("func", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": the function option needs a number.\n";
        exit(1);
      }
      else{
        which = atoi(argv[i]);
        continue;
      }
    }
    else if(strcmp("seed", argv[i]) == 0){
      if(++i < argc) continue;
      continue;
    }
    else {
      cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
      cerr << "valid arguments include standard GAlib arguments plus:\n";
      cerr << "  f\tfunction to use (" << which << ")\n";
      cerr << "\n";
      exit(1);
    }
  }

  ga.evolve();
  cout << "best individual is " << ga.statistics().bestIndividual() << "\n\n";
  cout << ga.statistics() << "\n";

  return 0;
}














/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_1                                                */
/* Description: 2D tooth                                                     */
/* Boundaries:  -6 < x < 6                                                   */
/*              -6 < y < 6                                                   */
/* Source:      modified Himmelblau's function from Deb, K.                  */
/*              'GA in multimodal function optimazation' Masters thesis      */
/*		TCGA Rep. 89002 / U. of Alabama                              */
/*****************************************************************************/
float
Function1(float x, float y) {
  float z = -((x*x+y-11)*(x*x+y-11)+(x+y*y-7)*(x+y*y-7))/200 + 10;
  return z;
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_2                                                */
/* Description: Foxholes (25)                                                */
/* Boundaries:  -60 < x < 60                                                 */
/*              -60 < y < 60                                                 */
/* Source:      Shekel's Foxholes problem from De Jong's Diss.(1975)         */
/*              'GA in multimodal function optimazation' Masters thesis      */
/*		TCGA Rep. 89002 / U. of Alabama                              */
/*****************************************************************************/
float
Function2(float x, float y) {
  int i;
  float sum = 0;
  for (i=0; i<25; i++) {
    sum += (1 / (1 + i + pow((x-ai[i]),6) + pow((y-bi[i]),6)));
  }
  float z = 100.0 - (1 / (0.02 + sum));
  return z;
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_3                                                */
/* Description: Schwefel's nasty (1 glob. Max bei (420.96/420.96)            */
/* Boundaries:  -500 < x < 500                                               */
/*              -500 < y < 500                                               */
/* Source:      Schwefel's function in Schoeneburg                           */
/*****************************************************************************/
float
Function3(float x, float y) {
  float z = fabs(x) * sin(sqrt(fabs(x))) + fabs(y) * sin(sqrt(fabs(y)));
  //float z = 100  *  ( sin(sqrt(fabs(x))) * sin(sqrt(fabs(y))) );
  return (z);
}


/*****************************************************************************/
/* Type:        2D FUNCTION                                                  */
/* Name:        Objective2D_4                                                */
/* Description: Mexican Hat                                                  */
/* Boundaries:  -10 < x < 10                                                 */
/*              -10 < y < 10                                                 */
/* Source:                                                                   */
/*****************************************************************************/
float
Function4(float x, float y) {
  float z = sin(sqrt(x*x + y*y))*sin(sqrt(x*x + y*y)) - 0.5;
  z /= ((1.0 + 0.001*(x*x + y*y))*(1.0 + 0.001*(x*x + y*y)));
  z = (0.5 - z);
  return (z);
}













// These are the operators that we'll use for the real number genome.
float
Objective(GAGenome& g) {
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  return (obj[which])(genome.gene(0), genome.gene(1));
}

void
Initializer(GAGenome& g) {
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  genome.gene(0, GARandomFloat(minx[which], maxx[which]));
  genome.gene(1, GARandomFloat(miny[which], maxy[which]));
}

int
Mutator(GAGenome& g, float pmut) {
  GA1DArrayGenome<float>& genome = (GA1DArrayGenome<float>&)g;
  int nmut = 0;

  if(GAFlipCoin(pmut)){
    genome.gene(0, genome.gene(0) +
		10*GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    genome.gene(1, genome.gene(1) +
		10*GARandomFloat() * (GARandomFloat() - GARandomFloat()));
    nmut++;
  }

  if(genome.gene(0) < minx[which]) genome.gene(0, minx[which]);
  if(genome.gene(0) > maxx[which]) genome.gene(0, maxx[which]);
  if(genome.gene(1) < minx[which]) genome.gene(1, minx[which]);
  if(genome.gene(1) > maxx[which]) genome.gene(1, maxx[which]);

  return nmut;
}

int
Crossover(const GAGenome& g1,const GAGenome& g2,GAGenome* c1,GAGenome* c2){
  GA1DArrayGenome<float>& mom = (GA1DArrayGenome<float>&)g1;
  GA1DArrayGenome<float>& dad = (GA1DArrayGenome<float>&)g2;

  int n = 0;
  float distance = 0.0, midpoint = 0.0;

  if(c1) {
    GA1DArrayGenome<float>& sis = (GA1DArrayGenome<float>&)*c1;
    distance = midpoint = 0.0;

    midpoint = (mom.gene(0) + dad.gene(0)) / 2;
    distance = fabs(mom.gene(0) - dad.gene(0));
    sis.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    midpoint = (mom.gene(1) + dad.gene(1)) / 2;
    distance = fabs(mom.gene(1) - dad.gene(1));
    sis.gene(1, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    if(sis.gene(0) < minx[which]) sis.gene(0, minx[which]);
    if(sis.gene(0) > maxx[which]) sis.gene(0, maxx[which]);
    if(sis.gene(1) < minx[which]) sis.gene(1, minx[which]);
    if(sis.gene(1) > maxx[which]) sis.gene(1, maxx[which]);

    n += 1;
  }
  if(c2) {
    GA1DArrayGenome<float>& bro = (GA1DArrayGenome<float>&)*c2;
    distance = midpoint = 0.0;

    midpoint = (mom.gene(0) + dad.gene(0)) / 2;
    distance = fabs(mom.gene(0) - dad.gene(0));
    bro.gene(0, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    midpoint = (mom.gene(1) + dad.gene(1)) / 2;
    distance = fabs(mom.gene(1) - dad.gene(1));
    bro.gene(1, midpoint + distance * (GARandomFloat() - GARandomFloat()));

    if(bro.gene(0) < minx[which]) bro.gene(0, minx[which]);
    if(bro.gene(0) > maxx[which]) bro.gene(0, maxx[which]);
    if(bro.gene(1) < minx[which]) bro.gene(1, minx[which]);
    if(bro.gene(1) > maxx[which]) bro.gene(1, maxx[which]);

    n += 1;
  }

  return n;
}

float
Comparator(const GAGenome& g1, const GAGenome& g2) {
  GA1DArrayGenome<float>& a = (GA1DArrayGenome<float>&)g1;
  GA1DArrayGenome<float>& b = (GA1DArrayGenome<float>&)g2;

  float valx=(a.gene(0)-b.gene(0)) * (a.gene(0)-b.gene(0));
  float valy=(a.gene(1)-b.gene(1)) * (a.gene(1)-b.gene(1));
  return sqrt(valx+valy);
}

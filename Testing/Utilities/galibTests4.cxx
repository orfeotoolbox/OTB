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
REGISTER_TEST(galibTests4);
}
/* ----------------------------------------------------------------------------
  ex4.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SteadyStateGA class and 3DBinaryStringGenome 
class.  This program tries to fill the 3Dgenome with alternating 1's and
0's.  Notice that the steady state ga needs many more 'generations' than the
simple ga, but it will usually converge to the optimum with fewer evaluations
of the objective function.  So if you have a compute-intensive objective
function, steady state ga may be the way to go.
  You can control the amount of overlap in a steady-state GA by using the
pReplacement member function.
  This example also illustrates some of the statistics members of the genetic
algorithm object.
---------------------------------------------------------------------------- */
#include <stdio.h>

#include <ga/GASStateGA.h>	// include the steady-state GA header
#include <ga/GA3DBinStrGenome.h> // and the header for the genome we'll use
#include <ga/std_stream.h>

#define cout STD_COUT

float objective(GAGenome &);

int
galibTests4(int argc, char * argv[])
{
  cout << "Example 4\n\n";
  cout << "This program tries to fill a 3DBinaryStringGenome with\n";
  cout << "alternating 1s and 0s using a SteadyStateGA\n\n"; cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  const char * outputfile = argv[1];
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      GARandomSeed((unsigned int)atoi(argv[ii+1]));
    }
  }

  int depth    = 3;
  int width    = 10;
  int height   = 5;

// Now create the GA and run it.  First we create a genome of the type that
// we want to use in the GA.  The ga doesn't use this genome in the
// optimization - it just uses it to clone a population of genomes.

  GA3DBinaryStringGenome genome(width, height, depth, objective);

// Now that we have the genome, we create the genetic algorithm and set
// its parameters - number of generations, mutation probability, and crossover
// probability.  By default the GA keeps track of the best of generation scores
// and also keeps one genome as the 'best of all' - the best genome 
// that it encounters from all of the generations.  Here we tell the GA to
// keep track of all scores, not just the best-of-generation.
  const char * outputfilename = argv[1];
  
  GASteadyStateGA ga(genome);
  ga.populationSize(100);
  ga.pReplacement(0.50);	// replace 50% of population each generation
//  ga.nReplacement(4);	          // number of individuals to replace each gen
  ga.nGenerations(200);
  ga.pMutation(0.001);
  ga.pCrossover(0.9);
  ga.scoreFilename(outputfilename);//"bog.dat");	// name of file for scores
  ga.scoreFrequency(10);	// keep the scores of every 10th generation
  ga.flushFrequency(50);	// specify how often to write the score to disk
  ga.selectScores(GAStatistics::AllScores);
  ga.evolve();

// Now we print out the best genome.

  cout << "the ga generated:\n" << ga.statistics().bestIndividual() << "\n";
  cout << "best of generation data are in '"<<ga.scoreFilename()<<"'\n";

// That's it!

  return 0;
}
 



// This is the objective function.  All it does is give one point for every
// odd bit that is set and one point for every even bit that is not set.

float
objective(GAGenome& g)
{
  GA3DBinaryStringGenome & genome = (GA3DBinaryStringGenome &)g;
  float value=0.0;
  int count=0;
  int I=genome.width(), J=genome.height(), K=genome.depth();
  for(int i=0; i<I; i++){
    for(int j=0; j<J; j++){
      for(int k=0; k<K; k++){
	if(genome.gene(i,j,k) == 0 && count%2 == 0)
	  value += 1.0;
	else if(genome.gene(i,j,k) == 1 && count%2 != 0)
	  value += 1.0;
	count++;
      }
    }
  }
  return value;
}

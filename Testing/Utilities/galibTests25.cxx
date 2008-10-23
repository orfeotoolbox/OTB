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
REGISTER_TEST(galibTests25);
}

/* ----------------------------------------------------------------------------
  ex25.C
  mbwall jan96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to do multiple populations on a single CPU.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <ga/GA1DBinStrGenome.h>
#include <ga/GADemeGA.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define endl STD_ENDL

float Objective(GAGenome &);

int
galibTests25(int argc, char* argv[]) {
  cout << "Example 25\n\n";
  cout << "This example uses a genetic algorithm with multiple populations.\n";
  cout << endl;

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GA1DBinaryStringGenome genome(32, Objective);
  GADemeGA ga(genome);
  ga.nPopulations(5);
  ga.populationSize(30);
  ga.nGenerations(100);
  ga.pMutation(0.03);
  ga.pCrossover(1.0);
  ga.parameters(argc, argv);
  cout << "initializing..."; cout.flush();
  ga.initialize(seed);
  cout << "evolving..."; cout.flush();
  while(!ga.done()) {
    ga.step();
    cout << "."; cout.flush();
  }
  cout << endl;

  cout << "best individual is: " << ga.statistics().bestIndividual() << "\n";
  cout << "\n" << ga.statistics() << "\n";  

  return 0;
}

// This is the 1-max objective function - try to maximize the number of 1s in 
// a bit string of arbitrary length.
float
Objective(GAGenome& g) {
  GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)g;
  float score=0.0;
  for(int i=0; i<genome.length(); i++)
    score += genome.gene(i);
  score /= genome.length();
  return score;
}

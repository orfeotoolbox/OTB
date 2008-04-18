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
REGISTER_TEST(galibTests9);
}

/* ----------------------------------------------------------------------------
  ex9.C
  mbwall 10apr95
  Copyright 1995-1996 Massachusetts Institute of Technology

 DESCRIPTION:
   Sample program that illustrates how to use a GA to find the maximum value
of a continuous function in two variables.  This program uses a binary-to-
decimal genome.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT

float objective(GAGenome &);

int
galibTests9(int argc, char * argv[])
{
  cout << "Example 9\n\n";
  cout << "This program finds the maximum value in the function\n";
  cout << "  y = - x1^2 - x2^2\n";
  cout << "with the constraints\n";
  cout << "     -5 <= x1 <= 5\n";
  cout << "     -5 <= x2 <= 5\n";
  cout << "\n\n"; cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int i=1; i<argc; i++) {
    if(strcmp(argv[i],"seed") == 0) {
      seed = atoi(argv[i+1]);
    }
  }

// Declare variables for the GA parameters and set them to some default values.

  int popsize  = 30;
  int ngen     = 100;
  float pmut   = 0.01;
  float pcross = 0.6;

// Create a phenotype for two variables.  The number of bits you can use to
// represent any number is limited by the type of computer you are using.  In
// this case, we use 16 bits to represent a floating point number whose value
// can range from -5 to 5, inclusive.  The bounds on x1 and x2 can be applied
// here and/or in the objective function.

  GABin2DecPhenotype map;
  map.add(16, -5, 5);
  map.add(16, -5, 5);

// Create the template genome using the phenotype map we just made.

  GABin2DecGenome genome(map, objective);

// Now create the GA using the genome and run it.  We'll use sigma truncation
// scaling so that we can handle negative objective scores.

  const char * outputfilename = argv[1];

  GASimpleGA ga(genome);
  GASigmaTruncationScaling scaling;
  ga.populationSize(popsize);
  ga.nGenerations(ngen);
  ga.pMutation(pmut);
  ga.pCrossover(pcross);
  ga.scaling(scaling);
  ga.scoreFilename(outputfilename);//"bog.dat");
  ga.scoreFrequency(10);
  ga.flushFrequency(50);
  ga.evolve(seed);

// Dump the results of the GA to the screen.

  genome = ga.statistics().bestIndividual();
  cout << "the ga found an optimum at the point (";
  cout << genome.phenotype(0) << ", " << genome.phenotype(1) << ")\n\n";
  cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

  return 0;
}
 

// This objective function tries to maximize the value of the function
//
//                  y = -(x1*x1 + x2*x2)
//
float
objective(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;

  float y;
  y = -genome.phenotype(0) * genome.phenotype(0);
  y -= genome.phenotype(1) * genome.phenotype(1);
  return y;
}

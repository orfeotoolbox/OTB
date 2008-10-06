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
REGISTER_TEST(galibTests15);
}
/* ----------------------------------------------------------------------------
  ex15.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example nearly identical to example 2, but it uses convergence as the
stopping criterion for the GA rather than number-of-generations.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT

float objective(GAGenome &);

int
galibTests15(int argc, char *argv[])
{
  cout << "Example 15\n\n";
  cout << "This program generates a sequence of random numbers then uses\n";
  cout << "a simple GA and binary-to-decimal genome to match the\n";
  cout << "sequence.  It uses the convergence of the best-of-generation\n";
  cout << "as the criterion for when to stop.\n\n";

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      seed = atoi(argv[ii+1]);
    }
  }

// Declare variables for the GA parameters and set them to some default values.
// When we use convergence as the completion measure we have to specify both
// a convergence value (larger means more converged) and a number-of-gen
// which specifies how many generations back to look to calculate the 
// convergence.  The number of generations back defaults to 20, so you do not
// have to set that if you don't want to.

  int popsize  = 30;
  float pmut   = 0.01;
  float pcross = 0.6;
  float pconv  = 0.99;		// threshhold for when we have converged
  int nconv    = 50;		// how many generations back to look

// Generate a sequence of random numbers using the values in the min and max
// arrays.  We also set one of them to integer value to show how you can get
// explicit integer representations by choosing your number of bits
// appropriately.

  GARandomSeed(seed);
  int i;
  int n=7;
  float *target = new float[n];
  float min[] = {0, 0,   3, -5, 100,    0.001, 0};
  float max[] = {1, 100, 3, -2, 100000, 0.010, 7};
  for(i=0; i<n; i++)
    target[i] = GARandomFloat(min[i], max[i]);
  target[6] = GARandomInt((int)min[6], (int)max[6]);

// Print out the sequence to be sure we got the right one.

  cout << "input sequence:\n";
  for(i=0; i<n; i++){
    cout.width(10);
    cout << target[i] << " ";
  }
  cout << "\n"; cout.flush();

// Create a phenotype then fill it with the phenotypes we will need to map to
// the values we read from the file.  The arguments to the add() method of a
// Bin2Dec phenotype are (1) number of bits, (2) min value, and (3) max value.
// The phenotype maps a floating-point number onto the number of bits that
// you designate.  Here we just make everything use 8 bits and use the max and
// min that were used to generate the target values.  You can experiment with
// the number of bits and max/min values in order to make the GA work better
// or worse.

  GABin2DecPhenotype map;
  for(i=0; i<n; i++)
    map.add(8, min[i], max[i]);

// Create the template genome using the phenotype map we just made.  The
// GA will use this genome to clone the population that it uses to do the
// evolution.

  GABin2DecGenome genome(map, objective, (void *)target);

// Now create the GA using the genome and run it.

  const char * outputfilename = argv[1];

  GASteadyStateGA ga(genome);
  ga.scoreFrequency(1);
  ga.flushFrequency(50);
  ga.scoreFilename(outputfilename);//"bog.dat");
  ga.populationSize(popsize);
  ga.pMutation(pmut);
  ga.pCrossover(pcross);
  ga.pConvergence(pconv);
  ga.nConvergence(nconv);
  ga.terminator(GAGeneticAlgorithm::TerminateUponConvergence);
  ga.evolve();

// Dump the results of the GA to the screen.

  genome.initialize();
  cout << "random values in the genome:\n";;
  unsigned int jj=0;
  for(jj=0; jj<map.nPhenotypes(); jj++){
    cout.width(10); cout << genome.phenotype(jj) << " ";
  }
  cout << "\n";
  genome = ga.statistics().bestIndividual();
  cout << "the ga generated:\n";
  for(jj=0; jj<map.nPhenotypes(); jj++){
    cout.width(10); cout << genome.phenotype(jj) << " ";
  }
  cout << "\n\n"; cout.flush();

// Clean up by freeing the memory we allocated.

  delete [] target;
  return 0;
}
 

// For this objective function we try to match the values in the array of float
// that is passed to us as userData.  If the values in the genome map to 
// values that are close, we return a better score.  We are limited to positive
// values for the objective value (because we're using linear scaling), so we
// take the reciprocal of the absolute value of the difference between the
// value from the phenotype and the value in the sequence.
float
objective(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  float *sequence = (float *)c.userData();

  float value=genome.nPhenotypes();
  for(int i=0; i<genome.nPhenotypes(); i++)
    value += 1.0 / (1.0 + fabs(genome.phenotype(i) - sequence[i]));
  return(value);
}

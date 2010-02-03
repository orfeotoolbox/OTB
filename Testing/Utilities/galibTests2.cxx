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
REGISTER_TEST(galibTests2);
}

/* ----------------------------------------------------------------------------
  ex2.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SimpleGA class and Bin2DecGenome class.  This
program generates randomly a series of numbers then tries to match those
values in a binary-to-decimal genome.  We use a simple GA (with linear
scaled fitness selection and non-steady-state population generation) and
binary-to-decimal, 1D genomes.  We also use the userData argument to the
objective function.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <math.h>

#include <ga/GASimpleGA.h>
#include <ga/GABin2DecGenome.h>
#include <ga/std_stream.h>

#define cout STD_COUT

float Objective(GAGenome &);

int
galibTests2(int argc, char *argv[])
{
  cout << "Example 2\n\n";
  cout << "This program generates a sequence of random numbers then uses\n";
  cout << "a simple GA and binary-to-decimal genome to match the\n";
  cout << "sequence.\n\n";

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

  int popsize  = 25;
  int ngen     = 100;
  float pmut   = 0.01;
  float pcross = 0.6;

// Generate a sequence of random numbers using the values in the min and max
// arrays.  We also set one of them to integer value to show how you can get
// explicit integer representations by choosing your number of bits
// appropriately.

  GARandomSeed(seed);
  unsigned int n=7;
  float *target = new float[n];
  float min[] = {0, 0,   3, -5, 100,    0.001, 0};
  float max[] = {1, 100, 3, -2, 100000, 0.010, 7};
  unsigned int i;
  for(i=0; i<n; i++)
    target[i] = GARandomFloat(min[i], max[i]);
  target[6] = GARandomInt((int)min[6], (int)max[6]);

// Print out the sequence to see what we got.

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
// evolution.  We pass the objective function to create the genome.  We
// also use the user data function in the genome to keep track of our
// target values.

  GABin2DecGenome genome(map, Objective, (void *)target);

// Now create the GA using the genome, set the parameters, and run it.

  const char * outputfilename = argv[1];

  GASimpleGA ga(genome);
  ga.populationSize(popsize);
  ga.nGenerations(ngen);
  ga.pMutation(pmut);
  ga.pCrossover(pcross);
  ga.scoreFilename(outputfilename);//"bog.dat");
  ga.flushFrequency(50);	// dump scores to disk every 50th generation
  ga.evolve(seed);

// Dump the results of the GA to the screen.  We print out first what a random
// genome looks like (so we get a bit of a feel for how hard it is for the
// GA to find the right values) then we print out the best genome that the
// GA was able to find.

  genome.initialize();
  cout << "random values in the genome:\n";;
  for(i=0; i<map.nPhenotypes(); i++){
    cout.width(10); cout << genome.phenotype(i) << " ";
  }
  cout << "\n";

  genome = ga.statistics().bestIndividual();
  cout << "the ga generated:\n";
  for(i=0; i<map.nPhenotypes(); i++){
    cout.width(10); cout << genome.phenotype(i) << " ";
  }
  cout << "\n\n"; cout.flush();

// We could print out the genome directly, like this:
// cout << genome << "\n";

  cout << "best of generation data are in '"<<ga.scoreFilename()<<"'\n";

// Clean up by freeing the memory we allocated.

  delete [] target;
  return 0;
}


// For this objective function we try to match the values in the array of float
// that is passed to us as userData.  If the values in the genome map to
// values that are close, we return a better score.  We are limited to positive
// values for the objective value (because we're using linear scaling - the
// default scaling method for SimpleGA), so we take the reciprocal of the
// absolute value of the difference between the value from the phenotype and
// the value in the sequence.
float
Objective(GAGenome& g)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)g;
  float *sequence = (float *)g.userData();

  float value=genome.nPhenotypes();
  for(int i=0; i<genome.nPhenotypes(); i++)
    value += 1.0 / (1.0 + fabs(genome.phenotype(i) - sequence[i]));
  return value;
}

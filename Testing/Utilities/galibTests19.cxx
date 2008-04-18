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
REGISTER_TEST(galibTests19);
}
/* ----------------------------------------------------------------------------
  ex19.C
  mbwall 5sep95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example runs all of the DeJong functions.  You can specify which 
function you want at the command line.  We use SigmaTruncation scaling to make
certain that negative objective scores won't be a problem for us.
   The DeJong functions in this code were pinched from the eval.c file in the
pga-2.8 genetic algorithm package (under the terms of the GNU General Public
License).
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR

double Gauss(double mean, double variance);

float DeJong1(GAGenome &);
float DeJong2(GAGenome &);
float DeJong3(GAGenome &);
float DeJong4(GAGenome &);
float DeJong5(GAGenome &);

GAGenome::Evaluator objective[5] = {DeJong1,DeJong2,DeJong3,DeJong4,DeJong5};

int
galibTests19(int argc, char *argv[])
{
  cout << "Example 19\n\n";
  cout << "This program runs the DeJong test problems.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);	// population size
  params.set(gaNpCrossover, 0.9);	// probability of crossover
  params.set(gaNpMutation, 0.001);	// probability of mutation
  params.set(gaNnGenerations, 400);	// number of generations
  params.set(gaNpReplacement, 0.25);	// how much of pop to replace each gen
  params.set(gaNscoreFrequency, 10);	// how often to record scores
  params.set(gaNflushFrequency, 50);	// how often to dump scores to file
  params.set(gaNscoreFilename, "bog.dat");
  params.parse(argc, argv, gaFalse);    // parse command line for GAlib args

  int whichFunction = 0;

  for(int i=1; i<argc; i++){
    if(strcmp("function", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": you must specify a function (1-5)\n";
        exit(1);
      }
      else{
	whichFunction = atoi(argv[i]) - 1;
	if(whichFunction < 0 || whichFunction > 4){
	  cerr << argv[0] << ": the function must be in the range [1,5]\n";
	  exit(1);
	}
        continue;
      }
    }
    else if(strcmp("seed", argv[i]) == 0){
      if(++i < argc) continue;
      continue;
    }
    else {
      cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
      cerr << "valid arguements include standard GAlib arguments plus:\n";
      cerr << "  f\twhich function to evaluate (all)\n";
      cerr << "parameters are:\n\n" << params << "\n\n";
      exit(1);
    }
  }

// Create the phenotype map depending on which dejong function we are going
// to be running.

  GABin2DecPhenotype map;
  switch(whichFunction){
  case 0:
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    break;

  case 1:
    map.add(16, -2.048, 2.048);
    map.add(16, -2.048, 2.048);
    break;

  case 2:
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    map.add(16, -5.12, 5.12);
    break;

  case 3:
    {
      for(int j=0; j<30; j++)
	map.add(16, -1.28, 1.28);
    }
    break;

  case 4:
    map.add(16, -65.536, 65.536);
    map.add(16, -65.536, 65.536);
    break;

  default:
    map.add(16, 0, 0);
    break;
  }

// Now create the sample genome and run the GA.

  GABin2DecGenome genome(map, objective[whichFunction]);
  //  GAStatistics stats;

  GASteadyStateGA ga(genome);
  ga.parameters(params);
  GASigmaTruncationScaling scaling;
  ga.scaling(scaling);

  cout << "running DeJong function number " << (whichFunction + 1) << " ...\n";

  ga.evolve(seed);

  cout << "the ga generated:\n" << ga.statistics().bestIndividual() << "\n";
  cout << "\nthe statistics for the run are:\n" << ga.statistics();
  cout << "\nbest-of-generation data are in 'bog.dat'\n";
  cout.flush();

  return 0;
}
 




// DeJong's first function is:
//
// f1(x1,x2,x3) = x1*x1 + x2*x2 + x3*x3
//
// where each x is in the range [-5.12,5.12]
float
DeJong1(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  float value=0;
  value += genome.phenotype(0) * genome.phenotype(0);
  value += genome.phenotype(1) * genome.phenotype(1);
  value += genome.phenotype(2) * genome.phenotype(2);
  return(value);
}



// DeJong's second function is:
//
// f2(x1,x2) = 100 * (x1*x1 - x2)^2 + (1 - x1)^2
//
// where each x is in the range [-2.048,2.048]
float
DeJong2(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  float value=100.0;
  value *= genome.phenotype(0) * genome.phenotype(0) - genome.phenotype(1);
  value *= genome.phenotype(0) * genome.phenotype(0) - genome.phenotype(1);
  value += (1 - genome.phenotype(0))*(1 - genome.phenotype(0));
  return(value);
}




// DeJong's third function is:
//
// f3(x1,x2,x3,x4,x5) = 
//      25 + floor(x1) + floor(x2) + floor(x3) + floor(x4) + floor(x5)
//
// where each x is in the range [-5.12,5.12]
float
DeJong3(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  float value=25.0;
  value -= floor(genome.phenotype(0));
  value -= floor(genome.phenotype(1));
  value -= floor(genome.phenotype(2));
  value -= floor(genome.phenotype(3));
  value -= floor(genome.phenotype(4));
  return(value);
}





// DeJong's fourth function is:
//
//             30
//            ___
// f4(xi) =   \   { i * xi^4 + Gauss(0,1) }
//            /__
//            i=1
//
// where each x is in the range [-1.28,1.28]
float
DeJong4(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  float value = 0;
  for(int i=0; i<30; i++){
    float v = genome.phenotype(i);
    v *= v;			// xi^2
    v *= v;			// xi^4
    v *= i;
    v += Gauss(0,1);
    value += v;
  }
  return(value);
}








// DeJong's fifth function is (Shekel's foxholes):
//
//
//               25                 1
//               ___    ---------------------------
// f5(x1,x2) =   \              2
//               /__           ___ 
//               j=1      j +  \   (x[i] - a[i][j])^6
//                             /__
//                             i=1
//
// where each x is in the range [-65.536,65.536]
//
static int a[2][25] ={
  {-32, -16, 0, 16, 32, -32, -16, 0, 16, 32, -32, -16, 0, 16, 32,
   -32, -16, 0, 16, 32, -32, -16, 0, 16, 32        },
  {-32, -32, -32, -32, -32, -16, -16, -16, -16, -16,
   16, 16, 16, 16, 16, 32, 32, 32, 32, 32  }
};

float
DeJong5(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;

  float lowtot,prod,total=0.002;

  for(int j=0; j<25; j+=1) {
    lowtot=1.0 + (double)j;
    for(int i=0; i<2; i+=1) {
      prod=1.0;
      for(int power=0; power<6; power+=1)
        prod*=genome.phenotype(i)-a[i][j];
      lowtot+=prod;
    }
    total+=1.0/lowtot;
  }

  return(500.0 - (1.0/total));
}






// Return a number from gaussian distribution.  This code was pinched from the
// GNU libg++ Normal.cc implementation of a normal distribution.  That is, in
// turn, based on Simulation, Modelling & Analysis by Law & Kelton, pp259.
//
// My random number generator (actually just the system's) isn't as good as 
// that in the libg++, but this should be OK for this purpose.
double
Gauss(double mean, double variance){
  for(;;) {
    double u1 = GARandomDouble();
    double u2 = GARandomDouble();
    double v1 = 2 * u1 - 1;
    double v2 = 2 * u2 - 1;
    double w = (v1 * v1) + (v2 * v2);

    if (w <= 1) {
      double y = sqrt( (-2 * log(w)) / w);
      double x1 = v1 * y;
//      double x2 = v2 * y;       // we don't use this one
      return(x1 * sqrt(variance) + mean);
    }
  }
}



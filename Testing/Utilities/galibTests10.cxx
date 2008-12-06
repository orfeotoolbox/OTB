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
REGISTER_TEST(galibTests10);
}

/* ----------------------------------------------------------------------------
  ex10.C
  mbwall 10apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Sample program that illustrates how to use a distance function to do
speciation.  This example does both gene-based and phenotype-based distance
calculations.  The differences are quite interesting.  Also, the length of the
bit string (i.e. the size of the search space) is also a significant factor in
the performance of the speciation methods.
   Notice that Goldberg describes fitness scaling speciation in the context of
a simple genetic algorithm.  You can try using it with a steady-state
algorithm, but you'll get bogus results unless you modify the algorithm.
---------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define endl STD_ENDL
#define ofstream STD_OFSTREAM

#define USE_RAW_SINE

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define NBITS     8

#ifdef USE_RAW_SINE
#define FUNCTION  Function1
#define MIN_VALUE 0
#define MAX_VALUE 5
#else
#define FUNCTION  Function2
#define MIN_VALUE -100
#define MAX_VALUE 100
#endif

float Function1(float);
float Function2(float);
float Objective(GAGenome &);
float BitDistance(const GAGenome & a, const GAGenome & b);
float PhenotypeDistance(const GAGenome & a, const GAGenome & b);

int
galibTests10(int argc, char *argv[])
{
  cout << "Example 10\n\n";
  cout << "This program uses sharing to do speciation.  The objective\n";
  cout << "function has more than one optimum, so different genomes\n";
  cout << "may have equally high scores.  Speciation keeps the population\n";
  cout << "from clustering at one optimum.\n";
  cout << "  Both gene-wise and phenotype-wise distance functions are used.\n";
  cout << "  Populations from all three runs are written to the files \n";
  cout << "pop.nospec.dat, pop.genespec.dat and pop.phenespec.dat.  The\n";
  cout << "function is written to the file sinusoid.dat\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.


  int i;
  const char * filename = argv[1];//"sinusoid.dat";
  const char * popfilename1 = argv[2];//"pop.nospec.dat";
  const char * popfilename2 = argv[3];//"pop.genespec.dat";
  const char * popfilename3 = argv[4];//"pop.phenespec.dat";
  ofstream outfile;

  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      GARandomSeed((unsigned int)atoi(argv[ii+1]));
    }
  }

// Create a phenotype for two variables.  The number of bits you can use to
// represent any number is limited by the type of computer you are using.  In
// this case, we use 16 bits to represent a floating point number whose value
// can range from the minimum to maximum value as defined by the macros.

  GABin2DecPhenotype map;
  map.add(NBITS, MIN_VALUE, MAX_VALUE);

// Create the template genome using the phenotype map we just made.

  GABin2DecGenome genome(map, Objective);

// Now create the GA using the genome and set all of the parameters.
// You'll get different results depending on the type of GA that you use.  The
// steady-state GA tends to converge faster (depending on the type of replace-
// ment method you specify).

  GASimpleGA ga(genome);
  ga.set(gaNpopulationSize, 200);
  ga.set(gaNnGenerations, 50);
  ga.set(gaNpMutation, 0.001);
  ga.set(gaNpCrossover, 0.9);
  ga.parameters(argc, argv);


// Do the non-speciated and write to file the best-of-generation.

  cout << "running with no speciation (fitness proportionate scaling)...\n";
  cout.flush();
  GALinearScaling lin;
  ga.scaling(lin);
  ga.evolve();
  genome = ga.statistics().bestIndividual();
  cout << "the ga found an optimum at the point "<<genome.phenotype(0)<<endl;

  outfile.open(popfilename1, (STD_IOS_OUT | STD_IOS_TRUNC));
  if(outfile.fail()){
    cerr << "Cannot open " << popfilename1 << " for output.\n";
    exit(1);
  }
  for(i=0; i<ga.population().size(); i++){
    outfile<<((GABin2DecGenome&)(ga.population().individual(i))).phenotype(0);
    outfile << "\t";
    outfile << ga.population().individual(i).score() << "\n";
  }
  outfile.close();



// Now do speciation using the gene-wise distance function

  cout << "running the ga with speciation (sharing using bit-wise)...\n";
  cout.flush();
  GASharing bitSharing(BitDistance);
  ga.scaling(bitSharing);
  ga.evolve();
  genome = ga.statistics().bestIndividual();
  cout << "the ga found an optimum at the point "<<genome.phenotype(0)<<endl;

  outfile.open(popfilename2, (STD_IOS_OUT | STD_IOS_TRUNC));
  if(outfile.fail()){
    cerr << "Cannot open " << popfilename2 << " for output.\n";
    exit(1);
  }
  for(i=0; i<ga.population().size(); i++){
    outfile<<((GABin2DecGenome&)(ga.population().individual(i))).phenotype(0);
    outfile << "\t";
    outfile << ga.population().individual(i).score() << "\n";
  }
  outfile.close();



// Now do speciation using the phenotype-wise distance function

  cout << "running the ga with speciation (sharing using phenotype-wise)...\n";
  cout.flush();
  GASharing pheneSharing(PhenotypeDistance);
  ga.scaling(pheneSharing);
  ga.evolve();
  genome = ga.statistics().bestIndividual();
  cout << "the ga found an optimum at the point "<<genome.phenotype(0)<<endl;

  outfile.open(popfilename3, (STD_IOS_OUT | STD_IOS_TRUNC));
  if(outfile.fail()){
    cerr << "Cannot open " << popfilename3 << " for output.\n";
    exit(1);
  }
  for(i=0; i<ga.population().size(); i++){
    outfile<<((GABin2DecGenome&)(ga.population().individual(i))).phenotype(0);
    outfile << "\t";
    outfile << ga.population().individual(i).score() << "\n";
  }
  outfile.close();


// Now dump the function to file for comparisons

  cout << "dumping the function to file..." << endl;
  outfile.open(filename, (STD_IOS_OUT | STD_IOS_TRUNC));
  if(outfile.fail()){
    cerr << "Cannot open " << filename << " for output.\n";
    exit(1);
  }
  float inc = MAX_VALUE - MIN_VALUE;
  inc /= pow(2.0,NBITS);
  for(float x=MIN_VALUE; x<=MAX_VALUE; x+=inc){
    outfile << x << "\t" << FUNCTION (x) << "\n";
  }
  outfile << "\n";
  outfile.close();

  return 0;
}





// You can choose between one of two sinusoidal functions.  The first one has
// peaks of equal amplitude.  The second is modulated.
float
Objective(GAGenome & c){
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  return FUNCTION (genome.phenotype(0));
}

float
Function1(float v) {
  return 1 + sin(v*2*M_PI);
}

float
Function2(float v) {
  float y;
  y = 100.0 * exp(-fabs(v) / 50.0) * (1.0 - cos(v * M_PI * 2.0 / 25.0));
  if(v < -100 || v > 100) y = 0;
  if(y < 0) y = 0;
  return y+0.00001;
}





// Here are a couple of possible distance functions for this problem.  One of
// them uses the genes to determine the same-ness, the other uses the
// phenotypes to determine same-ness.  If the genomes are the same, then
// we return a 0.  If they are completely different then we return a 1.
//   In either case, you should be sure that the distance function will return
// values only between 0 and 1 inclusive.  If your function returns values
// outside these limits, the GA will produce bogus results and it WILL NOT warn
// you that your distance function is brain-dead!

// This distance function uses the genes to determine same-ness.  All we do
// is check to see if the bit strings are identical.

float
BitDistance(const GAGenome & c1, const GAGenome & c2){
  GABin2DecGenome & a = (GABin2DecGenome &)c1;
  GABin2DecGenome & b = (GABin2DecGenome &)c2;

  float x=0;
  for(int i=a.length()-1; i>=0; i--)
    x += (a[i] != b[i] ? 1 : 0);

  return x/a.length();
}



// This distance function looks at the phenotypes rather than the genes of the
// genome.  This distance function will try to drive them to extremes.

float
PhenotypeDistance(const GAGenome & c1, const GAGenome & c2){
  GABin2DecGenome & a = (GABin2DecGenome &)c1;
  GABin2DecGenome & b = (GABin2DecGenome &)c2;

  return fabs(a.phenotype(0) - b.phenotype(0)) / (MAX_VALUE-MIN_VALUE);
}


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
REGISTER_TEST(galibTests21);
}

/* ----------------------------------------------------------------------------
  ex21.C
  mbwall 1jan96
  Copyright (c) 1995-1996 Massachusetts Institute of Technology

 DESCRIPTION:
   This example illustrates various uses of the AlleleSet in concert with the
ArrayAlleleGenome.  In particular, we use the RealGenome to show how you can
use the enumerated and bounded types of allele sets.
   You can define one allele set for an entire array, or you can define one
allele set for each element in the array.  The constructor that you use to
create the array determines which behaviour you'll get.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define endl STD_ENDL

#define INSTANTIATE_REAL_GENOME
#include <ga/GARealGenome.h>

float Objective1(GAGenome &);
float Objective2(GAGenome &);
float Objective3(GAGenome &);
float Objective4(GAGenome &);

int
galibTests21(int argc, char * argv[])
{
  cout << "Example 21\n\n";
  cout << "This example shows various uses of the allele set object\n";
  cout << "in combination with the real number genome.\n\n"; cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  const char * filename_bog1 = argv[1];//"bog1.dat";
  const char * filename_bog2 = argv[2];//"bog2.dat";
  const char * filename_bog2a = argv[3];//"bog2a.dat";
  const char * filename_bog3 = argv[4];//"bog3.dat";
  const char * filename_bog4 = argv[5];//"bog4.dat";

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      seed = atoi(argv[ii+1]);
    }
  }

// First make a bunch of genomes.  We'll use each one in turn for a genetic
// algorithm later on.  Each one illustrates a different method of using the
// allele set object.  Each has its own objective function.

  int length = 8;

// This genome uses an enumerated list of alleles.  We explictly add each
// allele to the allele set.  Any element of the genome may assume the value
// of any member of the allele set.

  GARealAlleleSet alleles1;
  alleles1.add(-10);
  alleles1.add(0.1);
  alleles1.add(1.0);
  alleles1.add(10);
  alleles1.add(100);
  GARealGenome genome1(length, alleles1, Objective1);

// This genome uses a bounded set of continous numbers.  The default arguments
// are INCLUSIVE for both the lower and upper bounds, so in this case the
// allele set is [0,1] and any element of the genome may assume a value [0,1].

  GARealAlleleSet alleles2(0, 1);
  GARealGenome genome2(length, alleles2, Objective2);

// Similar to the previous set, but this one has EXCLUSIVE bounds and we create
// the allele set explicitly (even though in this case

  GARealAlleleSetArray alleles2a;
  for(int i=0; i<length; i++)
    alleles2a.add(0, 1, GAAllele::EXCLUSIVE, GAAllele::EXCLUSIVE);
  GARealGenome genome2a(alleles2a, Objective2);

// Here we create a genome whose elements may assume any value in the interval
// [0.0, 10.0) discretized on the interval 0.5, i.e. the values 0.0, 0.5, 1.0,
// and so on up to but not including 10.0.
// Note that the default operators for the real genome are uniform initializer,
// gaussian mutator, and uniform crossover.  Since gaussian is not the behavior
// we want for mutation, we assign the flip mutator instead.

  GARealAlleleSet alleles3(0,10,0.5,GAAllele::INCLUSIVE,GAAllele::EXCLUSIVE);
  GARealGenome genome3(length, alleles3, Objective3);
  genome3.crossover(GARealUniformCrossover);
  genome3.mutator(GARealSwapMutator);

// This genome is created using an array of allele sets.  This means that each
// element of the genome will assume a value in its corresponding allele set.
// For example, since the first allele set is [0,10], the first element of the
// genome will be in [0,10].  Notice that you can add allele sets in many other
// ways than those shown.

  GARealAlleleSetArray alleles4;
  alleles4.add(0,10);
  alleles4.add(50,100);
  alleles4.add(-10,-5);
  alleles4.add(-0.01,-0.0001);
  alleles4.add(10000,11000);
  GARealGenome genome4(alleles4, Objective4);


// Now that we have the genomes, create a parameter list that will be used for
// all of the genetic algorithms and all of the genomes.

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNnGenerations, 500);
  params.set(gaNpopulationSize, 110);
  params.set(gaNscoreFrequency, 10);
  params.set(gaNflushFrequency, 50);
  params.set(gaNselectScores, (int)GAStatistics::AllScores);
  params.parse(argc, argv, gaFalse);


// Now do a genetic algorithm for each one of the genomes that we created.

  GASteadyStateGA ga1(genome1);
  ga1.parameters(params);
  ga1.set(gaNscoreFilename, filename_bog1);//"bog1.dat");
  cout << "\nrunning ga number 1 (alternate allele(0) and allele(3))..."<<endl;
  ga1.evolve(seed);
  cout << "the ga generated:\n" << ga1.statistics().bestIndividual() << endl;

  GASteadyStateGA ga2(genome2);
  ga2.parameters(params);
  ga2.set(gaNscoreFilename, filename_bog2);//"bog2.dat");
  cout << "\nrunning ga number 2 (continuous descending order)..." << endl;
  ga2.evolve();
  cout << "the ga generated:\n" << ga2.statistics().bestIndividual() << endl;

  GASteadyStateGA ga2a(genome2a);
  ga2a.parameters(params);
  ga2a.set(gaNscoreFilename, filename_bog2a);//"bog2a.dat");
  cout << "\nrunning ga number 2a (descending order, EXCLUSIVE)..." << endl;
  ga2a.evolve();
  cout << "the ga generated:\n" << ga2a.statistics().bestIndividual() << endl;

  GASteadyStateGA ga3(genome3);
  ga3.parameters(params);
  ga3.set(gaNscoreFilename, filename_bog3);//"bog3.dat");
  cout << "\nrunning ga number 3 (discretized ascending order)..." << endl;
  ga3.evolve();
  cout << "the ga generated:\n" << ga3.statistics().bestIndividual() << endl;

  GASteadyStateGA ga4(genome4);
  ga4.parameters(params);
  ga4.set(gaNscoreFilename, filename_bog4);//"bog4.dat");
  cout << "\nrunning ga number 4 (maximize each gene)..." << endl;
  ga4.evolve();
  cout << "the ga generated:\n" << ga4.statistics().bestIndividual() << endl;

  return 0;
}




// This objective function tries to maximize the occurance of the first and
// fourth alleles.  It tries to put the first allele in the even elements and
// the fourth allele in the odd elements.

float
Objective1(GAGenome& g)
{
  GARealGenome& genome = (GARealGenome&)g;
  float value=0.0;
  for(int i=0; i<genome.length(); i++){
    if(i%2 == 0 && genome.gene(i) == genome.alleleset().allele(0))
      value += 1.0;
    if(i%2 != 0 && genome.gene(i) == genome.alleleset().allele(3))
      value += 1.0;
  }
  return value;
}


// This objective function tries to generate a straight - it gives higher score
// to a genome whose elements descend in value.  If two genomes both have
// elements in strictly descending order, they get the same score regardless
// of their values.

float
Objective2(GAGenome& g)
{
  GARealGenome& genome = (GARealGenome&)g;
  float value=0.0;
  for(int i=1; i<genome.length(); i++)
    if(genome.gene(i) < genome.gene(i-1)) value += 1.0;
  return value;
}


// This objective function generates a straight by giving higher score to a
// genome whose elements ascend in value.

float
Objective3(GAGenome& g)
{
  GARealGenome& genome = (GARealGenome&)g;
  float value=0.0;
  for(int i=1; i<genome.length(); i++)
    if(genome.gene(i) > genome.gene(i-1)) value += 1.0;
  return value;
}


// This objective tries to maximize each element in the genome.

float
Objective4(GAGenome& g)
{
  GARealGenome& genome = (GARealGenome&)g;
  float value=0.0;
  for(int i=0; i<genome.length(); i++)
    value += genome.gene(i);
  return value;
}

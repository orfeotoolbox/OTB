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
REGISTER_TEST(galibTests13);
}

/* ----------------------------------------------------------------------------
  ex13.C
  mbwall 29apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
  This example illustrates the use of a GA-within-a-GA.  It uses a steady
state GA to find the smiley face, then it uses another steady state GA to match
a sequence of random numbers.  It doesn't try to do the random numbers until
it has gotten 90% of the way to the smiley face.
  This is very similar in function to the composite genome example, but 
here we evolve the two genomes separately rather than as a single entity.
  This kind of application can be useful for situations where the computational
cost of calculating feasibility is rather high and the feasible space is 
sparse.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define ifstream STD_IFSTREAM

typedef struct _UserData {
  int width, height;
  short **picture_target;
  float *numbers_target;
  GA2DBinaryStringGenome *picture_genome;
  GABin2DecGenome *numbers_genome;
} UserData;

float PictureObjective(GAGenome &);
float NumbersObjective(GAGenome &);

int
galibTests13(int argc, char *argv[])
{
  cout << "Example 13\n\n";
  cout << "This program runs a GA-within-GA.  The outer level GA tries to\n";
  cout << "match the pattern read in from a file.  The inner level GA is\n";
  cout << "run only when the outer GA reaches a threshhold objective score\n";
  cout << "then it tries to match a sequence of numbers that were generated\n";
  cout << "randomly at the beginning of the program's execution.\n\n";
  cout << "You might have to run the primary GA for more than 5000\n";
  cout << "generations to get a score high enough to kick in the secondary\n";
  cout << "genetic algorithm.  Use the ngen option to do this on the\n";
  cout << "command line.\n\n";

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.
  unsigned int seed = 0;
  for(int ii=2; ii<argc; ii++) {
    if(strcmp(argv[ii++],"seed") == 0) {
      seed = atoi(argv[ii]);
    }
  }


// Set the default values of the parameters and declare the params variable.
  const char * filename = argv[1];

  int i,j;
  GAParameterList params;
  GAIncrementalGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 150);
  params.set(gaNpCrossover, 0.8);
  params.set(gaNpMutation, 0.005);
  params.set(gaNnGenerations, 500);
  params.set(gaNscoreFilename, "bog.dat");
  params.set(gaNscoreFrequency, 10);
  params.set(gaNflushFrequency, 50);
  params.parse(argc, argv, gaFalse);   // don't complain about unknown args


// Create a user data object.  We'll keep all of the information for this 
// program in this object.
  UserData data;

// Read in the pattern from the specified file.  File format is pretty simple:
// two integers that give the height then width of the matrix, then the matrix
// of 1's and 0's (with whitespace inbetween).

  ifstream inStream(filename);
  if(!inStream){
    cerr << "Cannot open " << filename << " for input.\n";
    exit(1);
  }
  inStream >> data.height >> data.width;
  data.picture_target = new short*[data.width];
  for(i=0; i<data.width; i++)
    data.picture_target[i] = new short[data.height];
  for(j=0; j<data.height; j++)
    for(i=0; i<data.width; i++)
      inStream >> data.picture_target[i][j];
  inStream.close();

// Print out the pattern to be sure we got the right one.

  cout << "input pattern:\n";
  for(j=0; j<data.height; j++){
    for(i=0; i<data.width; i++)
      cout << (data.picture_target[i][j] == 1 ? '*' : ' ') << " ";
    cout << "\n";
  }
  cout << "\n"; cout.flush();

// Generate the random sequence of numbers.

  int n=7;
  float min[] = {0, 0,   3, -5, 100,    0.001, 0};
  float max[] = {1, 100, 3, -2, 100000, 0.010, 7};
  GARandomSeed(seed);
  data.numbers_target = new float[n];
  for(i=0; i<n; i++)
    data.numbers_target[i] = GARandomFloat(min[i], max[i]);
  data.numbers_target[6] = GARandomInt((int)min[6], (int)max[6]);

// Print out the sequence so we'll know what we have to match.

  cout << "input sequence:\n";
  for(i=0; i<n; i++){
    cout.width(10);
    cout << data.numbers_target[i] << " ";
  }
  cout << "\n"; cout.flush();


// Create a phenotype for the numbers to map them to a bin2dec genome.

  GABin2DecPhenotype map;
  for(i=0; i<n; i++)
    map.add(8, min[i], max[i]);

// Create a couple of genomes for keeping track in our user data.
  data.picture_genome = new GA2DBinaryStringGenome(data.width, data.height,
						   PictureObjective, 
						   (void *)&data);
  data.numbers_genome = new GABin2DecGenome(map,
					    NumbersObjective, 
					    (void *)&data);

// Now create the GA and run it.

  GA2DBinaryStringGenome picture_genome(*(data.picture_genome));
  GABin2DecGenome numbers_genome(*(data.numbers_genome));
  GAIncrementalGA ga(picture_genome);
  ga.parameters(params);
  ga.evolve();

// Now that we have evolved the best solution, put the best into our genomes
// then print them out.

  picture_genome = ga.statistics().bestIndividual();
  cout << "the ga generated:\n";
  for(j=0; j<data.height; j++){
    for(i=0; i<data.width; i++)
      cout << (picture_genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    cout << "\n";
  }
  cout << "\n"; cout.flush();

  numbers_genome = *data.numbers_genome;
  for(unsigned int jj=0; jj<map.nPhenotypes(); jj++){
    cout.width(10);
    cout << numbers_genome.phenotype(jj) << " ";
  }
  cout << "\n\n"; cout.flush();

  cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";


// free up all of the space we were using.
  
  for(i=0; i<data.width; i++)
    delete data.picture_target[i];
  delete [] data.picture_target;
  delete [] data.numbers_target;

  delete data.picture_genome;
  //  delete data.numbers_genome;
  
  return 0;
}
 




// This is the primary objective function.  If it gets a genome whose score
// is sufficiently high, then it runs another GA on the number sequence and
// returns a composite score.
float
PictureObjective(GAGenome & c)
{
  GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)c;
  UserData * data = (UserData *)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; j<genome.height(); j++)
      value += (float)(genome.gene(i,j) == data->picture_target[i][j]);

  float correct = value / ((float)genome.width() * (float)genome.height());

  // if we get at least 95% of the pixels right, then run the secondary ga.

  if(correct > 0.95) {
    GABin2DecGenome& num_genome = (GABin2DecGenome&)(*data->numbers_genome);
    GAIncrementalGA ga(num_genome);
    ga.populationSize(550);
    ga.nGenerations(50);
    ga.pMutation(0.01);
    ga.pCrossover(0.9);
    ga.evolve();
    *data->numbers_genome = ga.statistics().bestIndividual();

    correct += data->numbers_genome->score();
  }

  return correct;
}


// This is the objective function for matching the sequence of numbers.
float
NumbersObjective(GAGenome & c)
{
  GABin2DecGenome & genome = (GABin2DecGenome &)c;
  UserData * data = (UserData *)c.userData();

  float value=genome.nPhenotypes();
  for(int i=0; i<genome.nPhenotypes(); i++)
    value += 1.0 / (1.0 + fabs(genome.phenotype(i) - data->numbers_target[i]));

  value /= genome.nPhenotypes();

  return value;
}

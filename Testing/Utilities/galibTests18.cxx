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
REGISTER_TEST(galibTests18);
}
/* ----------------------------------------------------------------------------
  ex18.C
  mbwall 28jul94
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the SimpleGA class and 2DBinaryStringGenome class.
This program reads in a 2D pattern from a data file then tries to match the
pattern in a 2D binary string genome.  The type of GA can be specified at
the command line.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define ifstream STD_IFSTREAM

float objective(GAGenome &);
int cntr=0;

int
galibTests18(int argc, char *argv[])
{
  cout << "Example 18\n\n";
  cout << "This program is designed to compare the GA types.  You can\n";
  cout << "specify steady-state, incremental, or simple GA and tweak any\n";
  cout << "of the parameters for each of these GA types.  The objective\n";
  cout << "function tries to match a pattern read in from a file.\n\n";
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

// Set the default values of the parameters and declare the params variable.

  GAParameterList params;
  GASimpleGA::registerDefaultParameters(params);
  GASteadyStateGA::registerDefaultParameters(params);
  GAIncrementalGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);	// population size
  params.set(gaNpCrossover, 0.9);	// probability of crossover
  params.set(gaNpMutation, 0.001);	// probability of mutation
  params.set(gaNnGenerations, 400);	// number of generations
  params.set(gaNpReplacement, 0.25);	// how much of pop to replace each gen
  params.set(gaNscoreFrequency, 10);	// how often to record scores
  params.set(gaNflushFrequency, 50);	// how often to dump scores to file
  params.set(gaNscoreFilename, "bog.dat");
  params.parse(argc, argv, gaFalse);    // parse command line for GAlib args

  const int SIMPLE=0, STEADY_STATE=1, INCREMENTAL=2;
  int whichGA = SIMPLE;
  int i,j;
  char filename[2048] = "smiley.txt";

// Parse the command line for more arguments.

  for(i=1; i<argc; i++){
    if(strcmp("ga", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": which GA do you want? (simple, ss, or inc)\n";
        exit(1);
      }
      else{
	if(strcmp(argv[i],"simple") == 0)
	  whichGA = SIMPLE;
	else if(strcmp(argv[i],"ss") == 0)
	  whichGA = STEADY_STATE;
	else if(strcmp(argv[i],"inc") == 0)
	  whichGA = INCREMENTAL;
	else
	  cerr << argv[0] << ": ga must be one of: simple, ss, or inc.\n";
        continue;
      }
    }
    else if(strcmp("file", argv[i]) == 0 || strcmp("f", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": the file option needs a filename.\n";
        exit(1);
      }
      else{
	sprintf(filename, argv[i]);
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
      cerr << "  f\tfilename from which to read (" << filename << ")\n";
      cerr << "\n";
      cerr << "  ga simple|ss|inc\twhich GA to use (simple)\n";
      cerr << "\n";
      exit(1);
    }
  }

// Read in the pattern from the specified file.  File format is pretty simple:
// two integers that give the height then width of the matrix, then the matrix
// of 1's and 0's (with whitespace inbetween).

  ifstream inStream(filename);
  if(!inStream){
    cerr << "Cannot open " << filename << " for input.\n";
    exit(1);
  }

  int height, width;
  inStream >> height >> width;

  short **target = new short*[width];
  for(i=0; i<width; i++)
    target[i] = new short[height];

  for(j=0; j<height; j++)
    for(i=0; i<width; i++)
      inStream >> target[i][j];

  inStream.close();

// Print out the pattern to be sure we got the right one.

  cout << "input pattern:\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      cout << (target[i][j] == 1 ? '*' : ' ') << " ";
    cout << "\n";
  }
  cout << "\n"; cout.flush();

// Now create the GA and run it.

  GA2DBinaryStringGenome genome(width, height, objective, (void *)target);
  GAStatistics stats;

  switch(whichGA){
  case STEADY_STATE:
    {
      GASteadyStateGA ga(genome);
      ga.parameters(params);
      ga.evolve(seed);
      genome = ga.statistics().bestIndividual();
      stats = ga.statistics();
    }
    break;
  case INCREMENTAL:
    {
      GAIncrementalGA ga(genome);
      ga.parameters(params);
      ga.evolve(seed);
      genome = ga.statistics().bestIndividual();
      stats = ga.statistics();
    }
    break;
  case SIMPLE:
  default:
    {
      GASimpleGA ga(genome);
      ga.parameters(params);
      ga.evolve(seed);
      genome = ga.statistics().bestIndividual();
      stats = ga.statistics();
    }
    break;
  }

  cout << "the ga generated:\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++){
      cout << (genome.gene(i,j) == 1 ? '*' : ' ') << " ";
    }
    cout << "\n";
  }

  char name[2048];
  params.get(gaNscoreFilename,name);
  cout << "\nthe statistics for the run are:\n" << stats;
  cout << "\nthe objective function was called " << cntr << " times\n";
  cout << "\nbest of generation data are in '"<<name<<"'\n";
  cout.flush();

  for(i=0; i<width; i++)
    delete target[i];
  delete [] target;

  return 0;
}



float
objective(GAGenome & c)
{
  GA2DBinaryStringGenome & genome = (GA2DBinaryStringGenome &)c;
  short **pattern = (short **)c.userData();

  float value=0.0;
  for(int i=0; i<genome.width(); i++)
    for(int j=0; j<genome.height(); j++)
      value += (float)(genome.gene(i,j) == pattern[i][j]);

  cntr++;

  return(value);
}

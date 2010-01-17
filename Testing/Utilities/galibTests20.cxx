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
REGISTER_TEST(galibTests20);
}

/* ----------------------------------------------------------------------------
  ex20.C
  mbwall 5sep95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example runs the royal road problem.  See the comments near the
objective functions for details about the function itself.
   Some of this was copied (at least partially) from the galopps genetic
algorithm library and from the pga package.  I used a bunch of globals in this
example - not good programming style, but it gets the job done.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT

// This is the objective function for computing Holland's 1993 ICGA version
// of the Royal Road problem.  It has been corrected per GAList volume 7
// number 23, 8/26/93.  No bonus points are awarded for a given level until
// it has been achieved (this fixes Holland's coding error in GAList).
//   Holland posed this problem as a challenge to test the
// performance of genetic algorithms.  He indicated that, with the parameter
// settings of
//
//     schemata size = 8
//     bits between schemata = 7
//     m* = 4
//     U* = 1.0
//     u = 0.3
//     v = 0.02
//
// he could attain royal_road_level 3 most of the time within
// 10,000 function evaluations.  He challenged other GA users to match or beat
// that performance.  He indicated that he used a population size of 512 to
// obtain his solutions, and did NOT use a "simple genetic algorithm."
//   The genome for this problem is a single-dimension bit string with length
// defined by the block size and gap size as:
//
//     length = (blocksize+gapsize) * (2^K)
//
// where K= 1,2,3, or 4.  Holland used K = 4.

#define NBLOCKS 16		// this number is 2^K

const int BLOCKSIZE=8;	// block size - length of target schemata
const int GAPSIZE=7;	// gap size - number of bits between target schemata
const int MSTAR=4;	// Holland's m* - up to this many bits in low level
			// block gets reward
const float USTAR=1.0;     // Holland's U* - first block earns this
const float RR_U=0.3;      // Holland's u - increment for lowest level match
const float RR_V=0.02;     // Holland's v - reward/penalty per bit

int nbits = (BLOCKSIZE+GAPSIZE)*NBLOCKS;
int blockarray[NBLOCKS];
int highestLevel=0;

float
RoyalRoad(GAGenome & c){
  GA1DBinaryStringGenome & genome = (GA1DBinaryStringGenome &)c;

  float score = 0.0;
  int total, i, j, index, n;

// do the lowest level blocks first

  n = 0;
  for(i=0; i<NBLOCKS; i++) {
    total = 0;
    for(j=i*(BLOCKSIZE + GAPSIZE); j<i*(BLOCKSIZE+GAPSIZE)+BLOCKSIZE; j++)
      if(genome.gene(j) == 1) total++;  // count the bits in the block
    if(total > MSTAR && total < BLOCKSIZE)
      score -= (total-MSTAR)*RR_V;
    else if(total <= MSTAR)
      score += total * RR_V;
    if(total == BLOCKSIZE) {
      blockarray[i] = 1;
      n++;
    }
    else{
      blockarray[i] = 0;
    }
  }

// bonus for filled low-level blocks

  if(n > 0) score += USTAR + (n-1)*RR_U;

// now do the higher-level blocks

  n = NBLOCKS;		// n is now number of filled low level blocks
  int proceed = 1;		// should we look at the next higher level?
  int level = 0;
  while ((n > 1) && proceed) {
    proceed = 0;
    total = 0;
      /* there are n valid blocks in the blockarray each time */
      /* round, so n=2 is the last.                           */
    for(i=0,index=0; i<(n/2)*2; i+=2,index++) {
      if(blockarray[i] == 1 && blockarray[i+1] == 1) {
	total++;
	proceed = 1;
	blockarray[index] = 1;
      }
      else{
	blockarray[index] = 0;
      }
    }
    if(total > 0){
      score += USTAR + (total-1)*RR_U;
      level++;
    }
    n /= 2;
  }

  if(highestLevel < level) highestLevel = level;

  return(score);
}











// The rest of this is standard for the GAlib examples.
int
galibTests20(int argc, char *argv[])
{
  cout << "Example 20\n\n";
  cout << "Running Holland's Royal Road test problem with a genome that is\n";
  cout << nbits << " bits long (" << NBLOCKS << " blocks).  The parameters ";
  cout << "are as follows: \n\n";
  cout << "\tblock size: " << BLOCKSIZE << "\n";
  cout << "\t  gap size: " << GAPSIZE << "\n";
  cout << "\t        m*: " << MSTAR << "\n";
  cout << "\t        u*: " << USTAR << "\n";
  cout << "\t         u: " << RR_U << "\n";
  cout << "\t         v: " << RR_V << "\n";
  cout << "\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      seed = atoi(argv[ii+1]);
    }
  }

  GA1DBinaryStringGenome genome(nbits, RoyalRoad);
  GASteadyStateGA ga(genome);
  ga.populationSize(512);
  ga.pCrossover(0.9);
  ga.pMutation(0.001);
  ga.nGenerations(10000);
  ga.scoreFilename("bog.dat");
  ga.flushFrequency(100);
  ga.scoreFrequency(20);
  ga.parameters(argc, argv);
  GASigmaTruncationScaling trunc;
  ga.scaling(trunc);
  ga.evolve(seed);

  cout << "the ga generated:\n" << ga.statistics().bestIndividual() << "\n";
  cout << "the highest level achieved was " << highestLevel << "\n";
  cout << "\nthe statistics for the run are:\n" << ga.statistics();
  cout << "\nthe parameters for the run are:\n" << ga.parameters();
  cout.flush();

  return 0;
}

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
REGISTER_TEST(galibTests8);
}

/* ----------------------------------------------------------------------------
  ex8.C
  mbwall 14jan95
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the list genome.  This example contains
the code to run a genetic algorithm with a list genome.
  This program illustrates how to specialize member functions of the
template classes.  Here we specialize the default write() method so that we get
the contents of the nodes rather than the pointers to the node contents.  You
can specialize most functions of a template class (as long as they are not
inlined).
  The objective function for this example returns both positive and negative
values, depending on the genome it is evaluting.  So the example also shows how
to use the sigma truncation scaling method to handle the mixed scores.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define ostream STD_OSTREAM


// Objective function and initializer declarations.
float objective(GAGenome &);
void ListInitializer(GAGenome &);


int
galibTests8(int argc, char *argv[])
{
  cout << "Example 8\n\n";
  cout << "This program runs a steady-state GA whose objective function\n";
  cout << "tries to maximize the size of the list and tries to make lists\n";
  cout << "that contain the number 101 in the nodes.  The lists contain\n";
  cout << "ints in the nodes.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  for(int i=1; i<argc; i++) {
    if(strcmp(argv[i++],"seed") == 0)
      GARandomSeed((unsigned int)atoi(argv[i]));
  }

// Create the initial genome for the genetic algorithm to use.  Set the
// initializer and mutation before we make the genetic algorithm.

  GAListGenome<int> genome(objective);
  genome.initializer(ListInitializer);
//  genome.mutator(GAListGenome<int>::SwapMutator);
  genome.mutator(GAListGenome<int>::DestructiveMutator);

// Now that we have a genome, we use it to create our GA.  After creating the
// GA we set the parameters and tell the GA to use sigma truncation scaling
// rather than the default (linear scaling).  Set the crossover to single
// point crossover.  The genetic algorithm handles crossover since genomes
// don't know about other genomes.  We could set the crossover on the genome
// if we wanted - either way will work.

  GASteadyStateGA ga(genome);
  GASigmaTruncationScaling scale;
  ga.scaling(scale);
  ga.crossover(GAListGenome<int>::OnePointCrossover);

// Set the default parameters we want to use, then check the command line for
// other arguments that might modify these.

  ga.set(gaNpopulationSize, 40);	// population size
  ga.set(gaNpCrossover, 0.6);		// probability of crossover
  ga.set(gaNpMutation, 0.05);		// probability of mutation
  ga.set(gaNnGenerations, 50);		// number of generations
  ga.set(gaNscoreFrequency, 1);		// how often to record scores
  ga.set(gaNflushFrequency, 10);	// how often to dump scores to file
  ga.set(gaNselectScores,		// which scores should we track?
	 GAStatistics::Maximum|GAStatistics::Minimum|GAStatistics::Mean);
  ga.set(gaNscoreFilename, "bog.dat");
  ga.parameters(argc, argv);

// Evolve the genetic algorithm then dump out the results of the run.

  ga.evolve();

  genome = ga.statistics().bestIndividual();
//  cout << "the ga generated the list:\n" << genome << "\n";
  cout << "the list contains " << genome.size() << " nodes\n";
  cout << "the ga used the parameters:\n" << ga.parameters() << "\n";

  return 0;
}



/* ----------------------------------------------------------------------------
Objective function
  There is no limit to the size of a list (only the memory you have on your
computer).  This objective function tries to build a list that contains the
number 101 in all of its nodes.  If we don't put some control on this objective
then the list will grow without bound.  So we dampen it a bit with a penalty
for large size.  However, this will make the score go negative, so we must use
a scaling object that allows negative objective scores.
  We could get lists with no contents, so we have to check for that.  Just make
sure that head has contents.  If it does, then the rest of the genome will.
  When we access the node contents we have to dereference the member functions.
For example, we do *chrom.head() not chrom.head().  This is because the member
functions return a pointer to the node's contents, not the actual contents.
---------------------------------------------------------------------------- */
float
objective(GAGenome & c)
{
  GAListGenome<int> & genome = (GAListGenome<int> &)c;
  int count=0;
  if(!genome.head()) return 0;
  count = (*genome.head() == 101) ? 1 : 0; // move to head of the list
  for(int i=1; i<genome.size(); i++)
    count += (*genome.next() == 101) ? 1 : 0; // check each element of the list
  return 5*count - genome.size();
}



/* ----------------------------------------------------------------------------
Here is the initializer for our genomes.  It builds a list of n items of type
int.  Notice that we first destroy any list that is already in the genome
before we do our initialization.  This is so that the genomes can be re-used.
When you re-run a GA, it does not destroy the individuals in the population -
it reuses them.  Thus, the initializer must make sure that the genome is
cleaned up before it tries to initialize it.
---------------------------------------------------------------------------- */
void
ListInitializer(GAGenome & c)
{
  GAListGenome<int> &child=(GAListGenome<int> &)c;

  while(child.head()) child.destroy(); // destroy any pre-existing list

  int n=75;
  child.insert(100,GAListBASE::HEAD);
  for(int i=0; i<n; i++)
    child.insert(i+100);
  for(int j=0; j<n; j++)
    child.swap(GARandomInt(0,n-1), GARandomInt(0,n-1));
}





//   Here we specialize the write method for the List class.  This lets us see
// exactly what we want (the default write method dumps out pointers to the
// data rather than the data contents).
//   This routine prints out the contents of each element of the list,
// separated by a space.  It does not put a newline at the end of the list.
//   Notice that you can specialize ANY function of a template class, but
// some compilers are more finicky about how you do it than others.  For the
// metrowerks compiler this specialization must come before the forced
// instantiation.
template<> int
GAListGenome<int>::write(ostream & os) const
{
  int *cur, *head;
  GAListIter<int> tmpiter(*this);
  if((head=tmpiter.head()) != NULL) os << *head << " ";
  for(cur=tmpiter.next(); cur && cur != head; cur=tmpiter.next())
    os << *cur << " ";
  return os.fail() ? 1 : 0;
}





// force instantiations for compilers that do not do auto instantiation
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GAList.C>
#include <ga/GAListGenome.C>
GALIB_INSTANTIATION_PREFIX GAList<int>;
GALIB_INSTANTIATION_PREFIX GAListGenome<int>;
#endif

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
REGISTER_TEST(galibTests11);
}

/* ----------------------------------------------------------------------------
  ex11.C
  mbwall 13apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   This example shows how to use an order-based list genome.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define ostream STD_OSTREAM

// The objective function tells how good a genome is.  The Initializer defines
// how the lists should be initialized.
float objective(GAGenome &);
void ListInitializer(GAGenome &);


int
galibTests11(int argc, char *argv[])
{
  cout << "Example 11\n\n";
  cout << "This program illustrates the use of order-based lists.  The\n";
  cout << "list in this problem contains 25 numbers, 0 to 24.  It tries\n";
  cout << "to put them in descending order from 24 to 0.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      GARandomSeed((unsigned int)atoi(argv[ii+1]));
    }
  }

// Set the default values of the parameters.
  const char * outputfilename = argv[1];

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);	// population size
  params.set(gaNpCrossover, 0.6);	// probability of crossover
  params.set(gaNpMutation, 0.01);	// probability of mutation
  params.set(gaNnGenerations, 1000);	// number of generations
  params.set(gaNpReplacement, 0.5);	// how much of pop to replace each gen
  params.set(gaNscoreFrequency, 10);	// how often to record scores
  params.set(gaNnReplacement, 4);	// how much of pop to replace each gen
  params.set(gaNflushFrequency, 10);	// how often to dump scores to file
  params.set(gaNscoreFilename, outputfilename);//"bog.dat");
//  params.read("settings.txt");	        // grab values from file first
  params.parse(argc, argv, gaFalse); // parse command line for GAlib args

// Now create the GA and run it.  We first create a genome with the
// operators we want.  Since we're using a template genome, we must assign
// all three operators.  We use the order-based crossover site when we assign
// the crossover operator.

  GAListGenome<int> genome(objective);
  genome.initializer(ListInitializer);
  genome.mutator(GAListGenome<int>::SwapMutator);

// Now that we have our genome, we create the GA (it clones the genome to 
// make all of the individuals for its populations).  Set the parameters on 
// the GA then let it evolve.

  GASteadyStateGA ga(genome);
  ga.crossover(GAListGenome<int>::PartialMatchCrossover);
  ga.parameters(params);
  ga.evolve();

// Assign the best that the GA found to our genome then print out the results.

  genome = ga.statistics().bestIndividual();
  cout << "the ga generated the following list (objective score is ";
  cout << genome.score() << "):\n" << genome << "\n";
  cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";
  cout << ga.parameters() << "\n";

//  char *fn;
//  ga.get(gaNscoreFilename, &fn);
//  cout << "filename is '" << fn << "'\n";

  return 0;
}
 


/* ----------------------------------------------------------------------------
Objective function
  The objective function gives one point for every number in the correct
position.  We're trying to get a sequence of numbers from n to 0 in descending
order.
---------------------------------------------------------------------------- */
float
objective(GAGenome & c)
{
  GAListGenome<int> & genome = (GAListGenome<int> &)c;

  float score = (*genome.head() == genome.size()-1); // move to head of list
  for(int i=genome.size()-2; i>0; i--)
    score += (*genome.next() == i); // cycle through list and look at nodes
  return score;
}



/* ----------------------------------------------------------------------------
List Genome Operators
-------------------------------------------------------------------------------
The initializer creates a list with n elements in it and puts a unique digit
in each one.  After we make the list, we scramble everything up.
---------------------------------------------------------------------------- */
void
ListInitializer(GAGenome & c)
{
  GAListGenome<int> &child=(GAListGenome<int> &)c;
  while(child.head()) child.destroy(); // destroy any pre-existing list

  int n=25;
  child.insert(0,GAListBASE::HEAD); // the head node contains a '0'
  for(int i=1; i<n; i++)
    child.insert(i);		// each subsequent node contains a number
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
  if((head=tmpiter.head()) != 0) os << *head << " ";
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

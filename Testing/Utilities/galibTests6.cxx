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
REGISTER_TEST(galibTests6);
}

/* ----------------------------------------------------------------------------
  ex6.C
  mbwall 14jan95
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for the tree and list genomes.  This example contains
the code to run a tree genome.  The list genome is almost the same - 
just change tree to list and modify the initialization methods.
  This program illustrates how to specialize member functions of the 
template classes.  Here we specialize the default write() method so that we get
the contents of the nodes rather than the pointers to the node contents.  You
can specialize most functions of a template class (as long as they are not
inlined).
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define ostream STD_OSTREAM


// The objective function is declared here and defined below.
float objective(GAGenome &);

// This is the declaration for the initialization operator for our trees.
void TreeInitializer(GAGenome &);

// This is a recursive function that will be used in the 'write' method for 
// our tree genomes.
void WriteNode(ostream & os, GANode<int> * n);


int
galibTests6(int argc, char *argv[])
{
  cout << "Example 6\n\n";
  cout << "This example uses a SteadyState GA and Tree<int> genome.  It\n";
  cout << "tries to maximize the size of the tree genomes that it\n";
  cout << "contains.  The genomes contain ints in its nodes.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  unsigned int seed = 0;
  for(int i=1; i<argc; i++) {
    if(strcmp(argv[i++],"seed") == 0) {
      seed = atoi(argv[i]);
    }
  }

// Set the default values of the parameters.

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpopulationSize, 30);
  params.set(gaNpCrossover, 0.7);
  params.set(gaNpMutation, 0.01);
  params.set(gaNnGenerations, 100);
  params.set(gaNscoreFilename, "bog.dat");
  params.set(gaNscoreFrequency, 10); // record score every 10th generation
  params.set(gaNflushFrequency, 10); // dump scores every 10th recorded score
  params.parse(argc, argv, gaFalse); // Parse the command line for GAlib args.

// Now create the GA and run it.  We first create a chromsome with the
// operators we want.  Once we have the genome set up, create the genetic 
// algorithm, set the parameters, and let it go.

  GATreeGenome<int> genome(objective);
  genome.initializer(TreeInitializer);
  genome.mutator(GATreeGenome<int>::SwapSubtreeMutator);

  GASteadyStateGA ga(genome);
  ga.parameters(params);
  ga.evolve(seed);

  genome = ga.statistics().bestIndividual();
//  cout << "the ga generated this tree:\n" << genome << "\n";
  cout << genome.size() << " nodes, " << genome.depth() << " levels deep.\n";
  cout << "best of generation data are in '" << ga.scoreFilename() << "'\n";

  return 0;
}
 


/* ----------------------------------------------------------------------------
Objective function
  All we do in this objective function is try to maximize the size of the tree.
Just return the tree size.  This means that if you run this objective function
for many generations you'll run out of memory!  There is no limit to tree or
list sizes built-in to the GA library.
---------------------------------------------------------------------------- */
float
objective(GAGenome & c)
{
  GATreeGenome<int> & genome = (GATreeGenome<int> &)c;
  return genome.size();
}



/* ----------------------------------------------------------------------------
Here is the initializer for our genomes.  It builds a tree of n items of type
int.  Notice that we first destroy any tree that is already in the genome 
before we do our initialization.  This is so that the genomes can be re-used.
When you re-run a GA, it does not destroy the individuals in the population - 
it reuses them.  Thus, the initializer must make sure that the genome is 
cleaned up before it tries to initialize it.
---------------------------------------------------------------------------- */
void
TreeInitializer(GAGenome & c)
{
  GATreeGenome<int> &child=(GATreeGenome<int> &)c;

// destroy any pre-existing tree
  child.root();
  child.destroy();

// Create a new tree with depth of 'depth' and each eldest node containing
// 'n' children (the other siblings have none).
  int depth=2, n=3, count=0;
  child.insert(count++,GATreeBASE::ROOT);

  for(int i=0; i<depth; i++){
    child.eldest();
    child.insert(count++);
    for(int j=0; j<n; j++)
      child.insert(count++,GATreeBASE::AFTER);
  }
}






/* ----------------------------------------------------------------------------
Tree genome method overrides
-------------------------------------------------------------------------------
  Here we override the built-in methods for the tree class.  We can do this
because the tree class is template-ized - when the compiler looks for an
instance of the tree class methods, it finds these so it won't generate an
instance from the templates.  You can do this with ANY method of a template
class.  Here we do it only for the write method.
  The default write operator prints out pointers to the contents of each node.
Here we print out the actual contents of each node.  This assumes that the 
object in our node has the operator<< defined for it.
---------------------------------------------------------------------------- */
void 
WriteNode(ostream & os, GANode<int> * n)
{
  if(!n) return;
  GANodeBASE * node = (GANodeBASE *)n;

  os.width(10);
  os << ((GANode<int> *)node)->contents << " ";
  os.width(10); 
  if(node->parent) os << ((GANode<int> *)node->parent)->contents << " ";
  else os << "." << " ";
  os.width(10); 
  if(node->child) os << ((GANode<int> *)node->child)->contents << " ";
  else os << "." << " ";
  os.width(10);
  if(node->next) os << ((GANode<int> *)node->next)->contents << " ";
  else os << "." << " ";
  os.width(10);
  if(node->prev) os << ((GANode<int> *)node->prev)->contents << "\n";
  else os << ".\n";
  WriteNode(os, (GANode<int> *)node->child);

  for(GANodeBASE * tmp=node->next; tmp && tmp != node; tmp=tmp->next){
    os.width(10);
    os << ((GANode<int> *)tmp)->contents << " ";
    os.width(10);
    if(tmp->parent) os << ((GANode<int> *)tmp->parent)->contents << " ";
    else os << "." << " ";
    os.width(10); 
    if(tmp->child) os << ((GANode<int> *)tmp->child)->contents << " ";
    else os << "." << " ";
    os.width(10); 
    if(tmp->next) os << ((GANode<int> *)tmp->next)->contents << " ";
    else os << "." << " ";
    os.width(10); 
    if(tmp->prev) os << ((GANode<int> *)tmp->prev)->contents << "\n";
    else os << ".\n";
    WriteNode(os, (GANode<int> *)tmp->child);
  }
}

template<> int
GATreeGenome<int>::write(ostream & os) const
{
  os << "      node     parent      child       next       prev\n";
  WriteNode(os, (GANode<int> *)rt);
  return os.fail() ? 1 : 0;
}






// force instantiations for compilers that do not do auto instantiation
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GATree.C>
#include <ga/GATreeGenome.C>
GALIB_INSTANTIATION_PREFIX GATreeGenome<int>;
GALIB_INSTANTIATION_PREFIX GATree<int>;
#endif

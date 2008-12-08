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
REGISTER_TEST(galibTests16);
}
/* ----------------------------------------------------------------------------
  ex16.C
  mbwall 5may95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Illustration of how to use a non-trivial object in the nodes of a tree
genome.  This example uses points in the nodes.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define ostream STD_OSTREAM


// This is the object that we're going to put in the nodes.  Each point has
// three coordinates: x,y,z.
class Point {
public:
  Point(float x, float y, float z) { _x = x; _y = y; _z = z; }
  Point(const Point & p) { _x = p._x; _y = p._y; _z = p._z; }
  Point & operator=(const Point & p) { _x=p._x;_y=p._y;_z=p._z; return *this; }
  ~Point() {}

  float x() const { return _x; }
  float y() const { return _y; }
  float z() const { return _z; }
  float x(float val) { return _x=val; }
  float y(float val) { return _y=val; }
  float z(float val) { return _z=val; }

  friend ostream & operator<<(ostream & os, const Point & p){
    os << "(" << p._x << ", " << p._y << ", " << p._z << ")";
    return os;
  }

protected:
  float _x, _y, _z;
};




// These are the declarations for the functions defined in this file.  The
// objective function is pretty standard.  The tree initializer generates a
// random tree.  WriteNode is used in the write method for the tree - we
// override (specialize) the write method to print out the contents of the
// nodes rather than pointers to the contents.
float objective(GAGenome &);
void TreeInitializer(GAGenome &);
void WriteNode(ostream & os, GANode<Point> * n);


int
galibTests16(int argc, char *argv[])
{
  cout << "Example 16\n\n";
  cout << "This example uses a SteadyState GA and Tree<int> genome.  It\n";
  cout << "tries to maximize the size of the tree genomes that it\n";
  cout << "contains.  The genomes contain points in its nodes.  Two\n";
  cout << "different runs are made:  first with the swap subtree mutator,\n";
  cout << "second with the destructive mutator.\n\n";
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

  GATreeGenome<Point> genome(objective);
  genome.initializer(TreeInitializer);
  genome.crossover(GATreeGenome<Point>::OnePointCrossover);

  genome.mutator(GATreeGenome<Point>::SwapSubtreeMutator);
  GAPopulation swappop(genome, 50);

  genome.mutator(GATreeGenome<Point>::DestructiveMutator);
  GAPopulation destpop(genome, 50);

  GASteadyStateGA ga(genome);
  ga.nGenerations(10);

// first do evolution with subtree swap mutator.

  ga.population(swappop);

  cout << "initializing...";
  ga.initialize(seed);
  cout << "evolving for " << ga.nGenerations() << " generations...";
  while(!ga.done()){
    ga.step();
    cout << ".";
    cout.flush();
  }
  cout << "\n";

  genome = ga.statistics().bestIndividual();
  cout << "the ga generated a tree with " << genome.size();
  cout << " nodes, " << genome.depth() << " levels deep.\n";

// now do evolution with destructive swap mutator

  ga.population(destpop);

  cout << "\ninitializing...";
  ga.initialize();
  cout << "evolving for " << ga.nGenerations() << " generations...";
  while(!ga.done()){
    ga.step();
    cout << ".";
    cout.flush();
  }
  cout << "\n";

  genome = ga.statistics().bestIndividual();
  cout << "the ga generated a tree with " << genome.size();
  cout << " nodes, " << genome.depth() << " levels deep.\n";

  return 0;
}



/* ----------------------------------------------------------------------------
  All we do in this objective function is try to maximize the size of the tree.
Just return the tree size.  This means that if you run this objective function
for many generations you'll run out of memory!  There is no limit to tree or
list sizes built-in to the GA library.
---------------------------------------------------------------------------- */
float
objective(GAGenome & c) {
  GATreeGenome<Point> & chrom = (GATreeGenome<Point> &)c;
  return chrom.size();
}



/* ----------------------------------------------------------------------------
  This initializer creates a tree of random size (within limits).  The maximum
number of children any node can have is limited, so is the maximum depth of
the tree.  We do it recursively.  Each point that is inserted into the tree
has random contents.
  The initializer must first destroy any pre-existing tree or else we have a
memory leak (the initializer may be called more than once - for example when
you re-run the GA).
---------------------------------------------------------------------------- */
const int MAX_DEPTH = 3;
const int MAX_CHILDREN = 2;

void
DoChild(GATreeGenome<Point> & tree, int depth) {
  if(depth >= MAX_DEPTH) return;
  int n = GARandomInt(0,MAX_CHILDREN);	// maximum of 5 children

  Point p(GARandomFloat(0,25),GARandomFloat(0,25),GARandomFloat(0,25));
  tree.insert(p,GATreeBASE::BELOW);

  for(int i=0; i<n; i++)
    DoChild(tree, depth+1);

  tree.parent();		// move the iterator up one level
}

void
TreeInitializer(GAGenome & c) {
  GATreeGenome<Point> &tree=(GATreeGenome<Point> &)c;

// destroy any pre-existing tree
  tree.root();
  tree.destroy();

// create a root node with coordinates 0,0,0, then do the rest.
  Point p(0,0,0);
  tree.insert(p,GATreeBASE::ROOT);
  int n = GARandomInt(0,MAX_CHILDREN);	// maximum of 5 children
  for(int i=0; i<n; i++)
    DoChild(tree, 0);
}





/* ----------------------------------------------------------------------------
   This is a specialization of the write method for the TreeGenome class.  The
default write method prints out pointers to the nodes.  Here we print the
contents of the nodes.
   This is a recursive implementation (yuk) but it gets the job done.  Beware
that it could crash your machine if your stack is limited and your trees get
very big.
---------------------------------------------------------------------------- */
void
WriteNode(ostream & os, GANode<Point> * n) {
  if(!n) return;
  GANodeBASE * node = (GANodeBASE *)n;

  os.width(10);
  os << ((GANode<Point> *)node)->contents << " ";
  os.width(10);
  if(node->parent) os << ((GANode<Point> *)node->parent)->contents << " ";
  else os << "." << " ";
  os.width(10);
  if(node->child) os << ((GANode<Point> *)node->child)->contents << " ";
  else os << "." << " ";
  os.width(10);
  if(node->next) os << ((GANode<Point> *)node->next)->contents << " ";
  else os << "." << " ";
  os.width(10);
  if(node->prev) os << ((GANode<Point> *)node->prev)->contents << "\n";
  else os << ".\n";
  WriteNode(os, (GANode<Point> *)node->child);

  for(GANodeBASE * tmp=node->next; tmp && tmp != node; tmp=tmp->next){
    os.width(10);
    os << ((GANode<Point> *)tmp)->contents << " ";
    os.width(10);
    if(tmp->parent) os << ((GANode<Point> *)tmp->parent)->contents << " ";
    else os << "." << " ";
    os.width(10);
    if(tmp->child) os << ((GANode<Point> *)tmp->child)->contents << " ";
    else os << "." << " ";
    os.width(10);
    if(tmp->next) os << ((GANode<Point> *)tmp->next)->contents << " ";
    else os << "." << " ";
    os.width(10);
    if(tmp->prev) os << ((GANode<Point> *)tmp->prev)->contents << "\n";
    else os << ".\n";
    WriteNode(os, (GANode<Point> *)tmp->child);
  }
}

template <> int
GATreeGenome<Point>::write(ostream & os) const {
  os << "      node     parent      child       next       prev\n";
  WriteNode(os, (GANode<Point> *)rt);
  return os.fail() ? 1 : 0;
}





// force instantiations for compilers that do not do auto instantiation
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GATree.C>
#include <ga/GATreeGenome.C>
GALIB_INSTANTIATION_PREFIX GATreeGenome<Point>;
GALIB_INSTANTIATION_PREFIX GATree<Point>;
#endif

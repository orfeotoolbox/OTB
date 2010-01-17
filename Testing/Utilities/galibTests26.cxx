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
REGISTER_TEST(galibTests26);
}

/* ----------------------------------------------------------------------------
  ex26.C
  mbwall 24mar96
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

  The code for this example is adapted from an original implementation
  by Thomas Grueninger (from Uni Stuttgart, visiting scholar at MIT)

 DESCRIPTION:
   GAs are lousy at solving the travelling salesperson problem.  But here is an
example of how to do it anyway.  In this case we use a steady-state genetic
algorithm and give you the compile-time choice of partial match or edge
recombination crossover.
   This one looks really good when you draw an entire population of individuals
and watch them all evolve in real time.  It becomes very obvious what the
genetic algorithm is doing and how well the speciation is working (or not).
   This implementation is by no means efficient, so please do not complain
about all of the silly little inefficient aspects of the implementation.  But
it does get the job done.
---------------------------------------------------------------------------- */
#include <math.h>
#include <ga/GASStateGA.h>
#include <ga/GAListGenome.h>
#include <ga/garandom.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define endl STD_ENDL
#define ostream STD_OSTREAM
#define ifstream STD_IFSTREAM

// Set this up for your favorite TSP.  The sample one is a contrived problem
// with the towns laid out in a grid (so it is easy to figure out what the
// shortest distance is, and there are many different paths with the same
// shortest path).  File format is that used by the TSPLIB problems.  You can
// grab more problems from
//
//
// Apologies for using fixed-length arrays.  But this is an example, not
// production code ;)
#define MAX_TOWNS 50
#define TSP_FILE "tsp_rect_20.txt"

float DISTANCE[MAX_TOWNS][MAX_TOWNS];
double x[MAX_TOWNS],y[MAX_TOWNS];
int ntowns = 0;

// You can use either edge recombination crossover or partial match crossover.
// Which one you select makes a HUGE difference in the performance of the
// genetic algorithm.  Only one of the two following lines should be commented.
//#define XOVER PMXover       // (Partial Match Crossover)
#define XOVER ERXover         // (Edge Recombination Crossover)


float Objective(GAGenome&);
int   Mutator(GAGenome&, float);
void  Initializer(GAGenome&);
float Comparator(const GAGenome&, const GAGenome&);
int   ERXover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
int   PMXover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
void  ERXOneChild(const GAGenome&, const GAGenome&, GAGenome*);


int
galibTests26(int argc, char* argv[]) {
  cout << "Example 26\n\n";
  cout << "The Travelling Salesman Problem (TSP) demo program.\n" << endl;

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  const char * inputfile = argv[1];
  unsigned int seed = 0;
  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      seed = atoi(argv[ii+1]);
    }
  }

  double dump;
  ifstream in(inputfile/*TSP_FILE*/);
  if(!in) {
    cerr << "could not read data file " << TSP_FILE << "\n";
    exit(1);
  }
  ntowns=0;
  do {
    in >> dump;
    in >> x[ntowns];
    in >> y[ntowns];
    ntowns++;
  } while(!in.eof() && ntowns < MAX_TOWNS);
  in.close();
  if(ntowns >= MAX_TOWNS) {
    cerr << "data file contains more towns than allowed for in the fixed\n";
    cerr << "arrays.  Recompile the program with larger arrays or try a\n";
    cerr << "smaller problem.\n";
    exit(1);
  }

  double dx,dy;
  for(int i=0;i<ntowns;i++) {
    for(int j=i; j<ntowns;j++) {
      dx=x[i]-x[j]; dy=y[i]-y[j];
      DISTANCE[j][i]=DISTANCE[i][j]=sqrt(dx*dx+dy*dy);
    }
  }

  GAListGenome<int> genome(Objective);
  genome.initializer(::Initializer);
  genome.mutator(::Mutator);
  genome.comparator(::Comparator);
  genome.crossover(XOVER);

  GASteadyStateGA ga(genome);
  ga.minimize();
  ga.pReplacement(1.0);
  ga.populationSize(100);
  ga.nGenerations(1000);
  ga.pMutation(0.1);
  ga.pCrossover(1.0);
  ga.selectScores(GAStatistics::AllScores);
  ga.parameters(argc, argv);
  cout << "initializing..."; cout.flush();
  ga.initialize(seed);
  cout << "evolving..."; cout.flush();
  while(!ga.done()) {
    ga.step();
    if(ga.generation() % 10 == 0) {
      cout << ga.generation() << " "; cout.flush();
    }
  }

  genome = ga.statistics().bestIndividual();
  cout << "the shortest path found is " << genome.score() << "\n";
  cout << "this is the distance from the sequence\n";
  cout << genome << "\n\n";
  cout << ga.statistics() << "\n";

  return 0;
}






// Here are the genome operators that we want to use for this problem.
// Thanks to Jan Kees IJspeert for isolating an order-of-evaluation problem
// in the previous implementation of this function.
float
Objective(GAGenome& g) {
  GAListGenome<int> & genome = (GAListGenome<int> &)g;
  float dist = 0;
  if(genome.head()) {
    for(int i=0; i<ntowns; i++) {
      int xx = *genome.current();
      int yy = *genome.next();
      dist += DISTANCE[xx][yy];
    }
  }
  return dist;
}

void
Initializer(GAGenome& g) {
  GAListGenome<int> &child=(GAListGenome<int> &)g;
  while(child.head()) child.destroy(); // destroy any pre-existing list

  int i,town;
  static int visit[MAX_TOWNS];

  memset(visit, 0, MAX_TOWNS*sizeof(int));
  town=GARandomInt(0,ntowns-1);
  visit[town]=1;
  child.insert(town,GAListBASE::HEAD); // the head node

  for( i=1; i<ntowns; i++) {
    do {
      town=GARandomInt(0,ntowns-1);
    } while (visit[town]);
    visit[town]=1;
    child.insert(town);
  }		// each subsequent node
}

int
Mutator(GAGenome& g, float pmut) {
  GAListGenome<int> &child=(GAListGenome<int> &)g;
  register int n, i;
  if ((GARandomFloat() >= pmut) || (pmut <= 0)) return 0;

  n = child.size();

  if (GARandomFloat()<0.5) {
    child.swap(GARandomInt(0,n-1),GARandomInt(0,n-1)); // swap only one time
  }
  else {
    int nNodes = GARandomInt(1,((int)(n/2-1)));       // displace nNodes
    child.warp(GARandomInt(0,n-1));                   // with or without
    GAList<int> TmpList;                              // inversion
    for(i=0;i<nNodes;i++) {
      int *iptr = child.remove();
      TmpList.insert(*iptr,GAListBASE::AFTER);
      delete iptr;
      child.next();
    }
    int invert;
    child.warp(GARandomInt(0,n-nNodes));
    invert = (GARandomFloat()<0.5) ? 0 : 1;
    if (invert) TmpList.head(); else TmpList.tail();

    for(i=0;i<nNodes;i++) {
      int *iptr = TmpList.remove();
      child.insert(*iptr,GAListBASE::AFTER);
      delete iptr;
      if (invert) TmpList.prev(); else TmpList.next();
    }
  }
  child.head();		// set iterator to root node

  return (1);
}

int
ERXover(const GAGenome& g1, const GAGenome& g2, GAGenome* c1, GAGenome* c2) {
  int nc=0;
  if(c1) { ERXOneChild(g1,g2,c1); nc+=1; }
  if(c2) { ERXOneChild(g1,g2,c2); nc+=1; }
  return nc;
}

void
ERXOneChild(const GAGenome& g1, const GAGenome& g2, GAGenome* c1) {
  GAListGenome<int> &mate1=(GAListGenome<int> &)g1;
  GAListGenome<int> &mate2=(GAListGenome<int> &)g2;
  GAListGenome<int> &sis=(GAListGenome<int> &)*c1;

  int i,j,k,t1,t2,town;

  static char CM[MAX_TOWNS][MAX_TOWNS],visit[MAX_TOWNS];
  memset(CM, 0, MAX_TOWNS*MAX_TOWNS*sizeof(char));
  memset(visit, 0, MAX_TOWNS*sizeof(char));

  while (sis.head()) sis.destroy();

  // create connection matrix
  mate1.head();
  for(j=0; j<ntowns; j++) {
    t1 = *mate1.current(); t2 = *mate1.next();
    CM[t1][t2]=1; CM[t2][t1]=1;
  }
  mate2.head();
  for(j=0; j<ntowns; j++) {
    t1 = *mate2.current(); t2 = *mate2.next();
    CM[t1][t2]=1; CM[t2][t1]=1;
  }

  // select 1st town randomly
  town=GARandomInt(0,ntowns-1);
  visit[town]=1; memset(CM[town], 0, MAX_TOWNS*sizeof(char));
  sis.insert(town); // the head node

  GAList<int> PossFollowList;
  GAList<int> FollowersList[5];
  while (PossFollowList.head()) PossFollowList.destroy();
  for(k=0; k<5; k++) {
    while (FollowersList[k].head()) FollowersList[k].destroy();
  }

  // select the following town with the minimal no of next folling towns
  int nPoss,nFollow;
  for(i=1; i<ntowns; i++) {
    nPoss = 0;
    for(j=0; j<ntowns; j++) {          // no of poss. following towns
      if (CM[j][town]) {
	nPoss += 1;
	PossFollowList.insert(j);}
    }
    // nPoss = 0;
    if (nPoss == 0) {
      do {town=GARandomInt(0,ntowns-1);} while (visit[town]); // no follower
      visit[town]=1; memset(CM[town], 0, MAX_TOWNS*sizeof(char));
      sis.insert(town);
    }
    else {
      PossFollowList.head();
      for(j=0; j<nPoss; j++) {
	nFollow = 0;
	town = (*PossFollowList.current());
	for(k=0; k<ntowns; k++) {
	  if (CM[k][town]) nFollow++;
	}
	FollowersList[nFollow].insert(town);
	PossFollowList.next();
      }
      k=0;
      while (FollowersList[k].size() == 0) k++;
      FollowersList[k].warp(GARandomInt(0,FollowersList[k].size()));
      town = (*FollowersList[k].current());
      visit[town]=1; memset(CM[town], 0, MAX_TOWNS*sizeof(char));
      sis.insert(town);
    }
    while (PossFollowList.head()) PossFollowList.destroy();
    for(k=0; k<5; k++) {
      while (FollowersList[k].head()) FollowersList[k].destroy();
    }
  }
  sis.head();         // set iterator to head of list
}

int
PMXover(const GAGenome& g1, const GAGenome& g2, GAGenome* c1, GAGenome* c2) {
  GAListGenome<int> &mom=(GAListGenome<int> &)g1;
  GAListGenome<int> &dad=(GAListGenome<int> &)g2;

  int a = GARandomInt(0, mom.size());
  int b = GARandomInt(0, dad.size());
  int h;
  if (b<a) { h=a; a=b; b=h; }

  int* index;
  int i,j,nc=0;

  if(c1) {
    GAListGenome<int> &sis=(GAListGenome<int> &)*c1;
    sis.GAList<int>::copy(mom);
    GAListIter<int> diter(dad);
    index = diter.warp(a);
    for(i=a; i<b; i++, index=diter.next()){
      if(*sis.head() == *index){
	sis.swap(i,0);
      }
      else{
	for(j=1; (j<sis.size()) && (*sis.next() != *index); j++);
	sis.swap(i,j);  // no op if j>size
      }
    }
    sis.head();         // set iterator to head of list
    nc += 1;
  }
  if(c2) {
    GAListGenome<int> &sis=(GAListGenome<int> &)*c2;
    sis.GAList<int>::copy(mom);
    GAListIter<int> diter(dad);
    index = diter.warp(a);
    for(i=a; i<b; i++, index=diter.next()){
      if(*sis.head() == *index){
	sis.swap(i,0);
      }
      else{
	for(j=1; (j<sis.size()) && (*sis.next() != *index); j++);
	sis.swap(i,j);  // no op if j>size
      }
    }
    sis.head();         // set iterator to head of list
    nc += 1;
  }

  return nc;
}

float
Comparator(const GAGenome& g1, const GAGenome& g2) {
  GAListGenome<int> &a = (GAListGenome<int> &)g1;
  GAListGenome<int> &b = (GAListGenome<int> &)g2;

  int i,j,t1,t2;
  float dist=ntowns;

  static char CM1[MAX_TOWNS][MAX_TOWNS],CM2[MAX_TOWNS][MAX_TOWNS];
  memset(CM1, 0, MAX_TOWNS*MAX_TOWNS*sizeof(char));
  memset(CM2, 0, MAX_TOWNS*MAX_TOWNS*sizeof(char));

  // create connection matrix CM1
  a.head();
  for(i=0; i<ntowns; i++) {
    t1 = *a.current(); t2 = *a.next();
    CM1[t1][t2]=1; CM1[t2][t1]=1;
  }
  // create connection matrix CM2
  b.head();
  for(i=0; i<ntowns; i++) {
    t1 = *b.current(); t2 = *b.next();
    CM2[t1][t2]=1; CM2[t2][t1]=1;
  }
  //calc distance = how many edges are different
  for (i=0; i<ntowns; i++) {
    for (j=i; j<ntowns; j++) {
      if (CM1[i][j]&CM2[i][j]) dist--;
    }
  }
  return (dist);
}




//   Here we override the _write method for the List class.  This lets us see
// exactly what we want (the default _write method dumps out pointers to the
// data rather than the data contents).
//   This routine prints out the contents of each element of the list,
// separated by a space.  It does not put a newline at the end of the list.
//   Notice that you can override ANY function of a template class.  This is
// called "specialization" in C++ and it lets you tailor the behaviour of a
// template class to better fit the type.
template <> int
GAListGenome<int>::write(ostream & os) const
{
  int *cur, *head;
  GAListIter<int> tmpiter(*this);
  if((head=tmpiter.head()) != 0) {
    os << *head << " ";
    for(cur=tmpiter.next(); cur && cur != head; cur=tmpiter.next())
      os << *cur << " ";
  }

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

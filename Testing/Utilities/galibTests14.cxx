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
REGISTER_TEST(galibTests14);
}
/* ----------------------------------------------------------------------------
  ex14.C
  mbwall 29apr95
  Copyright (c) 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for a composite genome derived from the GAGenome and
containing a list of lists.  This example shows how to derive your own genome
class and illustrates the use of one of the template genomes (GAListGenome)
from the GAlib.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ga/ga.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define endl STD_ENDL
#define istream STD_ISTREAM
#define ostream STD_OSTREAM


// Here we specify how big the lists will be and how many lists will be in each
// composite genome.  These are the default values - you can change them from
// the command line.  Beware that this program will break if nrobots is bigger
// than the size of the lists.
int listsize = 10;
int nrobots = 6;


// This is the class definition.  We override the methods of the base class and
// define a few methods of our own to access the protected members.  The list
// genomes in the composite genome are assigned the 'List' operators
// by default (they can be overridden by using the 'Operator' members on the
// lists explicitly).
//   The ID can be any number over 200 (IDs under 200 are reserved for use by
// GAlib objects).
class RobotPathGenome : public GAGenome {
public:
  GADefineIdentity("RobotPathGenome", 251);
  static void Initializer(GAGenome&);
  static int Mutator(GAGenome&, float);
  static float Comparator(const GAGenome&, const GAGenome&);
  static float Evaluator(GAGenome&);
  static void PathInitializer(GAGenome&);
  static int Crossover(const GAGenome&, const GAGenome&, GAGenome*, GAGenome*);
public:
  RobotPathGenome(int nrobots, int pathlength);
  RobotPathGenome(const RobotPathGenome & orig) { n=l=0; list=0; copy(orig); }
  RobotPathGenome operator=(const GAGenome & arg) { copy(arg); return *this; }
  virtual ~RobotPathGenome();
  virtual GAGenome *clone(GAGenome::CloneMethod) const ;
  virtual void copy(const GAGenome & c);
  virtual int equal(const GAGenome& g) const;
  virtual int read(istream & is);
  virtual int write(ostream & os) const ;

  GAListGenome<int> & path(const int i){return *list[i];}
  int npaths() const { return n; }
  int length() const { return l; }

protected:
  int n, l;
  GAListGenome<int> **list;
};




RobotPathGenome::RobotPathGenome(int nrobots, int pathlength) :
GAGenome(Initializer, Mutator, Comparator){
  evaluator(Evaluator); crossover(Crossover); n = nrobots; l = pathlength;
  list = (n ? new GAListGenome<int> * [n] : (GAListGenome<int> **)0);
  for(int i=0; i<n; i++){
    list[i] = new GAListGenome<int>;
    list[i]->initializer(PathInitializer);
    list[i]->mutator(GAListGenome<int>::SwapMutator);
  }
}

void
RobotPathGenome::copy(const GAGenome& g) {
  if(&g != this && sameClass(g)){
    GAGenome::copy(g);		// copy the base class part
    RobotPathGenome & genome = (RobotPathGenome &)g;
    if(n == genome.n){
      for(int i=0; i<n; i++)
	list[i]->copy(*genome.list[i]);
    }
    else{
      int i;
      for(i=0; i<n; i++)
	delete list[i];
      delete [] list;
      n = genome.n;
      list = new GAListGenome<int> * [n];
      for(i=0; i<n; i++)
	list[i] = (GAListGenome<int> *)genome.list[i]->clone();
    }
  }
}

RobotPathGenome::~RobotPathGenome(){
  for(int i=0; i<n; i++)
    delete list[i];
  delete [] list;
}

GAGenome*
RobotPathGenome::clone(GAGenome::CloneMethod) const {
  return new RobotPathGenome(*this);
}

int
RobotPathGenome::equal(const GAGenome& g) const {
  RobotPathGenome& genome = (RobotPathGenome&)g;
  int flag=0;
  for(int i=0; i<n && flag==0; i++)
    flag = list[i]->equal(*genome.list[i]);
  return flag;
}

int
RobotPathGenome::read(istream & is) {
  for(int i=0; i<n; i++)
    is >> *(list[i]);
  return is.fail() ? 1 : 0;
}

int
RobotPathGenome::write(ostream & os) const {
  for(int i=0; i<n; i++)
    os << "list " << i << ":\t" << *(list[i]) << "\n";
  return os.fail() ? 1 : 0;
}




// These are the definitions of the operators for the robot path genome.
void
RobotPathGenome::Initializer(GAGenome& g) {
  RobotPathGenome & genome = (RobotPathGenome &)g;
  for(int i=0; i<genome.npaths(); i++)
    genome.path(i).initialize();
  genome._evaluated = gaFalse;
}

int
RobotPathGenome::Mutator(GAGenome& g, float pmut) {
  RobotPathGenome & genome = (RobotPathGenome &)g;
  int nMut = 0;
  for(int i=0; i<genome.npaths(); i++)
    nMut += genome.path(i).mutate(pmut);
  if(nMut) genome._evaluated = gaFalse;
  return nMut;
}

float
RobotPathGenome::Comparator(const GAGenome& a, const GAGenome& b) {
  RobotPathGenome& sis = (RobotPathGenome &)a;
  RobotPathGenome& bro = (RobotPathGenome &)b;
  float diff = 0;
  for(int i=0; i<sis.npaths(); i++)
    diff += sis.path(i).compare(bro.path(i));
  return diff/sis.npaths();
}

// The objective function should evaluate the genomes.  This one tries to
// put the node with value 0 into the nth position where n is the number of the
// list in the composite genome.  We're assuming that there are more nodes
// in the list than there are lists in the composite genome.
float
RobotPathGenome::Evaluator(GAGenome & c) {
  RobotPathGenome & genome = (RobotPathGenome &)c;
  float score=0;
  for(int i=0; i<genome.npaths(); i++)
    if(*genome.path(i).warp(i) == 0) score += 1;
  return score;
}


// This crossover method assumes that all of the robot path genomes have the
// same number of paths in them.  With a few modifications you could make the
// paths be variable length, but then you must use a crossover method other
// than the partial match crossover used here (defined in the robot path
// crossover object).
int
RobotPathGenome::
Crossover(const GAGenome& a, const GAGenome& b, GAGenome* c, GAGenome* d) {
  RobotPathGenome& mom = (RobotPathGenome &)a;
  RobotPathGenome& dad = (RobotPathGenome &)b;

  int n=0;
  if(c && d){
    RobotPathGenome& sis = (RobotPathGenome &)*c;
    RobotPathGenome& bro = (RobotPathGenome &)*d;
    for(int i=0; i<mom.npaths(); i++)
      GAListGenome<int>::PartialMatchCrossover(mom.path(i), dad.path(i),
					       &sis.path(i), &bro.path(i));
    sis._evaluated = gaFalse;
    bro._evaluated = gaFalse;
    n=2;
  }
  else if(c) {
    RobotPathGenome& sis = (RobotPathGenome &)*c;
    for(int i=0; i<mom.npaths(); i++)
      GAListGenome<int>::PartialMatchCrossover(mom.path(i), dad.path(i),
					       &sis.path(i), 0);
    sis._evaluated = gaFalse;
    n=1;
  }
  else if(d) {
    RobotPathGenome& bro = (RobotPathGenome &)*d;
    for(int i=0; i<mom.npaths(); i++)
      GAListGenome<int>::PartialMatchCrossover(mom.path(i), dad.path(i),
					       0, &bro.path(i));
    bro._evaluated = gaFalse;
    n=1;
  }
  return n;
}


// This is the initialization operator for our lists.  We create a list that is
// n long and whose nodes contain numbers in sequence.
//   The first thing to do in the initializer is to clear out any old
// contents - we do not want to build our new list on a previously existing
// one!  Notice that we have to cast the genome into the type of
// genome we're using (in this case a list).  The GA always operates on
// generic genomes.
//   All of our lists must be the same length since we're going to use the
// ordered crossover operators.
void
RobotPathGenome::PathInitializer(GAGenome & c){
  GAListGenome<int> & list = (GAListGenome<int> &)c;

// We must first destroy any pre-existing list.
  while(list.head()) list.destroy();

// Insert the head of the list.  This node has a random number in it, but the
// number is in a range different than those in the rest of the list.  This
// way we'll be able to see how the lists get scrambled up.  Since we're using
// ordered crossover (see the header file) we should never end up with more
// than one node in each list that has a given value.
  list.insert(0, GAListBASE::HEAD);

// Loop through n times and append nodes onto the end of the list.
  int i;
  for(i=0; i<listsize-1; i++)
    list.insert(i+20, GAListBASE::AFTER);

// Now randomize the contents of the list.
  for(i=0; i<listsize; i++)
    if(GARandomBit()) list.swap(i, GARandomInt(0, listsize-1));
}



// Here is the specialization of the write method for our lists.  The default
// write method just prints out pointers to the contents of the nodes (it has
// no way of knowing in advance how you'll want things printed).  Here we
// do almost the same thing, but print out the contents of the nodes rather
// than the pointers to the contents.
template <> int
GAListGenome<int>::write(ostream & os) const {
  int *cur, *head;
  GAListIter<int> iter(*this);
  if((head=iter.head()) != 0) os << *head << " ";
  for(cur=iter.next(); cur && cur != head; cur=iter.next())
    os << *cur << " ";
  return os.fail() ? 1 : 0;
}














int
galibTests14(int argc, char *argv[])
{
  cout << "Example 14\n\n";
  cout << "This example shows how to create a genome that contains\n";
  cout << "a list of lists.  We create a composite genome that has\n";
  cout << "lists in it.  Each list has some nodes, only one of which\n";
  cout << "contains the number 0.  The objective is to move the node with\n";
  cout << "number 0 in it to the nth position where n is the number of the\n";
  cout << "list within the composite genome.\n\n";
  cout.flush();

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  int i;

  for(i=1; i<argc; i++){
    if(strcmp("nr", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": number of robots needs a value.\n";
        exit(1);
      }
      else{
	nrobots = atoi(argv[i]);
        continue;
      }
    }
    else if(strcmp("pl", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": path length needs a value.\n";
        exit(1);
      }
      else{
	listsize = atoi(argv[i]);
        continue;
      }
    }
    else if(strcmp(argv[i],"seed") == 0) {
      if(++i >= argc){
        cerr << argv[0] << ": seed needs a value.\n";
        exit(1);
      }
      else {
	GARandomSeed((unsigned int)atoi(argv[i]));
      }
    }
    else if(strcmp("help",argv[i]) == 0){
      cerr << "valid arguements include standard GAlib arguments plus:\n";
      cerr << "  nr\tnumber of robots (" << nrobots << ")\n";
      cerr << "  pl\tlength of the paths (" << listsize << ")\n";
      cerr << "\n";
      exit(1);
    }
  }

  if(listsize < nrobots) {
    cerr << "path length must be greater than the number of robots.\n";
    exit(1);
  }

  RobotPathGenome genome(nrobots, listsize);
  GASteadyStateGA ga(genome);
  ga.parameters(argc,argv);
  ga.evolve();

  genome.initialize();
  cout << "a randomly-generated set of paths:\n" << genome << endl;
  cout << "the ga generated:\n" << ga.statistics().bestIndividual() << "\n";

  return 0;
}



// force instantiations for compilers that do not do auto instantiation.
// for some compilers (e.g. metrowerks) this must come after any
// specializations or you will get 'multiply-defined errors when you compile.
#if !defined(GALIB_USE_AUTO_INST)
#include <ga/GAList.C>
#include <ga/GAListGenome.C>
GALIB_INSTANTIATION_PREFIX GAList<int>;
GALIB_INSTANTIATION_PREFIX GAListGenome<int>;
#endif

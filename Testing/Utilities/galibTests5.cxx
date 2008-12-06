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
REGISTER_TEST(galibTests5);
}

/* ----------------------------------------------------------------------------
  ex5.C
  mbwall 10dec94
  Copyright 1995-1996  Massachusetts Institute of Technology

 DESCRIPTION:
   Example program for a composite genome derived from the GAGenome and
containing a 2DBinStr genome and a Bin2Dec genome.  This program uses
a steady-state GA to match a 2D pattern in the 2DBinStr part of the genome
and a sequence of numbers in the Bin2Dec part.
   In this example we derive a new genome and a new crossover object to be
used with it.  All of the operators (initialization, mutation, crossover,
comparison, and objective) are defined as member functions but are also
override-able on any instance of the new genome.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ga/GABin2DecGenome.h>
#include <ga/GA2DBinStrGenome.h>
#include <ga/GASStateGA.h>
#include <ga/std_stream.h>

#define cout STD_COUT
#define cerr STD_CERR
#define istream STD_ISTREAM
#define ostream STD_OSTREAM
#define ifstream STD_IFSTREAM


// This is the class definition for the new genome.  The default operators are
// defined as static member functions.  They can be overridden if necessary by
// anyone making an instance of this class - you don't need to derive a new
// class to change the behaviour of one or two of its operators.
class CompositeGenome : public GAGenome {
public:
  GADefineIdentity("CompositeGenome", 201);

  static void CompositeInitializer(GAGenome&);
  static int CompositeMutator(GAGenome&, float);
  static float CompositeComparator(const GAGenome&, const GAGenome&);
  static int CompositeCrossover(const GAGenome&, const GAGenome&,
				GAGenome*, GAGenome*);
public:
  CompositeGenome(int, int, GABin2DecPhenotype&,
		  GAGenome::Evaluator f=NULL, void* u=NULL);
  CompositeGenome(const CompositeGenome & orig);
  CompositeGenome& operator=(const GAGenome& g);
  virtual ~CompositeGenome();
  virtual GAGenome* clone(GAGenome::CloneMethod) const ;
  virtual void copy(const GAGenome & c);
  virtual int equal(const GAGenome& g) const;
  virtual int read(istream & is);
  virtual int write(ostream & os) const;

  GA2DBinaryStringGenome & binstr() const {return *str;}
  GABin2DecGenome & bin2dec() const {return *b2d;}

protected:
  GA2DBinaryStringGenome *str;
  GABin2DecGenome *b2d;
};







// Member functions for the composite genome object
CompositeGenome::
CompositeGenome(int element, int bond, GABin2DecPhenotype& p,
		GAGenome::Evaluator f, void* u) :
		GAGenome(  CompositeInitializer,
			 CompositeMutator,
			 CompositeComparator) {
		  evaluator(f); userData(u); crossover(CompositeCrossover);
		  str = new GA2DBinaryStringGenome(element, bond, f, u);
		  b2d = new GABin2DecGenome(p, f, u);
		}

CompositeGenome::CompositeGenome(const CompositeGenome & orig) {
  str = new GA2DBinaryStringGenome(orig.binstr());
  b2d = new GABin2DecGenome(orig.bin2dec());
  copy(orig);
}

CompositeGenome&
CompositeGenome::operator=(const GAGenome& g) { copy(g); return *this; }

CompositeGenome::~CompositeGenome() { delete str; delete b2d; }

GAGenome*
CompositeGenome::clone(GAGenome::CloneMethod) const {
  return new CompositeGenome(*this);
}

void
CompositeGenome::copy(const GAGenome & c){
  if(&c != this && sameClass(c)){
    GAGenome::copy(c);
    CompositeGenome & bc = (CompositeGenome &)c;
    str->copy(*(bc.str));
    b2d->copy(*(bc.b2d));
  }
}

int
CompositeGenome::equal(const GAGenome& g) const {
  CompositeGenome& genome = (CompositeGenome&)g;
  return ((*str == *genome.str) && (*b2d == *genome.b2d));
}

int
CompositeGenome::read(istream & is) {
  is >> *str >> *b2d;
  return is.fail() ? 1 : 0;
}

int
CompositeGenome::write(ostream & os) const {
  int i,j;
  for(j=0; j<str->height(); j++){
    for(i=0; i<str->width(); i++)
      os << (str->gene(i,j) == 1 ? '*' : ' ') << " ";
    os << "\n";
  }
  os << "\n" << *b2d << "\n";
  return os.fail() ? 1 : 0;
}



// These are the default initialization, mutation, and comparator operators for
// this genome class.  They are defined as static functions of the composite
// genome class and they're defaults for the class.  But they can be overridden
// on any instance of the genome.

// The initializer just calls the initializer for each of the genomes that are
// in the composite genome.

// I would have used simply 'Initializer', 'Mutator', etc rather than
// 'CompositeInitializer' but old versions of g++ are brain-dead and don't
// get the encapsulation properly.
void
CompositeGenome::CompositeInitializer(GAGenome & c) {
  CompositeGenome & child = (CompositeGenome &)c;
  child.binstr().initialize();
  child.bin2dec().initialize();
  child._evaluated = gaFalse;
}


// The mutator just calls the mutator for each of the component genomes.
int
CompositeGenome::CompositeMutator(GAGenome & c, float pmut) {
  CompositeGenome & child = (CompositeGenome &)c;
  int nmut = child.binstr().mutate(pmut) + child.bin2dec().mutate(pmut);
  if(nmut) child._evaluated = gaFalse;
  return nmut;
}

// The comparator just calls the comparators for each of the component genomes,
// then averages the score.
float
CompositeGenome::CompositeComparator(const GAGenome& a, const GAGenome& b) {
  CompositeGenome& sis = (CompositeGenome &)a;
  CompositeGenome& bro = (CompositeGenome &)b;
  return 0.5 * (sis.binstr().compare(bro) + sis.bin2dec().compare(bro));
}

// The crossover operator invokes the crossover for each of the genomes in the
// composite genome.  We use sexual crossover only, and we do not test to see
// if no crossover has been assigned.
int
CompositeGenome::
CompositeCrossover(const GAGenome& a, const GAGenome& b,
		   GAGenome* c, GAGenome* d){
  CompositeGenome& mom = (CompositeGenome&)a;
  CompositeGenome& dad = (CompositeGenome&)b;
  int n=0;

  GAGenome::SexualCrossover strcross = mom.str->sexual();
  GAGenome::SexualCrossover b2dcross = mom.b2d->sexual();

  if(c && d){
    CompositeGenome& sis = (CompositeGenome&)*c;
    CompositeGenome& bro = (CompositeGenome&)*d;
    (*strcross)(mom.binstr(), dad.binstr(), &sis.binstr(), &bro.binstr());
    (*b2dcross)(mom.bin2dec(),dad.bin2dec(), &sis.bin2dec(), &bro.bin2dec());
    sis._evaluated = gaFalse;
    bro._evaluated = gaFalse;
    n = 2;
  }
  else if(c){
    CompositeGenome& sis = (CompositeGenome&)*c;
    (*strcross)(mom.binstr(), dad.binstr(), &sis.binstr(), 0);
    (*b2dcross)(mom.bin2dec(), dad.bin2dec(), &sis.bin2dec(), 0);
    sis._evaluated = gaFalse;
    n = 1;
  }
  else if(d){
    CompositeGenome& bro = (CompositeGenome&)*d;
    (*strcross)(mom.binstr(), dad.binstr(), 0, &bro.binstr());
    (*b2dcross)(mom.bin2dec(), dad.bin2dec(), 0, &bro.bin2dec());
    bro._evaluated = gaFalse;
    n = 1;
  }

  return n;
}

















// This object is a container for the data that we are supposed to match in
// our objective function.
typedef struct _CompositeData {
  short ** str;
  float * b2d;
} CompositeData;

// In this objective function we try to match the pattern in the 2D part of the
// genome and match the sequence of values in the binary-to-decimal part of the
// genome.  The overall score is the sum of both parts.
float
Objective(GAGenome & g) {
  CompositeGenome & genome = (CompositeGenome &)g;
  GA2DBinaryStringGenome & str = genome.binstr();
  GABin2DecGenome & b2d = genome.bin2dec();

  int i;

  short **pattern = ((CompositeData *)g.userData())->str;

  float val1=0.0;
  for(i=0; i<str.width(); i++)
    for(int j=0; j<str.height(); j++)
      val1 += (float)(str.gene(i,j) == pattern[i][j]);

  float *sequence = ((CompositeData *)g.userData())->b2d;

  float val2=b2d.nPhenotypes();
  for(i=0; i<b2d.nPhenotypes(); i++)
    val2 += 1.0 / (1.0 + fabs(b2d.phenotype(i) - sequence[i]));

  return(val1 + val2);
}











int
galibTests5(int argc, char *argv[])
{
  cout << "Example 5\n\n";
  cout << "This program shows how to use a composite genome.  It reads\n";
  cout << "a matrix from a data file and a set of values to be matched in\n";
  cout << "a binary-to-decimal genome then uses a steady-state GA to\n";
  cout << "match the pattern and value set.\n\n";

// See if we've been given a seed to use (for testing purposes).  When you
// specify a random seed, the evolution will be exactly the same each time
// you use that seed number.

  for(int ii=1; ii<argc; ii++) {
    if(strcmp(argv[ii],"seed") == 0) {
      GARandomSeed((unsigned int)atoi(argv[ii+1]));
    }
  }

  GAParameterList params;
  GASteadyStateGA::registerDefaultParameters(params);
  params.set(gaNpReplacement, 0.5);
  params.set(gaNscoreFilename, "bog.dat");
  params.set(gaNflushFrequency, 10);
  params.set(gaNnGenerations, 800);
  params.parse(argc, argv, gaFalse);

  int i,j, n;
  char filename1[1024] = "smiley.txt";
  char filename2[1024] = "values.txt";

  for(i=1; i<argc; i++){
    if(strcmp("graph", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": you must specify a filename.\n";
        return(1);
      }
      else{
        sprintf(filename1, argv[i]);
        continue;
      }
    }
    else if(strcmp("values", argv[i]) == 0){
      if(++i >= argc){
        cerr << argv[0] << ": you must specify a filename.\n";
        return(1);
      }
      else{
        sprintf(filename2, argv[i]);
        continue;
      }
    }
    else if(strcmp("seed", argv[i]) == 0){
      if(++i < argc) continue;
      continue;
    }
    else {
      cerr << argv[0] << ":  unrecognized arguement: " << argv[i] << "\n\n";
      cerr << "valid arguements include standard GAlib flags plus:\n";
      cerr << "  graph\tname of graph filename (" << filename1 << ")\n";
      cerr << "  values\tname of values filename (" << filename2 << ")\n";
      cerr << "\n";
      return(1);
    }
  }

  ifstream infile;

// First we read in the pattern for the 2DBinStr genome.
// File format is pretty simple:
//   two integers that give the height then width of the matrix,
//   then the matrix of 1's and 0's (with whitespace inbetween).

  infile.open(filename1);
  if(!infile){
    cerr << "Cannot open " << filename1 << " for input.\n";
    return(1);
  }

  int height, width;
  infile >> height;
  infile >> width;

  short **target = new short*[width];
  for(i=0; i<width; i++)
    target[i] = new short[height];

  for(j=0; j<height; j++)
    for(i=0; i<width; i++)
      infile >> target[i][j];

  infile.close();

// Now we read in a sequence of numbers that the Bin2Dec genome is supposed
// to match for its objective.  File format is
// pretty simple:  a single integer that tells how many numbers will follow,
// then the sequence of numbers.

  infile.open(filename2);
  if(!infile){
    cerr << "Cannot open " << filename2 << " for input.\n";
    exit(1);
  }
  infile >> n;
  float *sequence = new float[n];
  for(i=0; i<n; i++)
      infile >> sequence[i];
  infile.close();

// Print out the pattern and sequence.

  cout << "input pattern:\n";
  for(j=0; j<height; j++){
    for(i=0; i<width; i++)
      cout << (target[i][j] == 1 ? '*' : ' ') << " ";
    cout << "\n";
  }
  cout << "\n"; cout.flush();

  cout << "input sequence:\n";
  for(i=0; i<n; i++)
    cout << sequence[i] << " ";
  cout << "\n"; cout.flush();

// Create a phenotype then fill it with the phenotypes we will need to map to
// the values we read from the file.  The arguments to the add() method of a
// Bin2Dec phenotype are (1) number of bits, (2) min value, and (3) max value.
// The phenotype maps a floating-point number onto the number of bits that
// you designate.  Here we just make everything use 8 bits and pick a min and
// max based upon the number we read in from the file.  You can experiment with
// the number of bits and max/min values in order to make the GA work better
// or worse.

  GABin2DecPhenotype map;
  for(i=0; i<n; i++)
    map.add(12, 0.5*sequence[i], 2.0*sequence[i]);

// Create an instance of our user data structure and stuff it with the values
// that we read in from the files.

  CompositeData mydata;
  mydata.str = target;
  mydata.b2d = sequence;

// Now create the GA and run it.  First a genome, then the GA.
  CompositeGenome genome(width, height, map, Objective, (void *)&mydata);
  GASteadyStateGA ga(genome);
  ga.parameters(params);
  ga.parameters(argc,argv);
  ga.evolve();

  genome = ga.statistics().bestIndividual();
  cout << "\nthe ga generated:\n" << genome << "\n";

// Don't forget to free up the memory we allocated.

  for(i=0; i<width; i++)
    delete target[i];
  delete [] target;
  delete [] sequence;

  return 0;
}


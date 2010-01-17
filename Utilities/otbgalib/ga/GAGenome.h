// $Header$
/* ----------------------------------------------------------------------------
  genome.h
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  The base genome class just defines the genome interface - how to mutate,
crossover, evaluate, etc.  When you create your own genome, multiply inherit
from the base genome class and the data type that you want to use.  Use the
data type to store the information and use the genome part to tell the GA how
it should operate on the data.  See comments below for further details.
---------------------------------------------------------------------------- */
#ifndef _ga_genome_h_
#define _ga_genome_h_

#include <ga/gaid.h>
#include <ga/gaconfig.h>
#include <ga/gaerror.h>
#include <ga/GAEvalData.h>
#include <ga/std_stream.h>

class GAGeneticAlgorithm;
class GAGenome;


/* ----------------------------------------------------------------------------
Genome
-------------------------------------------------------------------------------

Deriving your own genomes:
  For any derived class be sure to define the canonical methods:  constructor,
copy constructor, operator=, and destructor.  Make sure that you check for a
self-copy in your copy method (it is possible that a genome will be
selected to cross with itself, and self-copying is not out of the question)
  To work properly with the GAlib, you MUST define the following:

       YourGenome( -default-args-for-your-genome )
       YourGenome(const YourGenome&)
       virtual ~YourGenome()
       virtual GAGenome* clone(GAGenome::CloneMethod)
       virtual copy(const GAGenome&)

  If your genome class defines any new properties you should to define:

       virtual int read(istream&)
       virtual int write(ostream&) const
       virtual int equal(const GAGenome&) const
  
    When you derive a genome, don't forget to use the _evaluated flag to 
  indicate when the state of the genome has changed and an evaluation is 
  needed.
    Assign a default crossover method so that users don't have to assign one
  unless they want to.  Do this in the constructor.  
    It is a good idea to define an identity for your genome (especially if 
  you will be using it in an environment with multiple genome types running 
  around).  Use the DefineIdentity/DeclareIdentity macros (defined in id.h) 
  to do this in your class definition.


Brief overview of the member functions:

initialize
  Use this method to set the initial state of your genomes once they have
  been created.  This initialization is for setting up the genome's state,
  not for doing the basic mechanics of genome class management.  The 
  default behaviour of this method is to change randomly the contents of the 
  genome.  If you want to bias your initial population, this is where to
  make that happen.
     The initializer is used to initialize the genome (duh).  Notice that the
  state of the genome is unknown - memory may or may not have been allocated, 
  and the genome may or may not have been used before.  So your initializer 
  should first clean up as needed, then do its thing.  The initializer may be 
  called any number of times (unlike a class constructor which is called only 
  once for a given instance).
 
mutate
  Mutate the genome with probability as specified.  What mutation means
  depends upon the data type of the genome.  For example, you could have 
  a bit string in which 50% mutation means each bit has a 50% chance of
  getting flipped, or you could have a tree in which 50% mutation means each
  node has a 50% chance of getting deleted, or you could have a bit string
  in which 50% mutation means 50% of the bits ACTUALLY get flipped.
    The mutations member returns the number of mutations since the genome 
  was initialized.
    The mutator makes a change to the genome with likeliehood determined by the
  mutation rate parameter.  The exact meaning of mutation is up to you, as is 
  the specific meaning of the mutation rate.  The function returns the number 
  of mutations that actually occurred.

crossover
  Genomes don't really have any clue about other genomes, so we don't make
  the crossover a member function.  Instead, each genome kind of knows how
  to mate with other genomes to generate offspring, but they are not
  capable of doing it themselves.  The crossover member function is used to
  set the default mating mode for the genomes - it does not actually perform
  the crossover.  This way the GA can use asexual crossover if it wants to
  (but genomes only know how to do the default sexual crossover).
    This also lets you do funky stuff like crossover between different data
  types and group sex to generate new offspring.
     We define two types of crossover:  sexual and asexual.  Most GAlib
  algorithms use the sexual crossover, but both are available.  Each genome 
  knows the preferred crossover method, but none is capable of reproducing.  
  The genetic algorithm must actually perform the mating because it involves 
  another genome (as parent and/or child).

evaluator
  Set the genome's objective function.  This also sets marks the evaluated
  flag to indicate that the genome must be re-evaluated.
    Evaluation happens on-demand - the objective score is not calculated until
  it is requested.  Then it is cached so that it does not need to be re-
  calculated each time it is requested.  This means that any member function
  that modifies the state of the genome must also set the evaluated flag to
  indicate that the score must be recalculated.
    The genome objective function is used by the GA to evaluate each member of
  the population.

comparator
  This method is used to determine how similar two genomes are.  If you want 
  to use a different comparison method without deriving a new class, then use 
  the comparator function to do so.  For example, you may want to do phenotype-
  based comparisons rather than genotype-based comparisons.
    In many cases we have to compare two genomes to determine how similar or
  different they are.  In traditional GA literature this type of function is 
  referred to as a 'distance' function, probably because bit strings can be
  compared using the Hamming distance as a measure of similarity.  In GAlib, we
  define a genome comparator function that does exactly this kind of 
  comparison.
    If the genomes are identical, the similarity function should return a 
  value of 0.0, if completely different then return a value greater than 0.  
  The specific definition of what "the same" and what "different" mean is up 
  to you.  Most of the default comparators use the genotype for the comparison,
  but you can use the phenotype if you prefer.  There is no upper limit to the
  distance score as far as GAlib is concerned.
    The no-op function returns a -1 to signify that the comparison failed.

evalData
  The evalData member is useful if you do not want to derive a new genome class
  but want to store data with each genome.  When you clone a genome, the eval
  data also gets cloned so that each genome has its own eval data (unlike the
  user data pointer described next which is shared by all genomes).

userData
  The userData member is used to provide all genomes access to the same user
  data.  This can be a pointer to anything you want.  Any genome cloned from
  another will share the same userData as the original.  This means that all
  of the genomes in a population, for example, share the same userData.

score
  Evaluate the 'performance' of the genome using the objective function.
  The score is kept in the 'score' member.  The 'evaluated' member tells us
  whether or not we can trust the score.  Be sure to set/unset the 'evaluated'
  member as appropriate (eg cross and mutate change the contents of the 
  genome so they unset the 'evaluated' flag).
    If there is no objective function, then simply return the score.  This 
  allows us to use population-based evaluation methods (where the population
  method sets the score of each genome).

clone
  This method allocates space for a new genome and copies the original into
  the new space.  Depending on the argument, it either copies the entire 
  original or just parts of the original.  For some data types, clone contents
  and clone attributes will do the same thing.  If your data type requires
  significant overhead for initialization, then you'll probably want to 
  distinguish between cloning contents and cloning attributes.
clone(cont)
  Clone the contents of the genome.  Returns a pointer to a GAGenome
  (which actually points to a genome of the type that was cloned).  This is
  a 'deep copy' in which every part of the genome is duplicated.
clone(attr)
  Clone the attributes of the genome.  This method does nothing to the
  contents of the genome.  It does NOT call the initialization method.  For
  some data types this is the same thing as cloning the contents.
---------------------------------------------------------------------------- */
class GAGenome : public GAID {
public:
  GADefineIdentity("GAGenome", GAID::Genome);

public:
  typedef float (*Evaluator)(GAGenome &);
  typedef void  (*Initializer)(GAGenome &);
  typedef int   (*Mutator)(GAGenome &, float);
  typedef float (*Comparator)(const GAGenome&, const GAGenome&);
  typedef int   (*SexualCrossover)(const GAGenome&, const GAGenome&, 
				   GAGenome*, GAGenome*);
  typedef int   (*AsexualCrossover)(const GAGenome&, GAGenome*);
  
public:
  static void  NoInitializer(GAGenome &);
  static int   NoMutator(GAGenome &, float);
  static float NoComparator(const GAGenome&, const GAGenome&);

public:
  enum Dimension {LENGTH = 0, WIDTH = 0, HEIGHT = 1, DEPTH = 2};
  enum CloneMethod {CONTENTS = 0, ATTRIBUTES = 1};
  enum {FIXED_SIZE = -1, ANY_SIZE = -10};

public:
// The GNU compiler sucks.  It won't recognize No*** as a member of the genome
// class.  So we have to use 0 as the defaults then check in the constructor.
  GAGenome(Initializer i=0, Mutator m=0, Comparator c=0);
  GAGenome(const GAGenome&);
  GAGenome & operator=(const GAGenome & arg){copy(arg); return *this;}
  virtual ~GAGenome();
  virtual GAGenome* clone(CloneMethod flag=CONTENTS) const;
  virtual void copy(const GAGenome &);

#ifdef GALIB_USE_STREAMS
  virtual int read(STD_ISTREAM &)
    { GAErr(GA_LOC, className(), "read", gaErrOpUndef); return 0; }
  virtual int write(STD_OSTREAM &) const
    { GAErr(GA_LOC, className(), "write", gaErrOpUndef); return 0; }
#endif

  virtual int equal(const GAGenome &) const
    { GAErr(GA_LOC, className(), "equal", gaErrOpUndef); return 1; }
  virtual int notequal(const GAGenome & g) const
    { return (equal(g) ? 0 : 1); }

public:
  int nevals() const {return _neval;}
  float score() const { evaluate(); return _score; }
  float score(float s){ _evaluated=gaTrue; return _score=s; }
  float fitness(){return _fitness;}
  float fitness(float f){return _fitness = f;}

  GAGeneticAlgorithm* geneticAlgorithm() const {return ga;}
  GAGeneticAlgorithm* geneticAlgorithm(GAGeneticAlgorithm& g){return(ga=&g);}

  void * userData() const {return ud;}
  void * userData(void * u){return(ud=u);}

  GAEvalData * evalData() const {return evd;}
  GAEvalData * evalData(const GAEvalData& o)
    {delete evd; evd = o.clone(); return evd;}

  float evaluate(GABoolean flag = gaFalse) const;
  Evaluator evaluator() const {return eval;}
  Evaluator evaluator(Evaluator f) { _evaluated=gaFalse; return(eval=f); }

  void initialize(){_evaluated=gaFalse; _neval=0; (*init)(*this);}
  Initializer initializer() const {return init;}
  Initializer initializer(Initializer op){return (init=op);}

  int mutate(float p){ return((*mutr)(*this,p)); }
  Mutator mutator() const {return mutr;}
  Mutator mutator(Mutator op){return (mutr=op);}

  float compare(const GAGenome& g) const {return (*cmp)(*this,g);}
  Comparator comparator() const {return cmp;}
  Comparator comparator(Comparator c){return (cmp=c);}

  SexualCrossover crossover(SexualCrossover f) { return sexcross = f; }
  SexualCrossover sexual() const { return sexcross; }
  AsexualCrossover crossover(AsexualCrossover f) { return asexcross = f; }
  AsexualCrossover asexual() const { return asexcross; }

protected:
  float _score;			// value returned by the objective function
  float _fitness;		// (possibly scaled) fitness score
  GABoolean _evaluated;		// has this genome been evaluated?
  unsigned int _neval;		// how many evaluations since initialization?
  GAGeneticAlgorithm* ga;	// the ga that is using this genome
  void* ud;		        // pointer to user data
  Evaluator eval;		// objective function
  GAEvalData* evd;		// evaluation data (specific to each genome)
  Mutator mutr;			// the mutation operator to use for mutations
  Initializer init;		// how to initialize this genome
  Comparator cmp;		// how to compare two genomes of this type

  SexualCrossover sexcross;	// preferred sexual mating method
  AsexualCrossover asexcross;	// preferred asexual mating method
};


#ifdef GALIB_USE_STREAMS
inline STD_OSTREAM & operator<< (STD_OSTREAM& os, const GAGenome& genome)
{ genome.write(os); return(os); }
inline STD_ISTREAM & operator>> (STD_ISTREAM & is, GAGenome& genome)
{ genome.read(is); return(is); }
#endif

inline int operator== (const GAGenome& a, const GAGenome& b)
{ return a.equal(b); }
inline int operator!= (const GAGenome& a, const GAGenome& b)
{ return a.notequal(b); }

#endif

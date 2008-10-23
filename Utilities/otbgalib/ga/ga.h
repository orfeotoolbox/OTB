// $Header$
/* ----------------------------------------------------------------------------
  ga.h
  mbwall 28jul94
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved

  Main header for the GAlibrary.  This header is provided to make it easy to 
include GAlib components in your code.  It includes all of the genetic 
algorithm classes as well as all of the genome classes (plus a bunch of 
comments giving an overview of the library).
  References to 'Goldberg's Book' are to David E Goldberg's book, and 
references to 'Numerical Recipes' are to the Numerical Recipes in C book.

"Genetic Algorithms in Search, Optimization, and Machine Learning"
    Goldberg, David Edward, 1953- 
    Addison-Wesley Pub. Co., (c) 1989
    ISBN 0-201-15767-5

"Numerical Recipes in C:  The Art of Scientific Computing"
    Cambridge University Press, (c) 1988-1992
    ISBN 0-521-43108-5

-------------------------------------------------------------------------------
Overview of who does what in the GAlibrary -- see http://lancet.mit.edu/ga/
-------------------------------------------------------------------------------
Here are the library capabilities and which parts of the library are involved:

Genetic Algorithm
    This object contains the operators and data needed to perform the 
  optimization.  There are a few basic GA implementations in the library, 
  including the basic 'simple' genetic algorithm described by Goldberg, the
  'steady-state' genetic algorithm, the 'incremental' genetic algorithm based
  on the Genitor model, and the 'parallel' genetic algorithm that uses multiple
  populations in parallel on a single CPU.
    The base GA object defines many of the parameters and statistics useful
  when running an experiment.  It also defines the basic interface for most
  genetic algorithms.

Genome
    The genome object contains the actual problem-specific data as well as the
  mapping from data-domain to problem-domain (by way of the objective functions
    All of the genomes are derived from a base genome class.  The base genome
  class defines the interface used by genetic algorithms to modify the data.
  Any genome used in an experiment must be derived from the base genom class
  as well as a user-defined data type (or one of the sample types provided in
  GAlib).  The library includes array, tree, list, and binary-string data types
  to use if you do not want to define your own.

Population
    Each population contains a bunch of genomes.  Populations know how to 
  select genomes.  They also have housekeeping routines such as replace,
  remove, and add.  A population can be given some intelligence by using its
  evaluation member rather than evaluting each individual on its own.

Selection
    Selection is implemented as a member function of the population object.
  Any GA simply calls the population's 'select' member to get the genome that
  it needs to work with.  

Crossover/Mating
    Crossover is implemented as a separate object.  We have various crossover
  objects, each corresponding to a genome type.  These could be templatized,
  but that wouldn't buy us much since the behaviour depends upon the data type
  anyway.
    Crossover is not a member function of the genomes.  I fretted over how to
  do this properly and decided to keep crossover as an operator separate from
  the genomes.  The other viable option was to make crossover a member function
  of the genomes, but then changing the crossover during the course of the
  evolution becomes more difficult.  Also, it is easier to define asexual (or
  other non-standard mating schemes) with an operator that is not a member of
  the genome object.
    When you create a GA, you specify a genome (or population of genomes).  The
  GA asks the genome how it mates, and the genome returns a pointer to its
  preferred crossover method.  You can set the crossover explicitly on the GA
  if you prefer.  Using this scheme we can assign default crossover operators
  so that you don't have to specify a crossover unless you want to.
    Each genome type must have a corresponding crossover operator defined that
  can operate on that data type.  The genetic algorithms don't care about the
  specific crossover type - they call the mating methods based upon the high
  level interface definition.

Mutation 
    Mutation is defined at the genome level and is implemented as a member 
  function of the base genome class.  Each genome must define its own
  mutation method that will operate on its specific data type.

Replacement strategies
    The replacement strategy defines how a child is inserted into a population.
    Replacement strategies include child-replaces-parent, child-replaces-worst,
  and child-replaces-random.  These make sense only for GAs with overlapping
  populations.

Convergence and completion
    The GA defines a 'done' member that calls a user-defineable completion
  function to determine whether or not the GA is finished.  Built-in completion
  routines include population-converged, best-converged and 
  number-of-generations.

Fitness vs Objective
    Note the difference between fitness function and objective 
  function.  In this library, the objective function is user-defined and tells
  (on an arbitrary scale) how well a genome performs relative to other
  genomes.  The fitness function, on the other hand, takes objective function 
  scores and processes them to produce a number for each genome that 
  represents its fitness for mating/selection.
    The selection strategies include roulette wheel, tournament, linear
  ranking, and stochastic sampling.  See the selector headers for more details
  about each strategy and its implementation.
    Speciation is included as a type of selection.  To use the speciating 
  selector you must define a distance function.  See the selector header for 
  more details.
    Scaling methods are loosely coupled with the selection strategies.  They
  include linear scaling, sigma truncation, and ranking.  See the fitness
  headers for more details about how you can mix and match scaling methods with
  selection strategies.

Elitism
    Elitism is optional.  If you want the best of each generation to be carried
  over to the next generation, turn on the elitism.  If not, turn it off.  This
  only works with non-overlapping populations.

Initialization
    Each genome has its own initialization operator.  When a GA is
  initialized, it initializes its populations, which in turn tells each 
  chromomosome to initialize itself.  You can use the initialization method to
  bias an initial population.  This can be done either at the population level
  by customizing the population's initialization operator or at the genome
  level by customizing the genome's initilizer.
    Default initializers are included for most genomes.  See the documentation
  and genome headers for more details.

-------------------------------------------------------------------------------
Basic usage

  You create a GA by first instantiating the non-default parts that you want to
use.  For example, if you want a binary string genome with uniform crossover
and roulette wheel selection, you would define an objective function, 
instantiate a genome, instantiate a uniform crossover operator, then 
instantiate the GA.  Since the roulette wheel selector is the default, you do
not need to instantiate one to use that method.
  In every case you MUST define an objective function and you MUST instantiate
one copy of the genome type you want to use.

---------------------------------------------------------------------------- */
#ifndef _ga_ga_h_
#define _ga_ga_h_

// Make sure that we get the configuration into each of the galib components
// that will be used.
#include <ga/gaconfig.h>

// These are the headers for all of the genetic algorithm classes.
#include <ga/GASimpleGA.h>
#include <ga/GASStateGA.h>
#include <ga/GAIncGA.h>
#include <ga/GADemeGA.h>
#include <ga/GADCrowdingGA.h>

// Here we include the headers for all of the various genome types.
#include <ga/GA1DBinStrGenome.h>
#include <ga/GA2DBinStrGenome.h>
#include <ga/GA3DBinStrGenome.h>
#include <ga/GABin2DecGenome.h>

#ifndef GALIB_USE_NO_TEMPLATES
#include <ga/GA1DArrayGenome.h>
#include <ga/GA2DArrayGenome.h>
#include <ga/GA3DArrayGenome.h>
#include <ga/GAListGenome.h>
#include <ga/GATreeGenome.h>

// We do *not* include the headers for template specializations.  This prevents
// unnecessary instantiations of template objects which causes grief to some
// compilers.
//#include <ga/GAStringGenome.h>
//#include <ga/GARealGenome.h>
#endif

#endif

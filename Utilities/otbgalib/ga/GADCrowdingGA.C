// $Header$
/* ----------------------------------------------------------------------------
  gadcrowdingga.C
  mbwall 29mar99
  Copyright (c) 1999 Matthew Wall, all rights reserved
---------------------------------------------------------------------------- */
#include "GADCrowdingGA.h"
#include "GAList.h"
#include "garandom.h"

// this assumes that all of the genomes in the population are the same class
void
GADCrowdingGA::initialize(unsigned int seed)
{
  GARandomSeed(seed);

  pop->initialize();
  pop->evaluate(gaTrue);

  stats.reset(*pop);

  if(!scross) 
    GAErr(GA_LOC, className(), "initialize", gaErrNoSexualMating);
}

void
GADCrowdingGA::step() { 
  if(pop->size() == 0) return;

  GAGenome *child = pop->individual(0).clone();

  GAList<int> indpool;

  for (int i=0; i<pop->size(); i++) 
    indpool.insert(i);
    
  do {
    int *ip;
    indpool.warp(GARandomInt(0,indpool.size()-1)); // select mom
    ip=indpool.remove();
    GAGenome *mom = &pop->individual(*ip);
    delete ip;
    
    indpool.warp(GARandomInt(0,indpool.size()-1)); // select dad
    ip=indpool.remove();
    GAGenome *dad = &pop->individual(*ip);
    delete ip;
    
    stats.numsel += 2;		                   // create child	
    stats.numcro += (*scross)(*mom, *dad, child, 0);
    stats.nummut += child->mutate(pMutation());
    stats.numeval += 1;	
    
    float d1 = child->compare(*mom);      // replace closest parent
    float d2 = child->compare(*dad);
    if (d1 < d2) {
      if (minmax == MINIMIZE) {
	if (child->score() < mom->score()) {
	  mom->copy(*child);
	  stats.numrep += 1;	
	}
      }
      else {
	if (child->score() > mom->score()) {
	  mom->copy(*child);
	  stats.numrep += 1;	
	}
      }
    }
    else {
      if (minmax == MINIMIZE) {
	if (child->score() < dad->score()) {
	  dad->copy(*child);
	  stats.numrep += 1;	
	}
      }
      else {
	if (child->score() > dad->score()) {
	  dad->copy(*child);
	  stats.numrep += 1;	
	}
      }
    }
  } while (indpool.size()>1);

  pop->evaluate(gaTrue);
  stats.update(*pop);

  delete child;
}

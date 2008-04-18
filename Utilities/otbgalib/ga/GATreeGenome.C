// $Header$
/* ----------------------------------------------------------------------------
  tree.C
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the tree genome.
---------------------------------------------------------------------------- */
#ifndef _ga_tree_C_
#define _ga_tree_C_

#include <stdio.h>
#include <stdlib.h>
#include <ga/GATreeGenome.h>
#include <ga/garandom.h>


extern int _GATreeCompare(GANodeBASE * anode, GANodeBASE * bnode);

template <class T> const char *
GATreeGenome<T>::className() const {return "GATreeGenome";}
template <class T> int
GATreeGenome<T>::classID() const {return GAID::TreeGenome;}

template <class T>
GATreeGenome<T>::GATreeGenome(GAGenome::Evaluator f, void * u) : 
GATree<T>(),
GAGenome(DEFAULT_TREE_INITIALIZER,
	 DEFAULT_TREE_MUTATOR,
	 DEFAULT_TREE_COMPARATOR) {
  evaluator(f);
  userData(u);
  crossover(DEFAULT_TREE_CROSSOVER);
}


template <class T>
GATreeGenome<T>::GATreeGenome(const GATreeGenome<T> & orig) : 
GATree<T>(),
GAGenome() {
  GATreeGenome<T>::copy(orig);
}


template <class T>
GATreeGenome<T>::~GATreeGenome() { }


template <class T> GAGenome *
GATreeGenome<T>::clone(GAGenome::CloneMethod flag) const {
  GATreeGenome<T> *cpy = new GATreeGenome<T>();
  if(flag == (int)CONTENTS){cpy->copy(*this);} // cast is for metrowerks...
  else{cpy->GAGenome::copy(*this);}
  return cpy;
}


template <class T> void
GATreeGenome<T>::copy(const GAGenome & orig) {
  if(&orig == this) return;
  const GATreeGenome<T>* c = DYN_CAST(const GATreeGenome<T>*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GATree<T>::copy(*c);
  }
}


#ifdef GALIB_USE_STREAMS
// Traverse the tree (breadth-first) and dump the contents as best we can to
// the stream.  We don't try to write the contents of the nodes - we simply 
// write a . for each node in the tree.
//   We allocate space for x,y coord pair for each node in the tree.  Then we
// do a depth-first traversal of the tree and assign coords to the nodes in the
// order we get them in the traversal.  Each coord pair is measured relative to
// the parent of the node.
template <class T> void
_tt(STD_OSTREAM & os, GANode<T> * n)
{
  if(!n) return;
  GANodeBASE * node = DYN_CAST(GANodeBASE*, n);

  os.width(10); os << node << " ";
  os.width(10); os << node->parent << " ";
  os.width(10); os << node->child << " ";
  os.width(10); os << node->next << " ";
  os.width(10); os << node->prev << " ";
  os.width(10); os << &(n->contents) << "\n";
  _tt(os, DYN_CAST(GANode<T>*, node->child));

  for(GANodeBASE * tmp=node->next; tmp && tmp != node; tmp=tmp->next){
    os.width(10); os << tmp << " ";
    os.width(10); os << tmp->parent << " ";
    os.width(10); os << tmp->child << " ";
    os.width(10); os << tmp->next << " ";
    os.width(10); os << tmp->prev << " ";
    os.width(10); os << &(DYN_CAST(GANode<T>*, tmp)->contents) << "\n";
    _tt(os, DYN_CAST(GANode<T>*, tmp->child));
  }
}

template <class T> int
GATreeGenome<T>::write(STD_OSTREAM & os) const 
{
  os << "node       parent     child      next       prev       contents\n";
  _tt(os, (GANode<T> *)(this->rt));
  return 0;
}
#endif


template <class T> int  
GATreeGenome<T>::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  const GATreeGenome<T>& b = DYN_CAST(const GATreeGenome<T>&, c);
  return _GATreeCompare(this->rt, b.rt) ? 0 : 1;
}











/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
//  This mutation method is destructive.  We randomly pick a node in the tree
// then delete the subtree and node at that point.  Each node in the tree has
// a pmut probability of getting nuked.
//  After the mutation the iterator is left at the root of the tree.
template <class T> int
GATreeGenome<T>::DestructiveMutator(GAGenome & c, float pmut)
{
  GATreeGenome<T> &child=DYN_CAST(GATreeGenome<T> &, c);
  register int n, i;
  if(pmut <= 0.0) return 0;

  n = child.size();
  float nMut = pmut * STA_CAST(float,n);
  if(nMut < 1.0){		// we have to do a flip test for each node
    nMut = 0;
    for(i=0; i<n; i++){
      if(GAFlipCoin(pmut) && child.warp(i)){
	child.destroy();
	nMut++;
      }
    }
  }
  else{				// only nuke the number of nodes we need to
    for(i=0; i<nMut; i++){
      if(child.warp(GARandomInt(0, n-1)))
	child.destroy();
    }
  }
  child.root();		// set iterator to root node

  return(STA_CAST(int,nMut));
}


// This is a rearranging mutation operator.  It randomly picks two nodes in the
// tree and swaps them.  Any node has a pmut chance of getting
// swapped, and the swap could happen to any other node.  And in the case of
// nMut < 1, the swap may generate a swap partner that is the same node, in 
// which case no swap occurs (we don't check).
//   After the mutation the iterator is left at the root of the tree.
template <class T> int
GATreeGenome<T>::SwapNodeMutator(GAGenome & c, float pmut)
{
  GATreeGenome<T> &child=DYN_CAST(GATreeGenome<T> &, c);
  register int n, i;
  if(pmut <= 0.0) return 0;

  n = child.size();
  float nMut = pmut * STA_CAST(float,n);
  nMut *= 0.5;			// swapping one node swaps another as well
  if(nMut < 1.0){		// we have to do a flip test for each node
    nMut = 0;
    for(i=0; i<n; i++){
      if(GAFlipCoin(pmut)){
	child.swap(i,GARandomInt(0,n-1));
	nMut++;
      }
    }
  }
  else{				// only nuke the number of nodes we need to
    for(i=0; i<nMut; i++)
      child.swap(GARandomInt(0,n-1),GARandomInt(0,n-1));
  }
  child.root();		// set iterator to root node

  return(STA_CAST(int,nMut*2));
}



// This is a rearranging mutation operator with subtree swap.  It does the same
// thing as the rearranging mutator above, but swaps subtrees as well as the
// nodes that are selected.
//   After the mutation the iterator is left at the root of the tree.
//   We check to make sure that we don't try to swap ancestral nodes.  If it is
// an ancestral swap, we give up and don't do anything to the tree.  This could
// result in mutation rates that are lower than the specified rate!
// *** mutation rates are not exact!
template <class T> int
GATreeGenome<T>::SwapSubtreeMutator(GAGenome & c, float pmut)
{
  GATreeGenome<T> &child=DYN_CAST(GATreeGenome<T> &, c);
  register int n, i;
  int a, b;
  if(pmut <= 0.0) return 0;

  n = child.size();
  float nMut = pmut * STA_CAST(float,n);
  nMut *= 0.5;			// swapping one node swaps another as well
  if(nMut < 1.0){		// we have to do a flip test for each node
    nMut = 0;
    for(i=0; i<n; i++){
      if(GAFlipCoin(pmut)){
	b = GARandomInt(0,n-1);
	if(!child.ancestral(i,b)) child.swaptree(i,b);
	nMut++;
      }
    }
  }
  else{				// only nuke the number of nodes we need to
    for(i=0; i<nMut; i++){
      a = GARandomInt(0,n-1);
      b = GARandomInt(0,n-1);
      if(!child.ancestral(a,b)) child.swaptree(a,b);
    }
  }
  child.root();		// set iterator to root node

  return(STA_CAST(int, nMut*2));
}


// We use the recursive tree function to compare the tree structures.  This
// does not compare the contents of the nodes.
template <class T> float
GATreeGenome<T>::TopologyComparator(const GAGenome& a, const GAGenome& b) 
{
  if(&a == &b) return 0;
  const GATreeGenome<T>& sis=DYN_CAST(const GATreeGenome<T>&, a);
  const GATreeGenome<T>& bro=DYN_CAST(const GATreeGenome<T>&, b);

  return STA_CAST(float, _GATreeCompare(sis.rt, bro.rt));
}









// The default crossover operator takes a node from parent a (with its entire
// sub-tree) and replaces it with a node from parent b (with its entire sub-
// tree).  If the crossover site is not set, then we pick a random site based
// on the trees in the genomes we're going to cross.  Once we have a valid
// crossover site, we copy the trees from the two genomes.
//   If the crossover site is out of bounds (ie refers to a node not in the
// tree) then we don't do anything to the child.
//   We allow crossover at ANY site in the genomes (including at the root
// node).
// *** we should be able to speed this up.  there is an extra traversal when we
//     do the check to see if the crossover site is valid.
template <class T> int
GATreeGenome<T>::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2, 
		  GAGenome* c1, GAGenome* c2){
  const GATreeGenome<T> &mom=DYN_CAST(const GATreeGenome<T> &, p1);
  const GATreeGenome<T> &dad=DYN_CAST(const GATreeGenome<T> &, p2);

  int nc=0;
  unsigned int a = GARandomInt(0, mom.size()-1);
  unsigned int b = GARandomInt(0, dad.size()-1);
  GATreeIter<T> momiter(mom), daditer(dad);
  GATree<T> * tree;

  if(c1 && c2){
    GATreeGenome<T> &sis=DYN_CAST(GATreeGenome<T> &, *c1);
    GATreeGenome<T> &bro=DYN_CAST(GATreeGenome<T> &, *c2);

// first do the sister...

    if(momiter.warp(a) && daditer.warp(b)){
      sis.GATree<T>::copy(mom);
      tree = dad.GATree<T>::clone(b);
      sis.warp(a);
      sis.swaptree(tree);
      delete tree;
      sis.warp(0);
    }

// ...now do the brother.

    if(momiter.warp(a) && daditer.warp(b)){
      bro.GATree<T>::copy(dad);
      tree = mom.GATree<T>::clone(a);
      bro.warp(b);
      bro.swaptree(tree);
      delete tree;
      bro.warp(0);
    }

    nc = 2;
  }
  else if(c1){
    GATreeGenome<T> &sis=DYN_CAST(GATreeGenome<T> &, *c1);

    if(GARandomBit()){
      if(momiter.warp(a) && daditer.warp(b)){
	sis.GATree<T>::copy(mom);
	tree = dad.GATree<T>::clone(b);
	sis.warp(a);
	sis.swaptree(tree);
	delete tree;
	sis.warp(0);
      }
    }
    else{
      if(momiter.warp(a) && daditer.warp(b)){
	sis.GATree<T>::copy(dad);
	tree = mom.GATree<T>::clone(a);
	sis.warp(b);
	sis.swaptree(tree);
	delete tree;
	sis.warp(0);
      }
    }

    nc = 1;
  }

  return nc;
}

#endif

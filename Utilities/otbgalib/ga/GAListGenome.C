// $Header$
/* ----------------------------------------------------------------------------
  list.C
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the list genome.
---------------------------------------------------------------------------- */
#ifndef _ga_list_C_
#define _ga_list_C_

#include <stdio.h>
#include <stdlib.h>
#include <ga/GAListGenome.h>
#include <ga/GAMask.h>
#include <ga/garandom.h>

template <class T> int 
GAListIsHole(const GAListGenome<T>&, const GAListGenome<T>&, int, int, int);






template <class T> const char *
GAListGenome<T>::className() const {return "GAListGenome";}
template <class T> int
GAListGenome<T>::classID() const {return GAID::ListGenome;}

template <class T> 
GAListGenome<T>::GAListGenome(GAGenome::Evaluator f, void * u) : 
GAList<T>(),
GAGenome(DEFAULT_LIST_INITIALIZER,
	 DEFAULT_LIST_MUTATOR,
	 DEFAULT_LIST_COMPARATOR) {
  evaluator(f);
  userData(u);
  crossover(DEFAULT_LIST_CROSSOVER);
}


template <class T> 
GAListGenome<T>::GAListGenome(const GAListGenome<T> & orig) : 
GAList<T>(),
GAGenome() {
  GAListGenome<T>::copy(orig);
}


template <class T>
GAListGenome<T>::~GAListGenome() { }


template <class T> GAGenome *
GAListGenome<T>::clone(GAGenome::CloneMethod flag) const {
  GAListGenome<T> *cpy = new GAListGenome<T>();
  if(flag == (int)CONTENTS){cpy->copy(*this);} // the cast is for metrowerks...
  else{cpy->GAGenome::copy(*this);}
  return cpy;
}


template <class T> void
GAListGenome<T>::copy(const GAGenome & orig){
  if(&orig == this) return;
  const GAListGenome<T>* c = DYN_CAST(const GAListGenome<T>*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GAList<T>::copy(*c);
  }
}


#ifdef GALIB_USE_STREAMS
// Traverse the list (breadth-first) and dump the contents as best we can to
// the stream.  We don't try to write the contents of the nodes - we simply 
// write a . for each node in the list.
template <class T> int
GAListGenome<T>::write(STD_OSTREAM & os) const 
{
  os << "node       next       prev       contents\n";
  if(!this->hd) return 0;;
  os.width(10); os << this->hd << " ";
  os.width(10); os << this->hd->next << " ";
  os.width(10); os << this->hd->prev << " ";
  os.width(10); os << &(DYN_CAST(GANode<T>*, this->hd)->contents) << "\n";

  for(GANodeBASE * tmp=this->hd->next; tmp && tmp != this->hd; tmp=tmp->next){
    os.width(10); os << tmp << " ";
    os.width(10); os << tmp->next << " ";
    os.width(10); os << tmp->prev << " ";
    os.width(10); os << &(DYN_CAST(GANode<T>*, tmp)->contents) << "\n";
  }
  return 0;
}
#endif


// Both the == and != operators assume that both operator== and operator!= are
// defined for the object that is store in the node of your list.  If it is
// not, you'll get an error message.  If you're storing pointers in the nodes,
// then you have nothing to worry about.
//   Neither of these operators affects the internal iterator of either
// list genome in any way.
template <class T> int 
GAListGenome<T>::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  const GAListGenome<T> & b = DYN_CAST(const GAListGenome<T>&, c);
  if(this->size() != b.size()) return 0;

  GAListIter<T> iterA(*this), iterB(b);
  T *tmpA = iterA.head(), *tmpB = iterB.head();
  T *head = tmpA;
  do{
    if(tmpA && tmpB && *tmpA != *tmpB) return gaFalse;
    tmpB = iterB.next();
    tmpA = iterA.next();
  }while(tmpA && tmpA != head);
  return 1;
}









/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
// Mutate a list by nuking nodes.  Any node has a pmut chance of getting nuked.
// This is actually kind of bogus for the second part of the if clause (if nMut
// is greater than or equal to 1).  Nodes end up having more than pmut 
// probability of getting nuked.
//   After the mutation the iterator is left at the head of the list.
template <class T> int
GAListGenome<T>::DestructiveMutator(GAGenome & c, float pmut)
{
  GAListGenome<T> &child=DYN_CAST(GAListGenome<T> &, c);
  register int n, i;
  if(pmut <= 0.0) return 0;

  n = child.size();
  float nMut = pmut * STA_CAST(float, n);
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
  child.head();		// set iterator to root node

  return(STA_CAST(int,nMut));
}


// Mutate a list by swapping two nodes.  Any node has a pmut chance of getting
// swapped, and the swap could happen to any other node.  And in the case of
// nMut < 1, the swap may generate a swap partner that is the same node, in 
// which case no swap occurs (we don't check).
//   After the mutation the iterator is left at the head of the list.
template <class T> int
GAListGenome<T>::SwapMutator(GAGenome & c, float pmut)
{
  GAListGenome<T> &child=DYN_CAST(GAListGenome<T> &, c);
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
  child.head();		// set iterator to root node

  return(STA_CAST(int, nMut*2));
}


// This comparator returns the number of elements that the two lists have
// in common (both in position and in value).  If they are different lengths
// then we just say they are completely different.  This is probably barely 
// adequate for most applications - we really should give more credit for
// nodes that are the same but in different positions.  But that would be
// pretty nasty to compute.
//   This is somewhat problematic since there is no absolute measure against
// which to compare this diversity measure.  Its kind of hard to define this
// one in a general way...
template <class T> float
GAListGenome<T>::NodeComparator(const GAGenome& a, const GAGenome& b) 
{
  if(&a == &b) return 0;
  const GAListGenome<T>& sis=DYN_CAST(const GAListGenome<T>&, a);
  const GAListGenome<T>& bro=DYN_CAST(const GAListGenome<T>&, b);

  if(sis.size() > bro.size()) return (float)(sis.size() - bro.size());
  if(sis.size() < bro.size()) return (float)(bro.size() - sis.size());
  if(sis.size() == 0) return 0;

  float count = 0;
  GAListIter<T> biter(bro), siter(sis);
  T *sptr, *bptr;
  for(int i=siter.size()-1; i>=0; i--) {
    sptr = siter.next();
    bptr = biter.next();
    if(sptr != 0 && bptr != 0)
      count += ((*sptr == *bptr) ? 0 : 1);
  }
  return count;
}








#define SWAP(a,b) {unsigned int tmp=a; a=b; b=tmp;}

// This crossover picks a site between nodes in each parent.  It is the same 
// as single point crossover on a resizeable binary string genome.  The site
// in the mother is not necessarily the same as the site in the father!
//   When we pick a crossover site, it is between nodes of the list (otherwise
// we won't be able to do NULL lists or get an empty list from one parent or
// the other).  Beware that a list is numbered from 0 to size-1, inclusive,
// whereas the cross site possibilities are numbered from 0 to size, inclusive.
// This means we have to map the site to the list to determine whether an
// insertion should occur before or after a node.
//   We first copy the mother into the child (this deletes whatever contents 
// were in the child originally).  Then we clone the father from the cross site
// to the end of the list.  Then we delete the tail of the child from the
// mother's cross site to the end of the list.  Finally, we insert the clone
// at the end of the child.
//   The last thing we do is delete the clone (the contents of the clone are
// now owned by the child, but the clone itself uses memory that we must free).
//   This implementation isn't particularly efficient.  For example, we copy
// the mother then proceed to destroy much of the copy we just made.  We could
// do better by copying only what we need of the mother.
template <class T> int
GAListGenome<T>::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2, 
		  GAGenome* c1, GAGenome* c2){
  const GAListGenome<T> &mom=DYN_CAST(const GAListGenome<T> &, p1);
  const GAListGenome<T> &dad=DYN_CAST(const GAListGenome<T> &, p2);

  int nc=0;
  int a = GARandomInt(0, mom.size());
  int b = GARandomInt(0, dad.size());
  GAList<T> * list;

  if(c1){
    GAListGenome<T> &sis=DYN_CAST(GAListGenome<T> &, *c1);
    sis.GAList<T>::copy(mom);
    list = dad.GAList<T>::clone(b);
    if(a < mom.size()){
      T *site = sis.warp(a);
      while(sis.tail() != site)
	sis.destroy();		// delete the tail node
      sis.destroy();		// trash the trailing node (list[a])
    }
    else{
      sis.tail();		// move to the end of the list
    }
    sis.insert(list);		// stick the clone onto the end
    delete list;
    sis.head();			// set iterator to head of list
    nc += 1;
  }

  if(c2){
    GAListGenome<T> &bro=DYN_CAST(GAListGenome<T> &, *c2);
    bro.GAList<T>::copy(dad);
    list = mom.GAList<T>::clone(a);
    if(b < dad.size()){
      T *site = bro.warp(b);
      while(bro.tail() != site)
	bro.destroy();		// delete the tail node
      bro.destroy();		// trash the trailing node (list[a])
    }
    else{
      bro.tail();		// move to the end of the list
    }
    bro.insert(list);		// stick the clone onto the end
    delete list;
    bro.head();			// set iterator to head of list
    nc += 1;
  }

  return nc;
}







// Partial match crossover for list genomes.  We need two versions of this
// routine:  one for lists whose nodes are pointers to objects (each genome
// points to the same objects as all of the other genomes) and one for
// lists whose nodes contain independent objects (each node has its own copy
// of the object).
//   This version of the partial match crossover uses objects that are multiply
// instantiated - each list genome contains its own objects in its nodes.
// The operator== method must be defined on the object for this implementation
// to work!  In this case, the 'object' is an int, so we're OK.  If you are 
// putting your own objects in the nodes, be sure you have operator== defined
// for your object.  You must also have operator!= defined for your object.  We
// do not do any assignments, so operator= and/or copy is not required.
//   We assume that none of the nodes will return a NULL pointer.  Also assume
// that the cross site has been selected properly.
//   First we make a copy of the mother.  Then we loop through the match 
// section and try to swap each element in the child's match section with its
// partner (as defined by the current node in the father's match section).
//   Mirroring will work the same way - just swap mom & dad and you're all set.
//   The parents should be the same size as the child (and they should contain
// the same nodes, but in any order).  We do not check for this!!
template <class T> int
GAListGenome<T>::
PartialMatchCrossover(const GAGenome& p1, const GAGenome& p2,
		     GAGenome* c1, GAGenome* c2){
  const GAListGenome<T> &mom=DYN_CAST(const GAListGenome<T> &, p1);
  const GAListGenome<T> &dad=DYN_CAST(const GAListGenome<T> &, p2);

  if(mom.size() != dad.size()){
    GAErr(GA_LOC, mom.className(), "cross", gaErrBadParentLength);
    return 0;
  }

  int a = GARandomInt(0, mom.size());
  int b = GARandomInt(0, dad.size());
  if(b<a) SWAP(a,b);
  T* index;
  int i,j,nc=0;

  if(c1){
    GAListGenome<T> &sis=DYN_CAST(GAListGenome<T> &, *c1);
    sis.GAList<T>::copy(mom);
    GAListIter<T> diter(dad);
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

  if(c2){
    GAListGenome<T> &bro=DYN_CAST(GAListGenome<T> &, *c2);
    bro.GAList<T>::copy(dad);
    GAListIter<T> miter(mom);
    index = miter.warp(a);
    for(i=a; i<b; i++, index=miter.next()){
      if(*bro.head() == *index){
	bro.swap(i,0);
      }
      else{
	for(j=1; (j<bro.size()) && (*bro.next() != *index); j++);
	bro.swap(i,j);  // no op if j>size
      }
    }
    bro.head();         // set iterator to head of list
    nc += 2;
  }

  return nc;
}





// Order crossover for lists.  As described in Goldberg's book.
//   We assume that we'll never get a NULL pointer while iterating through the
// list.  Also we assume that the lists are the same size and non-NULL.
template <class T> int
GAListIsHole(const GAListGenome<T> &child, const GAListGenome<T> &parent,
	     int index, int a, int b){
  GAListIter<T> citer(child), piter(parent);
  citer.warp(index);
  piter.warp(a);
  for(int i=a; i<b; i++){
    if(*citer.current() == *piter.current()) return 1;
    piter.next();
  }
  return 0;
}

template <class T> int
GAListGenome<T>::
OrderCrossover(const GAGenome& p1, const GAGenome& p2,
	       GAGenome* c1, GAGenome* c2){
  const GAListGenome<T> &mom=DYN_CAST(const GAListGenome<T> &, p1);
  const GAListGenome<T> &dad=DYN_CAST(const GAListGenome<T> &, p2);

  if(mom.size() != dad.size()){
    GAErr(GA_LOC, mom.className(), "cross", gaErrBadParentLength);
    return 0;
  }

  int a = GARandomInt(0, mom.size());
  int b = GARandomInt(0, dad.size());
  if(b<a) SWAP(a,b);
  int i,j, index, nc=0;

  if(c1){
    GAListGenome<T> &sis=DYN_CAST(GAListGenome<T> &, *c1);
    sis.GAList<T>::copy(mom);
    GAListIter<T> siter(sis);
    GAListIter<T> diter(dad);

// Move all the 'holes' into the crossover section and maintain the ordering of
// the non-hole elements.
    for(i=0, index=b; i<sis.size(); i++, index++){
      if(index >= sis.size()) index=0;
      if(GAListIsHole(sis,dad,index,a,b)) break;
    }
    for(; i<sis.size()-b+a; i++, index++){
      if(index >= sis.size()) index=0;
      j=index;
      do{
	j++;
	if(j >= sis.size()) j=0;
      } while(GAListIsHole(sis,dad,j,a,b));
      sis.swap(index,j);
    }

// Now put the 'holes' in the proper order within the crossover section.
    for(i=a, sis.warp(a), diter.warp(a);
	i<b; i++, sis.next(), diter.next()){
      if(*sis.current() != *diter.current()){
	siter.warp(i);
	for(j=i+1; j<b; j++)
	  if(*siter.next() == *diter.current()){
	    sis.swap(i,j);
	    sis.warp(siter);	// move iterator back to previous location
	    break;
	  }
      }
    }

    sis.head();		// set iterator to head of list
    nc += 1;
  }

  if(c2){
    GAListGenome<T> &bro=DYN_CAST(GAListGenome<T> &, *c2);
    bro.GAList<T>::copy(dad);
    GAListIter<T> biter(bro);
    GAListIter<T> miter(mom);

// Move all the 'holes' into the crossover section and maintain the ordering of
// the non-hole elements.
    for(i=0, index=b; i<bro.size(); i++, index++){
      if(index >= bro.size()) index=0;
      if(GAListIsHole(bro,mom,index,a,b)) break;
    }

    for(; i<bro.size()-b+a; i++, index++){
      if(index >= bro.size()) index=0;
      j=index;
      do{
	j++;
	if(j >= bro.size()) j=0;
      } while(GAListIsHole(bro,mom,j,a,b));
      bro.swap(index,j);
    }

// Now put the 'holes' in the proper order within the crossover section.
    for(i=a, bro.warp(a), miter.warp(a);
	i<b; i++, bro.next(), miter.next()){
      if(*bro.current() != *miter.current()){
	biter.warp(i);
	for(j=i+1; j<b; j++)
	  if(*biter.next() == *miter.current()){
	    bro.swap(i,j);
	    bro.warp(biter);	// move iterator back to previous location
	    break;
	  }
      }
    }
    bro.head();		// set iterator to head of list
    nc += 1;
  }

  return nc;
}











template <class T> int
GAListGenome<T>::
CycleCrossover(const GAGenome& p1, const GAGenome& p2,
	       GAGenome* c1, GAGenome* c2){
  const GAListGenome<T> &mom=DYN_CAST(const GAListGenome<T> &, p1);
  const GAListGenome<T> &dad=DYN_CAST(const GAListGenome<T> &, p2);

  if(mom.size() != dad.size()){
    GAErr(GA_LOC, mom.className(), "cross", gaErrBadParentLength);
    return 0;
  }

  GAMask mask;
  mask.size(mom.size());
  mask.clear();
  int i, nc=0;

  if(c1){
    GAListGenome<T> &sis=DYN_CAST(GAListGenome<T> &, *c1);
    sis.GAList<T>::copy(mom);
    GAListIter<T> diter(dad);

// Cycle through mom & dad to get the cyclic part of the crossover.
    mask[0] = 1;
    diter.head();
    while(*diter.current() != *sis.head()){
      for(i=0; i<sis.size(); i++, sis.next()){
	if(*sis.current() == *diter.current()){
	  mask[i] = 1;
	  diter.warp(i);
	  break;
	}
      }
    }

// Now fill in the rest of the sis with dad's contents that we didn't use
// in the cycle.
    sis.head();
    diter.head();
    for(i=0; i<sis.size(); i++){
      if(mask[i] == 0) *sis.current() = *diter.current();
      sis.next(); diter.next();
    }
    sis.head();		// set iterator to head of list
    nc += 1;
  }

  if(c2){
    GAListGenome<T> &bro=DYN_CAST(GAListGenome<T> &, *c2);
    bro.GAList<T>::copy(dad);
    GAListIter<T> miter(mom);

// Cycle through mom & dad to get the cyclic part of the crossover.
    mask[0] = 1;
    miter.head();
    while(*miter.current() != *bro.head()){
      for(i=0; i<bro.size(); i++, bro.next()){
	if(*bro.current() == *miter.current()){
	  mask[i] = 1;
	  miter.warp(i);
	  break;
	}
      }
    }

// Now fill in the rest of the bro with dad's contents that we didn't use
// in the cycle.
    bro.head();
    miter.head();
    for(i=0; i<bro.size(); i++){
      if(mask[i] == 0) *bro.current() = *miter.current();
      bro.next(); miter.next();
    }
    bro.head();		// set iterator to head of list
    nc += 1;
  }

  return nc;
}

#endif

// $Header$
/* ----------------------------------------------------------------------------
  listtmpl.h
  mbwall 25feb95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the template-ized list objects.
---------------------------------------------------------------------------- */
#ifndef _ga_listtmpl_h_
#define _ga_listtmpl_h_

#include <ga/gaerror.h>
#include <ga/GAListBASE.h>

/* ----------------------------------------------------------------------------
 GAList
-------------------------------------------------------------------------------
  This object is a container for nodes that have a list structure.  The base
list object is responsible for maintaining the list heirarchy.  This object is
responsible for doing the memory management (allocating and de-allocating the
nodes in the list).  We insulate the user entirely from nodes - when you use
a list, you don't get nodes back, you get the contents of nodes (ie the user
doesn't have to think about the list parts of a node, they can simply assume
that their data is organized into a list structure).
  We include an iterator in this object so that you can navigate through the
list.  You can create another iterator and assign it to your list so you can
have multiple iterators.
  All of the actions take place relative to the current location of the 
embedded iterator.  None of the iterators change the state of the list.  Be
careful so that you don't end up with an iterator dangling with a pointer to
a part of a list that no longer exists (I would need some kind of reference
counting and/or message passing to take care of this at a lower level, and I'm 
not ready to implement that at this point).
  For now we allocate nodes on the fly.  Eventually I would like to do some
better memory management (arrays perhaps?) so we don't have to do so much
alloc and dealloc and recursion.
  We depend on the template-ized GAListIter routine, thus the declaration.

copy
  Make a complete copy of the list and return a pointer to the new list.

remove
  Remove the current node from the list, free the memory used by the node, and
  return a pointer to a copy of the object that the node contained.

destroy
  Remove the current node from the list and free the memory it was using.
  Destroys the object that the node contained.

insert - list
  Inserts the contents of list in to the current list and removes it from the
  original list.  Does NOT delete the original list, but DOES assume
  responsibility for the memory used by the contents of the original list.

insert - object
  Insert the object into the list at the specified place relative to the
  current location of the embedded iterator.

current, head, tail, next, prev, warp
  These iterator methods are defined as access to the embedded iterator of the
  list.  Use these methods to move the insertion point and to traverse the 
  list.  You can also create other iterators for this list, but they won't
  affect the contents.
---------------------------------------------------------------------------- */
template <class T> class GAListIter;

template <class T>
class GAList : public GAListBASE {
public:
  GAList() : GAListBASE() { iter(*this); }
  GAList(const T & t):GAListBASE(new GANode<T>(t)), iter(*this) {}
  GAList(const GAList<T> & orig){iter(*this); copy(orig);}
  GAList<T> & operator=(const GAList<T> & orig)
    {if(&orig != this) copy(orig); return *this;}
  virtual ~GAList();
  GAList<T> * clone(unsigned int i=0) const;

// methods that modify the state of the list
  void copy(const GAList<T> & orig);
  int destroy();
  int swap(unsigned int, unsigned int);
  T * remove();
  int insert(GAList<T> * t, GAListBASE::Location where=GAListBASE::AFTER){ 
    if(this == t){
      GAErr(GA_LOC, "GAList", "insert", gaErrCannotInsertIntoSelf); 
      return GAListBASE::ERR;
    }
    if(GAListBASE::insert(t->hd, iter.node, where) == GAListBASE::ERR){
      return GAListBASE::ERR;
    }
    iter.node = (t->hd ? t->hd : iter.node);
    t->hd=(GANodeBASE *)0; t->iter.node=(GANodeBASE *)0;
    return GAListBASE::NO_ERR;
  }
  int insert(const T & t, GAListBASE::Location where=GAListBASE::AFTER){
    GANode<T> * c = new GANode<T>(t);
    if(GAListBASE::insert(c, iter.node, where) == GAListBASE::ERR){
      delete c;
      return GAListBASE::ERR;
    }
    iter.node = c;
    return GAListBASE::NO_ERR;
  }

// typesafes on iteration methods.  These call the built-in iterator then 
// return the contents of the now-current node.  They do not affect the state
// of the list.
  T * head(){return iter.head();}
  T * tail(){return iter.tail();}
  T * current(){return iter.current();}
  T * next(){return iter.next();}
  T * prev(){return iter.prev();}
  T * warp(unsigned int i){return iter.warp(i);}
  T * warp(const GAListIter<T> & i)
    {return((i.list == this) ? iter.warp(i) : (T *)0);}
  T * operator[](unsigned int i){return iter.warp(i);}

protected:
  int insert(GANode<T> *n, GANode<T> *idx,
	     GAListBASE::Location where=GAListBASE::AFTER){
    if(GAListBASE::insert(n, idx, where) == GAListBASE::ERR)
      return GAListBASE::ERR;
    iter.node = n;
    return GAListBASE::NO_ERR;
  }
  GAListIter<T> iter;	// the embedded iterator
  friend class GAListIter<T>;
};



/* ----------------------------------------------------------------------------
 GAListIter
-------------------------------------------------------------------------------
  This is a type-safe derivation of the base ListIter object.  I copied the
methods from the base class (I know, a no-no) rather than doing calls to the
base class methods.
  We depend on the template-ized GAList, thus the declaration.
  Behaviour for the iterator methods is defined as follows.  If the current
node is null, attempts to access a derived position from the current position
will return NULL.  The only way to reset the current node is to call the head()
locater (you always have to start at the list head to navigate the list).  If 
the current node is non-null and the derived node is null, the current node is
NOT changed, but NULL is returned.  You can also warp to a new position if you
have another iterator or a list with an embedded iterator.
  When we create a new list iterator, it defaults to the same node as the one
used to create it.  If it is created with a list as its argument, it defaults
to the list's iterator's current position.
---------------------------------------------------------------------------- */
template <class T> class GAList;

template <class T>
class GAListIter : public GAListIterBASE {
public:
  GAListIter() : GAListIterBASE(){}
  GAListIter(const GAList<T> & t) : GAListIterBASE(t){node=t.iter.node;}
  GAListIter(const GAListIter<T> & i) : GAListIterBASE(i){}
  T * current()
    {return(node ? 
	    &((GANode<T> *)node)->contents : (T *)0);}
  T * head()
    {return(((node=GAListIterBASE::head()) != (GANodeBASE *)0) ? 
	    &((GANode<T> *)GAListIterBASE::head())->contents : (T *)0);}
  T * tail()
    {return(((node=GAListIterBASE::tail()) != (GANodeBASE *)0) ? 
	    &((GANode<T> *)GAListIterBASE::tail())->contents : (T *)0);}
  T * next()
    {return((node && node->next) ? 
	    &((GANode<T> *)(node=node->next))->contents : (T *)0);}
  T * prev()
    {return((node && node->prev) ? 
	    &((GANode<T> *)(node=node->prev))->contents : (T *)0);}
  T * warp(const GAList<T> & t){
    list = &t;
    node = t.iter.node;
    return(t.iter.node ? &((GANode<T> *)(node=t.iter.node))->contents :(T *)0);
  }
  T * warp(const GAListIter<T> & i){
    list = i.list;
    node = i.node;
    return(i.node ? &((GANode<T> *)(node=i.node))->contents : (T *)0);
  }
  T * warp(unsigned int i){
    GANodeBASE * n = GAListIterBASE::warp(i);
    return(n ? &((GANode<T> *)(node=n))->contents : (T *)0);
  }

private:
  friend class GAList<T>;	// do I need to do this?
};

#ifdef GALIB_USE_BORLAND_INST
#include <ga/GAList.C>
#endif

#endif

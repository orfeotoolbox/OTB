// $Header$
/* ----------------------------------------------------------------------------
  treetmpl.h
  mbwall 25feb95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the templatized tree objects.
---------------------------------------------------------------------------- */
#ifndef _ga_treetmpl_h_
#define _ga_treetmpl_h_

#include <ga/gaerror.h>
#include <ga/GATreeBASE.h>

/* ----------------------------------------------------------------------------
 GATree
-------------------------------------------------------------------------------
  This object is a container for nodes that have a tree structure.  The base
tree object is responsible for maintaining the tree heirarchy.  This object is
responsible for doing the memory management (allocating and de-allocating the
nodes in the tree).  We insulate the user entirely from nodes - when you use
a tree, you don't get nodes back, you get the contents of nodes (ie the user
doesn't have to think about the tree parts of a node, they can simply assume
that their data is organized into a tree structure).
  We include an iterator in this object so that you can navigate through the
tree.  You can create another iterator and assign it to your tree so you can
have multiple iterators.
  All of the actions take place relative to the current location of the 
embedded iterator.  None of the iterators change the state of the tree.  Be
careful so that you don't end up with an iterator dangling with a pointer to
a part of a tree that no longer exists (I would need some kind of reference
counting and/or message passing to take care of this at a lower level, and I'm 
not ready to implement that at this point).
  For now we allocate nodes on the fly.  Eventually I would like to do some
better memory management (arrays perhaps?) so we don't have to do so much
alloc and dealloc and recursion.
  We depend on the template-ized GATreeIter routine, thus the declaration.

copy
  Make a copy of the specified tree.  Iterator goes to root node (should go to
  appropriate node in copy, but we don't do that yet).

clone
  Allocate space and make a copy of the tree and return a pointer to the new
  one.  The iterator of the original is not affected.  The iterator of the
  clone is set to the appropriate place in the clone.  If you specify a node
  index when you call clone then a clone of the subtree is made and the 
  iterator in the clone is set to the root node (the top of the subtree).

remove
  Remove the current node (and its subtree) from the tree and stick it into a
  new tree.  Returns a pointer to the new tree.  Leaves the original iterator
  pointing to the eldest child or parent of the node that was removed.  Iter
  of the new tree points to the root node.

destroy
  Destroys the node and subtree where the iterator is currently pointing. 
  Moves the iterator to the eldest sibling or parent of the node that was 
  deleted from the tree.

swap
  Swap nodes in a tree, leaves the nodes' subtrees in place (subtrees do not
  move with the nodes in the swap).

swaptree - tree
  Swap subtrees referenced by the iterators of this and the second tree.  The
  iterators are reset to point to the new subtrees (same point in the trees,
  but different nodes due to the swap).

swaptree - indices
  Swap the subtrees referenced by the integer values.  Indices must not be
  related (ie one cannot be ancestor of the other).  Iterator is not changed.
  
insert - tree
  Inserts the contents of tree in to the current tree and removes it from the
  original tree.  Does NOT delete the original tree, but DOES assume 
  responsibility for the memory used by original tree contents.

insert - object
  Inserts the object into a new node relative to the location of the iterator

root, current, next, prev, parent, child, warp
  These iterator methods are defined as access to the embedded iterator of the
  tree.  Use these methods to move the insertion point and to traverse the 
  tree.  You can also create other iterators for this tree, but they won't
  affect the contents.
---------------------------------------------------------------------------- */
template <class T> class GATreeIter;

template <class T>
class GATree : public GATreeBASE {
public:
  GATree() : GATreeBASE() { iter(*this); }
  GATree(const T & t) : GATreeBASE(new GANode<T>(t)), iter(*this) {}
  GATree(const GATree<T> & orig){iter(*this); copy(orig);}
  GATree<T> & operator=(const GATree<T> & orig)
    {if(&orig != this) copy(orig); return *this;}
  virtual ~GATree();
  GATree<T> * clone(unsigned int i=0) const;

// methods that modify the state of the tree
  void copy(const GATree<T> & orig);
  int destroy();
  int swaptree(GATree<T> * t);
  int swaptree(unsigned int, unsigned int);
  int swap(unsigned int, unsigned int);
  GATree<T> * remove();
  int insert(GATree<T> * t, GATreeBASE::Location where=GATreeBASE::BELOW){
    if(this == t){
      GAErr(GA_LOC, "GATree", "insert", gaErrCannotInsertIntoSelf); 
      return GATreeBASE::ERR;
    }
    if(GATreeBASE::insert(t->rt, iter.node, where) == GATreeBASE::ERR){
      return GATreeBASE::ERR;
    }
    iter.node = (t->rt ? t->rt : iter.node);
    t->rt=(GANodeBASE *)0; t->iter.node=(GANodeBASE *)0;
    return GATreeBASE::NO_ERR;
  }
  int insert(const T & t, GATreeBASE::Location where=GATreeBASE::BELOW){
    GANode<T> * c = new GANode<T>(t);
    if(GATreeBASE::insert(c, iter.node, where) == GATreeBASE::ERR){
      delete c;
      return GATreeBASE::ERR;
    }
    iter.node = c;
    return GATreeBASE::NO_ERR;
  }

// typesafes on iteration methods.  These call the built-in iterator then 
// return the contents of the now-current node.  They do not affect the state
// of the tree.
  T * root(){return iter.root();}
  T * current(){return iter.current();}
  T * next(){return iter.next();}
  T * prev(){return iter.prev();}
  T * parent(){return iter.parent();}
  T * child(){return iter.child();}
  T * eldest(){return iter.eldest();}
  T * youngest(){return iter.youngest();}
  T * warp(unsigned int i){return iter.warp(i);}
  T * warp(const GATreeIter<T> & i)
    {return((i.tree == this) ? iter.warp(i) : (T *)0);}
  int nchildren(){return iter.nchildren();}
  int nsiblings(){return iter.nsiblings();}

protected:
  int insert(GANode<T> *n, GANode<T> *idx,
	     GATreeBASE::Location where=GATreeBASE::BELOW){
    if(GATreeBASE::insert(n, idx, where) == GATreeBASE::ERR)
      return GATreeBASE::ERR;
    iter.node = n;
    return GATreeBASE::NO_ERR;
  }
  GATreeIter<T> iter;
  friend class GATreeIter<T>;
};



/* ----------------------------------------------------------------------------
 GATreeIter
-------------------------------------------------------------------------------
  This is a type-safe derivation of the base TreeIter object.  I copied the
methods from the base class (I know, a no-no) rather than doing calls to the
base class methods.
  We depend on the template-ized GATree, thus the declaration.
  Behaviour for the iterator methods is defined as follows.  If the current
node is null, attempts to access a derived position from the current position
will return NULL.  The only way to reset the current node is to call the root()
locater (you always have to start at the tree root to navigate the tree).  If 
the current node is non-null and the derived node is null, the current node is
NOT changed, but NULL is returned.
  When we create a new tree iterator, it defaults to the same node as the one
used to create it.  If it is created with a tree as its argument, it defaults
to the tree's iterator's current position.
---------------------------------------------------------------------------- */
template <class T> class GATree;

template <class T>
class GATreeIter : public GATreeIterBASE {
public:
  GATreeIter() : GATreeIterBASE(){}
  GATreeIter(const GATree<T> & t) : GATreeIterBASE(t){node=t.iter.node;}
  GATreeIter(const GATreeIter<T> & i) : GATreeIterBASE(i){}
  T * current()
    {return(node ? 
	    &((GANode<T> *)node)->contents : (T *)0);}
  T * root()
    {return(((node=GATreeIterBASE::root()) != (GANodeBASE *)0) ? 
	    &((GANode<T> *)GATreeIterBASE::root(node))->contents : (T *)0);}
  T * next()
    {return((node && node->next) ? 
	    &((GANode<T> *)(node=node->next))->contents : (T *)0);}
  T * prev()
    {return((node && node->prev) ? 
	    &((GANode<T> *)(node=node->prev))->contents : (T *)0);}
  T * parent()
    {return((node && node->parent) ? 
	    &((GANode<T> *)(node=node->parent))->contents : (T *)0);}
  T * child()
    {return((node && node->child) ? 
	    &((GANode<T> *)(node=node->child))->contents : (T *)0);}
  T * eldest()
    {return(node ? &((GANode<T> *)GATreeIterBASE::eldest(node))->contents :
	    (T *)0);}
  T * youngest()
    {return(node ? &((GANode<T> *)GATreeIterBASE::youngest(node))->contents : 
	    (T *)0);}
  T * warp(const GATree<T> & t){
    tree = &t;
    node = t.iter.node;
    return(t.iter.node ? &((GANode<T> *)(node=t.iter.node))->contents :(T *)0);
  }
  T * warp(const GATreeIter<T> & i){
    tree = i.tree;
    node = i.node;
    return(i.node ? &((GANode<T> *)(node=i.node))->contents : (T *)0);
  }
  T * warp(unsigned int i){
    GANodeBASE * n = GATreeIterBASE::warp(i);
    return(n ? &((GANode<T> *)(node=n))->contents : (T *)0);
  }

private:
  friend class GATree<T>;
};


#ifdef GALIB_USE_BORLAND_INST
#include <ga/GATree.C>
#endif

#endif

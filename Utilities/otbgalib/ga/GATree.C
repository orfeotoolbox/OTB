// $Header$
/* ----------------------------------------------------------------------------
  treetmpl.C
  mbwall 25feb95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the templatized tree objects.

 TO DO:
  Make insert work better with size and depth so not so many recalcs needed.
  Implement better memory mangement, faster allocation, referencing.
  Use array representation of nodes so we don't have to do so much recursion.
  Figure better way to do traversals so that we speed up the swaps.
---------------------------------------------------------------------------- */
#ifndef _ga_treetmpl_C_
#define _ga_treetmpl_C_

#include <ga/GATree.h>

extern GANodeBASE * _GATreeTraverse(unsigned int, unsigned int&, GANodeBASE *);
//template <class T> void _GATreeDestroy(GANode<T> *);
//template <class T> GANode<T> * _GATreeCopy(GANode<T> *, GANode<T> *);


/* ----------------------------------------------------------------------------
Tree
---------------------------------------------------------------------------- */
// The destructor just goes through the tree and deletes every node.  As with
// any method that uses the BASE tree, we have to use its members so it doesn't
// get messed up.
template <class T>
GATree<T>::~GATree()
{
  _GATreeDestroy(DYN_CAST(GANode<T>*, rt));
  iter.node = (GANodeBASE *)0;
}


// Yes, this is really ugly.  We do a complete destruction of the existing tree
// then we copy the new one.  No caching, no nothing.  Oh well.  The iterator
// is set to the root node - it should be set to the corresponding node, but
// I won't do that for now.  THIS IS A BUG!
template <class T> void
GATree<T>::copy(const GATree<T> & orig)
{
  _GATreeDestroy(DYN_CAST(GANode<T>*, rt));
  rt = _GATreeCopy(DYN_CAST(GANode<T>*, orig.rt), (GANode<T> *)0);
  iter.node = rt;
  sz=orig.sz; csz=orig.csz;
  dpth=orig.dpth; cdpth=orig.cdpth;
}


// This remove method returns a tree with the removed node as its root.  The
// node we remove is the current node.  We allocate memory for the tree, but
// we don't allocate any memory for the node or its children.  That is taken
// from the previous (this) tree and it no longer has to worry about it.  It
// is the responsibility of the new tree to delete that memory.
//   The iterator gets set to the elder child of the node that was removed.  If
// there is no elder child, then it gets set to the parent.  If no parent, then
// it gets set to NULL.
//   Thank you to Uli Bubenheimer (uli@aisun1.ai.uga.edu) for the bug fix here.
// I forgot to fix the pointers in the root node of the sub-tree.
template <class T> GATree<T> *
GATree<T>::remove()
{
  GATree<T> * t = new GATree<T>;
  GANode<T> * node = DYN_CAST(GANode<T>*, iter.node);
  if(!node) return t;

  if(node->prev != node) iter.eldest();
  else if(node->parent) iter.parent();
  else iter.node = (GANodeBASE *)0;

  GANode<T> * tmpnode = DYN_CAST(GANode<T>*, GATreeBASE::remove(node));
  tmpnode->prev = tmpnode;
  tmpnode->next = tmpnode;
  tmpnode->parent = (GANodeBASE *)0;

  t->insert(tmpnode, (GANode<T> *)0, GATreeBASE::ROOT);

  return t;
}


// Recursively duplicate a subtree given a base node.  This uses the _copy
// method (which does a deep and wide copy).  Here we just copy a node, then
// sic the _copy method on the child if it exists.  The parent of the new node
// is set to NULL - this makes it a root node in the new tree object.
//   We do the cloning based on the valued passed to the routine.  0 is the
// root node and makes a clone of the entire tree.  This routine has no effect
// on the iterator in the original tree.
//   The iterator in the clone is left pointing to the root node of the clone.
template <class T> GATree<T> *
GATree<T>::clone(unsigned int i) const
{
  GATree<T> * t = new GATree<T>;
  GANode<T> * node;
  unsigned int w=0;
  if(i == 0) node = DYN_CAST(GANode<T>*, rt);
  else node = DYN_CAST(GANode<T>*, _GATreeTraverse(i, w, rt));
  if(!node) return t;

  GANode<T> * newnode = new GANode<T>(node->contents);
  newnode->child = _GATreeCopy(DYN_CAST(GANode<T>*, node->child), newnode);
  if(newnode->child) newnode->child->parent = newnode;

  t->insert(newnode, (GANode<T> *)0, GATreeBASE::ROOT);

  return t;
}


// Destroy the specified node and all nodes attached to it looking downward. 
// This does NOT destroy any nodes above the specified node.  If this node is
// in a tree, it will be removed before the nuking occurs.  This gives the tree
// object a chance to flag for any recalculations it might need.  The destroy
// method effect on the tree as a remove, but it is destructive (it frees up 
// the memory as well).
//   We do the nuking recursively, so its not really that efficient.  I'll 
// figure out a better way to track these nodes one of these days.
//   We use the _destroy routine to do the recursion.  _destroy kills all of 
// the siblings of the node whereas this routine kills only descendents.
//   This uses the current node as the one to destroy, so be sure to use the
// iteration methods to move to the node you want to destroy.  Once the node is
// gone, we set the current node to the eldest child or parent of the node that
// was destroyed.
template <class T> int
GATree<T>::destroy()
{
  GANodeBASE * node = iter.node;
  if(!node) return GATreeBASE::NO_ERR;
  if(node->prev == node || !node->prev)
    if(node->parent) iter.node = node->parent;
    else iter.node = (GANodeBASE *)0;
  else 
    iter.eldest();
  _GATreeDestroy(DYN_CAST(GANode<T>*, node->child));
  delete GATreeBASE::remove(node);
  return GATreeBASE::NO_ERR;
}


// Swap two subtrees.  We use the nodes pointed to by the iterators in the
// current tree and the one that was passed to us.  The TreeBASE swaptree
// changes the next, prev, parent, child pointers on the nodes we pass it as
// well as the nodes that those nodes point to.
//   Notice that this routine uses the current location of the iterators to
// do its job, so be sure to set them properly before you call this routine!
//   The iterators are reset to the nodes where the swaps occurred.  Sizes and
// depths are possibly changed - the insert method flags them for a recalc.
//   If an iterator is NULL then we do an insert ONLY if the root node of that
// iterator's tree is NULL.  If the tree's root is non-NULL, we don't do 
// anything (most likely the iterator was unset or badly set).
template <class T> int
GATree<T>::swaptree(GATree<T> * t)
{
  GANodeBASE * tmp;
  if(t->iter.node && iter.node){
    if(GATreeBASE::swaptree(t->iter.node, iter.node) == GATreeBASE::ERR)
      return GATreeBASE::ERR;
    if(t->rt == t->iter.node) t->rt = iter.node;
    tmp = t->iter.node;
    t->iter.node = iter.node;
    iter.node = tmp;

    t->csz = 1; t->cdpth = 1;	// remember to flag the changes in t!
    csz = 1; cdpth = 1;
  }
  else if(t->iter.node){	// iter.node is NULL, so we have no root
    if(!rt){			// should always be true at this point
      tmp = t->GATreeBASE::remove(t->iter.node);
//      tmp->next = tmp;
//      tmp->prev = tmp;
      t->iter.node = (GANodeBASE *)0;
      if(insert(DYN_CAST(GANode<T>*, tmp), (GANode<T> *)0, GATreeBASE::ROOT) ==
	 GATreeBASE::ERR)
	return GATreeBASE::ERR;
    }
  }
  else if(iter.node){		// t's iter is NULL, so it has no root
    if(!t->rt){			// should always be true
      tmp = GATreeBASE::remove(iter.node);
//      tmp->next = tmp;
//      tmp->prev = tmp;
      iter.node = (GANodeBASE *)0;
      if(t->insert(DYN_CAST(GANode<T>*,tmp),(GANode<T>*)0,GATreeBASE::ROOT) ==
	 GATreeBASE::ERR)
	return GATreeBASE::ERR;
    }
  }
// else both t->iter.node and iter.node are NULL, so don't do anything
  return GATreeBASE::NO_ERR;
}


// Same as the swaptree above, but this routine uses the node indices to do
// the swap.  This can be dangerous:  if one of the nodes is a decendent of the
// other then we could end up with a fragmented tree, so we'll have to check 
// for that situation.  Unfortunately this check slows things down quite a bit.
// If one is the ancestor of the other, then we don't do the swap.
//   This routine does not affect the size of the tree, but it could change the
// depth of the tree.  We leave the iterator where it was pointing before the
// swap.
template <class T> int
GATree<T>::swaptree(unsigned int a, unsigned int b)
{
  unsigned int aw=0, bw=0;
  GANodeBASE * anode = _GATreeTraverse(a, aw, rt);
  GANodeBASE * bnode = _GATreeTraverse(b, bw, rt);
  return GATreeBASE::swaptree(anode,bnode);
}


// Swap two nodes in a tree, leave their subtrees intact.  This routine does
// not affect the iterator or the size or depth of the tree.
template <class T> int
GATree<T>::swap(unsigned int a, unsigned int b)
{
  unsigned int aw=0, bw=0;
  GANodeBASE * anode = _GATreeTraverse(a, aw, rt);
  GANodeBASE * bnode = _GATreeTraverse(b, bw, rt);
  return GATreeBASE::swapnode(anode,bnode);
}







/* ----------------------------------------------------------------------------
Recursive routines for the Tree objects
---------------------------------------------------------------------------- */
// Recursively copy a node, including all of its siblings.  This routine copies
// a row, then it calls itself to copy the next generation if it finds a next
// generation in the next node.
template <class T> GANode<T> *
_GATreeCopy(GANode<T> * node, GANode<T> * parent)
{
  if(!node) return (GANode<T> *)0;

  GANode<T> * newnode = new GANode<T>(node->contents);
  newnode->parent = parent;
  newnode->child = _GATreeCopy(DYN_CAST(GANode<T>*, node->child), newnode);

  GANode<T> *lasttmp = newnode, *newtmp = (GANode<T> *)0;
  GANode<T> *tmp = DYN_CAST(GANode<T>*, node->next);
  while(tmp && tmp != node){
    newtmp = new GANode<T>(tmp->contents);
    newtmp->parent = parent;
    newtmp->child = _GATreeCopy(DYN_CAST(GANode<T>*, tmp->child), newtmp);
    newtmp->prev = lasttmp;
    lasttmp->next = newtmp;

    lasttmp = newtmp;
    tmp = DYN_CAST(GANode<T>*, tmp->next);
  }

  if(newtmp){
    newtmp->next = newnode;
    newnode->prev = newtmp;
  }
  else{
    newnode->next = newnode;
    newnode->prev = newnode;
  }

  return newnode;
}


// This routine destroys the specified node, its children, its siblings, and
// all of their children, their childrens' siblings, etc.  Since we kill off
// all of the siblings, we need to set the parent's link to its child to NULL.
template <class T> void
_GATreeDestroy(GANode<T> * node)
{
  if(!node) return;

  if(node->parent)
    node->parent->child = (GANodeBASE *)0;
  _GATreeDestroy(DYN_CAST(GANode<T>*, node->child));

  GANodeBASE * tmp;
  while(node->next && node->next != node){
    tmp = node->next;
    node->next = tmp->next;
    tmp->next->prev = node;
    _GATreeDestroy(DYN_CAST(GANode<T>*, tmp->child));
    delete tmp;
  }
  delete node;
}

#endif

// $Header$
/* ----------------------------------------------------------------------------
  listtmpl.C
  mbwall 25feb95
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the templatized list objects.

 TO DO:
  Make insert work better with size and depth so not so many recalcs needed.
  Implement better memory mangement, faster allocation, referencing.
  Use array representation of nodes so we don't have to do so much recursion.
---------------------------------------------------------------------------- */
#ifndef _ga_listtmpl_C_
#define _ga_listtmpl_C_

#include <ga/GAList.h>

extern GANodeBASE * _GAListTraverse(unsigned int index, unsigned int & cur,
				    GANodeBASE * node);
//template <class T> GANode<T> * _GAListCopy(GANode<T> *, GANode<T> *);


/* ----------------------------------------------------------------------------
List
---------------------------------------------------------------------------- */
// The destructor just goes through the list and deletes every node.
template <class T>
GAList<T>::~GAList()
{
  while(hd) delete GAListBASE::remove(DYN_CAST(GANode<T>*, hd));
  iter.node = (GANodeBASE *)0;
}


// Yes, this is really ugly.  We do a complete destruction of the existing list
// then we copy the new one.  No caching, no nothing.  Oh well.  We set the
// iterator to the head node - it should be set to the corresponding node, but
// I won't do that right now.  THIS IS A BUG!
template <class T> void
GAList<T>::copy(const GAList<T> & orig)
{
  while(hd) delete GAListBASE::remove(DYN_CAST(GANode<T>*, hd));
  hd = _GAListCopy(DYN_CAST(GANode<T>*, orig.hd), 
		   DYN_CAST(GANode<T>*, orig.hd));
  iter.node = hd;
  sz = orig.sz; csz = orig.csz;
}


// This remove method returns a pointer to the contents of the node that was
// removed.  The node itself is destroyed.
//   The iterator gets set to the next node toward the head of the list.
//   This routine makes a copy of the node contents using the copy initializer
// of the T object, so the copy initializer MUST be defined and accessible.
//   We return a pointer to the contents rather than the contenst for the same
// reason we return a pointer from all the iter routines - we don't want to
// pass big objects around.
template <class T> T *
GAList<T>::remove()
{
  GANode<T> * node = DYN_CAST(GANode<T>*, iter.node);
  if(!node) return (T *)0;

  if(node->prev != node) iter.node = node->prev;
  else iter.node = (GANodeBASE *)0;
  node = DYN_CAST(GANode<T>*, GAListBASE::remove(node));
  T * contents = new T(node->contents);
  delete node;
  return contents;
}


// Make a copy of a list and return the pointer to the new list.  The cloning
// is based on the value passed to this routine.  A value of 0 will clone the
// entire list.  Any other value will clone the list from the index to the end
// of the list.  This routine has no effect on the iterator in the original
// list.
template <class T> GAList<T> *
GAList<T>::clone(unsigned int i) const
{
  GAList<T> * t = new GAList<T>;
  GANode<T> * node;
  unsigned int w=0;
  if(i == 0) node = DYN_CAST(GANode<T>*, hd);
  else node = DYN_CAST(GANode<T>*, _GAListTraverse(i, w, hd));
  if(!node) return t;

  GANode<T> * newnode = _GAListCopy(node, DYN_CAST(GANode<T>*, hd));

  t->insert(newnode, (GANode<T> *)0, GAListBASE::HEAD);

// need to set iterator to right spot in the clone!!  for now its at the head

  return t;
}


// Destroy the specified node.  This uses the current node as the one to 
// destroy, so be sure to use the iteration methods to move to the node you 
// want to destroy.  Once the node is gone, we set the current node to the
// prev node of the one that was destroyed.  If the node that was nuked was the
// head node then we set the current node to the new head.
template <class T> int
GAList<T>::destroy()
{
  GANodeBASE * node = iter.node;
  if(!node) return GAListBASE::NO_ERR;
  if(node->prev && node->prev != node)
    if(hd == node) iter.node = node->next;
    else iter.node = node->prev;
  else
    iter.node = (GANodeBASE *)0;
  delete GAListBASE::remove(node);
  return GAListBASE::NO_ERR;
}


// Swap two nodes in the list.  This has no effect on the size or the iterator.
// If either index is out of bounds then we don't do anything.
template <class T> int
GAList<T>::swap(unsigned int a, unsigned int b)
{
  if(a == b || a > (unsigned int)size() || b > (unsigned int)size())
    return GAListBASE::NO_ERR;
  GANodeBASE *tmp=hd, *anode=(GANodeBASE *)0, *bnode=(GANodeBASE *)0;
  unsigned int cur=0;
  while(tmp && tmp->next != hd){
    if(a == cur) anode = tmp;
    if(b == cur) bnode = tmp;
    tmp = tmp->next;
    cur++;
  }
  if(a == cur) anode = tmp;
  if(b == cur) bnode = tmp;
  return GAListBASE::swapnode(anode,bnode);
}




/* ----------------------------------------------------------------------------
Utility routines for the List objects
---------------------------------------------------------------------------- */
// Copy a node, including all of its siblings up to the end of the list.  We do
// NOT wrap around the list until we return the first node - we stop at the
// tail of the list.  Return the pointer to the first node in the list.
template <class T> GANode<T> *
_GAListCopy(GANode<T> * node, GANode<T> * head)
{
  if(!node) return (GANode<T> *)0;
  GANode<T> *newnode = new GANode<T>(node->contents);
  GANode<T> *lasttmp = newnode, *newtmp = (GANode<T> *)0;
  GANode<T> *tmp = DYN_CAST(GANode<T>*, node->next);
  while(tmp && tmp != head){
    newtmp = new GANode<T>(tmp->contents);
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

#endif

// $Header$
/* ----------------------------------------------------------------------------
  listbase.C
  mbwall 10dec94
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the list objects.

 TO DO:
  Make insert work better with size and depth so not so many recalcs needed.
  Implement better memory mangement, faster allocation, referencing.
  Use array representation of nodes so we don't have to do so much recursion.
---------------------------------------------------------------------------- */
#include <ga/GAListBASE.h>
#include <ga/gaerror.h>

GANodeBASE * _GAListTraverse(unsigned int index, unsigned int & cur,
			     GANodeBASE * node);


/* ----------------------------------------------------------------------------
ListBASE
---------------------------------------------------------------------------- */
// Insert node n into the list relative to node idx.  There are three different
// insertion modes:  head, before, and after.  If there is no head node,
// then the node n becomes the head no matter what.  
//   We have to check to see if the node that is going in is an isolated node
// or contains other nodes.  If it is linked to other nodes, then we put the
// entire set of nodes into the list.  If the nodes' prev and next are NULL or
// point to itself, then we stick only it into the list.
//   head:  idx is ignored.  The previous head pointer becomes 'next' to the
// new head.
//   before:  If node idx is the first in a row in the list, then n becomes 
// the first in the row.  If idx is the head node, then we post an error 
// message (can't have more than one head node).
//   after:  If the node idx is the last in a row, n becomes the last node.  If
// idx is the head node, we post an error.
int
GAListBASE::insert(GANodeBASE * n, GANodeBASE * idx, Location where)
{
  if(!n) return NO_ERR;
  if(!idx){
    if(!hd) where = HEAD;
    else if(where != HEAD && where != TAIL){
      GAErr(GA_LOC, "GAListBASE", "insert", gaErrCannotInsertOnNilNode);
      return ERR;
    }
  }

  GANodeBASE * last = (n->prev && n->prev != n) ? n->prev : n;

  switch(where){
  case HEAD:
    if(hd){
      last->next = hd;
      n->prev = hd->prev;
      hd->prev->next = n;
      hd->prev = last;      
    }
    hd = n;
    if(!hd->next) hd->next = hd;
    if(!hd->prev) hd->prev = hd;
    break;

  case TAIL:
    if(hd){
      last->next = hd;
      n->prev = hd->prev;
      hd->prev->next = n;
      hd->prev = last;
    }
    else{
      hd = n;
      if(!hd->next) hd->next = hd;
      if(!hd->prev) hd->prev = hd;
    }
    break;

  case BEFORE:
    last->next = idx;
    n->prev = idx->prev;
    idx->prev->next = n;
    idx->prev = last;
    break;

  case AFTER:
    n->prev = idx;
    last->next = idx->next;
    idx->next->prev = last;
    idx->next = n;
    break;

  default:
    GAErr(GA_LOC, "GAListBASE", "insert", gaErrBadWhereIndicator);
    break;
  }

  csz = 1;
  return NO_ERR;
}


// Remove the specified node from the list.  We don't cruise through the list
// to make certain that the node is in the list.  But we do check to make sure
// that the connections were ok before we prune the node.  If there is any
// problem with the links, we return a NULL.  If we get a NULL node, then we
// don't do anything.
//   We don't do anything to the next, prev, etc links of the node that is
// being removed (they are left pointing to where they used to point) so be
// careful!!
//   If the removal is on the head node, set the head node to the next node.
GANodeBASE *
GAListBASE::remove(GANodeBASE * n)
{
  if(!n) return (GANodeBASE *)0;

  if(n->next && n->next != n){	// reset links on neighbors if they exist
    n->prev->next = n->next;
    n->next->prev = n->prev;
  }
  if(n == hd){			// reset head node if needed
    if(n->next != n) hd = n->next;
    else hd = (GANodeBASE *)0;
  }

// uncomment these to modify the node that is getting removed
//  n->prev = n;
//  n->next = n;

  csz = 1;
  return n;
}


// Swap the specified nodes.  If we get NULL nodes or the nodes are the same,
// don't do anything.  This moves the two nodes.  Note
// that swapping two nodes shouldn't change the size or depth of the list, so
// we don't set the change flags.
//   If either of the nodes is the head of the list, we change the head to 
// point to the node that got swapped.  Note that if you pass nodes that are
// in different trees and one of the nodes is a head node, you'll lose an 
// entire list!
//   There is a special case we must handle when the nodes are adjacent.  We 
// assume that the nodes are intact - we don't check for bogus node 
// configurations.  We assume that there are no NULL prev or next pointers on
// either node.
//   For now we assume that both nodes are in the same list.  If we ever
// implement a swap that operates on two lists then we'll have to change
// that...
int
GAListBASE::swapnode(GANodeBASE * a, GANodeBASE * b)
{
  if(!a || !b || a == b) return NO_ERR;

  GANodeBASE * aprev = a->prev;
  GANodeBASE * anext = a->next;
  GANodeBASE * bprev = b->prev;
  GANodeBASE * bnext = b->next;

  if(anext == b && bnext == a){
// do nothing - check for head/tail at end
  }
  else if(anext == b){		// same as b->prev == a
    a->prev = b;
    a->next = bnext;
    b->prev = aprev;
    b->next = a;
    aprev->next = b;
    bnext->prev = a;
  }
  else if(bnext == a){		// same as a->prev == b
    a->prev = bprev;
    a->next = b;
    b->prev = a;
    b->next = anext;
    anext->prev = b;
    bprev->next = a;
  }
  else{				// a and b are not adjacent
    if(bprev == b || bnext == b){
      a->prev = a;
      a->next = a;
    }
    else{
      a->prev = bprev;
      a->next = bnext;
      bprev->next = a;
      bnext->prev = a;
    }
    if(aprev == a || anext == a){
      b->prev = b;
      b->next = b;
    }
    else{
      b->prev = aprev;
      b->next = anext;
      aprev->next = b;
      anext->prev = b;
    }
  }

  if(hd == a) hd = b;		// this only works if they're in the same list!
  else if(hd == b) hd = a;

  return NO_ERR;
}


// Return the number of nodes in the list.  We do a complete traversal of the
// list and count the number of nodes that we encounter.  Could do this breadth
// first or depth first - doesn't really matter.  We have to traverse the 
// entire list to do the count.
//   We have to do a little work-around here to get through the const-ness of
// the size method.  Its ok to call size on a const object because it does not
// modify the logical state of the object.  It does, however, modify the
// physical state of the object.  So to work around the strictness of the 
// const specifier, we do a little pointer magic and cast this to be non-const.
int
GAListBASE::size() const
{
  if(!csz) return sz;
  GAListBASE * This = CON_CAST(GAListBASE *, this);
  This->csz = 0;
  if(hd == (GANodeBASE *)0) return This->sz=0;
  GANodeBASE * tmp = hd;
  This->sz=1;
  while(tmp && tmp->next != hd){
    tmp = tmp->next;
    This->sz++;
  }
  return This->sz;
}





/* ----------------------------------------------------------------------------
ListIterBASE
---------------------------------------------------------------------------- */
// Set the current node to the node indexed by the integer x.  If x is out of
// bounds, we don't do anything to the state of the iterator and return NULL.
GANodeBASE *
GAListIterBASE::warp(unsigned int x)
{
  unsigned int cur=0;
  return(_GAListTraverse(x, cur, GAListIterBASE::head()));
}



/* ----------------------------------------------------------------------------
utilities
---------------------------------------------------------------------------- */
// Traverse the list until we come to the indexed node.  Return NULL if 
// cur != index.  We count relative to the node that is passed to this routine
// so to get an absolute count, pass 0 for 'cur' and the root node for 'node'.
//   If the index is out of bounds, we return NULL.
GANodeBASE *
_GAListTraverse(unsigned int index, unsigned int & cur, GANodeBASE * node)
{
  if(!node) return (GANodeBASE *)0;
  GANodeBASE * tmp = node;
  while(cur < index && tmp && tmp->next != node){
    tmp = tmp->next;
    cur++;
  }
  if(cur != index) tmp = 0;	// we didn't find the right node
  return tmp;
}

// $Header$
/* ----------------------------------------------------------------------------
  treebase.C
  mbwall 25nov94
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the tree objects.

 TO DO:
  Make insert work better with size and depth so not so many recalcs needed.
  Implement better memory mangement, faster allocation, referencing.
  Use array representation of nodes so we don't have to do so much recursion.
  Figure better way to do traversals so that we speed up the swaps.
---------------------------------------------------------------------------- */
#include <ga/GATreeBASE.h>
#include <ga/gaerror.h>

static int _GATreeSize(GANodeBASE *);
static int _GATreeDepth(GANodeBASE *);
GANodeBASE * _GATreeTraverse(unsigned int, unsigned int &, GANodeBASE *);
int _GATreeCompare(GANodeBASE * anode, GANodeBASE * bnode);


// Traverse up the tree until we find the root node.  If the node we got was
// NULL, return NULL.  Otherwise return the pointer to the root node.
inline GANodeBASE * _GARootOfNode(GANodeBASE * n){
  GANodeBASE * tmp = (GANodeBASE *)0;
  if((tmp=n) != (GANodeBASE *)0){while(tmp->parent) tmp = tmp->parent;}
  return tmp;
}


/* ----------------------------------------------------------------------------
TreeBASE
---------------------------------------------------------------------------- */
// Insert node n into the tree relative to node idx.  There are four different
// insertion modes:  root, before, after, and below.  If there is no root node,
// then the node n becomes the root no matter what.  If there is no root then
// we don't do anything.
//   If the node that you are inserting has any siblings then the siblings will
// be lost!  Therefore we post an error message and return without doing
// anything.
//   If you try to replace the existing root node, the previous root node
// becomes the eldest child of the node that is being inserted.
//   root:  idx is ignored.  Beware that the previous root ptr is lost (unless
// you keep you own copy of it somewhere else).
//   before:  If node idx is the first in a row in the tree, then n becomes 
// the first in the row.  If idx is the root node, then we post an error 
// message (can't have more than one root node).
//   after:  If the node idx is the last in a row, n becomes the last node.  If
// idx is the root node, we post an error.
//   below:  This is the default operation.  This is the only valid operation 
// on a root node.
int
GATreeBASE::insert(GANodeBASE * n, GANodeBASE * idx, Location where)
{
  if(!n) return NO_ERR;
  if(!idx){
    if(!rt) where = ROOT;
    else if(where != ROOT){
      GAErr(GA_LOC, "GATreeBASE", "insert", gaErrCannotInsertOnNilNode);
      return ERR;
    }
  }
  switch(where){
  case ROOT:
    if((n->prev && n->prev != n) || (n->next && n->next != n)){
      GAErr(GA_LOC, "GATreeBASE",  "insert", gaErrCannotInsertWithSiblings);
      return ERR;
    }
    if(rt){			// root exists, so make old root eldest child
      rt->parent = n;
      if(n->child){
	if(n->child->prev && n->child->prev != n->child){
	  rt->prev = n->child->prev;
	  rt->next = n->child;
	  n->child->prev->next = rt;
	  n->child->prev = rt;
	}
	else{
	  rt->next = n->child;
	  rt->prev = n->child->prev;
	  n->child->prev = rt;
	  n->child->next = rt;
	}
      }
      n->child = rt;
    }
    rt = n;
    n->parent = (GANodeBASE *)0;
    n->prev = n;
    n->next = n;
    break;

  case BEFORE:
    if(!idx->parent){
      GAErr(GA_LOC, "GATreeBASE", "insert", gaErrCannotInsertBeforeRoot);
      return ERR;
    }
    n->parent = idx->parent;
    n->next = idx;
    n->prev = idx->prev;
    idx->prev->next = n;
    idx->prev = n;
    if(idx->parent && idx->parent->child == idx)
      n->parent->child = n;
    break;

  case AFTER:
    if(!idx->parent){
      GAErr(GA_LOC, "GATreeBASE", "insert", gaErrCannotInsertAfterRoot);
      return ERR;
    }
    n->parent = idx->parent;
    n->prev = idx;
    n->next = idx->next;
    idx->next->prev = n;
    idx->next = n;
    break;

  case BELOW:
    n->parent = idx;
    if(idx->child){
      n->prev = idx->child->prev;
      n->next = idx->child;
      idx->child->prev->next = n;
      idx->child->prev = n;
    }
    else{
      n->prev = n;
      n->next = n;
      idx->child = n;
    }
    break;

  default:
    GAErr(GA_LOC, "GATreeBASE", "insert", gaErrBadWhereIndicator);
    break;
  }

  csz = 1;
  cdpth = 1;
  return NO_ERR;
}


// Remove the specified node from the tree.  We don't cruise through the tree
// to make certain that the node is in the tree.  But we do check to make sure
// that the connections were ok before we prune the node.  If there is any
// problem with the links, we return a NULL.  If we get a NULL node, then we
// don't do anything.
//   We don't do anything to the next, prev, etc links of the node that is
// being removed (they are left pointing to where they used to point) so be
// careful!!
//   If the removal is on the root node, set the root node to NULL.
GANodeBASE *
GATreeBASE::remove(GANodeBASE * n)
{
  if(!n) return (GANodeBASE *)0;

  if(!n->next || !n->prev || n->prev->next != n || n->next->prev != n){
    GAErr(GA_LOC, "GATreeBASE", "remove", gaErrBadTreeLinks);
    return (GANodeBASE*)0;
  }

  if(n->next == n || !n->next){
    if(n->parent && n->parent->child == n)
      n->parent->child = (GANodeBASE *)0;
  }
  else{
    if(n->parent && n->parent->child == n)
      n->parent->child = n->next;
    n->prev->next = n->next;
    n->next->prev = n->prev;
  }

  if(n == rt) rt = (GANodeBASE *)0;

// uncomment these to modify the node that is getting removed
  n->prev = n;
  n->next = n;
  n->parent = 0;

  csz = 1;
  cdpth = 1;
  return n;
}


// Swap the specified nodes.  If we get NULL nodes or the nodes are the same,
// don't do anything.  Swapping will work only on nodes that are not directly
// related, eg a cannot be the ancestor of b.  If you do a swap on nodes that
// are directly related, you'll end up with two separate trees.  For now we 
// don't check for this.
//   This swaps two nodes and their entire subtrees, its doesn't simply swap
// the nodes themselves!!
//   We have to be concerned about the type of node that we get.  If the node
// is pointing to itself in its prev and next members, then it is probably a
// root node.  At any rate, we have to make the node its getting swapped with
// have the same attributes (but not the same next and prev).
//   If either of the nodes is NULL, we don't do anything.
//   We check to see if the nodes are in the same tree (but only if we have to)
//   If the nodes are in the same tree then we check their ancestry before we
// do the swap (so that we don't end up with any dangling trees or leaked
// memory).
//   If the nodes are in the same tree and one of them is the root node, then
// the other must have been a descendent of the first, so trying to swap them
// is an error.  If they are in different trees and one is the root node of
// this tree, we can reset the root node of this tree.  We have no way of
// resetting the root node of the other tree, however, so beware that you 
// could lose the second tree (the one that used to be in this tree) if you
// swap into another root.
//   This implementation has much room for improvement.  We don't have to 
// traverse the tree multiple times, for example.
int
GATreeBASE::swaptree(GANodeBASE * a, GANodeBASE * b)
{
  if(!a || !b || a == b) return NO_ERR;

  GANodeBASE * aprev = a->prev;
  GANodeBASE * anext = a->next;
  GANodeBASE * aparent = a->parent;
  GANodeBASE * bprev = b->prev;
  GANodeBASE * bnext = b->next;
  GANodeBASE * bparent = b->parent;

  if(anext == b || bnext == a){	         // a & b are adjacent
    if(aparent && aparent->child == a) aparent->child = b;
    else if(bparent && bparent->child == b) bparent->child = a;

    if(anext == b && bnext != a){         // same as b->prev = a
      a->prev = b;
      a->next = bnext;
      b->prev = aprev;
      b->next = a;
      aprev->next = b;
      bnext->prev = a;
    }
    else if(bnext == a && anext != b){    // same as a->prev = b
      a->prev = bprev;
      a->next = b;
      b->prev = a;
      b->next = anext;
      anext->prev = b;
      bprev->next = a;
    }
    if(rt == a) rt = b;
    else if(rt == b) rt = a;
  }
  else{				// check for same tree then ancestry
    GANodeBASE * aroot = _GARootOfNode(a);
    GANodeBASE * broot = _GARootOfNode(b);
    if(aroot == broot){		// check ancestry
      while(aparent && aparent != b)
	aparent = aparent->parent;
      if(aparent == b){
	GAErr(GA_LOC, "GATreeBASE", "swaptree", gaErrCannotSwapAncestors);
	return ERR;
      }
      while(bparent && bparent != a)
	bparent = bparent->parent;
      if(bparent == a){
	GAErr(GA_LOC, "GATreeBASE", "swaptree", gaErrCannotSwapAncestors);
	return ERR;
      }
      aparent = a->parent;	// reset the values
      bparent = b->parent;
    }  // if not in same tree, we don't bother to check the ancestry
    if(aparent == bparent){
      if(aparent && aparent->child == a) aparent->child = b;
      else if(bparent && bparent->child == b) bparent->child = a;
    }
    else{
      if(aparent && aparent->child == a) aparent->child = b;
      if(bparent && bparent->child == b) bparent->child = a;
    }
    a->parent = bparent;
    b->parent = aparent;
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

    if(aroot != broot){
      if(rt == a) rt = b;
      else if(rt == b) rt = a;
// We could lose the other node here, but we have no way of knowing what the
// root node in the other tree is, so its up to the caller of this routine
// to take care of this!
    }
  }

  csz = 1;
  cdpth = 1;
  return NO_ERR;
}


// Swap the specified nodes.  If we get NULL nodes or the nodes are the same,
// don't do anything.  This moves the two nodes and leaves their subtrees in
// place (if they have any).  The subtrees do not move with the nodes.  Note
// that swapping two nodes shouldn't change the size or depth of the tree, so
// we don't set the change flags.
//   We have to look for a few special cases.  If a and b are parent/child or
// a and b are adjacent then we have to give them special treatment.  We don't
// care if they are otherwise related.
//   We don't check for bogus configurations (eg b is a's child and a's next)
// or for NULL members.  We assume the tree is correctly configured.
//   We assume that the nodes are both in the same tree.  If they are not, you
// may (or may not) get an error.  The error will occur if either of the nodes
// is a root node and the nodes are not in the same tree.
//   At this point this routine is used ONLY for node swapping within the same
// tree, so I won't worry about the different trees case.
int
GATreeBASE::swapnode(GANodeBASE * a, GANodeBASE * b)
{
  if(!a || !b || a == b) return NO_ERR;

  GANodeBASE * aprev = a->prev;
  GANodeBASE * anext = a->next;
  GANodeBASE * achild = a->child;
  GANodeBASE * aparent = a->parent;
  GANodeBASE * bprev = b->prev;
  GANodeBASE * bnext = b->next;
  GANodeBASE * bchild = b->child;
  GANodeBASE * bparent = b->parent;

  if(anext == b || bnext == a){	         // a & b are adjacent
    if(aparent && aparent->child == a) aparent->child = b;
    else if(bparent && bparent->child == b) bparent->child = a;
    a->child = bchild;
    b->child = achild;
    if(achild){
      achild->parent = b;
      for(GANodeBASE * n=achild->next; n && n != achild; n=n->next)
	n->parent = b;
    }
    if(bchild){
      bchild->parent = a;
      for(GANodeBASE * n=bchild->next; n && n != bchild; n=n->next)
	n->parent = a;
    }

    if(anext == b && bnext != a){         // same as b->prev = a
      a->prev = b;
      a->next = bnext;
      b->prev = aprev;
      b->next = a;
      aprev->next = b;
      bnext->prev = a;
    }
    else if(bnext == a && anext != b){    // same as a->prev = b
      a->prev = bprev;
      a->next = b;
      b->prev = a;
      b->next = anext;
      anext->prev = b;
      bprev->next = a;
    }
  }
  else{				    // a & b are not adjacent
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

    if(achild == b){		// same as b->parent = a
      a->parent = b;
      a->child = bchild;
      b->parent = aparent;
      b->child = a;
      if(aparent && aparent->child == a) aparent->child = b;
      for(GANodeBASE * n=a->next; n && n != a; n=n->next)
	n->parent = b;
      if(bchild){
	bchild->parent = a;
	for(GANodeBASE * n=bchild->next; n && n != bchild; n=n->next)
	  n->parent = a;
      }
    }
    else if(bchild == a){	// same as a->parent = b
      a->parent = bparent;
      a->child = b;
      b->parent = a;
      b->child = achild;
      if(bparent && bparent->child == b) bparent->child = a;
      if(achild){
	achild->parent = b;
	for(GANodeBASE * n=achild->next; n && n != achild; n=n->next)
	  n->parent = b;
      }
      for(GANodeBASE * n=b->next; n && n != b; n=n->next)
	n->parent = a;
    }
    else{			// a and b are not adjacent nor parent-child
      a->parent = bparent;
      a->child = bchild;
      b->parent = aparent;
      b->child = achild;
      if(aparent == bparent){
	if(aparent && aparent->child == a) aparent->child = b;
	else if(bparent && bparent->child == b) bparent->child = a;
      }
      else{
	if(aparent && aparent->child == a) aparent->child = b;
	if(bparent && bparent->child == b) bparent->child = a;
      }
      if(achild){
	achild->parent = b;
	for(GANodeBASE * n=achild->next; n && n != achild; n=n->next)
	  n->parent = b;
      }
      if(bchild){
	bchild->parent = a;
	for(GANodeBASE * n=bchild->next; n && n != bchild; n=n->next)
	  n->parent = a;
      }
    }
  }

  if(rt == a) rt = b;		// this only works if they're in the same tree!
  else if(rt == b) rt = a;

  return NO_ERR;
}


// Return the number of nodes in the tree.  We do a complete traversal of the
// tree and count the number of nodes that we encounter.  Could do this breadth
// first or depth first - doesn't really matter.  We have to traverse the 
// entire tree to do the count.
//   We have to do a little work-around here to get through the const-ness of
// the size method.  Its ok to call size on a const object because it does not
// modify the logical state of the object.  It does, however, modify the
// physical state of the object.  So to work around the strictness of the 
// const specifier, we do a little pointer magic and cast this to be non-const.
int
GATreeBASE::size() const
{
  if(!csz) return sz;
  GATreeBASE * This = CON_CAST(GATreeBASE *, this);
  This->csz = 0;
  return(This->sz = _GATreeSize(rt));
}


// Return the number of levels in the tree.  We do a complete traversal of the
// tree and keep a count of the number of times we go down and come up a level.
// I should be able to combine _size and _depth so we don't have to do two
// traversals...
int
GATreeBASE::depth() const
{
  if(!cdpth) return dpth;
  GATreeBASE * This = CON_CAST(GATreeBASE *, this);
  This->cdpth = 0;
  return(This->dpth = _GATreeDepth(rt));
}


// Is node i an ancestor of node j or vice versa?  If so, we return 1.  Other-
// wise we return a 0.
int 
GATreeBASE::ancestral(unsigned int i, unsigned int j) const {
  GATreeIterBASE aiter(*this);
  GATreeIterBASE biter(*this);
  GANodeBASE * aparent, *a;
  GANodeBASE * bparent, *b;
  aparent = a = aiter.warp(i); 
  bparent = b = biter.warp(j);

  while(aparent && aparent != b)
    aparent = aparent->parent;
  if(aparent == b) return 1;
  while(bparent && bparent != a)
    bparent = bparent->parent;
  if(bparent == a) return 1;

  return 0;
}


/* ----------------------------------------------------------------------------
Recursive routines for the TreeBASE objects
---------------------------------------------------------------------------- */
static int
_GATreeSize(GANodeBASE * node)
{
  if(!node) return 0;
  int count = 1 + _GATreeSize(node->child);
  for(GANodeBASE * tmp=node->next; tmp && tmp != node; tmp=tmp->next){
    count++;
    count += _GATreeSize(tmp->child);
  }
  return count;
}


static int
_GATreeDepth(GANodeBASE * node)
{
  if(!node) return 0;
  int depth;
  int maxdepth = 1 + _GATreeDepth(node->child);
  for(GANodeBASE * tmp=node->next; tmp != node; tmp=tmp->next){
    depth = 1 + _GATreeDepth(tmp->child);
    maxdepth = maxdepth > depth ? maxdepth : depth;
  }
  return maxdepth;
}





/* ----------------------------------------------------------------------------
TreeIterBASE
---------------------------------------------------------------------------- */
// Return the root of the specified node.
GANodeBASE * 
GATreeIterBASE::root(GANodeBASE * c)
{
  if(!c) return (GANodeBASE *)0;
  while(c->parent != (GANodeBASE *)0)
    c = c->parent;
  return(node = c);
}


// Return the eldest child in a row of siblings.  The eldest is the one that 
// the parent of that row points to.
//   This could get into an infinite loop if the tree structure were ever 
// corrupted, so be sure that it doesn't!  Also, it will break if the parent
// member is not set in any child, so be sure that never happens either.
//   Remember to set the current node to the one we found.
GANodeBASE * 
GATreeIterBASE::eldest(GANodeBASE * c)
{
  if(!c) return (GANodeBASE *)0;
  if(!c->parent) return(node=c);

  GANodeBASE * tmp = c;

  while(tmp->parent->child != tmp)
    tmp = tmp->next;
  return(node = tmp);
}


// Return the youngest child in a row of siblings.  The youngest is the last
// one in the row (ie the prev for the eldest since we're using circular lists)
// Basically we just find the eldest then take the one previous to that one.
//   Remember to set the current node to the one we found.
GANodeBASE * 
GATreeIterBASE::youngest(GANodeBASE * c)
{
  if(!c) return (GANodeBASE *)0;
  if(!c->parent) return(node=c);

  GANodeBASE * tmp = c;

  while(tmp->parent->child != tmp)
    tmp = tmp->next;
  return(node = tmp->prev);
}


// Return the number of siblings for the specified node.  Notice that this 
// function returns the number of siblings INCLUDING the specified node.
int
GATreeIterBASE::nsiblings(GANodeBASE * c)
{
  GANodeBASE * tmp = c;
  int n=1;
  while(tmp->next && ((tmp=tmp->next) != c)) n++;
  return n;
}


// Return the number of children for the specified node.  This is basically the
// same code as the nsiblings routine, but we look at the child of the node, 
// not the node itself.
int
GATreeIterBASE::nchildren(GANodeBASE * c)
{
  if(!c->child) return 0;
  GANodeBASE * tmp = c->child;
  int n=1;
  while(tmp->next && ((tmp=tmp->next) != c->child)) n++;
  return n;
}


// Set the current node to the node indexed by the integer x.  If x is out of
// bounds, we return NULL and don't change the state of the iterator.  This 
// method uses a depth-first traversal to find the node.  Root node is 0, then
// we go up from there.
GANodeBASE *
GATreeIterBASE::warp(unsigned int x)
{
  unsigned int w=0;
  GANodeBASE * tmp = _GATreeTraverse(x, w, root());
  if(tmp) node = tmp;
  return(tmp);
}


// Return the number of nodes in the tree from the specified node on down.  
// Similar to the TreeBASE size method, but we don't set the sz member and 
// we can't cache the size since this could be called on any node.
int
GATreeIterBASE::size(GANodeBASE * n)
{
  return(_GATreeSize(n));
}


// Return the depth of the tree from the specified node on down.
int
GATreeIterBASE::depth(GANodeBASE * n)
{
  return(_GATreeDepth(n));
}



// Traverse the tree (depth-first) until we come to the node with the index
// specified by 'index'.  Return NULL if cur != index.
GANodeBASE *
_GATreeTraverse(unsigned int index, unsigned int & cur, GANodeBASE * node)
{
  if(!node) return (GANodeBASE *)0;
  if(cur == index) return node;
  cur++;

  GANodeBASE * n;
  if((n = _GATreeTraverse(index, cur, node->child)) != (GANodeBASE *)0)
    return n;

  for(GANodeBASE * tmp=node->next; tmp && tmp != node; tmp=tmp->next){
    if(cur == index) return tmp;
    cur++;
    if((n = _GATreeTraverse(index, cur, tmp->child)) != (GANodeBASE *)0)
      return n;
  }
  return (GANodeBASE *)0;
}


// Comparison operators for a a tree check for identical tree structure
// only (no check for same contents of nodes).  These check for
// similar tree structure - we traverse one tree and expect the second to be
// just like it (depth-first traversal).  If there is any difference along the
// way, then we return a not equal.
//   Neither of these operators affects the internal iterator of either
// tree genome in any way.

//   Recursively traverse two trees at the same time.  Return 1 if they are 
// different, return a 0 if they are identical.  This checks only the tree
// structure, not the node contents.
int
_GATreeCompare(GANodeBASE * anode, GANodeBASE * bnode)
{
  if(anode==0 && bnode==0) return 0;
  if(anode==0 || bnode==0) return 1;
  int count = _GATreeCompare(anode->child, bnode->child);
  for(GANodeBASE * atmp=anode->next, * btmp=bnode->next;
      atmp && atmp != anode;
      atmp=atmp->next, btmp=btmp->next){
    count += _GATreeCompare(atmp->child, btmp->child);
  }
  return count;
}

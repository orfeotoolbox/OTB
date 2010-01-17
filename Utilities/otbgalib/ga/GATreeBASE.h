// $Header$
/* ----------------------------------------------------------------------------
  treebase.h
  mbwall 25nov94
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the tree objects.
---------------------------------------------------------------------------- */
#ifndef _ga_treebase_h_
#define _ga_treebase_h_

#include <ga/GANode.h>


/* ----------------------------------------------------------------------------
 GATreeBASE
-------------------------------------------------------------------------------
  This is the base tree class from which template trees are derived.  This 
object does no memory management - it just keeps track of a tree structure.
Whoever calls the members of this object is responsible for allocating and 
deallocating the memory associated with each node.
  This class does not define any of the iteration operators for traversing the
tree.  That is left to the iterator friend of this class.
  We have to break through the const-ness of the tree in order to make things
work properly.  When you ask for the size of a tree, you don't change the size
but you do (possibly) change the value of the sz member.  This doesn't change
the state of the tree, so it is, in effect, a const-correct operation.  But
const is too strict, so we have to work around it.  See the size() definition
to see how this is done.
  Beware that when you copy this object, you copy pointers, NOT the contents of
the pointers.  You cannot simply say tree1 = tree2 or GATreeBASE t = tree1.  If
you do, you'll get a copy of the tree object but not a copy of the tree (the
copy will point to the same tree as the original).

creation
  Create a tree by passing a root node.  If you don't pass a root node, then
  the next node assigned to the tree with an insert or append operation will 
  become the root node (regardless of the idx node you pass).

insert
  Stick a node into the tree.  Where the node goes depends on the second node
  and the value of the flag.  There are three flag values:  before, after, and
  below.  The flag specifies where the new node should go relative to the old
  node.  If oldnode is the root node, the only valid value for flag is below.
  If there are already children on a node, specifying below puts the new node
  at the end of the row of children (it becomes the youngest sibling).  If you
  don't pass a value for flag, it defaults to below.  The syntax for using the
  insert method looks like this:

       mytree.insert(newnode, oldnode, GATreeBASE::before);
       mytree.insert(newnode, oldnode);

  Insertions assume that the node to be inserted has NO siblings.  If it does,
  they will be lost!  However, the node MAY have children (a subtree).
    If the insert was successful, return NO_ERR.  If there was a problem,
  return ERR.

remove
  Remove the specified node from the tree.  If the node does not exist, an 
  ERR message is posted and NULL is returned.  If the node has children, the
  children are removed from the tree as well (they stay with the node).  A 
  pointer to the node is returned if the removal is successful, otherwise NULL.

swaptree
  Move node a to node b's position in the tree and vice versa.  This swap
  maintains the integrity of all of a's and b's descendents.  It checks for 
  ancestry conflicts so that you cannot swap a node with one of its
  descendents.  You can swap nodes in different trees, but if you do, be sure
  to check for root nodes!  The swap routine can set only the root node of the
  current tree - it doesn't know about the root node of the other tree, so 
  you'll have to reset that one.
    If the swap was successful, return NO_ERR.  If there was a problem,
  return ERR.

swapnode
  Switch nodes a and b, leaving their subtrees (if any) in their original
  positions (the subtrees don't follow a and b).
    If the swap was successful, return NO_ERR.  If there was a problem,
  return ERR.

size
  How many nodes are in the tree?  We keep a flag to tell us if any operation
  has been performed that would require a recalculation of the size.  If you
  change the contents of the tree using any method other than those in this
  object (which you could do, by the way) then you risk screwing up the count.

depth
  How many levels (generations) are there in the tree?
---------------------------------------------------------------------------- */
class GATreeBASE {
public:
  enum Location {ROOT=0, BEFORE, AFTER, BELOW}; // values for 'where' to insert
  enum {NO_ERR = 0, ERR = -1};		// returns codes for tree funcs

  GATreeBASE(){rt=(GANodeBASE *)0; sz=0; dpth=0; csz=0; cdpth=0;}
  GATreeBASE(GANodeBASE * n){rt=n; csz=1; cdpth=1;}
  GANodeBASE * remove(GANodeBASE * n);
  int insert(GANodeBASE * n, GANodeBASE * idx, Location where=BELOW);
  int swaptree(GANodeBASE * a, GANodeBASE * b);
  int swapnode(GANodeBASE * a, GANodeBASE * b);
  int size() const;
  int depth() const;
  int ancestral(unsigned int i, unsigned int j) const;

protected:
  int sz, dpth;			// number of nodes, number of levels in tree
  short csz, cdpth;		// have the contents changed since last update?
  GANodeBASE *rt;		// the root node of the tree

private:
  GATreeBASE(const GATreeBASE &){} // we don't allow copying
  GATreeBASE & operator=(const GATreeBASE &){return *this;} // or op=
  friend class GATreeIterBASE;
};


/* ----------------------------------------------------------------------------
 GATreeIterBASE
-------------------------------------------------------------------------------
  This is the base class for iterators for the tree objects.  We define this
class separately from the Tree object so that you can have multiple interators
for each tree and so that you can more easily customize the traversal
algorithms within the iterator.  From the object point of view, the way you
traverse a tree is independent of how you represent the tree.
  Like the TreeBASE object, this object doesn't do any memory allocation or
deallocation.  All we do is provide tree traversal.
  Notice that we keep a 'current location' in the tree - whatever your last
query was is stored as the node, so if you refer to the current member, you'll
get your last query.
  If you pass a NULL node to these routines they will break.  In the interest 
of speed we don't do any error checking.

creation
  Create an iterator by passing either a tree or another iterator.  If you pass
  a tree, the iterator will default to the root node of the tree.  If you pass
  another iterator, the new iterator will point to the same node that the 
  original iterator points to.

nchildren
  Returns the number of children that are direct offspring of the specified 
  node (or current node if none is specified).  

nsiblings
  Returns the number of nodes at the same level as the specified or current
  node.  This number includes the specified or current node.

current, root, next, prev, parent, child, eldest, youngest, warp
  Set the iterator to the specified node and return a pointer to the node that
  the iterator now points to.  If current is NULL or a NULL is passed to one of
  these routines, a NULL is returned the iterator does not move.

warp
  Move the iterator to the node referenced by index.  The root node is node '0'
  then the count increases from there using a depth-first search.  This means
  that any subtree in a tree will have a contiguous chunk of indices.
---------------------------------------------------------------------------- */
class GATreeIterBASE {
public:
  GATreeIterBASE(){node=(GANodeBASE *)0; tree=(GATreeBASE *)0;}
  GATreeIterBASE(const GATreeBASE & t){tree = &t; node = t.rt;}
  GATreeIterBASE(const GATreeIterBASE & i){tree = i.tree; node = i.node;}
  void operator()(const GATreeBASE & t){tree = &t; node = t.rt;}
  GANodeBASE * current(GANodeBASE * c)
    {return(c ? (node=c) : (GANodeBASE *)0);}
  GANodeBASE * current(){return node;}
  GANodeBASE * next(){return(node ? (node=node->next) : (GANodeBASE *)0);}
  GANodeBASE * next(GANodeBASE * c)
    {return(c ? (node=c->next) : (GANodeBASE *)0);}
  GANodeBASE * prev(){return(node ? (node=node->prev) : (GANodeBASE *)0);}
  GANodeBASE * prev(GANodeBASE * c)
    {return(c ? (node=c->prev) : (GANodeBASE *)0);}
  GANodeBASE * parent()
    {return(node ? (node=node->parent) : (GANodeBASE *)0);}
  GANodeBASE * parent(GANodeBASE * c)
    {return(c ? (node=c->parent) : (GANodeBASE *)0);}
  GANodeBASE * child(){return(node ? (node=node->child) : (GANodeBASE *)0);}
  GANodeBASE * child(GANodeBASE * c)
    {return(c ? (node=c->child) : (GANodeBASE *)0);}
  GANodeBASE * root(){return(tree ? (node=tree->rt) : (GANodeBASE *)0);}
  GANodeBASE * root(GANodeBASE * c);
  GANodeBASE * eldest()
    {return(node ? (node=eldest(node)) : (GANodeBASE *)0);}
  GANodeBASE * eldest(GANodeBASE * c);
  GANodeBASE * youngest()
    {return(node ? (node=youngest(node)) : (GANodeBASE *)0);}
  GANodeBASE * youngest(GANodeBASE * c);
  GANodeBASE * warp(unsigned int);
  GANodeBASE * warp(const GATreeIterBASE & i){
    tree=i.tree; node=(GANodeBASE *)0;
    return(i.node ? (node=i.node) : (GANodeBASE *)0);
  }
  int size(){return(node ? size(node) : 0);}
  int size(GANodeBASE * c);
  int depth(){return(node ? depth(node) : 0);}
  int depth(GANodeBASE * c);
  int nchildren(){return(node ? nchildren(node) : 0);}
  int nchildren(GANodeBASE * c);
  int nsiblings(){return(node ? nsiblings(node) : 0);}
  int nsiblings(GANodeBASE * c);

protected:
  GANodeBASE * node;
  const GATreeBASE * tree;
};

#endif

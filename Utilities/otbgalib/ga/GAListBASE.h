// $Header$
/* ----------------------------------------------------------------------------
  listbase.h
  mbwall 25nov94
  Copyright 1995 Massachusetts Institute of Technology

 DESCRIPTION:
  This defines the list objects.

 TO DO:
  Probably should put the size (and depth for trees) into the templateized 
class since those take care of memory management.  BASE class has no concept of
memory management, nor does it know the best way to count what its got.
---------------------------------------------------------------------------- */
#ifndef _ga_listbase_h_
#define _ga_listbase_h_

#include <ga/GANode.h>

/* ----------------------------------------------------------------------------
 GAListBASE
-------------------------------------------------------------------------------
  This is the base list class from which template lists are derived.  This 
object does no memory management - it just keeps track of a list structure.
Whoever calls the members of this object is responsible for allocating and 
deallocating the memory associated with each node.
  This class does not define any of the iteration operators for traversing the
list.  That is left to the iterator friend of this class.
  The iterator class is declared as a friend so that it can access our
internals.  It iterates over the list without changing anything.
  We have to break through the const-ness of the list in order to make things
work properly.  When you ask for the size of a list, you don't change the size
but you do (possibly) change the value of the sz member.  This doesn't change
the state of the tree, so it is, in effect, a const-correct operation.  But
const is too strict, so we have to work around it.  See the size() definition
to see how this is done.
  Beware that copying this object will copy pointers, NOT the contents of the
pointers.  You cannot simply say list1 = list2 or GAListBASE l = list1.  If you
do this, you'll get a copy of the list object, but not a duplicate of the list.

creation
  You can create a list by passing a node (the node becomes the head of the 
  list) or by passing nothing (the head of the list is NULL, and the next 
  insert automatically becomes the head).

insert
  Stick a node into the list.  Where the node goes depends on the second node
  and the value of the flag.  There are three flag values:  head, before, and
  after.  The flag specifies where the new node should go relative to the old
  node.  If you don't pass a value for flag, it defaults to after.  The syntax
  for using the insert method looks like this:

       mylist.insert(newnode, oldnode, GAListBASE::before);
       mylist.insert(newnode, oldnode);

  The node can be a member of a list.  If it is, then the list will be broken
  just previous to the node, then inserted into the list.
    If the insert was successful, return NO_ERR.  If there was a problem,
  return ERR.

remove
  Remove the specified node from the list.  If the node does not exist, an 
  ERR message is posted and NULL is returned.  The node is returned if the 
  removal is successful, otherwise NULL.

swapnode
  Switch nodes a and b in the list.  Leave the rest of the list intact.
    If the swap was successful, return NO_ERR.  If there was a problem,
  return ERR.

size
  How many nodes are in the list?  We keep a flag to tell us if any operation
  has been performed that would require a recalculation of the size.  If you
  change the contents of the list using any method other than those in this
  object (which you could do, by the way) then you risk screwing up the count.
---------------------------------------------------------------------------- */
class GAListBASE {
public:
  enum Location {HEAD=0, TAIL, BEFORE, AFTER}; // values for 'where' to insert
  enum {NO_ERR=0, ERR= -1};		       // return codes

  GAListBASE(){hd=(GANodeBASE *)0; sz=0; csz=0;}
  GAListBASE(GANodeBASE * n){hd=n; csz=1;}
  GANodeBASE * remove(GANodeBASE * n);
  int insert(GANodeBASE * n, GANodeBASE * idx, Location where=AFTER);
  int swapnode(GANodeBASE * a, GANodeBASE * b);
  int size() const;

protected:
  int sz, csz;			// number of nodes, have contents changed?
  GANodeBASE *hd;		// the head node of the list

private:
  GAListBASE(const GAListBASE &){} // copying is not allowed
  GAListBASE & operator=(const GAListBASE &){return *this;} // or op=
  friend class GAListIterBASE;
};


/* ----------------------------------------------------------------------------
 GAListIterBASE
-------------------------------------------------------------------------------
  This is the base class for iterators for the list objects.  We define this
class separately from the List object so that you can have multiple interators
for each list and so that you can more easily customize the traversal
algorithms within the iterator.  From the object point of view, the way you
traverse a list is independent of how you represent the list.
  Like the ListBASE object, this object doesn't do any memory allocation or
deallocation.  All we do is provide list traversal.
  Notice that we keep a 'current location' in the list - whatever your last
query was is stored as the node, so if you refer to the current member, you'll
get your last query.
  If you pass a NULL node to these routines they will not break; passing a NULL
will result in a no-op, and NULL will be returned.

creation
  When you create an iterator, you should pass another iterator (the new one
  will copy the first) or a list (the iterator will default to the head node
  of the list).

current, head, tail, next, prev, warp
  Set the iterator to the specified node and return a pointer to the node that
  the iterator now points to.  If current is NULL or a NULL is passed to one of
  these routines, a NULL is returned and the iterator does not move.

warp
  Move the iterator to the node referenced by index.  The head node is node '0'
  then the count increases from there.
---------------------------------------------------------------------------- */
class GAListIterBASE {
public:
  GAListIterBASE(){node=(GANodeBASE *)0; list=(GAListBASE *)0;}
  GAListIterBASE(const GAListBASE & t){list = &t; node = t.hd;}
  GAListIterBASE(const GAListIterBASE & i){list = i.list; node = i.node;}
  void operator()(const GAListBASE & t){list = &t; node = t.hd;}
  GANodeBASE * current(GANodeBASE * c)
    {return(c ? (node=c) : (GANodeBASE *)0);}
  GANodeBASE * current(){return node;}
  GANodeBASE * next(){return(node ? (node=node->next) : (GANodeBASE *)0);}
  GANodeBASE * next(GANodeBASE * c)
    {return(c ? (node=c->next) : (GANodeBASE *)0);}
  GANodeBASE * prev(){return(node ? (node=node->prev) : (GANodeBASE *)0);}
  GANodeBASE * prev(GANodeBASE * c)
    {return(c ? (node=c->prev) : (GANodeBASE *)0);}
  GANodeBASE * head(){return(list ? (node=list->hd) : (GANodeBASE *)0);}
  GANodeBASE * tail()
    {return((list && list->hd)?(node=list->hd->prev) : (GANodeBASE *)0);}
  GANodeBASE * warp(unsigned int);
  GANodeBASE * warp(const GAListIterBASE & i){
    list=i.list; node=(GANodeBASE *)0;
    return(i.node ? (node=i.node) : (GANodeBASE *)0);
  }
  int size(){return(list ? list->size() : 0);}

protected:
  GANodeBASE * node;
  const GAListBASE * list;
};

#endif

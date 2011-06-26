/*! \file kbool/src/node.cpp
    \brief Holds a GDSII node structure
    \author Probably Klaas Holwerda

    Copyright: 2001-2004 (C) Probably Klaas Holwerda 

    Licence: wxWidgets Licence

    RCS-ID: $Id: node.cpp 19473 2011-05-03 15:04:19Z gpotts $
*/
#ifdef __GNUG__
#pragma implementation 
#endif

#include <ossim/kbool/node.h>
#include <ossim/kbool/link.h>
#include <ossim/kbool/line.h>
#include <cmath>
#include <iostream>

//this here is to initialize the static iterator of node
//with NOLIST constructor
//TDLI<KBoolLink> 	Node::_linkiter=TDLI<KBoolLink>(_GC);

Node::Node(Bool_Engine* /*GC*/) : LPoint(0,0)
{
//     _GC=GC;
	_linklist=new DL_List<void*>();
}


Node::Node(B_INT const X, B_INT const Y, Bool_Engine* /*GC*/) : LPoint(X,Y)
{
//     _GC=GC;
	_linklist=new DL_List<void*>();
}


Node::Node(LPoint* const a_point, Bool_Engine* /*GC*/) : LPoint(a_point)
{
//     _GC=GC;
	_linklist=new DL_List<void*>();
}


//Node::Node(Node * const other) : LPoint(other)
Node::Node(Node * const other, Bool_Engine* /*GC*/)
{
//     _GC=GC;
	_x = other->_x;
	_y = other->_y;
	_linklist=new DL_List<void*>();
}

Node& Node::operator=(const Node &other_node)
{
	_x = other_node._x;
	_y = other_node._y;

	return *this;
}


// x and y of the point will be rounded to the nearest
// xnew=N*grid and ynew=N*grid
void Node::RoundInt(B_INT grid)
{
   _x=(B_INT) std::floor((_x + grid * 0.5) / grid) * grid;
   _y=(B_INT) std::floor((_y + grid * 0.5) / grid) * grid;
}

Node::~Node()
{
   if(_linklist)
   {
      delete _linklist;
      _linklist = 0;
   }
//    _GC = 0;
}

DL_List<void*>* Node::GetLinklist()
{
	return _linklist;
}

void Node::AddLink(KBoolLink *a_link)
{
   if(!_linklist) return;
//	assert(a_link);
	_linklist->insbegin(a_link);
}

KBoolLink* Node::GetIncomingLink()
{
   if(!_linklist) return 0;
	if (((KBoolLink*)_linklist->headitem())->GetEndNode() == this)
		return (KBoolLink*)_linklist->headitem();
   else
		return (KBoolLink*)_linklist->tailitem();
}

KBoolLink* Node::GetOutgoingLink()
{
   if(!_linklist) return 0;
	if (((KBoolLink*)_linklist->headitem())->GetBeginNode() == this)
		return (KBoolLink*)_linklist->headitem();
   else
		return (KBoolLink*)_linklist->tailitem();
}

//
// Returns the number of connected links
//
int	Node::GetNumberOfLinks()
{
   if(!_linklist) return 0;
   return _linklist->count();
}

KBoolLink* Node::GetOtherLink(KBoolLink* prev)
{
   if(!_linklist) return 0;
   if (prev==(KBoolLink*)_linklist->headitem())
      return (KBoolLink*)_linklist->tailitem();
   if (prev==(KBoolLink*)_linklist->tailitem())
      return (KBoolLink*)_linklist->headitem();
   
   return NULL;
}


int Node::Merge(Node *other)
{
   if (this==other) //they are already merged dummy
      return 0;
   TDLI<KBoolLink> 	linkiter(_linklist);
//    if(!_GC) return 0;
//    if(!_GC->_linkiter) return 0;
//   if(_GC->_linkiter->isAttached())
   {
//       _GC->_linkiter->Detach();
   }
//    _GC->_linkiter->Attach(_linklist);
   int Counter;
	// used to delete Iterator on other->_linklist
	// otherwise there can't be a takeover, because for takeover there can't
	// be an iterator on other->_linklist;
   {
      TDLI<KBoolLink> Iother(other->_linklist);
      KBoolLink* temp;

      Counter = Iother.count();

      Iother.tohead();
      while (!Iother.hitroot())
      {
         temp=Iother.item();
         //need to test both nodes because it may be a zero length link
         if (temp->GetEndNode()==other)
            temp->SetEndNode(this);
         if (temp->GetBeginNode()==other)
            temp->SetBeginNode(this);
         Iother++;
      }
      linkiter.takeover(&Iother);
   }
//    _GC->_linkiter->Detach();
   
   //at this moment the other nodes has no link pointing to it so it needs to be deleted
   delete other;
   return Counter;
}


void Node::RemoveLink(KBoolLink *a_link)
{
//	assert(a_link);
//    if(!_GC) return;
//    if(!_GC->_linkiter) return;

//    if(_GC->_linkiter->isAttached())
//    {
//       _GC->_linkiter->Detach();
//    }
   TDLI<KBoolLink> 	linkiter(_linklist);
//    _GC->_linkiter->Attach(_linklist);
   
   if (linkiter.toitem(a_link))	// find the link
      linkiter.remove();
//    _GC->linkiter->Detach();
}

// This function will determinate if the given three points
// can be simplified to two points
//
// input : three nodes, the first and the second must be points of
//			  a line in correct order, the third point is a point of another
//         line.
// output: -
// return: true if points can be simplified
//			  false if points can't be simplified
bool Node::Simplify(Node *First, Node *Second, B_INT Marge)
{
	double distance=0;

	// The first and second point are a zero line, if so we can
	// make a line between the first and third point
	if (First->Equal(Second,Marge))
		return true;

	// Are the first and third point equal, if so
	// we can delete the second point
	if (First->Equal(this, Marge))
		return true;

	// Used tmp_link.set here, because the link may not be linked in the graph,
	// because the point of the graphs are used, after use of the line we have
	//to set the link to zero so the nodes will not be destructed by exit of the function
	KBoolLink tmp_link(_GC);
	tmp_link.Set(First,Second);
	KBoolLine tmp_line(_GC);
	tmp_line.Set(&tmp_link);

	//	If third point is on the same line which is made from the first
	// and second point then we can delete the second point
	if (tmp_line.PointOnLine(this,distance, (double) Marge) == ON_AREA)
	{
		tmp_link.Set(NULL,NULL);
		return true;
	}
	//
	//
	tmp_link.Set(Second,this);
	tmp_line.Set(&tmp_link);
	if (tmp_line.PointOnLine(First,distance, (double) Marge) == ON_AREA)
	{
		tmp_link.Set(NULL,NULL);
		return true;
	}
	tmp_link.Set(NULL,NULL);
	return false;
}


KBoolLink* Node::GetNextLink()
{
   if(!_linklist) return 0;
   int Aantal = _linklist->count();
   
//	assert (Aantal != 0);
   
   // there is one link, so there is no previous link
   if (Aantal == 1)
      return NULL;
   int Marked_Counter = 0;
   KBoolLink *the_link = NULL;
   TDLI<KBoolLink> 	linkiter(_linklist);
   
   // count the marked links
//         if(!_GC) return 0;
//         if(!_GC->_linkiter) return 0;
// 	_GC->_linkiter->Attach(_linklist);
   linkiter.tohead();
   while (!linkiter.hitroot())
   {
      if (linkiter.item()->IsMarked())
         Marked_Counter++;
      else
      {
         if (!the_link)
            the_link = linkiter.item();
      }
      (++linkiter);
   }
// 	_GC->_linkiter->Detach();
   if (Aantal - Marked_Counter != 1)
      // there arent two unmarked links
      return NULL;
   else
   {
      if (the_link->GetBeginNode() == this)
         return the_link;
      else
         return NULL;
   }
}


KBoolLink* Node::GetPrevLink()
{
   int Aantal;
   if (!_linklist)
      return 0;
//         if(!_GC) return 0;
//         if(!_GC->_linkiter) return 0;
   TDLI<KBoolLink> 	linkiter(_linklist);
   
   Aantal = _linklist->count();
   
//	assert (Aantal != 0);
   
   // there is one link, so there is no previous link
   if (Aantal == 1)
      return NULL;
   
   int Marked_Counter = 0;
   KBoolLink *the_link = NULL;
   
// 	_GC->_linkiter->Attach(_linklist);
   // count the marked links
   linkiter.tohead();
   while (!linkiter.hitroot())
   {
      if (linkiter.item()->IsMarked())
         Marked_Counter++;
      else
      {
         if (!the_link)
            the_link = linkiter.item();
      }
      (++linkiter);
   }
// 	_GC->_linkiter->Detach();
   if (Aantal - Marked_Counter != 1)
      // there arent two unmarked links
      return NULL;
   else
   {
      if (the_link->GetEndNode() == this)
         return the_link;
      else
         return NULL;
   }
}

bool Node::SameSides( KBoolLink* const prev , KBoolLink* const link, BOOL_OP operation )
{
   bool directedLeft;
   bool directedRight;
   if ( prev->GetEndNode() == this ) //forward direction
   {
      directedLeft = prev->IsMarkedLeft( operation );
      directedRight = prev->IsMarkedRight( operation );
      if ( link->GetBeginNode() == this ) //forward direction
      {
         return directedLeft == link->IsMarkedLeft( operation ) && 
                directedRight == link->IsMarkedRight( operation );
      }

      return directedLeft == link->IsMarkedRight( operation ) && 
             directedRight == link->IsMarkedLeft( operation );
   }

   directedLeft = prev->IsMarkedRight( operation );
   directedRight = prev->IsMarkedLeft( operation );
   if ( link->GetBeginNode() == this ) //forward direction
   {
      return directedLeft == link->IsMarkedLeft( operation ) && 
             directedRight == link->IsMarkedRight( operation );
   }
   return directedLeft == link->IsMarkedRight( operation ) && 
          directedRight == link->IsMarkedLeft( operation );
}

//  on the node get the link
//  is the most right or left one
//  This function is used to collect the simple graphs from a graph
KBoolLink* Node::GetMost( KBoolLink* const prev ,LinkStatus whatside, BOOL_OP operation )
{
   KBoolLink *reserve=0;
   KBoolLink *Result = NULL,*link;
   Node* prevbegin = prev->GetOther(this);
   
   if(!_linklist) return 0;
   if (_linklist->count() == 2) // only two links to this node take the one != prev
   {
      if ( (link = (KBoolLink*)_linklist->headitem()) == prev )      //this is NOT the one to go on
         link = (KBoolLink*)_linklist->tailitem();
      if (!link->BeenHere() && SameSides( prev, link, operation ) )
         //we are back where we started (bin is true) return Null
         return link;
      return(0);
   }
   
   TDLI<KBoolLink> 	linkiter(_linklist);
//         if(!_GC) return 0;
//         if(!_GC->_linkiter) return 0;
// 	_GC->_linkiter->Attach(_linklist);
   linkiter.tohead();
   //more then 2 links to the Node
   while(!linkiter.hitroot())
   {
      link = linkiter.item();
      if ( !link->BeenHere() &&
           SameSides( prev, link, operation ) && 
           link != prev   //should be set to bin already
           )
      {
         if (prevbegin == link->GetOther(this) )//pointers equal
            //we are going back in the same direction on a parallel link
            //only take this possibility if nothing else is possible
            reserve = link;
         else
         {  //this link is in a different direction
            if (!Result)
               Result = link; //first one found sofar
            else
            {
               if	(prev->PointOnCorner(Result, link) == whatside )
                  //more to the whatside than take this one
                  Result = link;
            }
         }
      }
      (++linkiter);
   }
   
   // if there is a next link found return it
   // else if a parallel link is found return that one
   // else return NULL
//    _GC->_linkiter->Detach();
   return ((Result) ? Result : reserve);
}

//  on the node get the link
//  is the most right or left one
//  This function is used to collect the simple graphs from a graph
KBoolLink* Node::GetMostHole( KBoolLink* const prev, LinkStatus whatside, BOOL_OP operation )
{
   if(!_linklist) return 0;
   KBoolLink *reserve=0;
   KBoolLink *Result=NULL,*link;
   Node* prevbegin = prev->GetOther(this);
   
   if (_linklist->count() == 2) // only two links to this node take the one != prev
   {
      if ( (link = (KBoolLink*)_linklist->headitem()) == prev )      //this is NOT the one to go on
         link = (KBoolLink*)_linklist->tailitem();
      if ( link->GetHole() && !link->GetHoleLink() && !link->BeenHere() && SameSides( prev, link, operation ) )
         //we are back where we started (bin is true) return Null
         return link;
      return(0);
   }
   
   TDLI<KBoolLink> 	linkiter(_linklist);
//         if(!_GC) return 0;
//         if(!_GC->_linkiter) return 0;
// 	_GC->_linkiter->Attach(_linklist);
   linkiter.tohead();
   //more then 2 links to the Node
   while(!linkiter.hitroot())
   {
      link = linkiter.item();
      if ( !link->BeenHere() &&
           link->GetHole() &&
           !link->GetHoleLink() && 
           SameSides( prev, link, operation ) &&
           link != prev   //should be set to bin already
           )
      {
         if (prevbegin == link->GetOther(this) )//pointers equal
            //we are going back in the same direction on a parallel link
            //only take this possibility if nothing else is possible
            reserve = link;
         else
         {  //this link is in a different direction
            if (!Result)
               Result = link; //first one found sofar
            else
            {
               if	(prev->PointOnCorner(Result, link) == whatside )
                  //more to the whatside than take this one
                  Result = link;
            }
         }
      }
      (++linkiter);
   }
   
   // if there is a next link found return it
   // else if a parallel link is found return that one
   // else return NULL
//    _GC->_linkiter->Detach();
   return ((Result) ? Result : reserve);
}

// this function gets the highest not flat link
KBoolLink* Node::GetHoleLink( KBoolLink* const prev, bool checkbin, BOOL_OP operation )
{
   if(!_linklist) return 0;
	KBoolLink *Result=NULL,*link;

//         if(!_GC) return 0;
        TDLI<KBoolLink> 	linkiter(_linklist);
//         if(!_GC->_linkiter) return 0;
// 	_GC->_linkiter->Attach(_linklist);

//	for(_GC->_linkiter->tohead();!_GC->_linkiter->hitroot();(*_GC->_linkiter)++)
	for(linkiter.tohead();!linkiter.hitroot();++linkiter)
	{
// 		link=_GC->_linkiter->item();
           link = linkiter.item();
           if ( link->GetHoleLink() && 
                ( !checkbin || ( checkbin && !link->BeenHere()) ) &&
                SameSides( prev, link, operation ) 
                )
           {
              Result=link; 
              break;
           }
	}
        
// 	_GC->_linkiter->Detach();
	return (Result);
}

// this function gets the highest not flat link
KBoolLink* Node::GetNotFlat()
{
   if(!_linklist) return 0;
   KBoolLink *Result=NULL,*link;
   TDLI<KBoolLink> 	linkiter(_linklist);
   
//    if(!_GC) return 0;
//    if(!_GC->_linkiter) return 0;
//    _GC->_linkiter->Attach(_linklist);

  double tangold = 0.0;
  double tangnew = 0.0;

// 	for(_GC->_linkiter->tohead();!_GC->_linkiter->hitroot();(*_GC->_linkiter)++)
  for(linkiter.tohead();!linkiter.hitroot();++linkiter)
  {
//		link=_GC->_linkiter->item();
     link=linkiter.item();
     if (!linkiter.item()->BeenHere())
     {
        B_INT dx=link->GetOther(this)->GetX()-_x;
        B_INT dy=link->GetOther(this)->GetY()-_y;
        if (dx!=0)
        {
           tangnew=fabs( (double) dy / (double) dx );
        }
        else
        {
           tangnew=MAXDOUBLE;
        }
        
        if (!Result)
        {
           //this link is in a different direction
           Result=link; //first one found sofar
           tangold=tangnew;
        }
        else
        {
           if(tangnew < tangold)
           {
              //this one is higher (more horizontal) then the old Result
              Result=link;
              tangold=tangnew;
           }
        }
     }
  }
  
  // if there is a next link found return it
  // else if a parallel link is found return that one
  // else return NULL
//   _GC->_linkiter->Detach();
  return (Result);
}

//  on the node get the link that is not BIN
//  and that has the same graphnumber and is in same direction
KBoolLink *Node::Follow(KBoolLink* const prev )
{
   if(!_linklist) return 0;
   KBoolLink *temp;
   TDLI<KBoolLink> 	linkiter(_linklist);
//    if(!_GC) return 0;
//    if(!_GC->_linkiter) return 0;
//    _GC->_linkiter->Attach(_linklist);
   
   linkiter.tohead();
   while(!linkiter.hitroot())
   {
      if (( linkiter.item() != prev ) &&
          ( !linkiter.item()->BeenHere()) &&
          ( linkiter.item()->GetGraphNum() == prev->GetGraphNum()) &&
          (
             ( (prev->GetEndNode()   == this) &&
               (linkiter.item()->GetEndNode()  !=this)
               )
             ||
             ( (prev->GetBeginNode() == this) &&
               (linkiter.item()->GetBeginNode() !=this)
               )
             )
          )
      {
         temp=linkiter.item();
         linkiter.Detach();
         return(temp);
      }
      (++linkiter);
   }
   
//    _GC->_linkiter->Detach();
   return (0);
}

// this function gets the highest (other node) link ascending from the node
// that has the bin flag set as the argument binset
// if no such link exists return 0
KBoolLink* Node::GetBinHighest(bool binset)
{
   if(!_linklist) return 0;
   TDLI<KBoolLink> 	linkiter(_linklist);
//    if(!_GC) return 0;
//    if(!_GC->_linkiter) return 0;

   KBoolLink *Result=NULL,*link;
//    _GC->_linkiter->Attach(_linklist);
   
   double tangold = 0.0;
   double tangnew = 0.0;
   
   for(linkiter.tohead();!linkiter.hitroot();(++linkiter))
   {
      link=linkiter.item();
      if (linkiter.item()->BeenHere() == binset)
      {
         B_INT dx=link->GetOther(this)->GetX()-_x;
         B_INT dy=link->GetOther(this)->GetY()-_y;
         if (dx!=0)
         {
            tangnew = (double) dy / (double) dx;
         }
         else if (dy > 0)
         {
            tangnew = MAXDOUBLE;
         }
         else
         {
            tangnew = -MAXDOUBLE;
         }
         
         if (!Result)
         {
            Result = link; //first one found sofar
            tangold = tangnew;
         }
         else
         {
            if(tangnew > tangold)
            {
               //this one is higher then the old Result
               Result = link;
               tangold = tangnew;
            }
         }
      }
   }
   
   // if there is a link found return it
   // else return NULL
//    _GC->_linkiter->Detach();
   return (Result);
}



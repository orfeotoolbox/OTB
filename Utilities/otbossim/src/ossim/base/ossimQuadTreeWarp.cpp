//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQuadTreeWarp.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/base/ossimQuadTreeWarp.h>
#include <algorithm>
#include <stack>
#include <iostream>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceExec  ("ossimQuadTreeWarp:exec");
static ossimTrace traceDebug ("ossimQuadTreeWarp:debug");

RTTI_DEF1(ossimQuadTreeWarp, "ossimQuadTreeWarp", ossim2dTo2dTransform);

ossimQuadTreeWarpVertex::ossimQuadTreeWarpVertex(
   const ossimDpt& position, const ossimDpt& delta)
   :thePosition(position),
    theDelta(delta),
    theLockedFlag(false)
{
}

ossimQuadTreeWarpVertex::~ossimQuadTreeWarpVertex()
{
   for(ossim_uint32 i = 0;i< theSharedNodeList.size();++i)
   {
      theSharedNodeList[i]->removeVertex(this);
   }
                  
}

void ossimQuadTreeWarpVertex::setPosition(const ossimDpt& position)
{
   thePosition = position;
}

void ossimQuadTreeWarpVertex::setDelta(const ossimDpt& delta)
{
   theDelta    = delta;
}

const ossimDpt& ossimQuadTreeWarpVertex::getDelta()const
{
   return theDelta;
}

const ossimDpt& ossimQuadTreeWarpVertex::getPosition()const
{
   return thePosition;
}

void ossimQuadTreeWarpVertex::addSharedNode(ossimQuadTreeWarpNode* node)
{
   if(node)
   {
      theSharedNodeList.push_back(node);
   }
}

void ossimQuadTreeWarpVertex::removeNode(ossimQuadTreeWarpNode* node)
{
   std::vector<ossimQuadTreeWarpNode*>::iterator iter = theSharedNodeList.begin();
   bool found = false;
   ossimQuadTreeWarpNode* removedNode = NULL;
   while((iter != theSharedNodeList.end())&&!found)
   {
      if(*iter == node)
      {
         removedNode = *iter;
         iter = theSharedNodeList.erase(iter);
         found = true;
      }
      else
      {
         ++iter;
      }
   }

   if(removedNode)
   {
      removedNode->removeVertex(this);
   }
}

bool ossimQuadTreeWarpVertex::isShared()const
{
   return (theSharedNodeList.size() > 0);
}

bool ossimQuadTreeWarpVertex::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   kwl.add(prefix,
           "x",
           thePosition.x,
           true);
   kwl.add(prefix,
           "y",
           thePosition.y,
           true);
   kwl.add(prefix,
           "dx",
           theDelta.x,
           true);
   kwl.add(prefix,
           "dy",
           theDelta.y,
           true);
   kwl.add(prefix,
           "lock_flag",
           (int)theLockedFlag,
           true);
   
   return true;
}

bool ossimQuadTreeWarpVertex::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   const char* x = kwl.find(prefix,
                            "x");
   const char* y = kwl.find(prefix,
                            "y");
   const char* dx = kwl.find(prefix,
                             "dx");
   const char* dy = kwl.find(prefix,
                             "dy");
   const char* lockedFlag = kwl.find(prefix,
                                     "lock_flag");
   if(x&&y&&dx&&dy&&lockedFlag)
   {
      thePosition.x = ossimString(x).toDouble();
      thePosition.y = ossimString(y).toDouble();
      theDelta.x    = ossimString(dx).toDouble();
      theDelta.y    = ossimString(dy).toDouble();
      theLockedFlag = ossimString(lockedFlag).toBool();
      
      return true;
   }
   
   return false;
}

ossimQuadTreeWarpNode::ossimQuadTreeWarpNode()
   :
   theUlVertex(NULL),
   theUrVertex(NULL),
   theLrVertex(NULL),
   theLlVertex(NULL),
   theParent(NULL)
{
   theBoundingRect.makeNan();
}

ossimQuadTreeWarpNode::ossimQuadTreeWarpNode(
   const ossimDrect& bounds,
   ossimQuadTreeWarpNode* parent,
   ossimQuadTreeWarpVertex* ulVertex,
   ossimQuadTreeWarpVertex* urVertex,
   ossimQuadTreeWarpVertex* lrVertex,
   ossimQuadTreeWarpVertex* llVertex)
   :
   theBoundingRect(bounds),
   theUlVertex(ulVertex),
   theUrVertex(urVertex),
   theLrVertex(lrVertex),
   theLlVertex(llVertex),
   theParent(parent)
{
}

ossimQuadTreeWarpNode::~ossimQuadTreeWarpNode()
{
   removeVertices();

   theUlVertex = NULL;
   theUrVertex = NULL;
   theLrVertex = NULL;
   theLlVertex = NULL;
   
}

bool ossimQuadTreeWarpNode::hasValidVertices()const
{
   return (theUlVertex&&theUrVertex&&theLrVertex&&theLlVertex);
}

void ossimQuadTreeWarpNode::removeVertex(ossimQuadTreeWarpVertex* v)
{
   if(!v) return;
   
   if(theUlVertex == v)
   {
      theUlVertex = NULL;
      v->removeNode(this);
   }
   if(theUrVertex == v)
   {
      theUrVertex = NULL;
      v->removeNode(this);
   }
   if(theLrVertex == v)
   {
      theLrVertex = NULL;
      v->removeNode(this);
   }
   if(theLlVertex == v)
   {
      theLlVertex = NULL;
      v->removeNode(this);
   }
}
void ossimQuadTreeWarpNode::removeVertices()
{
   removeVertex(theUlVertex);
   removeVertex(theUrVertex);
   removeVertex(theLrVertex);
   removeVertex(theLlVertex);
}

bool ossimQuadTreeWarpNode::isLeaf()const
{
   return (theChildren.size() == 0);
}

void ossimQuadTreeWarpNode::clear()
{
   theBoundingRect.makeNan();
   theChildren.clear();
   theParent = NULL;
   removeVertex(theUlVertex);
   removeVertex(theUrVertex);
   removeVertex(theLrVertex);
   removeVertex(theLlVertex);
}

void ossimQuadTreeWarpNode::removeChild(ossimQuadTreeWarpNode* node)
{
   std::vector<ossimQuadTreeWarpNode*>::iterator iter =  theChildren.begin();

   while(iter != theChildren.end())
   {
      if(*iter == node)
      {
         theChildren.erase(iter);
         return;
      }
      ++iter;
   }
}

bool ossimQuadTreeWarpNode::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::UL_X_KW,
           theBoundingRect.ul().x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::UL_Y_KW,
           theBoundingRect.ul().y,
           true);
   kwl.add(prefix,
           ossimKeywordNames::LR_X_KW,
           theBoundingRect.lr().x,
           true);
   kwl.add(prefix,
           ossimKeywordNames::LR_Y_KW,
           theBoundingRect.lr().y,
           true);
   
   return true;
}

bool ossimQuadTreeWarpNode::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   const char* ul_x = kwl.find(prefix, ossimKeywordNames::UL_X_KW);
   const char* ul_y = kwl.find(prefix, ossimKeywordNames::UL_Y_KW);
   const char* lr_x = kwl.find(prefix, ossimKeywordNames::LR_X_KW);
   const char* lr_y = kwl.find(prefix, ossimKeywordNames::LR_Y_KW);

   if(ul_x&&ul_y&&lr_x&&lr_y)
   {
      theBoundingRect = ossimDrect(ossimString(ul_x).toDouble(),
                                   ossimString(ul_y).toDouble(),
                                   ossimString(lr_x).toDouble(),
                                   ossimString(lr_y).toDouble());
      return true;
   }
   
   return false;
}


ossimQuadTreeWarp::ossimQuadTreeWarp()
   :theTree(NULL)
{
}

ossimQuadTreeWarp::ossimQuadTreeWarp(const ossimDrect& boundingRect,
                                     const ossimDpt& ulShift,
                                     const ossimDpt& urShift,
                                     const ossimDpt& lrShift,
                                     const ossimDpt& llShift)
   :theTree(NULL)
{
   create(boundingRect, ulShift, urShift, lrShift, llShift);
}

ossimQuadTreeWarp::ossimQuadTreeWarp(const ossimQuadTreeWarp& rhs)
   :ossim2dTo2dTransform(),
    theWarpEnabledFlag(true),
    theTree(NULL)
{
   ossimKeywordlist kwl;
   
   rhs.saveState(kwl);
   
   loadState(kwl);
}

ossimObject* ossimQuadTreeWarp::dup()const
{
   return new ossimQuadTreeWarp(*this);
}

ossimQuadTreeWarp::~ossimQuadTreeWarp()
{
   clear();
}
   
ossimDpt ossimQuadTreeWarp::getOrigin()const
{
   if(!isEmpty())
   {
      if(!theTree->theBoundingRect.hasNans())
      {
         return theTree->theBoundingRect.midPoint();
      }
      
   }
   
   return ossimDpt(0,0);
}

void ossimQuadTreeWarp::create(const ossimDrect& boundingRect,
                               const ossimDpt& ulShift,
                               const ossimDpt& urShift,
                               const ossimDpt& lrShift,
                               const ossimDpt& llShift)
{
   clear();

   theTree = new ossimQuadTreeWarpNode(boundingRect);

   ossimQuadTreeWarpVertex* ul = new ossimQuadTreeWarpVertex(boundingRect.ul(),
                                                             ulShift);
   ossimQuadTreeWarpVertex* ur = new ossimQuadTreeWarpVertex(boundingRect.ur(),
                                                             urShift);
   ossimQuadTreeWarpVertex* lr = new ossimQuadTreeWarpVertex(boundingRect.lr(),
                                                             lrShift);
   ossimQuadTreeWarpVertex* ll = new ossimQuadTreeWarpVertex(boundingRect.ll(),
                                                             llShift);
   ul->addSharedNode(theTree);
   ur->addSharedNode(theTree);
   lr->addSharedNode(theTree);
   ll->addSharedNode(theTree);

   theVertexList.push_back(ul);
   theVertexList.push_back(ur);
   theVertexList.push_back(lr);
   theVertexList.push_back(ll);
   
   theTree->theUlVertex = ul;
   theTree->theUrVertex = ur;
   theTree->theLrVertex = lr;
   theTree->theLlVertex = ll;
   
}

void ossimQuadTreeWarp::printVertices(std::ostream& out)const
{
   for(ossim_uint32 i = 0; i < theVertexList.size();++i)
   {
      out << *theVertexList[i] << "\n";
   }
}

ossimQuadTreeWarpNode* ossimQuadTreeWarp::getRoot()
{
   return theTree;
}

const ossimQuadTreeWarpNode* ossimQuadTreeWarp::getRoot()const
{
   return theTree;
}

bool ossimQuadTreeWarp::isEmpty()const
{
   return (theTree==NULL);
}

std::ostream& ossimQuadTreeWarp::print(std::ostream& out) const
{
   if(!isEmpty())
   {
      out << "___________VERTEX LIST____________________\n";
      printVertices(out);
      out << "___________TREE LIST____________________\n";
      
      recursivePrint(out, theTree);
   }
   else
   {
      out << "<empty tree>\n";
   }
   return out;
}

ossimQuadTreeWarpVertex* ossimQuadTreeWarp::findClosestVertex(ossimDpt& position)
{
   if(position.hasNans()) return (ossimQuadTreeWarpVertex*)NULL;
      
   double dist = 1.0/DBL_EPSILON;
   ossimQuadTreeWarpVertex* result = (ossimQuadTreeWarpVertex*)NULL;
   for(ossim_uint32 i = 0; i < theVertexList.size(); ++i)
   {
      double d = (position-theVertexList[i]->thePosition).length();
      if( d < dist)
      {
         result = theVertexList[i];
         dist = d;
      }
   }

   return result;
//    ossimQuadTreeWarpNode* node = findNode(position);

//    if(node&&node->hasValidVertices())
//    {
//       double ulDist = (position-node->theUlVertex->thePosition).length();
//       double urDist = (position-node->theUrVertex->thePosition).length();
//       double lrDist = (position-node->theLrVertex->thePosition).length();
//       double llDist = (position-node->theLlVertex->thePosition).length();

//       double minDist = std::min(ulDist, std::min(urDist, std::min(lrDist, llDist)));

//       if(minDist == ulDist)
//       {
//          return node->theUlVertex;
//       }
//       else if(minDist == urDist)
//       {
//          return node->theUrVertex;
//       }
//       else if(minDist == lrDist)
//       {
//          return node->theLrVertex;
//       }
//       else if(minDist == llDist)
//       {
//          return node->theLlVertex;
//       }
//    }

//    return ((ossimQuadTreeWarpVertex*)NULL);
}


ossimQuadTreeWarpVertex* ossimQuadTreeWarp::findVertex(const ossimDpt& position)
{
   ossim_uint32 i = 0;
   ossimQuadTreeWarpVertex* result = (ossimQuadTreeWarpVertex*)NULL;
   ossimQuadTreeWarpNode*   currentNode = theTree;
   
   if((currentNode)&&
      (currentNode->theBoundingRect.pointWithin(position)))
   {
      while((currentNode)&&
            (!currentNode->isLeaf()))
      {
         for(i = 0; i < currentNode->theChildren.size(); ++i)
         {
            if(currentNode->theChildren[i]->theBoundingRect.pointWithin(position))
            {
               currentNode = currentNode->theChildren[i];
               break;
            }
         }
      }

      if(currentNode&&currentNode->hasValidVertices())
      {
         if(currentNode->theUlVertex->thePosition == position)
         {
            result = currentNode->theUlVertex;
         }
         else if(currentNode->theUrVertex->thePosition == position)
         {
            result = currentNode->theUrVertex;            
         }
         else if(currentNode->theLrVertex->thePosition == position)
         {
            result = currentNode->theLrVertex;
         }
         else if(currentNode->theLlVertex->thePosition == position)
         {
            result = currentNode->theLlVertex;
         }
      }
               
   }

   return result;   
}

ossimQuadTreeWarpVertex* ossimQuadTreeWarp::getVertex(const ossimDpt& position)
{
   std::vector<ossimQuadTreeWarpVertex*>::iterator iter = theVertexList.begin();

   while(iter != theVertexList.end())
   {
      if(position == (*iter)->getPosition())
      {
         return (*iter);
      }
      ++iter;
   }
   
   return (ossimQuadTreeWarpVertex*)NULL;
}

void ossimQuadTreeWarp::clear()
{
   if(theTree)
   {     
      recursiveDelete(theTree);
      theTree = NULL;
   }

   for(ossim_uint32 i = 0; i < theVertexList.size(); ++i)
   {
      delete theVertexList[i];
   }
   
   theVertexList.clear();
}

void ossimQuadTreeWarp::forward(const ossimDpt& pt,
                                ossimDpt& result)const
{
   if(theWarpEnabledFlag)
   {
      ossimDpt shift;
      
      getShift(shift,
               pt);
      
      result = pt + shift;
   }
   else
   {
      result = pt;
   }
}

void ossimQuadTreeWarp::forward(ossimDpt& pt)const
{
   if(theWarpEnabledFlag)
   {
      ossimDpt shift;
      
      getShift(shift,
               pt);
      
      pt += shift;
   }
}

void ossimQuadTreeWarp::getShift(ossimDpt& result,
                                 const ossimDpt& pt)const
{
   getShift(result,
            findNode(pt),
            pt);
}

void ossimQuadTreeWarp::split(const ossimDpt& point,
                              double splitHoriCoefficient,
                              double splitVertCoefficient)
{
   ossimQuadTreeWarpNode* node = findNode(point);

   if(node)
   {
      if(splitHoriCoefficient == 0.0)
      {
         splitHoriCoefficient = (point.x - node->theBoundingRect.ul().x)/
                                node->theBoundingRect.width();
      }
      if(splitVertCoefficient == 0.0)
      {
         splitVertCoefficient = (point.y - node->theBoundingRect.ul().y)/
                                node->theBoundingRect.height();
      }
      split(node,
            splitHoriCoefficient,
            splitVertCoefficient);
   }
}


void ossimQuadTreeWarp::split(ossimQuadTreeWarpNode* node,
                              double splitHoriCoefficient,
                              double splitVertCoefficient)
{
   if(!node) return;

   splitHoriCoefficient = splitHoriCoefficient>1?1:splitHoriCoefficient;
   splitHoriCoefficient = splitHoriCoefficient<0?0:splitHoriCoefficient;
   splitVertCoefficient = splitVertCoefficient>1?1:splitVertCoefficient;
   splitVertCoefficient = splitVertCoefficient<0?0:splitVertCoefficient;

   if( ((splitHoriCoefficient == 0)&&(splitVertCoefficient == 0)) ||
       ((splitHoriCoefficient == 1)&&(splitVertCoefficient == 1)))
   {
      return;
   }

   if(node->isLeaf())
   {
      if(node->theBoundingRect.hasNans())
      {
         ossimNotify(ossimNotifyLevel_WARN)  << "WARNING: " << "ossimQuadTreeWarp::split, Node has nans for the rect and can't split\n";
      }
      else
      {
         ossimDpt ul = node->theBoundingRect.ul();
         ossimDpt ur = node->theBoundingRect.ur();
         ossimDpt lr = node->theBoundingRect.lr();
         ossimDpt ll = node->theBoundingRect.lr();
         
         
         double xmid = ul.x + (ur.x - ul.x)*splitHoriCoefficient;
         double ymid = ul.y + (ll.y - ul.y)*splitVertCoefficient;
         
         ossimDpt left(ul.x, ymid);
         
         ossimDpt right(ur.x,
                        ymid);
         
         ossimDpt top(xmid,
                      ul.y);
         
         ossimDpt bottom(xmid,
                         lr.y);


         ossimDrect ulRect(ul.x,
                           ul.y,
                           xmid,
                           ymid);
         
         ossimDrect urRect(top.x,
                           top.y,
                           right.x,
                           right.y);
         
         ossimDrect lrRect(xmid,
                           ymid,
                           lr.x,
                           lr.y);
         
         ossimDrect llRect(left.x,
                           left.y,
                           bottom.x,
                           bottom.y);
         
         ossimQuadTreeWarpNode* ulNode=(ossimQuadTreeWarpNode*)NULL;
         ossimQuadTreeWarpNode* urNode=(ossimQuadTreeWarpNode*)NULL;
         ossimQuadTreeWarpNode* lrNode=(ossimQuadTreeWarpNode*)NULL;
         ossimQuadTreeWarpNode* llNode=(ossimQuadTreeWarpNode*)NULL;

         getNewQuads(node,
                     ulRect,
                     urRect,
                     lrRect,
                     llRect,
                     ulNode,
                     urNode,
                     lrNode,
                     llNode);

         if(ulNode&&urNode&&lrNode&&llNode)
         {
            node->theChildren.push_back(ulNode);
            node->theChildren.push_back(urNode);
            node->theChildren.push_back(lrNode);
            node->theChildren.push_back(llNode);            
            
            // it's no longer a leaf so remove the vertices
            // from the list
            node->removeVertex(node->theUlVertex);
            node->removeVertex(node->theUrVertex);
            node->removeVertex(node->theLrVertex);
            node->removeVertex(node->theLlVertex);
         }         
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: " << "ossimQuadTreeWarp::split, can only split leaf nodes\n";
   }
   
   updateAllVericeLockFlags();
}

void ossimQuadTreeWarp::getNewQuads(ossimQuadTreeWarpNode* parent,
                                    const ossimDrect& ul,
                                    const ossimDrect& ur,
                                    const ossimDrect& lr,
                                    const ossimDrect& ll,
                                    ossimQuadTreeWarpNode*& ulNode,
                                    ossimQuadTreeWarpNode*& urNode,
                                    ossimQuadTreeWarpNode*& lrNode,
                                    ossimQuadTreeWarpNode*& llNode)
{
   ossimDpt midShift;
   getShift(midShift,
            parent,
            ul.lr());
   
   ossimQuadTreeWarpVertex* midV = new ossimQuadTreeWarpVertex(ul.lr(),
                                                               midShift);

   ulNode = new ossimQuadTreeWarpNode(ul,
                                      parent);
   urNode = new ossimQuadTreeWarpNode(ur,
                                      parent);
   lrNode = new ossimQuadTreeWarpNode(lr,
                                      parent);
   llNode = new ossimQuadTreeWarpNode(ll,
                                      parent);

   midV->addSharedNode(ulNode);
   midV->addSharedNode(urNode);
   midV->addSharedNode(lrNode);
   midV->addSharedNode(llNode);
   
   // get the shared vertices first.  We will add ourself
   // to the pointer list. when we construct
   // the quad nodes.  Note the mid point will be shared
   // by all quads and will be marked as adjustable
   //
   ossimQuadTreeWarpVertex* ulSharedV = getVertex(ul.ul());
   ossimQuadTreeWarpVertex* urSharedV = getVertex(ur.ur());
   ossimQuadTreeWarpVertex* lrSharedV = getVertex(lr.lr());
   ossimQuadTreeWarpVertex* llSharedV = getVertex(ll.ll());
   
   if(!ulSharedV||
      !urSharedV||
      !lrSharedV||
      !llSharedV)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: " << "ossimQuadTreeWarp::split, can't locating shared vertices.  This Shouldn't happen!!!\n";
      return;
   }

   // we know that the midpoint is new and is shared by all quads
   // so we have 2 more to check
   ossimQuadTreeWarpVertex* topSharedV    = getVertex(ul.ur());
   ossimQuadTreeWarpVertex* bottomSharedV = getVertex(lr.ll());

   ossimQuadTreeWarpVertex* leftSharedV   = getVertex(ul.ll());
   ossimQuadTreeWarpVertex* rightSharedV  = getVertex(ur.lr());
   
   ossimDpt tempShift;
   
   if(!topSharedV)
   {
      getShift(tempShift, parent, ul.ur());
      topSharedV = new ossimQuadTreeWarpVertex(ul.ur(),
                                               tempShift);
      theVertexList.push_back(topSharedV);
   }
   if(!bottomSharedV)
   {
      getShift(tempShift, parent, ll.lr());
      bottomSharedV = new ossimQuadTreeWarpVertex(ll.lr(),
                                                  tempShift);
      
      theVertexList.push_back(bottomSharedV);
   }
   if(!leftSharedV)
   {
      getShift(tempShift, parent, ul.ll());
      leftSharedV = new ossimQuadTreeWarpVertex(ul.ll(),
                                               tempShift);
      theVertexList.push_back(leftSharedV);
   }
   if(!rightSharedV)
   {
      getShift(tempShift, parent, ur.lr());
      rightSharedV = new ossimQuadTreeWarpVertex(ur.lr(),
                                               tempShift);
      theVertexList.push_back(rightSharedV);
   }
   theVertexList.push_back(midV);

   topSharedV->addSharedNode(ulNode);
   topSharedV->addSharedNode(urNode);
   
   bottomSharedV->addSharedNode(llNode);
   bottomSharedV->addSharedNode(lrNode);
   leftSharedV->addSharedNode(ulNode);
   leftSharedV->addSharedNode(llNode);
   rightSharedV->addSharedNode(urNode);
   rightSharedV->addSharedNode(lrNode);

   ulSharedV->addSharedNode(ulNode); 
   urSharedV->addSharedNode(urNode);
   lrSharedV->addSharedNode(lrNode);
   llSharedV->addSharedNode(llNode);
  
   ulNode->theUlVertex = ulSharedV;
   ulNode->theUrVertex = topSharedV;
   ulNode->theLrVertex = midV;
   ulNode->theLlVertex = leftSharedV;

   urNode->theUlVertex = topSharedV;
   urNode->theUrVertex = urSharedV;
   urNode->theLrVertex = rightSharedV;
   urNode->theLlVertex = midV;

   lrNode->theUlVertex = midV;
   lrNode->theUrVertex = rightSharedV;
   lrNode->theLrVertex = lrSharedV;
   lrNode->theLlVertex = bottomSharedV;

   llNode->theUlVertex = leftSharedV;
   llNode->theUrVertex = midV;
   llNode->theLrVertex = bottomSharedV;
   llNode->theLlVertex = llSharedV;   
}

void ossimQuadTreeWarp::getShift(ossimDpt& result,
                                 const ossimQuadTreeWarpNode* node,
                                 const ossimDpt& pt)const
{
   result.x = 0.0;
   result.y = 0.0;

   if(!node)
   {
      return;
   }
   if(!node->isLeaf())
   {
      return;
   }

   if(node->hasValidVertices())
   {
      ossimDpt ulShift = node->theUlVertex->getDelta();
      ossimDpt urShift = node->theUrVertex->getDelta();
      ossimDpt lrShift = node->theLrVertex->getDelta();
      ossimDpt llShift = node->theLlVertex->getDelta();

      ossimDpt ul = node->theBoundingRect.ul();
      ossimDpt ur = node->theBoundingRect.ur();
      ossimDpt ll = node->theBoundingRect.ll();
      
      double horizontalPercent = fabs((pt.x-ul.x))/
                                 (ur.x-ul.x);
      
      double verticalPercent   = fabs((pt.y - ul.y))/
                                 (ll.y-ul.y);

      ossimDpt upper = ulShift + (urShift - ulShift)*horizontalPercent;
      ossimDpt lower = llShift + (lrShift - llShift)*horizontalPercent;
      
      result = upper + (lower - upper)*verticalPercent;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimQuadTreeWarp::getShift, " << "Node does not have valid vertices in ossimQuadTreeWarp::getShift\n";
   }
}

void ossimQuadTreeWarp::pruneTree(ossimQuadTreeWarpNode* node)
{
   if(node&&
      !node->isLeaf())
   {
      ossimQuadTreeWarpVertex* ulV = getVertex(node->theBoundingRect.ul());
      ossimQuadTreeWarpVertex* urV = getVertex(node->theBoundingRect.ur());
      ossimQuadTreeWarpVertex* lrV = getVertex(node->theBoundingRect.lr());
      ossimQuadTreeWarpVertex* llV = getVertex(node->theBoundingRect.ll());
      
      recursivePruneTree(node);

      if(ulV&&urV&&lrV&&llV)
      {
         node->theUlVertex = ulV;
         node->theUrVertex = urV;
         node->theLrVertex = lrV;
         node->theLlVertex = llV;

         ulV->addSharedNode(node);
         urV->addSharedNode(node);
         lrV->addSharedNode(node);
         llV->addSharedNode(node);
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimQuadTreeWarp::pruneTree, invlaid vertex find\n";
      }
      pruneSharedVertices();
      updateAllVericeLockFlags();
   }
}

void ossimQuadTreeWarp::recursivePruneTree(ossimQuadTreeWarpNode* node)
{
   if(!node||node->isLeaf()) return;
   
   for(ossim_uint32 i = 0; i < node->theChildren.size(); ++i)
   {
      recursivePruneTree(node->theChildren[i]);
      delete node->theChildren[i];
      node->theChildren[i] = NULL;
   }
   node->theChildren.clear();
}


ossimQuadTreeWarpNode* ossimQuadTreeWarp::findNode(const ossimDpt& pt)
{
   if((!pt.hasNans())&&(!isEmpty()))
   {
      if(theTree->theBoundingRect.pointWithin(pt))
      {
         return findNode(theTree,
                         pt);
      }
   }
   
   return (ossimQuadTreeWarpNode*)NULL;
}

ossimDpt ossimQuadTreeWarp::getShift(const ossimDpt& pt)const
{
   ossimDpt result;
   
   getShift(result, pt);
   
   return result;
}

const ossimQuadTreeWarpNode* ossimQuadTreeWarp::findNode(const ossimDpt& pt)const
{
   if((!pt.hasNans())&&(!isEmpty()))
   {
      if(theTree->theBoundingRect.pointWithin(pt))
      {
         return findNode(theTree,
                         pt);
      }
   }
   
   return (const ossimQuadTreeWarpNode*)NULL;
}

ossimQuadTreeWarpNode* ossimQuadTreeWarp::findNode(ossimQuadTreeWarpNode* node,
                                                   const ossimDpt& pt)
{
   ossimQuadTreeWarpNode* result = (ossimQuadTreeWarpNode*)NULL;
   
   if(!node)
   {
      return result;
   }
   if(node->isLeaf())
   {
      result = node;
   }
   else
   {
      bool found = false;
      for(ossim_uint32 i = 0; (i < node->theChildren.size())&&(!found); ++i)
      {
         if(node->theChildren[i]->theBoundingRect.pointWithin(pt))
         {
            result = findNode(node->theChildren[i],
                              pt);
            found = true;
         }
      }
   }

   return result;
}

void ossimQuadTreeWarp::findAllNodes(std::vector<ossimQuadTreeWarpNode*>& result,
                                     const ossimDpt& pt)
{
   if((!pt.hasNans())&&(!isEmpty()))
   {
      if(theTree->theBoundingRect.pointWithin(pt))
      {
         findAllNodes(result,
                      theTree,
                      pt);
      }
   }
   
}

void ossimQuadTreeWarp::findAllNodes(std::vector<const ossimQuadTreeWarpNode*>& result,
                                     const ossimDpt& pt)const
{
   if(!pt.hasNans()&&(!isEmpty()))
   {
      if(theTree->theBoundingRect.pointWithin(pt))
      {
         findAllNodes(result,
                      theTree,
                      pt);
      }
   }
   
}

const ossimQuadTreeWarpNode* ossimQuadTreeWarp::findNode(const ossimQuadTreeWarpNode* node,
                                                         const ossimDpt& pt)const
{
   const ossimQuadTreeWarpNode* result = (const ossimQuadTreeWarpNode*)NULL;
   
   if(!node)
   {
      return result;
   }
   if(node->isLeaf())
   {
      result = node;
   }
   else
   {
      bool found = false;
      for(ossim_uint32 i = 0; (i < node->theChildren.size())&&(!found); ++i)
      {
         if(node->theChildren[i]->theBoundingRect.pointWithin(pt))
         {
            result = findNode(node->theChildren[i],
                              pt);
            found = true;
         }
      }
   }

   return result;
}

void ossimQuadTreeWarp::findAllNodes(std::vector<ossimQuadTreeWarpNode*>& result,
                                     ossimQuadTreeWarpNode* node,
                                     const ossimDpt& pt)
{   
   if(node->isLeaf())
   {
      result.push_back(node);
   }
   else
   {
      for(ossim_uint32 i = 0;
          i < node->theChildren.size();
          ++i)
      {
         if(node->theChildren[i]->theBoundingRect.pointWithin(pt))
         {
            findAllNodes(result,
                         node->theChildren[i],
                         pt);
         }
      }
   }
}

void ossimQuadTreeWarp::findAllNodes(std::vector<const ossimQuadTreeWarpNode*>& result,
                                     ossimQuadTreeWarpNode* node,
                                     const ossimDpt& pt)const
{   
   if(!node) return;
   if(node->isLeaf())
   {
      result.push_back(node);
   }
   else
   {
      for(ossim_uint32 i = 0;
          i < node->theChildren.size();
          ++i)
      {
         if(node->theChildren[i]->theBoundingRect.pointWithin(pt))
         {
            findAllNodes(result,
                         node->theChildren[i],
                         pt);
         }
      }
   }
}
void ossimQuadTreeWarp::pruneSharedVertices()
{
   std::vector<ossimQuadTreeWarpVertex*>::iterator iter = theVertexList.begin();

   while(iter != theVertexList.end())
   {
      if( !(*iter)->isShared())
      {
         delete (*iter);
         iter = theVertexList.erase(iter);
      }
      else
      {
         ++iter;
      }
   }
}

void ossimQuadTreeWarp::removeSharedVertex(ossimQuadTreeWarpVertex* v)
{
   std::vector<ossimQuadTreeWarpVertex*>::iterator iter = std::find(theVertexList.begin(),
                                                                    theVertexList.end(),
                                                                    v);
   if(iter != theVertexList.end())
   {
      delete (*iter);
      iter = theVertexList.erase(iter);
   }
}


void ossimQuadTreeWarp::recursivePrint(std::ostream& out,
                                       ossimQuadTreeWarpNode* node)const
{
   if(node)
   {
      out << (*node) << "\n";
   }

   if(!node->isLeaf())
   {
      for(ossim_uint32 i =0; i < node->theChildren.size();++i)
      {
         recursivePrint(out, node->theChildren[i]);
      }
   }
}

void ossimQuadTreeWarp::recursiveDelete(ossimQuadTreeWarpNode* node)
{
   if(node->isLeaf())
   {
      delete node;
   }
   else
   {
      for(ossim_uint32 i = 0; i < node->theChildren.size(); ++ i)
      {
         recursiveDelete(node->theChildren[i]);
      }
      
      delete node;
   }
}
 
bool ossimQuadTreeWarp::isOnEdge(ossimQuadTreeWarpNode* node,
                                 const ossimDpt& point)const
{
   if(!node) return false;

   if(node->theBoundingRect.pointWithin(point))
   {
      double minx, maxx;
      double miny, maxy;
      node->theBoundingRect.getBounds(minx, miny, maxx, maxy);

      return ( (point.x == minx) ||
               (point.x == miny) ||
               (point.y == miny) ||
               (point.y == maxy) );
   }

   return false;
}

bool ossimQuadTreeWarp::isOnPoint(ossimQuadTreeWarpNode* node,
                                 const ossimDpt& point)const
{
   if(!node) return false;

   return ( (point == node->theBoundingRect.ul())||
            (point == node->theBoundingRect.ur())||
            (point == node->theBoundingRect.lr())||
            (point == node->theBoundingRect.ll()) );
}

void ossimQuadTreeWarp::updateLockFlag(ossimQuadTreeWarpVertex* v)
{
   std::vector<ossimQuadTreeWarpNode*> nodeList;
   
   findAllNodes(nodeList,
                v->getPosition());

   if(nodeList.size() != v->theSharedNodeList.size())
   {
      if(isOnEdge(theTree, v->getPosition()))
      {
         v->theLockedFlag = false;
      }
      else
      {
         v->theLockedFlag = true;
      }
   }
   else
   {
      v->theLockedFlag = false;
   }

   // if the original was not locked
   // then we need to make sure we change the delta
   // along the locked edge so to produce no artifacts
   //
   if(v->theLockedFlag)
   {
      updateDelta(v);
   }
}

void ossimQuadTreeWarp::updateDelta(ossimQuadTreeWarpVertex* v)
{
   ossimQuadTreeWarpVertex* top    = NULL;
   ossimQuadTreeWarpVertex* bottom = NULL;
   ossimQuadTreeWarpVertex* left   = NULL;
   ossimQuadTreeWarpVertex* right  = NULL;

   std::vector<ossimQuadTreeWarpVertex*>::iterator iter = theVertexList.begin();
   
   while(iter != theVertexList.end())
   {
      ossimQuadTreeWarpVertex* testV = (*iter);

      // test along the vertical
      if( (testV->thePosition.x == v->thePosition.x)&&
          (testV->thePosition.y != v->thePosition.y)&&
          (!testV->theLockedFlag))
      {
         if(testV->thePosition.y >  v->thePosition.y)
         {
            if(bottom)
            {
               if(bottom->thePosition.y > testV->thePosition.y)
               {
                  bottom = testV;
               }
            }
            else
            {
               bottom = testV;
            }
         }
         else
         {
            if(top)
            {
               if(top->thePosition.y < testV->thePosition.y)
               {
                  top = testV;
               }
            }
            else
            {
               top = testV;
            }            
         }
      }

      if( (testV->thePosition.y == v->thePosition.y)&&
          (testV->thePosition.x != v->thePosition.x)&&
          (!testV->theLockedFlag))
      {
         if(testV->thePosition.x >  v->thePosition.x)
         {
            if(right)
            {
               if(right->thePosition.x > testV->thePosition.x)
               {
                  right = testV;
               }
            }
            else
            {
               right = testV;
            }
         }
         else
         {
            if(left)
            {
               if(left->thePosition.x < testV->thePosition.x)
               {
                  left = testV;
               }
            }
            else
            {
               left = testV;
            }            
         }
      }
      
      ++iter;
   }
   ossimDpt topBottomDelta;
   ossimDpt leftRightDelta;

   topBottomDelta.makeNan();
   leftRightDelta.makeNan();
   
   if(top&&bottom)
   {
      double t = (v->thePosition.y - top->thePosition.y)/
                 (bottom->thePosition.y - top->thePosition.y);
      
      topBottomDelta = top->theDelta + (bottom->theDelta-top->theDelta)*t;
      v->theDelta = topBottomDelta;
      
   }
   if(left&&right)
   {
      double t = (v->thePosition.x - left->thePosition.x)/
                 (right->thePosition.x - left->thePosition.x);
      
      leftRightDelta = left->theDelta + (right->theDelta-left->theDelta)*t;
      v->theDelta = leftRightDelta;
   }

   if(top&&bottom&&left&&right)
   {
      v->theDelta = (topBottomDelta+leftRightDelta)*.5;
   }
}
   
void ossimQuadTreeWarp::updateAllVericeLockFlags()
{
   std::vector<ossimQuadTreeWarpVertex*>::iterator iter = theVertexList.begin();

   while(iter != theVertexList.end())
   {
      if(*iter)
      {
         updateLockFlag(*iter);
      }
      
      ++iter;
   }
}

const std::vector<ossimQuadTreeWarpVertex*>& ossimQuadTreeWarp::getVertices()const
{
   return theVertexList;
}

void ossimQuadTreeWarp::setWarpEnabledFlag(bool flag)
{
   theWarpEnabledFlag = flag;
}

void ossimQuadTreeWarp::setToIdentity()
{
   for(ossim_uint32 i = 0; i < theVertexList.size(); ++i)
   {
      theVertexList[i]->theDelta = ossimDpt(0,0);
   }
}

bool ossimQuadTreeWarp::saveState(ossimKeywordlist& kwl,
                                  const char* prefix)const
{
   for(ossim_uint32 i = 0; i < theVertexList.size(); ++i)
   {
      ossimString newPrefix = ossimString(prefix)+ "v" + ossimString::toString(i) +".";

      theVertexList[i]->saveState(kwl, newPrefix.c_str());
   }

   recursiveSave(theTree, kwl, prefix);
   
   return ossim2dTo2dTransform::saveState(kwl, prefix);   
}

bool ossimQuadTreeWarp::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   clear();
   ossimString newPrefix = ossimString(prefix);

   // load the vertices first
   //
   ossimString regExpression =  ossimString("^(") + newPrefix + "v[0-9]+\\.)";

   ossim_uint32 result = kwl.getNumberOfSubstringKeys(regExpression);
   
   ossim_uint32 numberOfMatches = 0;
   ossim_uint32 count = 0;
   while(numberOfMatches < result)
   {
      ossimString newPrefix = ossimString(prefix)+ossimString("v") + ossimString::toString(count) +".";
      
      ossimQuadTreeWarpVertex* vert = new ossimQuadTreeWarpVertex;
      
      if(!vert->loadState(kwl, newPrefix.c_str()))
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: "<< " ossimQuadTreeWarp::loadState, invalid load on vertex\n";
         delete vert;
         clear();
         
         return false;
      }
      else
      {
         ++numberOfMatches;
         theVertexList.push_back(vert);
      }
      
      ++count;
   }

   theTree = new ossimQuadTreeWarpNode;

   if(!theTree->loadState(kwl, prefix))
   {
      clear();
      return false;
   }
   if(!recursiveLoad(theTree, kwl, prefix))
   {
      clear();
      return false;
   }
   
   if(!ossim2dTo2dTransform::loadState(kwl, prefix))
   {
      clear();
      return false;
   }

   return true;
}

bool ossimQuadTreeWarp::recursiveSave(ossimQuadTreeWarpNode* node,
                                      ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   if(!node) return false;
   
   if(!node->saveState(kwl,
                      prefix))
   {
      return false;
   }
   
   if(node->isLeaf())
   {
      return true;
   }
   else
   {
     for(ossim_uint32 i = 0; i < node->theChildren.size(); ++ i)
       {
	 ossimString newPrefix = ossimString(prefix) + ossimString::toString(i) + ".";
      
	 if(!recursiveSave(node->theChildren[i],
                           kwl,
                           newPrefix.c_str()))
         {
            return false;
         }
       }
   }

   return true;
}

bool ossimQuadTreeWarp::recursiveLoad(ossimQuadTreeWarpNode* node,
                                      const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   if(!node) return false;
   ossimString copyPrefix = prefix;
   
   ossimQuadTreeWarpNode* ul = new ossimQuadTreeWarpNode;
   ossimQuadTreeWarpNode* ur = new ossimQuadTreeWarpNode;
   ossimQuadTreeWarpNode* lr = new ossimQuadTreeWarpNode;
   ossimQuadTreeWarpNode* ll = new ossimQuadTreeWarpNode;
   
   ossimString ulPre = copyPrefix + "0.";
   ossimString urPre = copyPrefix + "1.";
   ossimString lrPre = copyPrefix + "2.";
   ossimString llPre = copyPrefix + "3.";

   if(ul->loadState(kwl,
                    ulPre.c_str()))
   {
      ul->theParent = node;
      node->theChildren.push_back(ul);
      recursiveLoad(ul,
                    kwl,
                    ulPre.c_str());
   }
   else
   {
      delete ul;
      ul = NULL;
   }
   
   if(ur->loadState(kwl,
                    urPre.c_str()))
   {
      ur->theParent = node;
      node->theChildren.push_back(ur);
      recursiveLoad(ur,
                    kwl,
                    urPre.c_str());
   }
   else
   {
      delete ur;
      ur = NULL;
   }

   if(lr->loadState(kwl,
                    lrPre.c_str()))
   {
      lr->theParent = node;
      node->theChildren.push_back(lr);
      recursiveLoad(lr,
                    kwl,
                    lrPre.c_str());
   }
   else
   {
      delete lr;
      lr = NULL;
   }

   if(ll->loadState(kwl,
                    llPre.c_str()))
   {
      ll->theParent = node;
      node->theChildren.push_back(ll);
      recursiveLoad(ll,
                    kwl,
                    llPre.c_str());
   }
   else
   {
      delete ll;
      ll = NULL;
   }

   if(node->isLeaf())
   {
      node->theUlVertex = getVertex(node->theBoundingRect.ul());
      node->theUrVertex = getVertex(node->theBoundingRect.ur());
      node->theLrVertex = getVertex(node->theBoundingRect.lr());
      node->theLlVertex = getVertex(node->theBoundingRect.ll());

      if(node->hasValidVertices())
      {
         node->theUlVertex->addSharedNode(node);
         node->theUrVertex->addSharedNode(node);
         node->theLrVertex->addSharedNode(node);
         node->theLlVertex->addSharedNode(node);
      }
      else
      {
         return false;
      }
   }
   
   return true;
}

std::ostream& operator<<(std::ostream& out, const ossimQuadTreeWarpVertex& rhs)
{
   out << "Position:     " << rhs.thePosition
       << "\nDelta:        " << rhs.theDelta
       << "\nLocked flag:  " << rhs.theLockedFlag
       << "\nShared nodes: " << rhs.theSharedNodeList.size() << std::endl;
   
   return out;
}

std::ostream& operator <<(std::ostream& out,
                          const ossimQuadTreeWarpNode& rhs)
{
   out << "Bounding rect: " << rhs.theBoundingRect << std::endl;
   
   if(rhs.theUlVertex)
   {
      out << "ulVertex:\n" << *rhs.theUlVertex<< std::endl;
   }
   if(rhs.theUrVertex)
   {
      out << "urVertex:\n" << *rhs.theUrVertex<< std::endl;
   }
   if(rhs.theLrVertex)
   {
      out << "lrVertex:\n" << *rhs.theLrVertex<< std::endl;
   }
   if(rhs.theLlVertex)
   {
      out << "llVertex:\n" << *rhs.theLlVertex;
   }
   
   return out;
}

std::ostream& operator<<(std::ostream& out, const ossimQuadTreeWarp& rhs)
{
   rhs.print(out);
   
   return out;
}


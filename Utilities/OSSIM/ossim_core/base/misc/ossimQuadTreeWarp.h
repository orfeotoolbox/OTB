//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License 
// as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQuadTreeWarp.h,v 1.23 2003/12/22 18:48:32 dburken Exp $
#ifndef ossimQuadTreeWarp_HEADER
#define ossimQuadTreeWarp_HEADER
#include "base/data_types/ossimDpt.h"
#include "base/data_types/ossimDrect.h"
#include "base/common/ossimConstants.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/common/ossimKeywordNames.h"
#include "base/misc/ossim2dTo2dTransform.h"
#include <vector>
#include <iostream>

class OSSIMDLLEXPORT ossimQuadTreeWarpNode;

class OSSIMDLLEXPORT ossimQuadTreeWarpVertex
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimQuadTreeWarpVertex& rhs)
      {
         out << "Position:     " << rhs.thePosition   << endl
             << "Delta:        " << rhs.theDelta      << endl
             << "Locked flag:  " << rhs.theLockedFlag << endl
             << "Shared nodes: " << rhs.theSharedNodeList.size();

         return out;
      }
   
   ossimQuadTreeWarpVertex(const ossimDpt& position=ossimDpt(0,0),
                       const ossimDpt& delta=ossimDpt(0,0))
      :thePosition(position),
       theDelta(delta),
       theLockedFlag(false)
      {
      }
   ~ossimQuadTreeWarpVertex();
   
   void setPosition(const ossimDpt& position)
      {
         thePosition = position;
      }
   void setDelta(const ossimDpt& delta)
      {
         theDelta    = delta;
      }
   const ossimDpt& getDelta()const
      {
         return theDelta;
      }
   const ossimDpt& getPosition()const
      {
         return thePosition;
      }
   void addSharedNode(ossimQuadTreeWarpNode* node)
      {
         if(node)
         {
            theSharedNodeList.push_back(node);
         }
      }
   void removeNode(ossimQuadTreeWarpNode* node);
   bool isShared()const
      {
         return (theSharedNodeList.size() > 0);
      }

   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix);
   
   vector<ossimQuadTreeWarpNode*> theSharedNodeList;
   
   ossimDpt                       thePosition;
   ossimDpt                       theDelta;
   bool                           theLockedFlag;
   
};

class ossimQuadTreeWarpNode
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                   const ossimQuadTreeWarpNode& rhs)
      {
         out << "Bounding rect: " << rhs.theBoundingRect << endl;

         if(rhs.theUlVertex)
         {
            out << "ulVertex:\n" << *rhs.theUlVertex<< endl;
         }
         if(rhs.theUrVertex)
         {
            out << "urVertex:\n" << *rhs.theUrVertex<< endl;
         }
         if(rhs.theLrVertex)
         {
            out << "lrVertex:\n" << *rhs.theLrVertex<< endl;
         }
         if(rhs.theLlVertex)
         {
            out << "llVertex:\n" << *rhs.theLlVertex;
         }

         return out;
      }
   ~ossimQuadTreeWarpNode();
   ossimQuadTreeWarpNode()
      :
        theUlVertex(NULL),
        theUrVertex(NULL),
        theLrVertex(NULL),
        theLlVertex(NULL),
        theParent(NULL)
      {
         theBoundingRect.makeNan();
      }
   
   ossimQuadTreeWarpNode(const ossimDrect& bounds,
                         ossimQuadTreeWarpNode* parent=NULL,
                         ossimQuadTreeWarpVertex* ulVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* urVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* lrVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* llVertex=(ossimQuadTreeWarpVertex*)NULL)
      : theBoundingRect(bounds),
        theUlVertex(ulVertex),
        theUrVertex(urVertex),
        theLrVertex(lrVertex),
        theLlVertex(llVertex),
        theParent(parent)
      {
      }

   bool hasValidVertices()const
      {
         return (theUlVertex&&theUrVertex&&theLrVertex&&theLlVertex);
      }
   
   void removeVertex(ossimQuadTreeWarpVertex* v)
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
   void removeVertices()
      {
         removeVertex(theUlVertex);
         removeVertex(theUrVertex);
         removeVertex(theLrVertex);
         removeVertex(theLlVertex);
      }
   
   bool isLeaf()const
      {
         return (theChildren.size() == 0);
      }
   void clear()
      {
         theBoundingRect.makeNan();
         theChildren.clear();
         theParent = NULL;
         removeVertex(theUlVertex);
         removeVertex(theUrVertex);
         removeVertex(theLrVertex);
         removeVertex(theLlVertex);
      }
   void removeChild(ossimQuadTreeWarpNode* node);
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix = 0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix = 0);

   ossimDrect                 theBoundingRect;
   ossimQuadTreeWarpVertex*   theUlVertex;
   ossimQuadTreeWarpVertex*   theUrVertex;
   ossimQuadTreeWarpVertex*   theLrVertex;
   ossimQuadTreeWarpVertex*   theLlVertex;
   ossimQuadTreeWarpNode*     theParent;
   
   vector<ossimQuadTreeWarpNode*> theChildren;
};

class ossimQuadTreeWarp : public ossim2dTo2dTransform
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimQuadTreeWarp& rhs)
      {
         rhs.print(out);

         return out;
      }

   ossimQuadTreeWarp()
      :theTree(NULL)
      {
      }
   
   ossimQuadTreeWarp(const ossimDrect& boundingRect,
                     const ossimDpt& ulShift=ossimDpt(0,0),
                     const ossimDpt& urShift=ossimDpt(0,0),
                     const ossimDpt& lrShift=ossimDpt(0,0),
                     const ossimDpt& llShift=ossimDpt(0,0))
      :theTree(NULL)
      {
         create(boundingRect, ulShift, urShift, lrShift, llShift);
      }

   ossimQuadTreeWarp::ossimQuadTreeWarp(const ossimQuadTreeWarp& rhs);
   virtual ossimObject* dup()const
      {
         return new ossimQuadTreeWarp(*this);
      }
   
   virtual ~ossimQuadTreeWarp()
      {
         clear();
      }
   
   virtual ossimDpt getOrigin()const
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

   void create(const ossimDrect& boundingRect,
               const ossimDpt& ulShift=ossimDpt(0,0),
               const ossimDpt& urShift=ossimDpt(0,0),
               const ossimDpt& lrShift=ossimDpt(0,0),
               const ossimDpt& llShift=ossimDpt(0,0));
               
   /*!
    * Will warp the passed in point and placed the warped value in result
    */
   virtual void forward(const ossimDpt& pt,
                        ossimDpt& result)const;   
   /*!
    * Will warp the passed in point and overwrite it
    */
   virtual void forward(ossimDpt& pt)const;
   
//    void inverse(const ossimDpt& input,
//                 ossimDpt&       output) const;
   
   virtual std::ostream& print(std::ostream& out) const;
   virtual void printVertices(std::ostream& out)const;

   ossimQuadTreeWarpNode* getRoot()
      {
         return theTree;
      }
   const ossimQuadTreeWarpNode* getRoot()const
      {
         return theTree;
      }
   
   bool isEmpty()const
      {
         return (theTree==NULL);
      }

   ossimQuadTreeWarpVertex* findClosestVertex(ossimDpt& position);
   
   /*!
    * This method will go through the tree and try to locate the
    * vertex.  This is faster sine the tree is sorted.  Use
    * the getVertex to just search the internal shared vertex list.
    */
   ossimQuadTreeWarpVertex* findVertex(const ossimDpt& position);

   /*!
    * Searches the shared vetex list.  This will not go through the tree.
    * this is needed for load states to perform correctly.
    */
   ossimQuadTreeWarpVertex* getVertex(const ossimDpt& position);
   
   /*!
    * Will get the shift or delta value for the passed in pt.
    */
   void getShift(ossimDpt& result,
                 const ossimDpt& pt)const;

   /*!
    * The initial passed in node must not be a leaf.  It will
    * turn the passed in node into a leaf by recursively
    * pruning its children.  
    */
   void pruneTree(ossimQuadTreeWarpNode* node);
   
   // Search methods
   ossimQuadTreeWarpNode* findNode(const ossimDpt& pt);
   
   const ossimQuadTreeWarpNode* findNode(const ossimDpt& pt)const;
   
    /*!
    * Will get the shift or delta value for the passed in pt.
    */
   ossimDpt getShift(const ossimDpt& pt)const
      {
         ossimDpt result;
         
         getShift(result, pt);
         
         return result;
      }

   
   void clear();

   void split(const ossimDpt& point,
              double splitHoriCoefficient=0.0,
              double splitVertCoefficient=0.0);

   /*!
    * The passed in node is suppose to be a leaf node.
    * it splits the leaf node horizontally and vertically
    * by the passed in percentage.  Use find node to
    * find the leaf node of a given point 
    */
   void split(ossimQuadTreeWarpNode* node,
              double splitHoriCoefficient=.5,
              double splitVertCoefficient=.5);


   const ossimQuadTreeWarpNode* findNode(const ossimQuadTreeWarpNode* node,
                                         const ossimDpt& pt)const;
   
   ossimQuadTreeWarpNode* findNode(ossimQuadTreeWarpNode* node,
                                   const ossimDpt& pt);

   void findAllNodes(std::vector<ossimQuadTreeWarpNode*>& result,
                     const ossimDpt& pt);
   void findAllNodes(std::vector<const ossimQuadTreeWarpNode*>& result,
                     const ossimDpt& pt)const;
   void findAllNodes(std::vector<ossimQuadTreeWarpNode*>& result,
                     ossimQuadTreeWarpNode* node,
                     const ossimDpt& pt);
   void findAllNodes(std::vector<const ossimQuadTreeWarpNode*>& result,
                     ossimQuadTreeWarpNode* node,
                     const ossimDpt& pt)const;
  
   bool isOnEdge(ossimQuadTreeWarpNode* node,
                 const ossimDpt& point)const;

   bool isOnPoint(ossimQuadTreeWarpNode* node,
                  const ossimDpt& point)const;

   void updateAllVericeLockFlags();

   const std::vector<ossimQuadTreeWarpVertex*>& getVertices()const
      {
         return theVertexList;
      }
   void setWarpEnabledFlag(bool flag)
      {
         theWarpEnabledFlag = flag;
      }
   void setToIdentity();
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
      
protected:
   bool                             theWarpEnabledFlag;
   ossimQuadTreeWarpNode*           theTree;
   std::vector<ossimQuadTreeWarpVertex*> theVertexList;

   void getShift(ossimDpt& result,
                 const ossimQuadTreeWarpNode* node,
                 const ossimDpt& pt)const;

   void recursivePruneTree(ossimQuadTreeWarpNode* node);
   void pruneSharedVertices();
   void removeSharedVertex(ossimQuadTreeWarpVertex* v);

   void recursivePrint(std::ostream& out,
                       ossimQuadTreeWarpNode* node)const;
   void recursiveDelete(ossimQuadTreeWarpNode* node);

   void getNewQuads(ossimQuadTreeWarpNode* parent,
                    const ossimDrect& ul,
                    const ossimDrect& ur,
                    const ossimDrect& lr,
                    const ossimDrect& ll,
                    ossimQuadTreeWarpNode*& ulNode,
                    ossimQuadTreeWarpNode*& urNode,
                    ossimQuadTreeWarpNode*& lrNode,
                    ossimQuadTreeWarpNode*& llNode);
   
   void updateLockFlag(ossimQuadTreeWarpVertex* v);
   void updateDelta(ossimQuadTreeWarpVertex* v);
   
   bool recursiveSave(ossimQuadTreeWarpNode* node,
                      ossimKeywordlist& kwl,
                      const char* prefix)const;

   bool recursiveLoad(ossimQuadTreeWarpNode* node,
                      const ossimKeywordlist& kwl,
                      const char* prefix);

//   bool resetAllSharedLists();
   
TYPE_DATA
};

#endif

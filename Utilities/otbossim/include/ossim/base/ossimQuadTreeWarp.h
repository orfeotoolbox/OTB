//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQuadTreeWarp.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimQuadTreeWarp_HEADER
#define ossimQuadTreeWarp_HEADER

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <vector>
#include <iosfwd>

class ossimQuadTreeWarpNode;

class OSSIMDLLEXPORT ossimQuadTreeWarpVertex
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                              const ossimQuadTreeWarpVertex& rhs);
   
   ossimQuadTreeWarpVertex(const ossimDpt& position=ossimDpt(0,0),
                           const ossimDpt& delta=ossimDpt(0,0));

   ~ossimQuadTreeWarpVertex();
   
   void setPosition(const ossimDpt& position);

   void setDelta(const ossimDpt& delta);

   const ossimDpt& getDelta()const;

   const ossimDpt& getPosition()const;

   void addSharedNode(ossimQuadTreeWarpNode* node);

   void removeNode(ossimQuadTreeWarpNode* node);

   bool isShared()const;

   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix);
   
   std::vector<ossimQuadTreeWarpNode*> theSharedNodeList;
   
   ossimDpt                       thePosition;
   ossimDpt                       theDelta;
   bool                           theLockedFlag;
   
};

class ossimQuadTreeWarpNode
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimQuadTreeWarpNode& rhs);

   ossimQuadTreeWarpNode();

   ossimQuadTreeWarpNode(const ossimDrect& bounds,
                         ossimQuadTreeWarpNode* parent=NULL,
                         ossimQuadTreeWarpVertex* ulVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* urVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* lrVertex=(ossimQuadTreeWarpVertex*)NULL,
                         ossimQuadTreeWarpVertex* llVertex=(ossimQuadTreeWarpVertex*)NULL);

   ~ossimQuadTreeWarpNode();

   bool hasValidVertices()const;
   
   void removeVertex(ossimQuadTreeWarpVertex* v);

   void removeVertices();
   
   bool isLeaf()const;

   void clear();

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
   
   std::vector<ossimQuadTreeWarpNode*> theChildren;
};

class ossimQuadTreeWarp : public ossim2dTo2dTransform
{
public:
   friend std::ostream& operator<<(std::ostream& out,
                                   const ossimQuadTreeWarp& rhs);

   ossimQuadTreeWarp();
   
   ossimQuadTreeWarp(const ossimDrect& boundingRect,
                     const ossimDpt& ulShift=ossimDpt(0,0),
                     const ossimDpt& urShift=ossimDpt(0,0),
                     const ossimDpt& lrShift=ossimDpt(0,0),
                     const ossimDpt& llShift=ossimDpt(0,0));

   ossimQuadTreeWarp(const ossimQuadTreeWarp& rhs);

   virtual ossimObject* dup()const;

   
   virtual ossimDpt getOrigin()const;

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

   ossimQuadTreeWarpNode* getRoot();

   const ossimQuadTreeWarpNode* getRoot()const;

   bool isEmpty()const;

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
   ossimDpt getShift(const ossimDpt& pt)const;
   
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

   const std::vector<ossimQuadTreeWarpVertex*>& getVertices()const;

   void setWarpEnabledFlag(bool flag);

   void setToIdentity();
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
      
protected:
   virtual ~ossimQuadTreeWarp();
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

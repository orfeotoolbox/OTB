//*****************************************************************************
// FILE: ossimPolygon.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimPolygon.
//   This class provides utilities associated with N-vertex, convex
//   (i.e., only two intersections for any line passing through the polygon).
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimPolygon.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimPolygon_HEADER
#define ossimPolygon_HEADER
#include <iostream>
#include <vector>
using namespace std;

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>

class OSSIMDLLEXPORT ossimLine;

/******************************************************************************
 *
 * CLASS:  ossimPolygon
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimPolygon
{
public:
   ossimPolygon();
   ossimPolygon(const vector<ossimIpt>& polygon);
   ossimPolygon(const vector<ossimDpt>& polygon);
   ossimPolygon(int numVertices, const ossimDpt* vertex_array);

   ossimPolygon(const ossimPolygon& copy_this);
   
   /**
    * CONSTRUCTOR: Provided for convenience. Does not imply the polygon is
    * limited to four vertices:
    */
   ossimPolygon(ossimDpt v1,
                ossimDpt v2,
                ossimDpt v3,
                ossimDpt v4);
   
   ossimPolygon(const ossimIrect& rect);
   ossimPolygon(const ossimDrect& rect);

   ~ossimPolygon();

   ossimDpt& operator[](int index);
   
   const ossimDpt& operator[](int index)const;

   ossim_uint32 getVertexCount()const;
   
   ossim_uint32 getNumberOfVertices()const;

   double area()const;

   void getIntegerBounds(ossim_int32& minX,
                         ossim_int32& minY,
                         ossim_int32& maxX,
                         ossim_int32& maxY)const;

   void getBoundingRect(ossimIrect& rect)const;

   /**
    * Initializes minRect with the minimum area rect (not-necessarily
    * aligned with axes) that bounds this polygon.
    *
    * @param minRect Polygon to initialize with the minimum rect.
    */
   void getMinimumBoundingRect(ossimPolygon& minRect) const;
   
   void roundToIntegerBounds(bool compress=false);

   void clear();

   void addPoint(const ossimDpt& pt);
   void addPoint(double x, double y);

   ossimDpt midPoint()const;
   
   /**
    * will sequence through the polygon and check to see if any values are NAN
    */
   bool hasNans()const;

   const vector<ossimDpt>& getVertexList()const;
      
   /**
    * Uses the ossimPolyArea2d class for the intersection
    */
   bool clipToRect(vector<ossimPolygon>& result,
                   const ossimDrect& rect)const;

   
   /**
    * METHOD: clipLineSegment(p1, p2)
    * Implements Cyrus-Beck clipping algorithm as described in:
    * http://www.daimi.au.dk/~mbl/cgcourse/wiki/cyrus-beck_line-clipping_.html
    * Clips the line segment defined by thw two endpoints provided. The
    * endpoints are modified as needed to represent the clipped line. Returnes
    * true if intersection present.
    */
   bool clipLineSegment(ossimDpt& p1, ossimDpt& p2) const;

   /**
    * METHOD: pointWithin(ossimDpt)
    * Returns TRUE if point is inside polygon.
    */
   bool pointWithin(const ossimDpt& point) const;

   bool isPointWithin(const ossimDpt& point) const;

   /**
    * METHOD: vertex(index)
    * Returns the ossimDpt vertex given the index. Returns false if no vertex
    * defined.
    */
   bool vertex(int index, ossimDpt& tbd_vertex) const;

   /**
    * METHOD: nextVertex()
    * Assigns the ossimDpt tbd_vertex following the current vertex. The current
    * vertex is initialized with a call to vertex(int), or after the last
    * vertex is reached. Returns false if no vertex defined. Intended to be
    * when cycling through all vertices.
    */
   bool nextVertex(ossimDpt& tbd_vertex) const;

   void reverseOrder();
   /**
    * OPERATORS: (Some are inlined at bottom) 
    */
   const ossimPolygon& operator= (const ossimPolygon& copy_this);
   const ossimPolygon& operator= (const vector<ossimDpt>& vertexList);
   const ossimPolygon& operator= (const vector<ossimIpt>& vertexList);
   const ossimPolygon& operator= (const ossimIrect& rect);
   const ossimPolygon& operator= (const ossimDrect& rect);
   bool                operator==(const ossimPolygon& compare_this) const;
   bool                operator!=(const ossimPolygon& compare_this) const;

   const ossimPolygon& operator *=(const ossimDpt& scale);
   const ossimPolygon& operator *=(double scale);
   
   ossimPolygon operator *(const ossimDpt& scale)const;
   ossimPolygon operator *(double scale)const;
   
   void resize(ossim_uint32 newSize);

   /**
    * METHOD: print()
    */
   void print(ostream& os) const;
   friend ostream& operator<<(ostream&, const ossimPolygon&);


   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);

   ossimVertexOrdering checkOrdering()const;

   ossimVertexOrdering getOrdering()const;

protected:
   /**
    * EMBEDDED CLASS: PolygonEdge
    * A polygon consists of an ordered list of edges. The ordering corresponds
    * to the ordering of the vertices. Each edge has a precomputed normal.
    * The normals point towards the exterior.
    */
   vector<ossimDpt> theVertexList;

   
   mutable ossim_int32 theCurrentVertex;

   /**
    * This enumeration is found in ossimConstants.h
    */
   mutable ossimVertexOrdering theOrderingType;


   void intersectEdge(ossimDpt& result,
                      const ossimLine& segment,
                      const ossimDrect& rect,
                      int edge);
   
   bool isInsideEdge(const ossimDpt& pt,
                     const ossimDrect& rect,
                     int edge)const;
};

#endif /* End of "#ifndef ossimPolygon_HEADER" */

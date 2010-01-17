//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimPolygon.
//   This class provides utilities associated with N-vertex, convex
//   (i.e., only two intersections for any line passing through the polygon).
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimPolyLine.h 15833 2009-10-29 01:41:53Z eshirschorn $

#ifndef ossimPolyLine_HEADER
#define ossimPolyLine_HEADER
#include <iostream>
#include <vector>
using namespace std;

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimString.h>
class ossimLine;
class ossimPolygon;

/*!****************************************************************************
 *
 * CLASS:  ossimPolyLine
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimPolyLine
{
public:
   ossimPolyLine()
      :theCurrentVertex(0)
      {}
   ossimPolyLine(const vector<ossimIpt>& polygon);
   ossimPolyLine(const vector<ossimDpt>& polygon);
   ossimPolyLine(int numVertices, const ossimDpt* vertex_array);
   
   ossimPolyLine(const ossimPolyLine& copy_this);
   ossimPolyLine(const ossimPolygon& polygon);
   
   /*!
    * CONSTRUCTOR: Provided for convenience. Does not imply the polygon is
    * limited to four vertices:
    */
   ossimPolyLine(ossimDpt v1,
                ossimDpt v2,
                ossimDpt v3,
                ossimDpt v4);
   
   ossimPolyLine(const ossimIrect& rect);
   ossimPolyLine(const ossimDrect& rect);

   ~ossimPolyLine();

   ossimDpt& operator[](int index)
      {
         return theVertexList[index];
      }
   const ossimDpt& operator[](int index)const
      {
         return theVertexList[index];
      }
   
   ossim_uint32 getNumberOfVertices()const
      {
         return (ossim_uint32)theVertexList.size();
      }
   
   void getIntegerBounds(ossim_int32& minX,
                         ossim_int32& minY,
                         ossim_int32& maxX,
                         ossim_int32& maxY)const;
   void getBounds(double& minX,
                  double& minY,
                  double& maxX,
                  double& maxY)const;
   void getBoundingRect(ossimDrect& rect)const
      {
         ossim_int32 minX;
         ossim_int32 minY;
         ossim_int32 maxX;
         ossim_int32 maxY;
         getIntegerBounds(minX, minY, maxX, maxY);
         rect = ossimDrect(minX, minY, maxX, maxY);
         
      }
   ossimDrect getBoundingRect()const
      {
         ossimDrect result;

         getBoundingRect(result);

         return result;
      }
   
   void roundToIntegerBounds(bool compress=true);
   void clear()
      {
         theVertexList.clear();
      }
   void addPoint(const ossimDpt& pt)
      {
         theVertexList.push_back(pt);
      }
   void addPoint(double x, double y)
      {
         theVertexList.push_back(ossimDpt(x, y));
      }
   void addAttribute( const ossimString& attribute )
     {
       theAttributeList.push_back( attribute );
     }

   ossimDpt midPoint()const;
   
   /*!
    * will sequence through the polygon and check to see if any values are NAN
    */
   bool hasNans()const;
  
  ossim_uint32 size()
  {
    return getNumberOfVertices();
  }
  void resize(ossim_uint32 newSize)
  {
    theVertexList.resize(newSize);
  }
   const vector<ossimDpt>& getVertexList()const
      {
         return theVertexList;
      }

   vector<ossimDpt>& getVertexList()
      {
         return theVertexList;
      }
   vector<ossimString>& getAttributeList()
      {
         return theAttributeList;
      }
      
   bool clipToRect(vector<ossimPolyLine>& result,
                   const ossimDrect& rect)const;

   /*!
    * Will clip this poly line list to the past in rect and
    * will return true if any part of this object is visible within
    * the rectangle;
    */
   bool isWithin(const ossimDrect& rect)const;
   
   
   /*!
    * METHOD: pointWithin(ossimDpt)
    * Returns TRUE if point is inside polygon.
    */
   bool pointWithin(const ossimDpt& point) const
      {
         return isPointWithin(point);
      }
   bool isPointWithin(const ossimDpt& point) const;

   /*!
    * METHOD: vertex(index)
    * Returns the ossimDpt vertex given the index. Returns false if no vertex
    * defined.
    */
   bool vertex(int index, ossimDpt& tbd_vertex) const;

   /*!
    * METHOD: nextVertex()
    * Assigns the ossimDpt tbd_vertex following the current vertex. The current
    * vertex is initialized with a call to vertex(int), or after the last
    * vertex is reached. Returns false if no vertex defined. Intended to be
    * when cycling through all vertices.
    */
   bool nextVertex(ossimDpt& tbd_vertex) const;

   void reverseOrder();
   /*!
    * OPERATORS: (Some are inlined at bottom) 
    */
   const ossimPolyLine& operator= (const ossimPolyLine& copy_this);
   const ossimPolyLine& operator= (const vector<ossimDpt>& vertexList);
   const ossimPolyLine& operator= (const vector<ossimIpt>& vertexList);
   const ossimPolyLine& operator= (const ossimIrect& rect);
   const ossimPolyLine& operator= (const ossimPolygon& polygon);
   const ossimPolyLine& operator= (const ossimDrect& rect);
   bool                operator==(const ossimPolyLine& compare_this) const;
   bool                operator!=(const ossimPolyLine& compare_this) const;

   const ossimPolyLine& operator *=(const ossimDpt& scale);
   const ossimPolyLine& operator *=(double scale)
      {
         return ((*this)*=ossimDpt(scale, scale));
      }
   
   ossimPolyLine operator *(const ossimDpt& scale)const;
   ossimPolyLine operator *(double scale)const
      {
         return ((*this)*ossimDpt(scale, scale));
      }

   /*!
    * METHOD: print()
    */
   void print(ostream& os) const;
   friend ostream& operator<<(ostream&, const ossimPolyLine&);


   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
protected:
   vector<ossimDpt> theVertexList;
   vector<ossimString> theAttributeList;
   
   mutable ossim_int32 theCurrentVertex;
};

inline bool ossimPolyLine::operator!=(const ossimPolyLine& compare_this) const
{
   return !(*this == compare_this);
}

inline ostream& operator<<(ostream& os, const ossimPolyLine& polyLine)
{
   polyLine.print(os);
   return os;
}

#endif

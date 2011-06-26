//*****************************************************************************
// FILE: ossimPolygon.cpp
//
// License:  LGPL
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimPolygon.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimLine.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimString.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iterator>

static const char* NUMBER_VERTICES_KW = "number_vertices";
static const char* VERTEX_ORDER_KW    = "order";

static const int RECT_LEFT_EDGE   = 0;
static const int RECT_TOP_EDGE    = 1;
static const int RECT_RIGHT_EDGE  = 2;
static const int RECT_BOTTOM_EDGE = 3;

ossimPolygon::ossimPolygon()
   : theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN),
    theVertexList(),
    theCurrentVertex(0)
   
{}

ossimPolygon::ossimPolygon(const vector<ossimIpt>& polygon)
   :theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN),
   theVertexList(polygon.size()),
   theCurrentVertex(0)
   
{
   // Assign std::vector<ossimIpt> list to std::vector<ossimDpt> theVertexList.
   for (std::vector<ossimIpt>::size_type i = 0; i < polygon.size(); ++i)
   {
      theVertexList[i] = polygon[i];
   }
}

ossimPolygon::ossimPolygon(const vector<ossimDpt>& polygon)
   :theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN),
   theVertexList(polygon),
    theCurrentVertex(0)
{
}

//*****************************************************************************
//  CONSTRUCTOR: ossimPolygon(int numVertices, const ossimDpt* vertex_array)
//  
//*****************************************************************************
ossimPolygon::ossimPolygon(int numVertices, const ossimDpt* v)
   : theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN),
     theCurrentVertex(0)
     
{
   theVertexList.insert(theVertexList.begin(),
                        v, v+numVertices);
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimPolygon(ossimPolygon)
//  
//*****************************************************************************
ossimPolygon::ossimPolygon(const ossimPolygon& polygon)
   :theCurrentVertex(0)
{
   *this = polygon;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimPolygon(p1, p2, p3, p4)
//  
//  Provided for convenience. Does not imply the polygon is limited to four
//  vertices
//  
//*****************************************************************************
ossimPolygon::ossimPolygon(ossimDpt v1,
                           ossimDpt v2,
                           ossimDpt v3,
                           ossimDpt v4)
   : theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN),
     theVertexList(4),
     theCurrentVertex(0)
     
{
   theVertexList[0] = v1;
   theVertexList[1] = v2;
   theVertexList[2] = v3;
   theVertexList[3] = v4;
}

ossimPolygon::ossimPolygon(const ossimIrect& rect)
: theOrderingType(OSSIM_CLOCKWISE_ORDER),
  theVertexList(4),
  theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}

ossimPolygon::ossimPolygon(const ossimDrect& rect)
: theOrderingType(OSSIM_CLOCKWISE_ORDER),
theVertexList(4),
theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}


//*****************************************************************************
//  DESTRUCTOR: ~ossimPolygon
//  
//*****************************************************************************
ossimPolygon::~ossimPolygon()
{
}


//*************************************************************************************************
//! Returns polygon area. Negative indicates CW ordering of vertices (in right-handed coordinates
//*************************************************************************************************
double ossimPolygon::area()const
{
   double area = 0;
   ossim_uint32 i=0;
   ossim_uint32 j=0;
   ossim_uint32 size = (ossim_uint32)theVertexList.size();
   
   for (i=0;i<size;i++)
   {
      j = (i + 1) % (int)size;
      area += theVertexList[i].x * theVertexList[j].y;
      area -= theVertexList[i].y * theVertexList[j].x;
   }

   area /= 2;

   return area;
}

void ossimPolygon::roundToIntegerBounds(bool compress)
{
   int i = 0;
   for(i = 0; i < (int)theVertexList.size(); ++i)
   {
      theVertexList[i] = ossimIpt(theVertexList[i]);
   }

  if(compress&&theVertexList.size())
   {
      vector<ossimDpt> polyLine;
      
      polyLine.push_back(theVertexList[0]);
      ossimDpt testPt = theVertexList[0];
      for(i=1; i < (int)theVertexList.size(); ++i)
      {
         if(testPt!=theVertexList[i])
         {
            testPt = theVertexList[i];
            polyLine.push_back(testPt);
         }
      }
      if(polyLine.size() == 1)
      {
         polyLine.push_back(polyLine[0]);
      }
      
      if(theVertexList.size() == 1)
      {
         polyLine.push_back(testPt);
      }
      theVertexList    = polyLine;
      theCurrentVertex = 0;
   }
}

ossimDpt ossimPolygon::midPoint()const
{
   int upper = (int)theVertexList.size();
   ossimDpt result(0.0, 0.0);
   int i = 0;

   if(!upper)
   {
      result.makeNan();
   }
   else
   {
      for(i = 0; i < upper; ++i)
      {
         result.x+=theVertexList[i].x;
         result.y+=theVertexList[i].y;
      }
      result.x/=(double)upper;
      result.y/=(double)upper;
   }
   
   return result;
}

bool ossimPolygon::hasNans()const
{
   int upper = (int)theVertexList.size();
   int i = 0;

   for(i = 0; i < upper; ++i)
   {
      if(theVertexList[i].hasNans())
      {
         return true;
      }
   }

   return false;
}

void ossimPolygon::getIntegerBounds(ossim_int32& minX,
   ossim_int32& minY,
   ossim_int32& maxX,
   ossim_int32& maxY)const
{
   ossim_int32 npoly = (ossim_int32)theVertexList.size();
   int i = 0;

   if(npoly)
   {
      minX = (ossim_int32)floor(theVertexList[0].x);
      maxX = (ossim_int32)ceil(theVertexList[0].x);
      minY = (ossim_int32)floor(theVertexList[0].y);
      maxY = (ossim_int32)ceil(theVertexList[0].y);

      for(i =1; i < npoly; ++i)
      {
         minX = std::min((ossim_int32)floor(theVertexList[i].x),
            (ossim_int32)minX);
         maxX = std::max((ossim_int32)ceil(theVertexList[i].x),
            (ossim_int32)maxX);
         minY = std::min((ossim_int32)floor(theVertexList[i].y),
            (ossim_int32)minY);
         maxY = std::max((ossim_int32)ceil(theVertexList[i].y),
            (ossim_int32)maxY);
      }
   }
   else
   {
      minX = OSSIM_INT_NAN;
      minY = OSSIM_INT_NAN;
      maxX = OSSIM_INT_NAN;
      maxY = OSSIM_INT_NAN;
   }
}

void ossimPolygon::getFloatBounds(ossim_float64& minX,
                                  ossim_float64& minY,
                                  ossim_float64& maxX,
                                  ossim_float64& maxY) const
{
   ossim_int32 npoly = (ossim_int32)theVertexList.size();
   int i = 0;

   if(npoly)
   {
      minX = floor(theVertexList[0].x);
      maxX = ceil(theVertexList[0].x);
      minY = floor(theVertexList[0].y);
      maxY = ceil(theVertexList[0].y);

      for(i =1; i < npoly; ++i)
      {
         minX = std::min<double>(floor(theVertexList[i].x), minX);
         maxX = std::max<double>(ceil(theVertexList[i].x),  maxX);
         minY = std::min<double>(floor(theVertexList[i].y), minY);
         maxY = std::max<double>(ceil(theVertexList[i].y),  maxY);
      }
   }
   else
   {
      minX = ossim::nan();
      minY = ossim::nan();
      maxX = ossim::nan();
      maxY = ossim::nan();
   }
}

bool ossimPolygon::clipToRect(vector<ossimPolygon>& result,
                              const ossimDrect& rect)const
{
   result.clear();
   ossimPolyArea2d p1(*this);
   ossimPolyArea2d p2(rect.ul(), rect.ur(), rect.lr(), rect.ll());
   
   p1&=p2;

   p1.getVisiblePolygons(result);

   return (result.size() > 0);
}   

//*****************************************************************************
//  METHOD: ossimPolygon::clipLineSegment(p1, p2)
//  
//  Implements Cyrus-Beck clipping algorithm as described in:
//  http://www.daimi.au.dk/~mbl/cgcourse/wiki/cyrus-beck_line-clipping_.html
//
//  Clips the line segment defined by the two endpoints provided. The
//  endpoints are modified as needed to represent the clipped line. Returns
//  true if intersection present.
//  
//*****************************************************************************
bool ossimPolygon::clipLineSegment(ossimDpt& P, ossimDpt& Q) const
{
   ossimDpt PQ (Q - P);
   double tE = 0.0;
   double tL = 1.0;
   ossimLine edge, edgeE, edgeL;
   bool intersected=false;
   double num, denom, t;
   ossim_uint32 npol = (ossim_uint32)theVertexList.size();

   checkOrdering();
   //***
   // clip the segment against each edge of the polygon
   //***
   ossim_uint32 i = 0;
   ossim_uint32 j = 0;
   for(i = 0, j = 1; i < npol;)
   {
      edge = ossimLine(theVertexList[i],
                       theVertexList[j]);
      
      ossimDpt normal = edge.normal();

      // Fix from CChuah@observera.com for counter clockwise polygons. (drb)
      if (theOrderingType == OSSIM_COUNTERCLOCKWISE_ORDER)
      {
         normal.x = -normal.x;
         normal.y = -normal.y;
      }
      
      denom = normal.x*PQ.x + normal.y*PQ.y;
      
      num = normal.x*(edge.theP1.x - P.x) + normal.y*(edge.theP1.y - P.y);
      
      if (denom < 0)
      {
         //***
         // Appears to be entering:
         //***
         t = num / denom;
         if (t > tE)
         {
            tE = t; //+ FLT_EPSILON;
            edgeE = edge;
         }
      }
      else if (denom > 0)
      {
         //***
         // Appears to be leaving:
         //***
         t = num / denom;
         if (t < tL)
         {
            tL = t;// - FLT_EPSILON;
            edgeL = edge;
         }
      }

      ++i;
      ++j;
      j%=npol;
   } 
   
   //***
   // Compute clipped end points:
   //***
   if(tL >= tE)
   {
       Q.x = P.x + tL*PQ.x;
       Q.y = P.y + tL*PQ.y;
       P.x += tE*PQ.x;
       P.y += tE*PQ.y;
       intersected = true;
   }
   
   return intersected;
}

/**
* METHOD: isRectWithin()
* Returns true if all the corner points of the given rect fit within.
*/
bool ossimPolygon::isRectWithin(const ossimIrect &rect) const 
{
    if(isPointWithin(rect.ul()) &&
       isPointWithin(rect.ur()) &&
       isPointWithin(rect.ll()) &&
       isPointWithin(rect.lr())) {
       	return true;
    }
    return false;
}
/**
* METHOD: isPolyWithin()
* Returns true if all the vertices of the given polygon fit within.
*/
bool ossimPolygon::isPolyWithin(const ossimPolygon &poly) const 
{
   bool ret=false;
   int numvertex=poly.getNumberOfVertices();
   if(getNumberOfVertices()>1 && numvertex) {
      ret=true;
      for(int v=0;v<numvertex;v++) {
         if(!isPointWithin(poly[v])) {
            ret=false;
            break;
         }
      }
   }
   return ret;
}

//*****************************************************************************
//  METHOD: ossimPolygon::pointWithin(const ossimDpt& point)
//  
//  Returns TRUE if point is inside polygon.
//  
//*****************************************************************************
bool ossimPolygon::isPointWithin(const ossimDpt& point) const
{

   int i, j, c = 0;
   int npol = (int)theVertexList.size();

   for (i = 0, j = npol-1; i < npol; j = i++)
   {
      if ((((theVertexList[i].y <= point.y) && (point.y < theVertexList[j].y)) ||
           ((theVertexList[j].y <= point.y) && (point.y < theVertexList[i].y))) &&
          (point.x < (theVertexList[j].x - theVertexList[i].x) * (point.y - theVertexList[i].y) /
           (theVertexList[j].y - theVertexList[i].y) + theVertexList[i].x))
      {
         c = !c;
      }
   }

   if(!c) // check if on if not within
   {
      for (i = 0, j = npol-1; i < npol; j = i++)
      {
         if(ossimLine(theVertexList[i], theVertexList[j]).isPointWithin(point))
         {
            return true;
         }
      }
   }

   return (c!=0);
}

//*****************************************************************************
//  METHOD: ossimPolygon::vertex(int)
//  
//  Returns the ossimDpt vertex given the index. Also initializes the current
//  edge (theCurrentEdge) to the edge corresponding to the index.
//  
//*****************************************************************************
bool ossimPolygon::vertex(int index, ossimDpt& tbd_vertex) const 
{
   if((index >= (int)theVertexList.size()) ||
      (index < 0))
   {
      return false;
   }

   tbd_vertex = theVertexList[index];
   theCurrentVertex = index;

   return true;
}

//*****************************************************************************
//  METHOD: ossimPolygon::nextVertex()
//  
//  Assigns the ossimDpt tbd_vertex following the current vertex. The current
//  vertex is initialized with a call to vertex(int), or after the last
//  vertex is reached (initialized to theFirstEdge. Returns false if no vertex
//  defined.
//
//*****************************************************************************
bool ossimPolygon::nextVertex(ossimDpt& tbd_vertex) const 
{
   ++theCurrentVertex;
   if(theCurrentVertex >= (ossim_int32)theVertexList.size())
   {
      return false;
   }
   tbd_vertex = theVertexList[theCurrentVertex];
   
   return true;
}

//*****************************************************************************
//  METHOD: operator=()
//  
//*****************************************************************************
const ossimPolygon& ossimPolygon::operator= (const ossimIrect& rect)
{
   theCurrentVertex = 0;
   theVertexList.resize(4);
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();

   return *this;
}

const ossimPolygon& ossimPolygon::operator= (const ossimDrect& rect)
{
   theCurrentVertex = 0;
   theVertexList.resize(4);
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();

   return *this;
}

const ossimPolygon&  ossimPolygon::operator=(const ossimPolygon& polygon)
{
   theVertexList    = polygon.theVertexList;
   theCurrentVertex = polygon.theCurrentVertex;
   theOrderingType  = polygon.theOrderingType;
   
   return *this;
}

const ossimPolygon& ossimPolygon::operator= (const vector<ossimDpt>& vertexList)
{
   theVertexList    = vertexList;
   theCurrentVertex = 0;
   theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
   
   return *this;
}

const ossimPolygon& ossimPolygon::operator=(const vector<ossimIpt>& vertexList)
{
   theVertexList.resize(vertexList.size());
   
   // Assign std::vector<ossimIpt> list to std::vector<ossimDpt> theVertexList.
   for (std::vector<ossimIpt>::size_type i = 0; i < vertexList.size(); ++i)
   {
      theVertexList[i] = vertexList[i];
   }
   
   theCurrentVertex = 0;
   theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
   
   return *this;
}

//*****************************************************************************
//  METHOD: operator==()
//  
//*****************************************************************************
bool ossimPolygon::operator==(const ossimPolygon& polygon) const
{
   if( (theVertexList.size() != polygon.theVertexList.size()))
   {
      return false;
   }
   if(!theVertexList.size() && polygon.theVertexList.size())
   {
      return true;
   }

   return (theVertexList == polygon.theVertexList);
}

const ossimPolygon& ossimPolygon::operator *=(const ossimDpt& scale)
{
   ossim_uint32 upper = (ossim_uint32)theVertexList.size();
   ossim_uint32 i = 0;
   for(i = 0; i < upper; ++i)
   {
      theVertexList[i].x*=scale.x;
      theVertexList[i].y*=scale.y;
   }
   
   return *this;
}

ossimPolygon ossimPolygon::operator *(const ossimDpt& scale)const
{
   ossimPolygon result(*this);

   ossim_uint32 upper = (ossim_uint32)theVertexList.size();
   ossim_uint32 i = 0;
   for(i = 0; i < upper; ++i)
   {
      result.theVertexList[i].x*=scale.x;
      result.theVertexList[i].y*=scale.y;
   }

   return result;
}


void ossimPolygon::reverseOrder()
{
   std::reverse(theVertexList.begin(), theVertexList.end());
   
   if(theOrderingType == OSSIM_COUNTERCLOCKWISE_ORDER)
   {
      theOrderingType = OSSIM_CLOCKWISE_ORDER;
   }
   else if(theOrderingType == OSSIM_CLOCKWISE_ORDER)
   {
      theOrderingType =  OSSIM_COUNTERCLOCKWISE_ORDER;
   }
   
}

//*****************************************************************************
//  METHOD: ossimPolygon::print(ostream)
//  
//*****************************************************************************
void ossimPolygon::print(ostream& os) const
{
   copy(theVertexList.begin(),
        theVertexList.end(),
        ostream_iterator<ossimDpt>(os, "\n"));
}


ossimVertexOrdering ossimPolygon::checkOrdering()const
{
   if(theOrderingType == OSSIM_VERTEX_ORDER_UNKNOWN)
   {
      double areaValue = area();
      if(areaValue > 0)
      {
         theOrderingType = OSSIM_COUNTERCLOCKWISE_ORDER;
      }
      else if(areaValue <= 0)
      {
         theOrderingType = OSSIM_CLOCKWISE_ORDER;
      }
   }

   return theOrderingType;
}

void ossimPolygon::intersectEdge(ossimDpt& result,
                                 const ossimLine& segment,
                                 const ossimDrect& rect,
                                 int edge)
{
   ossimLine edgeLine;
   switch(edge)
   {
   case RECT_LEFT_EDGE:
   {
      edgeLine.theP1 = rect.ll();
      edgeLine.theP2 = rect.ul();
      break;
   }
   case RECT_TOP_EDGE:
   {
      edgeLine.theP1 = rect.ul();
      edgeLine.theP2 = rect.ur();
      break;
   }
   case RECT_RIGHT_EDGE:
   {
      edgeLine.theP1 = rect.ur();
      edgeLine.theP2 = rect.lr();
      break;
   }
   case RECT_BOTTOM_EDGE:
   {
      edgeLine.theP1 = rect.lr();
      edgeLine.theP2 = rect.ll();
      break;
   }
   }
   
   result = segment.intersectInfinite(edgeLine);
}

bool ossimPolygon::isInsideEdge(const ossimDpt& pt,
                                const ossimDrect& rect,
                                int edge)const
{
   switch(edge)
   {
   case RECT_LEFT_EDGE:
   {
      return (pt.x>rect.ul().x);
      break;
   }
   case RECT_TOP_EDGE:
   {
      if(rect.orientMode() == OSSIM_LEFT_HANDED)
      {
         return (pt.y > rect.ul().y);
      }
      else
      {
         return (pt.y < rect.ul().y);
      }
      break;
   }
   case RECT_RIGHT_EDGE:
   {
      return (pt.x<rect.lr().x);
      
      break;
   }
   case RECT_BOTTOM_EDGE:
   {
      if(rect.orientMode() == OSSIM_LEFT_HANDED)
      {
         return (pt.y < rect.lr().y);
      }
      else
      {
         return (pt.y > rect.lr().y);
      }
      break;
   }
   }
   return false;
}


bool ossimPolygon::saveState(ossimKeywordlist& kwl,
                             const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPolygon",
           true);
   kwl.add(prefix,
           NUMBER_VERTICES_KW,
           static_cast<ossim_uint32>(theVertexList.size()),
           true);
   int i = 0;
   for(i = 0; i < (int)theVertexList.size();++i)
   {
      ossimString vert = "v"+ossimString::toString(i);;
      ossimString value = (ossimString::toString(theVertexList[i].x) + " " +
                           ossimString::toString(theVertexList[i].y) );
      kwl.add(prefix,
              vert.c_str(),
              value.c_str(),
              true);
   }
   ossimString order = "";
   
   switch(theOrderingType)
   {
   case OSSIM_VERTEX_ORDER_UNKNOWN:
   {
      order = "unknown";
      break;
   }
   case OSSIM_CLOCKWISE_ORDER:
   {
      order = "clockwise";
      break;
   }
   case OSSIM_COUNTERCLOCKWISE_ORDER:
   {
      order = "counter_clockwise";
      break;
   }
   }
   kwl.add(prefix,
           VERTEX_ORDER_KW,
           order,
           true);

   return true;
}
   
bool ossimPolygon::loadState(const ossimKeywordlist& kwl,
                             const char* prefix)
{
   ossimString order = kwl.find(prefix, VERTEX_ORDER_KW);
   const char* number_vertices = kwl.find(prefix, NUMBER_VERTICES_KW);
   ossimString x,y;
   if(order=="unknown")
   {
      theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
   }
   else if(order =="clockwise")
   {
      theOrderingType = OSSIM_CLOCKWISE_ORDER;
   }
   else if(order =="counter_clockwise")
   {
      theOrderingType = OSSIM_COUNTERCLOCKWISE_ORDER;
   }

   theVertexList.clear();
   int vertexCount = ossimString(number_vertices).toLong();
   int i = 0;
   for(i = 0; i < vertexCount; ++i)
   {
      ossimString v = kwl.find(prefix, (ossimString("v")+ossimString::toString(i)).c_str());
      v = v.trim();

      istringstream vStream(v.string());
      vStream >> x.string() >> y.string();
      theVertexList.push_back(ossimDpt(x.toDouble(),y.toDouble()));
   }

   return true;
}

void ossimPolygon::getMinimumBoundingRect(ossimPolygon& minRect) const
{
   static const double MIN_STEP = (0.5)*M_PI/180.0;
   double angle_step = M_PI/8.0;  // initial rotation step size for min area search = 22.5 deg
   double theta;
   double best_theta = M_PI/4.0;  // Initial guess is 45 deg orientation
   double center_theta;
   double cos_theta, sin_theta;
   ossimPolygon rotatedPolygon(*this);
   ossimDpt xlatedVertex;
   ossimDpt rotatedVertex(0.0, 0.0);
   double min_x, min_y, max_x, max_y;
   double area;
   double min_area = 1.0/DBL_EPSILON;
   rotatedPolygon.theVertexList[0] = ossimDpt(0, 0);  // first vertex always at origin
   bool first_time = true;
   ossimDrect best_rect;
   static const bool TESTING = false;

   //***
   // Loop to converge on best orientation angle for bounding polygon:
   //***
   while (angle_step > MIN_STEP)
   {
      //***
      // Try four different rotations evenly centered about the current best guess:
      //***
      center_theta = best_theta;
      for (int i=0; i<5; i++)
      {
         //***
         // Check for i=2 (center angle) since already computed quantities for this in last iteration
         // unless this is first time through:
         //***
         if ((i != 2) || (first_time)) 
         {
            theta = center_theta + (i - 2.0)*angle_step;
            cos_theta = cos(theta);
            sin_theta = sin(theta);
            min_x = rotatedPolygon.theVertexList[0].x;
            min_y = rotatedPolygon.theVertexList[0].y;
            max_x = min_x;
            max_y = min_y;

            //***
            // Translate polygon to origin and rotate all vertices by current theta:
            //***
            for (unsigned int vertex=1; vertex < theVertexList.size(); vertex++)
            {
               xlatedVertex.x = theVertexList[vertex].x - theVertexList[0].x;
               xlatedVertex.y = theVertexList[vertex].y - theVertexList[0].y;
               rotatedVertex.x = cos_theta*xlatedVertex.x + sin_theta*xlatedVertex.y;
               rotatedVertex.y = cos_theta*xlatedVertex.y - sin_theta*xlatedVertex.x;
               rotatedPolygon.theVertexList[vertex] = rotatedVertex;

               //***
               // Latch max and mins of bounding rect:
               //***
               if (min_x > rotatedVertex.x) min_x = rotatedVertex.x;
               if (min_y > rotatedVertex.y) min_y = rotatedVertex.y;
               if (max_x < rotatedVertex.x) max_x = rotatedVertex.x;
               if (max_y < rotatedVertex.y) max_y = rotatedVertex.y;
            }

            if (TESTING)
            {
               ossimDpt v1 (cos_theta*min_x - sin_theta*max_y + theVertexList[0].x,
                            cos_theta*max_y + sin_theta*min_x + theVertexList[0].y);
               ossimDpt v2 (cos_theta*max_x - sin_theta*max_y + theVertexList[0].x,
                            cos_theta*max_y + sin_theta*max_x + theVertexList[0].y);
               ossimDpt v3 (cos_theta*max_x - sin_theta*min_y + theVertexList[0].x,
                            cos_theta*min_y + sin_theta*max_x + theVertexList[0].y);
               ossimDpt v4 (cos_theta*min_x - sin_theta*min_y + theVertexList[0].x,
                            cos_theta*min_y + sin_theta*min_x + theVertexList[0].y);
               cout << v1.x << "\t" << v1.y << endl;
               cout << v2.x << "\t" << v2.y << endl;
               cout << v3.x << "\t" << v3.y << endl;
               cout << v4.x << "\t" << v4.y << endl << endl;
            }

            //***
            // Establish bounding rect and area about rotated polygon:
            //***
            area = (max_x - min_x) * (max_y - min_y);
            if (area < min_area)
            {
               best_theta = theta;
               min_area = area;
               best_rect = ossimDrect(min_x, max_y, max_x, min_y, OSSIM_RIGHT_HANDED);
            }
         } // end if (i != 2 || first_time)
      }  // end for-loop over surrounding rotations

      //***
      // Adjust step size by half to repeat process:
      //***
      angle_step /= 2.0;
      first_time = false;

   } // end while loop for convergence

   //***
   // best_theta now contains optimum rotation of bounding rect. Need to apply reverse
   // rotation and translation of best_rect:
   //***
   cos_theta = cos(best_theta);
   sin_theta = sin(best_theta);
   ossimDpt v1 (cos_theta*best_rect.ul().x - sin_theta*best_rect.ul().y + theVertexList[0].x,
                cos_theta*best_rect.ul().y + sin_theta*best_rect.ul().x + theVertexList[0].y);
   ossimDpt v2 (cos_theta*best_rect.ur().x - sin_theta*best_rect.ur().y + theVertexList[0].x,
                cos_theta*best_rect.ur().y + sin_theta*best_rect.ur().x + theVertexList[0].y);
   ossimDpt v3 (cos_theta*best_rect.lr().x - sin_theta*best_rect.lr().y + theVertexList[0].x,
                cos_theta*best_rect.lr().y + sin_theta*best_rect.lr().x + theVertexList[0].y);
   ossimDpt v4 (cos_theta*best_rect.ll().x - sin_theta*best_rect.ll().y + theVertexList[0].x,
                cos_theta*best_rect.ll().y + sin_theta*best_rect.ll().x + theVertexList[0].y);
    
   if (TESTING)
   {
      cout << v1.x << "\t" << v1.y << endl;
      cout << v2.x << "\t" << v2.y << endl;
      cout << v3.x << "\t" << v3.y << endl;
      cout << v4.x << "\t" << v4.y << endl << endl;
   }

   //***
   // Assign return value rect:
   //***
   minRect.clear();
   minRect.addPoint(v1);
   minRect.addPoint(v2);
   minRect.addPoint(v3);
   minRect.addPoint(v4);

   // Make sure we are always returning a positive clockwise area.
   minRect.checkOrdering();
   if(minRect.getOrdering()==OSSIM_COUNTERCLOCKWISE_ORDER)
      minRect.reverseOrder();
   return;
}

/**
* METHOD: remove() 
* Removes the vertex from the polygon.
*/
void ossimPolygon::removeVertex(int vertex)
{
   int numvertices=getNumberOfVertices();
   if(vertex>numvertices) {
      return;
   } else {
      vector<ossimDpt>::iterator it;
      int v=0;
      for(it=theVertexList.begin();it!=theVertexList.end();it++) {
         if(v++==vertex) {
            theVertexList.erase(it);
            break;
         }
      }
   }
}

/**
* METHOD: removeSmallestContributingVertex() 
* Removes the vertex that contributes the smallest area to the polygon.
*/
void ossimPolygon::removeSmallestContributingVertex()
{
   unsigned int numvertices=getNumberOfVertices();
   if (!numvertices)
      return;

   int smallest_vertex=-1,n1,n2;
   double smallest_area=1.0/DBL_EPSILON;
   ossimPolygon tmp;

   for(unsigned int v=0;v<numvertices;v++) {
      tmp.clear();
      if(v==0) {
         n1=numvertices-1;
         n2=1;
      } else if(v==numvertices-1) {
         n1=numvertices-2;
         n2=0;
      } else {
         n1=v-1;
         n2=v+1;
      }

      tmp.addPoint(theVertexList[n1]);
      tmp.addPoint(theVertexList[v]);
      tmp.addPoint(theVertexList[n2]);

      if(fabs(tmp.area())<smallest_area) {
         smallest_area=fabs(tmp.area());
         smallest_vertex=v;
      }
   }
   removeVertex(smallest_vertex);
}


ossimDpt& ossimPolygon::operator[](int index)
{
   return theVertexList[index];
}

const ossimDpt& ossimPolygon::operator[](int index)const
{
   return theVertexList[index];
}

ossim_uint32 ossimPolygon::getVertexCount()const
{
   return getNumberOfVertices();
}

ossim_uint32 ossimPolygon::getNumberOfVertices()const
{
   return (ossim_uint32)theVertexList.size();
}

void ossimPolygon::getBoundingRect(ossimIrect& rect)const
{
   ossim_int32 minX;
   ossim_int32 minY;
   ossim_int32 maxX;
   ossim_int32 maxY;
   getIntegerBounds(minX, minY, maxX, maxY);
   rect = ossimIrect(minX, minY, maxX, maxY);
}

void ossimPolygon::getBoundingRect(ossimDrect& rect)const
{
   ossim_float64 minX;
   ossim_float64 minY;
   ossim_float64 maxX;
   ossim_float64 maxY;
   getFloatBounds(minX, minY, maxX, maxY);
   rect = ossimDrect(minX, minY, maxX, maxY);
}

void ossimPolygon::clear()
{
   theVertexList.clear();
   theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
}

void ossimPolygon::addPoint(const ossimDpt& pt)
{
   theVertexList.push_back(pt);
   theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
}

void ossimPolygon::addPoint(double x, double y)
{
   theVertexList.push_back(ossimDpt(x, y));
   theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;
}

const vector<ossimDpt>& ossimPolygon::getVertexList()const
{
   return theVertexList;
}

 bool ossimPolygon::pointWithin(const ossimDpt& point) const
{
   return isPointWithin(point);
}

const ossimPolygon& ossimPolygon::operator *=(double scale)
{
   return ((*this)*=ossimDpt(scale, scale));
}

ossimPolygon ossimPolygon::operator *(double scale)const
{
   return ((*this)*ossimDpt(scale, scale));
}

void ossimPolygon::resize(ossim_uint32 newSize)
{
   theVertexList.resize(newSize);
   theOrderingType  = OSSIM_VERTEX_ORDER_UNKNOWN;
   theCurrentVertex = 0;
}

ossimVertexOrdering ossimPolygon::getOrdering()const
{
   return theOrderingType;
}

bool ossimPolygon::operator!=(const ossimPolygon& compare_this) const
{
   return !(*this == compare_this);
}

ostream& operator<<(ostream& os, const ossimPolygon& polygon)
{
   polygon.print(os);
   return os;
}

/**
* METHOD: getCentroid() 
* Assigns the ossimDpt centroid the polygon.
* Warning: centroid is not guaranteed to be inside the polygon!
*/
void ossimPolygon::getCentroid(ossimDpt &centroid) const
{
   int numpts = (int)theVertexList.size();
   unsigned int next;
   double area=0,parea;

   centroid=ossimDpt(0,0);
   for(int i=0;i<numpts;i++) {
      if(i<numpts-1) {
         next=i+1;
      } else {
         next=0;
      }
      parea=theVertexList[i].x*theVertexList[next].y-theVertexList[next].x*theVertexList[i].y;
      area+=parea;
      centroid.x+=(theVertexList[i].x+theVertexList[next].x)*parea;
      centroid.y+=(theVertexList[i].y+theVertexList[next].y)*parea;
   }
   area=area/2.0;
   centroid=centroid/(area*6.0);
}

/**
* METHOD: fitCircleInsideVertex() 
* Assigns destPt the point that fits a circle of given radius inside the polygon vertex.
* Warning: destPt is not guaranteed to be inside the polygon!
* (you may not be able to fit a circle of the given radius inside the polygon)
*/
void ossimPolygon::fitCircleInsideVertex(ossimDpt &destPt, unsigned int vertex, double radius) const
{
   ossim_uint32 num_vertices=(int)theVertexList.size(),n1,n2;
   ossimDpt side1,side2,bisection,currpt;
   double length_out,side1_side2_cross;
   bool concave=true;

   // don't be doing that dude.
   if(num_vertices<3 || vertex>=num_vertices) {
      destPt=ossimDpt(0,0);
      return;
   }
 
   if(vertex==0) {
      n1=num_vertices-1;
      n2=vertex+1;
   } else if(vertex==num_vertices-1) {
      n1=num_vertices-2;
      n2=0;
   } else {
      n1=vertex-1;
      n2=vertex+1;
   }

   currpt=theVertexList[vertex];
   // get the side vectors
   side1=theVertexList[n1]-currpt;
   side2=theVertexList[n2]-currpt;

   // normalize the sides
   side1 = side1/side1.length();
   side2 = side2/side2.length();

   side1_side2_cross=side1.x*side2.y-side2.x*side1.y;

   checkOrdering();
   if(getOrdering()==OSSIM_COUNTERCLOCKWISE_ORDER) {
      if(side1_side2_cross<0)
         concave=false;
   } else { //clockwise
      if(side1_side2_cross>0)
         concave=false;
   }

   bisection = side1+side2;
   bisection = bisection/bisection.length();

   if(concave) {
      bisection=bisection*-1.0;
      length_out=radius;
   } else {
      double cos_theta=(side1.x*bisection.x+side1.y*bisection.y);
      length_out=radius/sqrt(1-cos_theta*cos_theta);
   }
   destPt=ossimDpt(currpt+bisection*length_out);
   return;
}


/**
//! Shrinks this polygon by radius. Effectively, circles of given radius are placed inside 
//! the vertices just tangent to the polygon edges (via fitCircleInsideVertex()). The new
//! polygon's vertices will be the center of these circles. Return true if success. 
*/
bool ossimPolygon::shrink(ossimPolygon &dest, double inset) const 
{
   int numpts = (int) theVertexList.size();
   ossimDpt pt;
   
   //don't let people shrink themselves, that isn't going to work
   if(&dest==this) return false;

   dest.clear();
   for(int i=0;i<numpts;i++) {
      fitCircleInsideVertex(pt,i,inset);
      dest.addPoint(pt);
   }
   if(isPolyWithin(dest)) {
      return true;
   } else {
      //return an empty polygon
      dest=ossimPolygon();
      return false;
   }
}




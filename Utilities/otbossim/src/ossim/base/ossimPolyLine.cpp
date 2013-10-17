//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Garrett Potts (gpotts@imagelinks.com)
//
//*****************************************************************************
//  $Id: ossimPolyLine.cpp 22418 2013-09-26 15:01:12Z gpotts $
//
#include <ossim/base/ossimPolyLine.h>
#include <ossim/base/ossimCommon.h>
#include <algorithm>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimLine.h>
#include <ossim/base/ossimPolygon.h>
#include <sstream>
#include <iterator>
using namespace std;

static const char* NUMBER_VERTICES_KW = "number_vertices";

ossimPolyLine::ossimPolyLine(const vector<ossimIpt>& polyLine)
   :theCurrentVertex(0)
{
   for (std::vector<ossimIpt>::const_iterator iter = polyLine.begin();
      iter != polyLine.end(); ++iter)
   {
      theVertexList.push_back(ossimDpt(*iter));
   }
}

ossimPolyLine::ossimPolyLine(const vector<ossimDpt>& polyLine)
   :theCurrentVertex(0)
{
   theVertexList = polyLine;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimPolyLine(int numVertices, const ossimDpt* vertex_array)
//  
//*****************************************************************************
ossimPolyLine::ossimPolyLine(int numVertices, const ossimDpt* v)
   : theCurrentVertex(0)
{
   theVertexList.insert(theVertexList.begin(),
                        v, v+numVertices);
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimPolyLine(ossimPolyLine)
//  
//*****************************************************************************
ossimPolyLine::ossimPolyLine(const ossimPolyLine& polyLine)
   :theCurrentVertex(0)
{
   *this = polyLine;
}

ossimPolyLine::ossimPolyLine(ossimDpt v1,
                             ossimDpt v2,
                             ossimDpt v3,
                             ossimDpt v4)
   : theVertexList(4),
     theCurrentVertex(0)
{
   theVertexList[0] = v1;
   theVertexList[1] = v2;
   theVertexList[2] = v3;
   theVertexList[3] = v4;
}

ossimPolyLine::ossimPolyLine(const ossimIrect& rect)
   : theVertexList(4),
     theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}

ossimPolyLine::ossimPolyLine(const ossimDrect& rect)
   : theVertexList(4),
     theCurrentVertex(0)
{
   theVertexList[0] = rect.ul();
   theVertexList[1] = rect.ur();
   theVertexList[2] = rect.lr();
   theVertexList[3] = rect.ll();
}

ossimPolyLine::ossimPolyLine(const ossimPolygon& polygon)
   :theVertexList(polygon.getNumberOfVertices()+1),
    theCurrentVertex(0)
{
   ossim_uint32 n = polygon.getNumberOfVertices();
   
   if(n)
   {
      for(ossim_uint32 i = 0; i < n; ++i)
      {
         theVertexList[i] = polygon[i];
      }

      theVertexList[n] = polygon[n-1];
   }
   else
   {
      theVertexList.clear();
   }
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimPolyLine
//  
//*****************************************************************************
ossimPolyLine::~ossimPolyLine()
{
}

void ossimPolyLine::roundToIntegerBounds(bool compress)
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
      theVertexList    = polyLine;
      theCurrentVertex = 0;
    }
}

bool ossimPolyLine::hasNans()const
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

void ossimPolyLine::getIntegerBounds(ossim_int32& minX,
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

void ossimPolyLine::getBounds(double& minX,
                              double& minY,
                              double& maxX,
                              double& maxY)const
{
   ossim_int32 npoly = (ossim_int32)theVertexList.size();
   
   if(npoly)
   {
      int i = 0;
      minX = theVertexList[0].x;
      maxX = theVertexList[0].x;
      minY = theVertexList[0].y;
      maxY = theVertexList[0].y;
      
      for(i =1; i < npoly; ++i)
      {
         minX = std::min(theVertexList[i].x, minX);
         maxX = std::max(theVertexList[i].x, maxX);
         minY = std::min(theVertexList[i].y, minY);
         maxY = std::max(theVertexList[i].y, maxY);
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

bool ossimPolyLine::isWithin(const ossimDrect& rect)const
{
   if(theVertexList.size() == 1)
   {
      return rect.pointWithin(theVertexList[0]);
   }
   else if(theVertexList.size() > 1)
   {
      for(ossim_uint32 i = 0; i < (theVertexList.size() - 1); ++i)
      {
         ossimDpt p1 = theVertexList[i];
         ossimDpt p2 = theVertexList[i+1];
         
         if(rect.clip(p1, p2))
         {
            return true;
         }
      }
   }

   return false;
}


bool ossimPolyLine::clipToRect(vector<ossimPolyLine>& result,
                              const ossimDrect& rect)const
{
   result.clear();
   
   if(theVertexList.size() <1) return false;

   ossimPolyLine currentPoly;

   if(theVertexList.size() == 1)
   {
      rect.pointWithin(theVertexList[0]);
      currentPoly.addPoint(theVertexList[0]);
      result.push_back(currentPoly);
   }
   else
   {
      ossimDpt pt1 = theVertexList[0];
      ossimDpt pt2 = theVertexList[1];
      ossim_uint32 i = 1;
      
      while(i < theVertexList.size())
      {
         
         bool p1Inside = rect.pointWithin(pt1);
         bool p2Inside = rect.pointWithin(pt2);

         if(p1Inside&&p2Inside) // both inside so save the first
         {
            currentPoly.addPoint(pt1);
            pt1 = pt2;            
         }
         // going from inside to outside
         else if(p1Inside&&
                 !p2Inside)
         {
            currentPoly.addPoint(pt1);
            ossimDpt save = pt2;
            if(rect.clip(pt1, pt2))
            {
               currentPoly.addPoint(pt2);
               result.push_back(currentPoly);

               currentPoly.clear();
            }
            pt2  = save;
            pt1  = save;
         }// going outside to the inside
         else if(!p1Inside&&
                 p2Inside)
         {            
            if(rect.clip(pt1, pt2))
            {
               currentPoly.addPoint(pt1);
            }
            pt1 = pt2;
           
         }
         else // both outside must do a clip to see if crosses rect
         {
            ossimDpt p1 = pt1;
            ossimDpt p2 = pt2;

            if(rect.clip(p1, p2))
            {
               currentPoly.addPoint(p1);
               currentPoly.addPoint(p2);
            }
            pt1 = pt2;
         }
         ++i;
         
         if(i < theVertexList.size())
         {
            pt2 = theVertexList[i];
         }            
      }
      if(rect.pointWithin(pt2))
      {
         currentPoly.addPoint(pt2);
      }
   }

   if(currentPoly.getNumberOfVertices() > 0)
   {
      result.push_back(currentPoly);
   }
   
   return (result.size()>0);
}   

bool ossimPolyLine::isPointWithin(const ossimDpt& point) const
{
   if(theVertexList.size() == 1)
   {
      return (point == theVertexList[0]);
   }
   else
   {
      for(ossim_uint32 i = 1; i < theVertexList.size(); ++i)
      {
         if(ossimLine(theVertexList[i-1],
                      theVertexList[i]).isPointWithin(point))
         {
            return true;
         }
      }
   }
   
   return false;
}

bool ossimPolyLine::vertex(int index, ossimDpt& tbd_vertex) const 
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

bool ossimPolyLine::nextVertex(ossimDpt& tbd_vertex) const 
{
   ++theCurrentVertex;
   if(theCurrentVertex >= (ossim_int32)theVertexList.size())
   {
      return false;
   }
   tbd_vertex = theVertexList[theCurrentVertex];
   
   return true;
}


const ossimPolyLine&  ossimPolyLine::operator=(const ossimPolygon& polygon)
{
   theCurrentVertex = 0;
   ossim_uint32 n = polygon.getNumberOfVertices();
   
   if(n)
   {
      theVertexList.resize(n+1);
      
      for(ossim_uint32 i = 0; i < n; ++i)
      {
         theVertexList[i] = polygon[i];
      }
      
      theVertexList[n] = polygon[n-1];
   }
   else
   {
      theVertexList.clear();
   }

   return *this;
}

const ossimPolyLine&  ossimPolyLine::operator=(const ossimPolyLine& polyLine)
{
   theVertexList    = polyLine.theVertexList;
   theCurrentVertex = polyLine.theCurrentVertex;
   theAttributeList = polyLine.theAttributeList;
   
   return *this;
}

const ossimPolyLine& ossimPolyLine::operator= (const vector<ossimDpt>& vertexList)
{
   theVertexList    = vertexList;
   theCurrentVertex = 0;
   
   return *this;
}

const ossimPolyLine& ossimPolyLine::operator= (const vector<ossimIpt>& vertexList)
{
   theVertexList.clear();
   for (std::vector<ossimIpt>::const_iterator iter = vertexList.begin();
      iter != vertexList.end(); ++iter)
   {
      theVertexList.push_back(*iter);
   }
   
   theCurrentVertex = 0;
   
   return *this;
}

//*****************************************************************************
//  METHOD: operator==()
//  
//*****************************************************************************
bool ossimPolyLine::operator==(const ossimPolyLine& polyLine) const
{
   if( (theVertexList.size() != polyLine.theVertexList.size()))
   {
      return false;
   }
   if(!theVertexList.size() && polyLine.theVertexList.size())
   {
      return true;
   }

   return (theVertexList == polyLine.theVertexList);
}

const ossimPolyLine& ossimPolyLine::operator *=(const ossimDpt& scale)
{
   int upper = theVertexList.size();
   int i = 0;
   
   for(i = 0; i < upper; ++i)
   {
      theVertexList[i].x*=scale.x;
      theVertexList[i].y*=scale.y;
   }
   
   return *this;
}

ossimPolyLine ossimPolyLine::operator *(const ossimDpt& scale)const
{
   ossimPolyLine result(*this);

   int i = 0;
   int upper = theVertexList.size();
   for(i = 0; i < upper; ++i)
   {
      result.theVertexList[i].x*=scale.x;
      result.theVertexList[i].y*=scale.y;
   }

   return result;
}


void ossimPolyLine::reverseOrder()
{
   std::reverse(theVertexList.begin(), theVertexList.end());   
}

//*****************************************************************************
//  METHOD: ossimPolyLine::print(ostream)
//  
//*****************************************************************************
void ossimPolyLine::print(ostream& os) const
{
   copy(theVertexList.begin(),
        theVertexList.end(),
        ostream_iterator<ossimDpt>(os, "\n"));
}

bool ossimPolyLine::saveState(ossimKeywordlist& kwl,
                             const char* prefix)const
{
   int i = 0;
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPolyLine",
           true);
   kwl.add(prefix,
           NUMBER_VERTICES_KW,
           (int)theVertexList.size(),
           true);
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
   
   return true;
}
   
bool ossimPolyLine::loadState(const ossimKeywordlist& kwl,
                             const char* prefix)
{
   const char* number_vertices = kwl.find(prefix, NUMBER_VERTICES_KW);
   int i = 0;

   theVertexList.clear();
   int vertexCount = ossimString(number_vertices).toLong();
   double x = 0.0, y =0.0;
   for(i = 0; i < vertexCount; ++i)
   {
      ossimString v = kwl.find(prefix, (ossimString("v")+ossimString::toString(i)).c_str());
      v = v.trim();

      istringstream vStream(v);
      vStream >> x >> y;
      theVertexList.push_back(ossimDpt(x,y));
   }

   return true;
}

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Garrett Potts (gpotts@imagelinks.com)
//
//*****************************************************************************
//  $Id: ossimActiveEdgeTable.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <ossim/base/ossimActiveEdgeTable.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimLine.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimNotifyContext.h>
#include <iterator>
using namespace std;


ossimActiveEdgeTable::ossimActiveEdgeTable()
{
   
}

void ossimActiveEdgeTable::addPolygon(const ossimPolygon& polygon)
{
   createTable(polygon);
   if(theTable.size())
   {
      ossim_int32 iNextY, iY;
      ossimActiveEdgeTableNode edge;
      ossim_uint32 numPts = polygon.getVertexCount();
      theLastScanLine = 0;
      
      for (ossim_uint32 i=0; i<numPts; ++i)
      {
         // CURRENT EDGE IS FROM polygon[i] to polygon[(i+1)%numPts]
         int iNext = (i+1)%numPts;  // INDEX FOR SECOND EDGE VERTEX
         iNextY = ossim::round<ossim_int32>(polygon[iNext].y-theRectOrigin.y);
         iY = ossim::round<ossim_int32>(polygon[i].y-theRectOrigin.y);
         int dy = (iNextY - iY);
         if (dy != 0)   // ADD ONLY IF EDGE NOT HORIZONTAL
         {
            if (dy> 0)  // local max
            {
               double m = (polygon[iNext].x-polygon[i].x)/
                          (polygon[iNext].y-polygon[i].y);
                                 
               edge = ossimActiveEdgeTableNode(iNextY+1,
                                               m,
                                               polygon[i].x);

               addEdgeToTable(edge, iY);
                                               
            }
            else         // local min so 
            {
               double m = (polygon[i].x-polygon[iNext].x)/
                          (polygon[i].y-polygon[iNext].y);

               edge = ossimActiveEdgeTableNode(iY,
                                               m,
                                               polygon[iNext].x);
               
               addEdgeToTable(edge, iNextY);
            }
            
         }
      }
   }
}

void ossimActiveEdgeTable::createTable(const ossimPolygon& polygon)
{
   theBoundingRect.makeNan();
   theRectOrigin.makeNan();
   if(polygon.getVertexCount() >2)
   {
      theTable.clear();
      polygon.getBoundingRect(theBoundingRect);
      if(!theBoundingRect.hasNans())
      {
         theTable.resize(theBoundingRect.height()+1);
         theRectOrigin = theBoundingRect.ul();
      }
   }   
}

void ossimActiveEdgeTable::addEdgeToTable(const ossimActiveEdgeTableNode& edge,
                                          ossim_int32 scanLine)
{
   theTable[scanLine].push_back(edge);
}

void ossimActiveEdgeTable::initializeActiveList()
{
   int i = 0;
   for(i=0;i<(int)theTable.size();++i)
   {
      if(theTable[i].size())
      {
         theActiveList = theTable[i];
         theTable[i].clear();
         theCurrentScanLine = i;
         theActiveList.sort();
         return;
      }
   }
}

void ossimActiveEdgeTable::updateActiveEdges()
{
   std::list<ossimActiveEdgeTableNode>::iterator current = theActiveList.begin();

   while(current != theActiveList.end())
   {
      (*current).theCurrentX += (*current).theSlope;

      if((*current).theMaxY == theCurrentScanLine)
      {
         current = theActiveList.erase(current);
      }
      else
      {
         ++current;
      }
   }
};

void ossimActiveEdgeTable::mergeCurrentScanLine()
{
   if((theCurrentScanLine < (int)theTable.size())&&
      theTable[theCurrentScanLine].size())
   {
      theTable[theCurrentScanLine].sort();
      theActiveList.merge(theTable[theCurrentScanLine]);
      theActiveList.sort();

      theTable[theCurrentScanLine].clear();
   }
}

void ossimActiveEdgeTable::printActiveEdgeList()const
{
   copy(theActiveList.begin(),
        theActiveList.end(),
        ostream_iterator<ossimActiveEdgeTableNode>(ossimNotify(ossimNotifyLevel_INFO) << "->"));
   ossimNotify(ossimNotifyLevel_INFO) << "NULL\n";
}

ossimActiveEdgeTableHelper::ossimActiveEdgeTableHelper(ossimActiveEdgeTable* edgeTable)
   :theEdgeTable(edgeTable),
    theTableInitializedFlag(false)
{
}

bool ossimActiveEdgeTableHelper::advanceScanLine()
{
   if(!theEdgeTable) return false;
   
   if(!theTableInitializedFlag)
   {
      theEdgeTable->initializeActiveList();
      theTableInitializedFlag = true;
   }
   else
   {
      ++theEdgeTable->theCurrentScanLine;
      theEdgeTable->updateActiveEdges();
      theEdgeTable->mergeCurrentScanLine();
   }
   
   theIterator = theEdgeTable->theActiveList.begin();
   
   return (theEdgeTable->theActiveList.size());
}

bool ossimActiveEdgeTableHelper::getNextLineSegment(ossimIpt& start,
                                                    ossimIpt& end)
{
   if(!theEdgeTable) return false;
   if(!theTableInitializedFlag) advanceScanLine();
   
   if(theIterator == theEdgeTable->theActiveList.end())
   {
      return false;
   }
   else
   {
      int y = theEdgeTable->getCurrentScanLine()+getYShift();
      
      start = ossimIpt(ossim::round<int>((*theIterator).theCurrentX),
                       y);
      ++theIterator;
      if(theIterator == theEdgeTable->theActiveList.end())
      {
         end = start;
      }
      else
      {
         end = ossimIpt( ossim::round<int>((*theIterator).theCurrentX),
                         y);
         
         ++theIterator;
      }
   }

   if(start.x > end.x)
   {
      swap(start.x, end.x);
   }
   return true;
}

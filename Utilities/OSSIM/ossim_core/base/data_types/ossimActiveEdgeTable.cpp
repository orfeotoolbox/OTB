//*****************************************************************************
// FILE: ossimPolygon.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Garrett Potts (gpotts@imagelinks.com)
//
//*****************************************************************************
//  $Id: ossimActiveEdgeTable.cpp,v 1.8 2004/04/02 22:03:54 gpotts Exp $
#include <base/data_types/ossimActiveEdgeTable.h>
#include <base/data_types/ossimPolygon.h>
#include <base/data_types/ossimLine.h>
#include <base/data_types/ossimString.h>
#include <base/context/ossimNotifyContext.h>
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
         iNextY = irint(polygon[iNext].y-theRectOrigin.y);
         iY = (irint)(polygon[i].y-theRectOrigin.y);
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
      
      start = ossimIpt( irint((*theIterator).theCurrentX),
                        y);
      ++theIterator;
      if(theIterator == theEdgeTable->theActiveList.end())
      {
         end = start;
      }
      else
      {
         end = ossimIpt( irint((*theIterator).theCurrentX),
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

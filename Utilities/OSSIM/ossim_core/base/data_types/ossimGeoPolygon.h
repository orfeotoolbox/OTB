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
//  $Id: ossimGeoPolygon.h,v 1.13 2006/01/03 20:55:56 gpotts Exp $
#ifndef ossimGeoPolygon_HEADER
#define ossimGeoPolygon_HEADER
#include <vector>
#include "ossimGpt.h"
#include "ossimKeywordlist.h"

class OSSIMDLLEXPORT ossimGeoPolygon
{
public:
   friend OSSIM_DLL std::ostream& operator <<(std::ostream& out, const ossimGeoPolygon& poly);
   ossimGeoPolygon():theCurrentVertex(-1),
                     theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN)
      {}
   ossimGeoPolygon(const std::vector<ossimGpt>& points)
      :theVertexList(points),
       theOrderingType(OSSIM_VERTEX_ORDER_UNKNOWN)
      {
         theCurrentVertex = 0;
      }
   ossimGeoPolygon(const ossimGeoPolygon& rhs)
      {
         theVertexList   = rhs.theVertexList;
         theOrderingType = rhs.theOrderingType;
	 theAttributeList = rhs.theAttributeList;
	 theHoleList = rhs.theHoleList;
         theCurrentVertex = rhs.theCurrentVertex;
      }

    void addPoint(const ossimGpt& pt)
      {
         theVertexList.push_back(pt);
      }
   void addPoint(double lat, double lon, double h=OSSIM_DBL_NAN, const ossimDatum* datum=NULL)
      {
         theVertexList.push_back(ossimGpt(lat, lon, h, datum));
      }
   void addAttribute( const ossimString& attribute )
     {
       theAttributeList.push_back( attribute );
     }
   void addHole( const ossimGeoPolygon& polygon )
     {
       theHoleList.push_back( polygon );
     }
   ossimGpt& operator[](int index)
      {
         return theVertexList[index];
      }
   const ossimGpt& operator[](int index)const
      {
         return theVertexList[index];
      }
   
   const vector<ossimGpt>& getVertexList()const
      {
         return theVertexList;
      }
   vector<ossimString>& getAttributeList()
      {
         return theAttributeList;
      }
   vector<ossimGeoPolygon>& getHoleList()
      {
         return theHoleList;
      }

   void clear()
      {
         theVertexList.clear();
      }
   ossim_uint32 size()const
      {
         return (ossim_uint32)theVertexList.size();
      }
   void resize(ossim_uint32 newSize)
      {
         theVertexList.resize(newSize);
         theCurrentVertex = 0;
         theOrderingType  = OSSIM_VERTEX_ORDER_UNKNOWN;
      }
   const ossimGeoPolygon& operator = (const vector<ossimGpt>& rhs)
      {
         theVertexList = rhs;
         theCurrentVertex = 0;
         theOrderingType = OSSIM_VERTEX_ORDER_UNKNOWN;

         return *this;
      }

   const ossimGeoPolygon& operator = (const ossimGeoPolygon& rhs)
      {
         if(&rhs != this)
         {
            theVertexList = rhs.theVertexList;
            theCurrentVertex = rhs.theCurrentVertex;
	    theAttributeList = rhs.theAttributeList;
	    theHoleList = rhs.theHoleList;
         }
         theOrderingType = rhs.theOrderingType;

         return *this;
      }
   void stretchOut(ossimGeoPolygon& newPolygon,
                   double displacement);
   double area()const;

   ossimGpt computeCentroid()const;
   
   bool vertex(int index, ossimGpt& v) const;
   bool nextVertex(ossimDpt& v) const;
   bool hasNans()const;
   void reverseOrder();
   
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
    
   void checkOrdering()const;
   ossimVertexOrdering getOrdering()const
      {
         if(theOrderingType == OSSIM_VERTEX_ORDER_UNKNOWN)
         {
            checkOrdering();
         }
         return theOrderingType;
      }
   void setOrdering(ossimVertexOrdering ordering)
      {
         theOrderingType = ordering;
      }
protected:
   vector<ossimGpt> theVertexList;
   vector<ossimString> theAttributeList;
   vector<ossimGeoPolygon> theHoleList;
   mutable ossim_int32 theCurrentVertex;
   
   /*!
    * This enumeration is found in ossimConstants.h
    */
   mutable ossimVertexOrdering theOrderingType;
   
};

#endif

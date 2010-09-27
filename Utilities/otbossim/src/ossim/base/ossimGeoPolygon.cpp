//*****************************************************************************
// FILE: ossimPolygon.h
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Garrett Potts
//
//*****************************************************************************
//  $Id: ossimGeoPolygon.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <ostream>
#include <sstream>
#include <algorithm>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>

static const char* NUMBER_VERTICES_KW = "number_vertices";

std::ostream& operator <<(std::ostream& out, const ossimGeoPolygon& poly)
{
   if(poly.size())
   {
      if(poly.size() >1)
      {
         for(ossim_uint32 i = 0; i <  poly.size()-1; ++i)
         {
            out << "P" << i << ": " << poly[i] << std::endl;
         }
         out << "P"  << (poly.size()-1)
             << ": " << poly[poly.size()-1] << std::endl;
      }
      else
      {
         out << "P0: " << poly[0] << std::endl;
      }
   }

   return out;
}

bool ossimGeoPolygon::hasNans()const
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

bool ossimGeoPolygon::vertex(int index, ossimGpt& v) const 
{
   if((index >= (int)theVertexList.size()) ||
      (index < 0))
   {
      return false;
   }

   v = theVertexList[index];
   theCurrentVertex = index;

   return true;
}

bool ossimGeoPolygon::nextVertex(ossimDpt& v) const 
{
   ++theCurrentVertex;
   if(theCurrentVertex >= (ossim_int32)theVertexList.size())
   {
      return false;
   }
   v = theVertexList[theCurrentVertex];
   
   return true;
}

void ossimGeoPolygon::stretchOut(ossimGeoPolygon& newPolygon,
                                 double displacement)
{
   newPolygon.resize(size());
   if(size() >= 3)
   {
      const ossimDatum* datum = theVertexList[0].datum();
      checkOrdering();
      double signMult = 1.0;
      if(theOrderingType == OSSIM_COUNTERCLOCKWISE_ORDER)
      {
         signMult = -1.0;
      }
      
      ossimDpt prev, current, next;

      ossim_uint32 prevI;
      ossim_uint32 currentI;
      ossim_uint32 nextI;
      ossim_uint32 i = 0;
      ossim_uint32 upper = size();

      bool equalEndsFlag = false;
      if(theVertexList[0] == theVertexList[theVertexList.size()-1])
      {
         equalEndsFlag = true;
         prevI    = 0;
         currentI = 1;
         nextI    = 2;
         i = 1;
         --upper;
      }
      else
      {
         equalEndsFlag = false;
         prevI    = size()-1;
         currentI = 0;
         nextI    = 1;
      }
      for(; i < upper;++i)
      {
         prev    = theVertexList[prevI];
         current = theVertexList[currentI];
         next    = theVertexList[nextI];

         ossimDpt averageNormal;
         
         ossimDpt diffPrev = current - prev;
         ossimDpt diffNext = next - current;

         diffPrev = diffPrev*(1.0/diffPrev.length());
         diffNext = diffNext*(1.0/diffNext.length());

         ossimDpt diffPrevNormal(-diffPrev.y,
                                 diffPrev.x);
         ossimDpt diffNextNormal(-diffNext.y,
                                 diffNext.x);
         
         averageNormal     = (diffPrevNormal + diffNextNormal);
         averageNormal     = averageNormal*(signMult*(1.0/averageNormal.length()));
         ossimDpt newPoint = ossimDpt( theVertexList[i].lond(),
                                       theVertexList[i].latd()) +
                             averageNormal*displacement;
         newPolygon[i].latd(newPoint.lat);
         newPolygon[i].lond(newPoint.lon);
         newPolygon[i].height(theVertexList[i].height());
         newPolygon[i].datum(datum);
         
         ++prevI;
         ++currentI;
         ++nextI;

         prevI%=size();
         nextI%=size();
      }
      if(equalEndsFlag)
      {
         
         prev    = theVertexList[theVertexList.size()-2];
         current = theVertexList[0];
         next    = theVertexList[1];
         
         ossimDpt averageNormal;
         
         ossimDpt diffPrev = current - prev;
         ossimDpt diffNext = next - current;

         diffPrev = diffPrev*(1.0/diffPrev.length());
         diffNext = diffNext*(1.0/diffNext.length());

         ossimDpt diffPrevNormal(-diffPrev.y,
                                 diffPrev.x);
         ossimDpt diffNextNormal(-diffNext.y,
                                 diffNext.x);
         
         averageNormal     = (diffPrevNormal + diffNextNormal);
         averageNormal     = averageNormal*(signMult*(1.0/averageNormal.length()));
         ossimDpt newPoint = ossimDpt( theVertexList[i].lond(),
                                       theVertexList[i].latd()) +
                             averageNormal*displacement;
         newPolygon[0].latd(newPoint.lat);
         newPolygon[0].lond(newPoint.lon);
         newPolygon[0].height(theVertexList[i].height());
         newPolygon[0].datum(datum);
         
         newPolygon[(int)theVertexList.size()-1] = newPolygon[0];
      }
   }
}


double ossimGeoPolygon::area()const
{
   double area = 0;
   ossim_uint32 i=0;
   ossim_uint32 j=0;
   ossim_uint32 size = (ossim_uint32)theVertexList.size();
   
   for (i=0;i<size;i++)
   {
      j = (i + 1) % size;
      area += theVertexList[i].lon * theVertexList[j].lat;
      area -= theVertexList[i].lat * theVertexList[j].lon;
   }

   area /= 2;

   return area;
}

void ossimGeoPolygon::reverseOrder()
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

void ossimGeoPolygon::checkOrdering()const
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
}

ossimGpt ossimGeoPolygon::computeCentroid()const
{
   if(!size())
   {
      return ossimGpt();
   }
   ossimDpt average(0.0,0.0);
   double height=0.0;
   for(ossim_uint32 i = 0; i < size(); ++i)
   {
      average += ossimDpt(theVertexList[i]);
      height  += theVertexList[i].height();
   }

   
   average.x /= size();
   average.y /= size();
   height    /= size();

   return ossimGpt(average.y, average.x, height, theVertexList[0].datum());
}

bool ossimGeoPolygon::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   int i = 0;

   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimGeoPolygon",
           true);
   kwl.add(prefix,
           NUMBER_VERTICES_KW,
           static_cast<ossim_uint32>(theVertexList.size()),
           true);
   if(theVertexList.size())
   {
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              theVertexList[0].datum()->code(),
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::DATUM_KW,
              "WGE",
              true);
   }
   
   for(i = 0; i < (int)theVertexList.size();++i)
   {
      ossimString vert = "v"+ossimString::toString(i);
      ossimString value = (ossimString::toString(theVertexList[i].latd()) + " " +
                           ossimString::toString(theVertexList[i].lond())  + " " +
                           ( theVertexList[i].isHgtNan()?ossimString("nan"):ossimString::toString(theVertexList[i].height())));
      kwl.add(prefix,
              vert.c_str(),
              value.c_str(),
              true);
   }

   return true;
}

bool ossimGeoPolygon::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char* number_vertices = kwl.find(prefix, NUMBER_VERTICES_KW);
   ossimString datumStr = kwl.find(prefix, ossimKeywordNames::DATUM_KW);
   const ossimDatum* datum = ossimDatumFactoryRegistry::instance()->create(datumStr);
   
   theVertexList.clear();
   int i = 0;
   int vertexCount = ossimString(number_vertices).toLong();
   ossimString lat, lon, height;
   for(i = 0; i < vertexCount; ++i)
   {
      ossimString v = kwl.find(prefix, (ossimString("v")+ossimString::toString(i)).c_str());
      ossimString latString, lonString, heightString;
      v = v.trim();
      std::istringstream in(v);
      in>>lat>>lon>>height;
      theVertexList.push_back(ossimGpt(lat.toDouble(), lon.toDouble(), height.toDouble(), datum));
   }

   return true;
}

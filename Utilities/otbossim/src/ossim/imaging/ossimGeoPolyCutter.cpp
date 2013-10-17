//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimGeoPolyCutter.cpp 22303 2013-07-04 18:15:52Z dburken $

#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <algorithm>

static const char* NUMBER_POLYGONS_KW = "number_polygons";

RTTI_DEF2(ossimGeoPolyCutter, "ossimGeoPolyCutter", ossimPolyCutter, ossimViewInterface)

ossimGeoPolyCutter::ossimGeoPolyCutter()
   : ossimPolyCutter(),
     ossimViewInterface(0),
     m_viewGeometry(0),
     m_geoPolygonList(0)
{
   ossimViewInterface::theObject = this;
   m_geoPolygonList.push_back(ossimGeoPolygon());
}

ossimGeoPolyCutter::~ossimGeoPolyCutter()
{
}

bool ossimGeoPolyCutter::saveState(ossimKeywordlist& kwl,
                                   const char* prefix)const
{
    ossimString newPrefix = prefix;

    for(int i = 0; i < (int)thePolygonList.size();++i)
    {
       newPrefix = ossimString(prefix) + "geo_polygon" + ossimString::toString(i)+".";

       m_geoPolygonList[i].saveState(kwl, newPrefix.c_str());
    }
    kwl.add(prefix,
            NUMBER_POLYGONS_KW,
            static_cast<ossim_uint32>(m_geoPolygonList.size()),
            true);
   
    ossimString fillType = "null_inside";
    if(theCutType == OSSIM_POLY_NULL_OUTSIDE)
    {
       fillType = "null_outside";
    }
    kwl.add(prefix,
            "cut_type",
            fillType.c_str(),
            true);   

    if(m_viewGeometry.valid())
    {
       ossimString viewPrefix = prefix;
       viewPrefix += "view.";

       m_viewGeometry->saveState(kwl, viewPrefix.c_str());
    }
  
   return ossimImageSourceFilter::saveState(kwl, prefix);;
}

bool ossimGeoPolyCutter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   ossimString copyPrefix(prefix);
   ossimString polygons =  ossimString("^(") + copyPrefix + "geo_polygon[0-9]+.)";
   vector<ossimString> keys =
      kwl.getSubstringKeyList( polygons );
   int offset = (int)(copyPrefix+"geo_polygon").size();
   
   std::vector<int> numberList(keys.size());
   for(int idx = 0; idx < (int)numberList.size();++idx)
   {
      ossimString numberStr(keys[idx].begin() + offset,
                            keys[idx].end());
      numberList[idx] = numberStr.toInt();
   }
   std::sort(numberList.begin(), numberList.end());
   
   ossimString newPrefix;
   thePolygonList.clear();
   for(int i = 0; i < (int)numberList.size();++i)
   {
      m_geoPolygonList.push_back(ossimGeoPolygon());
      newPrefix = copyPrefix+"geo_polygon"+ossimString::toString(numberList[i])+".";
      m_geoPolygonList[i].loadState(kwl, newPrefix.c_str());
   }
   
   const char* lookup = kwl.find(prefix,
                                 "cut_type");
   if(lookup)
   {
      theCutType = OSSIM_POLY_NULL_INSIDE;
      ossimString test = lookup;
      if(test == "null_outside")
      {
         theCutType = OSSIM_POLY_NULL_OUTSIDE;
      }
   }
   else
   {
      theCutType = OSSIM_POLY_NULL_OUTSIDE;
   }

   ossimString viewPrefix = prefix;
   viewPrefix += "view.";
   m_viewGeometry = new ossimImageGeometry();
   if(m_viewGeometry->loadState(kwl,
                                viewPrefix))
   {
      transformVertices();
   }
   return ossimImageSourceFilter::loadState(kwl, prefix);
}


void ossimGeoPolyCutter::setPolygon(const vector<ossimDpt>& polygon,
                                    ossim_uint32 index)
{
   if(m_viewGeometry.valid())
   {
      ossimPolyCutter::setPolygon(polygon);
      invertPolygon(index);
   }
}

void ossimGeoPolyCutter::setPolygon(const vector<ossimIpt>& polygon,
                                    ossim_uint32 index)
{
   if(m_viewGeometry.valid())
   {
      ossimPolyCutter::setPolygon(polygon);
      invertPolygon(index);
   }
}

void ossimGeoPolyCutter::setPolygon(const vector<ossimGpt>& polygon,
                                    ossim_uint32 i)
{
   if(i < m_geoPolygonList.size())
   {
      m_geoPolygonList[i] = polygon;

      transformVertices(i);
   }
}

void ossimGeoPolyCutter::setPolygon(const ossimGeoPolygon& polygon,
                                    ossim_uint32 i)
{
   if(i < m_geoPolygonList.size())
   {
      m_geoPolygonList[i] = polygon.getVertexList();

      transformVertices(i);
   }
}


void ossimGeoPolyCutter::addPolygon(const vector<ossimGpt>& polygon)
{
   m_geoPolygonList.push_back(polygon);
   thePolygonList.push_back(ossimPolygon());

   if(m_viewGeometry.valid())
   {
      transformVertices(((int)m_geoPolygonList.size())-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const vector<ossimIpt>& polygon)
{
   if(m_viewGeometry.valid())
   {
      ossimPolyCutter::addPolygon(polygon);
      m_geoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon((int)thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const vector<ossimDpt>& polygon)
{
   if(m_viewGeometry.valid())
   {
      ossimPolyCutter::addPolygon(polygon);
      m_geoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon((int)thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const ossimPolygon& polygon)
{
   if(m_viewGeometry.valid())
   {
      ossimPolyCutter::addPolygon(polygon);
      m_geoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon((int)thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const ossimGeoPolygon& polygon)
{
   ossimPolyCutter::addPolygon(ossimPolygon());
   m_geoPolygonList.push_back(polygon);
   
   if(m_viewGeometry.valid())
   {
      transformVertices(((int)m_geoPolygonList.size())-1);
   }
}

void ossimGeoPolyCutter::setNumberOfPolygons(ossim_uint32 count)
{
   if(!count)
   {
      m_geoPolygonList.clear();
   }
   else
   {
      m_geoPolygonList.resize(count);
   }
   ossimPolyCutter::setNumberOfPolygons(count);
}

std::vector<ossimGeoPolygon>& ossimGeoPolyCutter::getGeoPolygonList()
{
   return m_geoPolygonList;
}

const std::vector<ossimGeoPolygon>& ossimGeoPolyCutter::getGeoPolygonList() const
{
   return m_geoPolygonList;
}

void ossimGeoPolyCutter::invertPolygon(int polygonNumber)
{
   if(!m_viewGeometry) return;
   
   
   ossimPolygon& poly      = thePolygonList[polygonNumber];
   ossimGeoPolygon& gpoly  = m_geoPolygonList[polygonNumber];
   
   gpoly.clear();
   gpoly.resize(poly.getVertexCount());
   int j = 0;
   for(j = 0; j < (int)poly.getVertexCount(); ++j)
   {
      m_viewGeometry->localToWorld(poly[j], gpoly[j]);
   }
}

bool ossimGeoPolyCutter::setView(ossimObject* baseObject)
{
   if ( baseObject )
   {
      ossimRefPtr<ossimImageGeometry> origGeom = m_viewGeometry;

      ossimRefPtr<ossimImageGeometry> geom = dynamic_cast<ossimImageGeometry*>(baseObject);
      if ( geom.valid() )
      {
         m_viewGeometry = geom;
      }
      else
      {
         ossimRefPtr<ossimProjection> proj = dynamic_cast<ossimProjection*>(baseObject);
         if( proj.valid() )
         {
            m_viewGeometry = new ossimImageGeometry( 0, proj.get() );
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimGeoPolyCutter::setView WARNING: Invalid class type!"
               << std::endl;
         }
      }
      
      if ( m_viewGeometry.valid() )
      {
         // If view changed transform points...
         if ( !origGeom ||
              ( m_viewGeometry->isEqualTo( *( (ossimObject*)(origGeom.get()) ) ) == false )
              )
         {
            transformVertices();
         }
      }
   }

   return m_viewGeometry.valid();
   
} // End: ossimGeoPolyCutter::setView(ossimObject* baseObject)

ossimObject* ossimGeoPolyCutter::getView()
{
   return m_viewGeometry.get();
}

const ossimObject* ossimGeoPolyCutter::getView()const
{
   return m_viewGeometry.get();
}

void ossimGeoPolyCutter::transformVertices()
{
   if( m_viewGeometry.valid() )
   {
      if( m_geoPolygonList.size() )
      {
         if(m_geoPolygonList.size() != thePolygonList.size())
         {
            thePolygonList.resize(m_geoPolygonList.size());
         }
         int i = 0;
         int j = 0;
         for(i = 0; i < (int)m_geoPolygonList.size(); ++i)
         {
            ossimGeoPolygon& gpoly = m_geoPolygonList[i];
            ossimPolygon&    poly  = thePolygonList[i];
            int nvert = (int)gpoly.size();
            if((int)poly.getVertexCount() != nvert)
            {
               poly.resize(nvert);
            }
            for(j = 0; j < nvert; ++j)
            {
//	   ossimDpt error;
               m_viewGeometry->worldToLocal(gpoly[j], poly[j]);
//	    m_viewGeometry->getRoundTripError(ossimIpt(poly[j]), error);
//	    poly[j] = poly[j] + error;
               poly[j] = ossimDpt(ossim::round<int>(poly[j].x),
                                  ossim::round<int>(poly[j].y));
            }
         }
         computeBoundingRect();
      }
   }
}

void ossimGeoPolyCutter::transformVertices(int i)
{
   if( m_viewGeometry.valid() )
   {
      ossimGeoPolygon& gpoly = m_geoPolygonList[i];
      ossimPolygon&    poly  = thePolygonList[i];
      int nvert = (int)gpoly.size();
      if((int)poly.getVertexCount() != nvert)
      {
         poly.resize(nvert);
      }
      int j = 0;
      for(j = 0; j < nvert; ++j)
      {
//     ossimDpt error;
         m_viewGeometry->worldToLocal(gpoly[j], poly[j]);
//     m_viewGeometry->getRoundTripError(poly[j], error);
//     poly[j] = poly[j] + error;
         poly[j] = ossimDpt(ossim::round<int>(poly[j].x),
                            ossim::round<int>(poly[j].y));
      }
      
      computeBoundingRect();
   }
}

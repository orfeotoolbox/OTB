//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Mingjie Su
// 
// Description: implementation for image generator
//
//*************************************************************************
// $Id: ossimTilingPoly.cpp 2725 2011-06-15 18:13:07Z david.burken $

#include <sstream>
#include <iomanip>
#include <ossim/imaging/ossimTilingPoly.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimEsriShapeFileInterface.h>

static ossimTrace traceDebug("ossimTilingPoly:debug");

RTTI_DEF1(ossimTilingPoly, "ossimTilingPoly", ossimTilingRect);

ossimTilingPoly::ossimTilingPoly()
      : ossimTilingRect(),
      m_shpFilename(""),
      m_useMbr(true),
      m_bufferDistance(0.0),
      m_exteriorCut(0),
      m_interiorCut(0),
      m_featureBoundingIntersect(true)
{
}

ossimTilingPoly::~ossimTilingPoly()
{
   m_exteriorCut = 0; // Ref ptr, not a leak.
   m_interiorCut = 0; // ditto...
   m_features.clear();
}

bool ossimTilingPoly::initialize(const ossimMapProjection& proj,
                             const ossimIrect& boundingRect)
{
   bool result = false;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimTilingPoly::initialize DEBUG: Entered..."
         << "\nBounding rect === " << boundingRect
         << std::endl;
   }

   ossimDpt convertedTilingDistance = theTilingDistance;
   result = initializeBase(proj, boundingRect, convertedTilingDistance);
   if (result)
   {
      return parseShpFile();
   }
   return result;
}

bool ossimTilingPoly::next(ossimRefPtr<ossimMapProjection>& resultProjection,
                           ossimIrect& resultingBounds,
                           ossimString& resultingName)
{
   bool result = true;
   
   if(theTileId < theTotalTiles)
   {
      ostringstream idString;
      ossim_int64 tileId = theTileId + 1;
      if (tileId < static_cast<ossim_int64>(m_features.size()))
      {
         idString  << m_features[tileId].m_fid;
         resultingName = theTileNameMask;
         resultingName = resultingName.substitute("%f%", idString.str().c_str());
      }
      result = nextFeature();
      resultingBounds = m_exteriorCut->getRectangle();

      resultProjection = theMapProjection;

      ++theTileId; 
   }
   else
   {
      result = false;
   }

   return result;
}

bool ossimTilingPoly::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   kwl.add(prefix,
           "tile_source",
           m_shpFilename.c_str(),
           true);

   kwl.add(prefix,
      "paddding_use_mbr",
      m_useMbr,
      true);

   return  ossimTilingRect::saveState(kwl, prefix);;
}

bool ossimTilingPoly::loadState(const ossimKeywordlist& kwl,
                            const char* prefix)
{
   bool result = ossimTilingRect::loadState(kwl, prefix);
   
   //Get the shp file if any
   m_shpFilename = kwl.find(prefix, "tile_source");

   // Get the mask if any.
   if (!m_shpFilename.empty())
   {
      if (theTileNameMask.contains("%r%_%c%"))
      {
         theTileNameMask = theTileNameMask.substitute("%r%_%c%", "%f%", true);
      }
      result = true;
   }
 
   const char* useMbr = kwl.find(prefix, "padding_use_mbr");
   if (useMbr)
   {
      if (!ossimString(useMbr).toBool())
      {
         m_useMbr = false;
         m_bufferDistance = thePaddingSizeInPixels.x;
      }
   }

   return result;
}

bool ossimTilingPoly::nextFeature()
{
   if (m_exteriorCut == 0)
   {
      m_exteriorCut = new ossimGeoPolyCutter;
      m_exteriorCut->setView(theMapProjection.get());
   }
   
   bool result = false;
   ossim_int64 tileId = theTileId + 1;//use (theTileId+1) because theTileId is intialized as -1
   if (tileId < static_cast<ossim_int64>(m_features.size())) 
   {
      if (m_features[tileId].m_polygon.size() > 0)
      {
         if (m_features[tileId].m_polyType == ossimGeoAnnotationPolyObject::OSSIM_POLY_INTERIOR_RING)
         {
            if (m_interiorCut == 0)
            {
               m_interiorCut = new ossimGeoPolyCutter;
               m_interiorCut->setView(theMapProjection.get());
               m_interiorCut->setCutType(ossimPolyCutter::OSSIM_POLY_NULL_INSIDE);
            }
            m_interiorCut->setPolygon(m_features[tileId].m_polygon);
         }
         else
         {
            // std::vector<ossimGpt> points = m_features[tileId].m_polygon;
            // for (ossim_uint32 i = 0; i < points.size(); i++)
            // {
            //    ossimGpt point = points[i];
            // }
            m_exteriorCut->setPolygon(m_features[tileId].m_polygon);
            setRect();
         }
         result = true;
      }
      else if (m_features[tileId].m_multiPolys.size() > 0)
      {
         for (ossim_uint32 i = 0; i < m_features[tileId].m_multiPolys.size(); i++)
         {
            ossimGeoPolygon geoPoly = m_features[tileId].m_multiPolys[i];
            std::vector<ossimGeoPolygon> holePolys = geoPoly.getHoleList();
            if (holePolys.size() > 0)
            {
               if (m_interiorCut == 0)
               {
                  m_interiorCut = new ossimGeoPolyCutter;
                  m_interiorCut->setView(theMapProjection.get());
                  m_interiorCut->setCutType(ossimPolyCutter::OSSIM_POLY_NULL_INSIDE);
               }
               m_interiorCut->clear();
               for (ossim_uint32 j = 0; j < holePolys.size(); j++)
               {
                  m_interiorCut->addPolygon(holePolys[j]);
               }
            }
            m_exteriorCut->setPolygon(m_features[tileId].m_multiPolys[i]);
            setRect();
         }
         result = true;
      }
   }
   else
   {
      result = false;
   }
   return result;
}

 void ossimTilingPoly::setRect()
 {
    ossimIrect rect = m_exteriorCut->getRectangle();
    if (rect.intersects(theImageRect))
    {
       m_featureBoundingIntersect = true;
       if (!rect.completely_within(theImageRect))
       {
          ossimIrect clipRect = rect.clipToRect(theImageRect);
          m_exteriorCut->setRectangle(clipRect);
          rect = clipRect;
       }
    }
    else
    {
       m_featureBoundingIntersect = false;
    }
    if (m_useMbr)
    {
       if (thePaddingSizeInPixels.x > 0 && thePaddingSizeInPixels.y > 0)
       {
          ossimIrect newRect = ossimIrect(rect.ul().x-(ossim_int32)thePaddingSizeInPixels.x,
             rect.ul().y-(ossim_int32)thePaddingSizeInPixels.y,
             rect.lr().x + (ossim_int32)thePaddingSizeInPixels.x,
             rect.lr().y + (ossim_int32)thePaddingSizeInPixels.y);
          ossimIrect clipRect = newRect.clipToRect(theImageRect);//in case the bounding is larger than input image after adding buffer
          m_exteriorCut->setRectangle(clipRect);
       }
    }
 }

bool ossimTilingPoly::parseShpFile()
{
   m_features.clear();

   ossimString query = "";
   bool result = false;
   if (m_shpFilename.contains("|"))
   {
      ossimString fileName = m_shpFilename;
      std::vector<ossimString> fileList = fileName.split("|");
      if (fileList.size() > 1)
      {
         m_shpFilename = fileList[0];
         query = fileList[1];
      }
   }

   ossimRefPtr<ossimImageHandler> shpHandler = ossimImageHandlerRegistry::instance()->open(m_shpFilename);
   ossimEsriShapeFileInterface* shpInterface = PTR_CAST(ossimEsriShapeFileInterface, shpHandler.get());
   if (shpInterface != 0)
   {
      if (!query.empty())
      {
         shpInterface->setQuery(query);
      }
      if (m_useMbr == false)
      {
         ossimProjection* proj = shpHandler->getImageGeometry()->getProjection();
         ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);
         if (mapProj)
         {
            if (mapProj->isGeographic())
            {
               m_bufferDistance = thePaddingSizeInPixels.x * theMapProjection->getDecimalDegreesPerPixel().x;
               shpInterface->setGeometryBuffer(m_bufferDistance, OSSIM_DEGREES);
            }
            else
            {
               m_bufferDistance = thePaddingSizeInPixels.x * theMapProjection->getMetersPerPixel().x;
               shpInterface->setGeometryBuffer(m_bufferDistance, OSSIM_METERS);
            }
         }
      }
      std::multimap<long, ossimAnnotationObject*> features = shpInterface->getFeatureTable();
      if (features.size() > 0)
      {
         std::multimap<long, ossimAnnotationObject*>::iterator it = features.begin();
         while (it != features.end())
         {
            ossimAnnotationObject* anno = it->second;
            if (anno != 0)
            {
               ossimGeoAnnotationPolyObject* annoPoly = PTR_CAST(ossimGeoAnnotationPolyObject, anno);
               ossimGeoAnnotationMultiPolyObject* annoMultiPoly = 0;
               if (annoPoly == 0)
               {
                  annoMultiPoly = PTR_CAST(ossimGeoAnnotationMultiPolyObject, anno);
               }
               if (annoPoly != 0)
               {
                  result = true;
                  std::vector<ossimGpt> polygon;

                  //get the points of a polygon
                  std::vector<ossimGpt> points = annoPoly->getPoints();
                  for (ossim_uint32 i = 0; i < points.size(); i++)
                  {
                     polygon.push_back(points[i]);
                  }

                  //get polygon type, if it is an internal polygon, initialize the internal cutter
                  ossimGeoAnnotationPolyObject::ossimPolyType polyType = annoPoly->getPolyType();
                  ossimShpFeature feature(it->first, polyType, polygon, std::vector<ossimGeoPolygon>()); 
                  m_features.push_back(feature);
               }
               else if (annoMultiPoly != 0)
               {
                  std::vector<ossimGeoPolygon> multiPolys = annoMultiPoly->getMultiPolygon();
                  ossimShpFeature feature(it->first, 0, std::vector<ossimGpt>(), multiPolys); 
               }
            }
            it++;
         }
      }
   }
   if (m_features.size() > 0)
   {
      theTotalTiles = m_features.size();
      result = true;
   }
   return result;
}

bool ossimTilingPoly::useMbr() const
{
   //if the use_mbr is set to true, no matter if the padding is set to 0 or greater than 0,
   //always clip the MBR of each feature
   return m_useMbr;
}

bool ossimTilingPoly::hasExteriorCut() const
{
   return m_exteriorCut.valid();
}

bool ossimTilingPoly::hasInteriorCut() const
{
   return m_interiorCut.valid();
}

ossimRefPtr<ossimGeoPolyCutter>& ossimTilingPoly::getExteriorCut()
{
   return m_exteriorCut;
}

ossimRefPtr<ossimGeoPolyCutter>& ossimTilingPoly::getInteriorCut()
{
   return m_interiorCut;
}

bool ossimTilingPoly::isFeatureBoundingIntersect() const
{
   return m_featureBoundingIntersect;
}

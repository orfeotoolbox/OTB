//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimEsriShapeFileFilter.cpp 11411 2007-07-27 13:53:51Z dburken $

#include <cstdio>
#include <cstdlib>
#include <sstream>
using namespace std;

#include <ossim/imaging/ossimEsriShapeFileFilter.h>
#include <ossim/vec/ossimShapeFile.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/imaging/ossimGeoAnnotationPolyLineObject.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationPolyObject.h>
#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>


RTTI_DEF2(ossimEsriShapeFileFilter,
          "ossimEsriShapeFileFilter",
          ossimAnnotationSource,
          ossimViewInterface);

static const ossimTrace traceDebug("ossimEsriShapeFileFilter:debug");

ossimEsriShapeFileFilter::ossimEsriShapeFileFilter(ossimImageSource* inputSource)
   :ossimAnnotationSource(inputSource),
    ossimViewInterface(),
    theViewProjection(NULL),
    theShapeFileProjection(NULL),
    theOwnsViewProjectionFlag(true),
    theCoordinateSystem(OSSIM_GEOGRAPHIC_SPACE),
    theUnitType(OSSIM_METERS),
    theTree((ossim_SHPTree*)NULL),
    theMaxQuadTreeLevels(10),
    thePenColor(255,255,255),
    theBrushColor(255,255,255),
    theFillFlag(false),
    theThickness(1),
    thePointWidthHeight(1, 1),
    theBorderSize(0.0),
    theBorderSizeUnits(OSSIM_DEGREES)
{
   ossimViewInterface::theObject = this;
   ossimAnnotationSource::setNumberOfBands(3);
   theBoundingRect.makeNan();
   theMinArray[0] = theMinArray[1] = theMinArray[2] = theMinArray[3] = ossim::nan();
   theMaxArray[0] = theMaxArray[1] = theMaxArray[2] = theMaxArray[3] = ossim::nan();
}

ossimEsriShapeFileFilter::~ossimEsriShapeFileFilter()
{
   removeViewProjection();
   
   if(theTree)
   {
      ossim_SHPDestroyTree(theTree);
   }

   deleteCache();
}

bool ossimEsriShapeFileFilter::setView(ossimObject* baseObject,
                                       bool ownsTheView)
{
   ossimProjection* proj = PTR_CAST(ossimProjection, baseObject);
   if(proj)
   {
      setProjection(proj, ownsTheView);
      
      return true;
   }
   else if(!baseObject)
   {
      setProjection((ossimProjection*)NULL, true);
   }

   return false;
}

ossimObject* ossimEsriShapeFileFilter::getView()
{
   return theViewProjection;
}

const ossimObject* ossimEsriShapeFileFilter::getView()const
{
   return theViewProjection;
}

bool ossimEsriShapeFileFilter::addObject(ossimAnnotationObject* anObject)
{
   ossimNotify(ossimNotifyLevel_WARN)
      << "ossimEsriShapeFileFilter::addObject\n"
      << "Can't add objects to layer, must go through Esri loadShapeFile"
      <<endl;
   
   return false;
}

void ossimEsriShapeFileFilter::computeBoundingRect()
{
//   ossimAnnotationSource::computeBoundingRect();
   
   std::multimap<int, ossimAnnotationObject*>::iterator iter = theShapeCache.begin();
   
   theBoundingRect.makeNan();
   while(iter != theShapeCache.end())
   {
       ossimDrect rect = (*iter).second->getBoundingRect();
      if(theBoundingRect.hasNans())
      {
         theBoundingRect = rect;
      }
      else
      {
         if(!rect.hasNans())
         {
            theBoundingRect = theBoundingRect.combine(rect);
         }
      }
      
      ++iter;
   }
}

ossimIrect ossimEsriShapeFileFilter::getBoundingRect(ossim_uint32 resLevel)const
{
   if(!isSourceEnabled()||
      getInput())
   {
      if(getInput())
      {
         ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface,
                                                     getInput());
         if(input)
         {
            return input->getBoundingRect(resLevel);
         }
      }
   }
   return theBoundingRect;
}

void ossimEsriShapeFileFilter::drawAnnotations(ossimRefPtr<ossimImageData> tile)
{
   ossimAnnotationSource::drawAnnotations(tile);
   
   if (!theTree||!theShapeFile.isOpen()) return;
   if(theViewProjection)
   {
      ossimIrect rect = tile->getImageRectangle();

      rect = ossimIrect(rect.ul().x,
                        rect.ul().y,
                        rect.lr().x,
                        rect.lr().y);
      double boundsMin[2];
      double boundsMax[2];
      
      ossimGpt gp1;
      ossimGpt gp2;
      ossimGpt gp3;
      ossimGpt gp4;

      theViewProjection->lineSampleToWorld(rect.ul(),
                                           gp1);
      theViewProjection->lineSampleToWorld(rect.ur(),
                                           gp2);
      theViewProjection->lineSampleToWorld(rect.lr(),
                                           gp3);
      theViewProjection->lineSampleToWorld(rect.ll(),
                                           gp4);

      ossimDrect boundsRect( ossimDpt(gp1.lond(),
                                      gp1.latd()),
                             ossimDpt(gp2.lond(),
                                      gp2.latd()),
                             ossimDpt(gp3.lond(),
                                      gp3.latd()),
                             ossimDpt(gp4.lond(),
                                      gp4.latd()),
                             OSSIM_RIGHT_HANDED);

      boundsMin[0] = boundsRect.ul().x;
      boundsMin[1] = boundsRect.lr().y;

      boundsMax[0] = boundsRect.lr().x;
      boundsMax[1] = boundsRect.ul().y;

      int n;
      int *array=(int*)NULL;
      
      array = ossim_SHPTreeFindLikelyShapes(theTree,
                                      boundsMin,
                                      boundsMax,
                                      &n);
      
      theImage->setCurrentImageData(tile);
      if(n&&array)
      {
         for(int i = 0; i < n; ++i)
         {
            std::multimap<int, ossimAnnotationObject*>::iterator iter = theShapeCache.find(array[i]);
            while( ((*iter).first == array[i]) && (iter != theShapeCache.end()) )
            {
               (*iter).second->draw(*theImage);
               ++iter;
            }
         }
         
         free(array);
      }
   }
}

void ossimEsriShapeFileFilter::transformObjects(ossimProjection* projection)
{
   std::multimap<int, ossimAnnotationObject*>::iterator iter = theShapeCache.begin();


   ossimProjection* proj = theViewProjection;
   if(projection)
   {
      proj = projection;
   }

   if(!proj) return;
   
   while(iter != theShapeCache.end())
   {
      ossimGeoAnnotationObject* obj = PTR_CAST(ossimGeoAnnotationObject,
                                               (*iter).second);
      if(obj)
      {
         obj->transform(proj);
      }
      ++iter;
   }

   computeBoundingRect();
}

void ossimEsriShapeFileFilter::setProjection(ossimProjection* projection,
                                             bool ownsProjectionFlag)
{
   removeViewProjection();

   theViewProjection         = projection;
   theOwnsViewProjectionFlag = ownsProjectionFlag;

   transformObjects();
}

bool ossimEsriShapeFileFilter::getImageGeometry(ossimKeywordlist& kwl,
                                                const char* prefix)
{
   if(theViewProjection)
   {
      return theViewProjection->saveState(kwl, prefix);
   }
   else
   {
      ossimImageSourceInterface* input = PTR_CAST(ossimImageSourceInterface, getInput(0));
      if(input)
      {
         return input->getImageGeometry(kwl, prefix);
      }
      else
      {
         kwl.add(prefix,
                 ossimKeywordNames::TYPE_KW,
                 "ossimEquDistCylProjection",
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT,
                 .1,
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON,
                 .1,
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                 0.0,
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::ORIGIN_LATITUDE_KW,
                 0.0,
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::DATUM_KW,
                 "WGE",
                 true);
         return true;
      }
   }
   
   return false;
}

void ossimEsriShapeFileFilter::removeViewProjection()
{
   if(theViewProjection&&theOwnsViewProjectionFlag)
   {
      delete theViewProjection;
   }

   theViewProjection = NULL;
}

void ossimEsriShapeFileFilter::deleteCache()
{
   std::multimap<int, ossimAnnotationObject*>::iterator iter = theShapeCache.begin();


   while(iter != theShapeCache.end())
   {
      if((*iter).second)
      {
         delete (*iter).second;
      }
      ++iter;
   }

   theShapeCache.clear();
}

void ossimEsriShapeFileFilter::checkAndSetDefaultView()
{
   if(!theViewProjection)
   {
      ossimKeywordlist tempKwl;
      getImageGeometry(tempKwl);
      
      theViewProjection = ossimProjectionFactoryRegistry::instance()->createProjection(tempKwl);
      
      if(theViewProjection)
      {
         transformObjects();
      }
   }
}

bool ossimEsriShapeFileFilter::loadShapeFile(const ossimFilename& shapeFile)
{
   if(theTree)
   {
      ossim_SHPDestroyTree(theTree);
      theTree = (ossim_SHPTree*)NULL;
   }
   theShapeFile.open(shapeFile);
   deleteCache();
   deleteAll();
   
   if(theShapeFile.isOpen())
   {
      theShapeFile.getBounds(theMinArray[0],theMinArray[1],theMinArray[2],theMinArray[3],
                             theMaxArray[0],theMaxArray[1],theMaxArray[2],theMaxArray[3]);

      theTree = ossim_SHPCreateTree(theShapeFile.getHandle(),
                              2,
                              theMaxQuadTreeLevels,
                              theMinArray,
                              theMaxArray);  
      
      ossimShapeObject obj;
      for(int index = 0 ; index < theShapeFile.getNumberOfShapes(); ++index)
      {
         obj.loadShape(theShapeFile,
                       index);
         
         if(obj.isLoaded())
         {
            switch(obj.getType())
            {
               case ossim_SHPT_POLYGON:
               case ossim_SHPT_POLYGONZ:
               {
                  loadPolygon(obj);
                  break;
               }
               case ossim_SHPT_POINT:
               case ossim_SHPT_POINTZ:
               {
                  loadPoint(obj);
                  break;
               }
               case ossim_SHPT_ARC:
               case ossim_SHPT_ARCZ:
               {
                  loadArc(obj);
                  break;
               }
               case ossim_SHPT_NULL:
               {
                  break;
               }
               default:
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "ossimEsriShapeFileFilter::loadShapeFile\n"
                     << "SHAPE " << obj.getTypeByName()
                     << " Not supported" <<  endl;
                  break;
               }
            }
         }
      }
      
      theCurrentObject = theShapeCache .begin();
      if(theViewProjection)
      {
         transformObjects();
      }
      else
      {
         checkAndSetDefaultView();
      }
   }
   
   return true;
}

void ossimEsriShapeFileFilter::loadPolygon(ossimShapeObject& obj)
{
   int starti = 0;
   int endi   = 0;
   if(obj.getNumberOfParts() > 1)
   {
      starti = obj.getShapeObject()->panPartStart[0];
      endi   = obj.getShapeObject()->panPartStart[1];
   }
   else
   {
      starti = 0;
      endi   = obj.getShapeObject()->nVertices;
   }
   
   vector<ossimGpt> groundPolygon;
   for(ossim_uint32 part = 0; part < obj.getNumberOfParts(); ++part)
   {
      if(obj.getPartType(part) != ossim_SHPP_RING)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimEsriShapeFileFilter::loadPolygon\n"
            << "Part = " << obj.getPartByName(part)
            << " not supported for shape = "
            << obj.getTypeByName() << endl;
         break;
      }
      groundPolygon.clear();
      for(ossim_int32 vertexNumber = starti; vertexNumber < endi; ++vertexNumber)
      {
         groundPolygon.push_back(ossimGpt(obj.getShapeObject()->padfY[vertexNumber],
                                          obj.getShapeObject()->padfX[vertexNumber]));
         
      }
      starti = endi;   
      if((part + 2) < obj.getNumberOfParts())
      {  
         endi = obj.getShapeObject()->panPartStart[part+2];
      }
      else
      {
         endi = obj.getShapeObject()->nVertices;
      }
      
      ossimRgbVector color;
      
      if(theFillFlag)
      {
         color = theBrushColor;
      }
      else
      {
         color = thePenColor;
      }

      if(theBorderSize != 0.0)
      {
         ossimGeoPolygon tempPoly(groundPolygon);
         ossimGeoPolygon tempPoly2;
         
         tempPoly.stretchOut(tempPoly2,
                             theBorderSize);
         groundPolygon = tempPoly2.getVertexList();

      }
      
      ossimGeoAnnotationObject *newGeoObj = new ossimGeoAnnotationPolyObject(groundPolygon,
                                                                             theFillFlag,
                                                                             color.getR(),
                                                                             color.getG(),
                                                                             color.getB(),
                                                                             theThickness);
      newGeoObj->setName(theFeatureName);
      theShapeCache.insert(make_pair(obj.getId(),
                                     newGeoObj));
   }
}

void ossimEsriShapeFileFilter::loadPoint(ossimShapeObject& obj)
{
   int n   = obj.getNumberOfVertices();

   if(n)
   {
      ossimGpt gpt(obj.getShapeObject()->padfY[0],
                   obj.getShapeObject()->padfX[0]);
      
      ossimRgbVector color;
      
      if(theFillFlag)
      {
         color = theBrushColor;
      }
      else
      {
         color = thePenColor;
      }
      ossimGeoAnnotationEllipseObject *newGeoObj =
         new ossimGeoAnnotationEllipseObject(gpt,
                                             thePointWidthHeight,
                                             theFillFlag,
                                             color.getR(),
                                             color.getG(),
                                             color.getB(),
                                             theThickness);
      newGeoObj->setEllipseWidthHeightUnitType(OSSIM_PIXEL);
      newGeoObj->setName(theFeatureName);
      theShapeCache.insert(make_pair(obj.getId(),
                                     newGeoObj));

   }
}

void ossimEsriShapeFileFilter::loadArc(ossimShapeObject& obj)
{
   int starti = 0;
   int endi   = 0;
   if(obj.getNumberOfParts() > 1)
   {
      starti = obj.getShapeObject()->panPartStart[0];
      endi   = obj.getShapeObject()->panPartStart[1];
   }
   else
   {
      starti = 0;
      endi   = obj.getShapeObject()->nVertices;
   }
   
   vector<ossimGpt> groundPolygon;
   for(ossim_uint32 part = 0; part < obj.getNumberOfParts(); ++part)
   {
      if(obj.getPartType(part) != ossim_SHPP_RING)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimEsriShapeFileFilter::loadArc\n"
            << "Part = " << obj.getPartByName(part)
            << " not supported for shape = "
            << obj.getTypeByName() << endl;
         break;
      }
      groundPolygon.clear();
      for(ossim_int32 vertexNumber = starti; vertexNumber < endi; ++vertexNumber)
      {
         groundPolygon.push_back(ossimGpt(obj.getShapeObject()->padfY[vertexNumber],
                                          obj.getShapeObject()->padfX[vertexNumber]));
         
      }
      starti = endi;   
      if((part + 2) < obj.getNumberOfParts())
      {  
         endi = obj.getShapeObject()->panPartStart[part+2];
      }
      else
      {
         endi = obj.getShapeObject()->nVertices;
      }
      
      ossimRgbVector color;
      
      if(theFillFlag)
      {
         color = theBrushColor;
      }
      else
      {
         color = thePenColor;
      }
      
      ossimGeoAnnotationObject *newGeoObj = new ossimGeoAnnotationPolyLineObject(groundPolygon,
                                                                                 color.getR(),
                                                                                 color.getG(),
                                                                                 color.getB(),
                                                                                 theThickness);
      newGeoObj->setName(theFeatureName);
      theShapeCache.insert(make_pair(obj.getId(),
                                     newGeoObj));
   }
}

bool ossimEsriShapeFileFilter::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   ossimString s;
   
   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theShapeFile.getFilename(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::MAX_QUADTREE_LEVELS_KW,
           theMaxQuadTreeLevels,
           true);

   s = ossimString::toString((int)thePenColor.getR()) + " " +
       ossimString::toString((int)thePenColor.getG()) + " " +
       ossimString::toString((int)thePenColor.getB());
   
   kwl.add(prefix,
           ossimKeywordNames::PEN_COLOR_KW,
           s.c_str(),
           true);

   s = ossimString::toString((int)theBrushColor.getR()) + " " +
       ossimString::toString((int)theBrushColor.getG()) + " " +
       ossimString::toString((int)theBrushColor.getB());
   
   kwl.add(prefix,
           ossimKeywordNames::BRUSH_COLOR_KW,
           s.c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::FILL_FLAG_KW,
           (int)theFillFlag,
           true);

   kwl.add(prefix,
           ossimKeywordNames::FEATURE_NAME_KW,
           theFeatureName.c_str(),
           true);

   kwl.add(prefix,
           ossimKeywordNames::THICKNESS_KW,
           theThickness,
           true);

   ossimString border;
   border = ossimString::toString(theBorderSize);
   border += " degrees";
   kwl.add(prefix,
           ossimKeywordNames::BORDER_SIZE_KW,
           border,
           true);
   
   s = ossimString::toString((int)thePointWidthHeight.x) + " " +
       ossimString::toString((int)thePointWidthHeight.y) + " ";
   
   kwl.add(prefix,
           ossimKeywordNames::POINT_WIDTH_HEIGHT_KW,
           s.c_str(),
           true);
   
   if(theViewProjection)
   {
      ossimString newPrefix = prefix;
      newPrefix += "view_proj.";
      theViewProjection->saveState(kwl,
                                   newPrefix.c_str());
   }
   
   return ossimAnnotationSource::saveState(kwl, prefix);
}

bool ossimEsriShapeFileFilter::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   
   const char* quadLevels  = kwl.find(prefix, ossimKeywordNames::MAX_QUADTREE_LEVELS_KW);
   const char* filename    = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   const char* penColor    = kwl.find(prefix, ossimKeywordNames::PEN_COLOR_KW);
   const char* brushColor  = kwl.find(prefix, ossimKeywordNames::BRUSH_COLOR_KW);
   const char* featureName = kwl.find(prefix, ossimKeywordNames::FEATURE_NAME_KW);
   const char* fillFlag    = kwl.find(prefix, ossimKeywordNames::FILL_FLAG_KW);
   const char* thickness   = kwl.find(prefix, ossimKeywordNames::THICKNESS_KW);
   const char* pointWh     = kwl.find(prefix, ossimKeywordNames::POINT_WIDTH_HEIGHT_KW);
   const char* border_size = kwl.find(prefix, ossimKeywordNames::BORDER_SIZE_KW);
   
   deleteCache();

   if(thickness)
   {
      theThickness = ossimString(thickness).toLong();
   }
   if(quadLevels)
   {
      theMaxQuadTreeLevels = ossimString(quadLevels).toLong();
   }
   
   if(penColor)
   {
      int r, g, b;
      istringstream s(penColor);
      s>>r>>g>>b;
      thePenColor = ossimRgbVector((ossim_uint8)r, (ossim_uint8)g, (ossim_uint8)b);
   }

   if(brushColor)
   {
      int r, g, b;
      istringstream s(brushColor);
      s>>r>>g>>b;
      theBrushColor = ossimRgbVector((ossim_uint8)r, (ossim_uint8)g, (ossim_uint8)b);
   }
   if(pointWh)
   {
      double w, h;
      istringstream s(pointWh);
      s>>w>>h;
      thePointWidthHeight = ossimDpt(w, h);
   }
   
   if(fillFlag)
   {
      theFillFlag = ossimString(fillFlag).toBool();
   }

   if(border_size)
   {
      istringstream input(border_size);

      ossimString s;
      input >> s;

      theBorderSize = s.toDouble();
      
      ossimString s2;
      
      input >> s2;

      s2 = s2.upcase();
      
      if(s2 == "US")
      {
         theBorderSizeUnits = OSSIM_US_SURVEY_FEET;
      }
      else if(s2 == "METERS")
      {
         theBorderSizeUnits = OSSIM_METERS;
      }
      else if(s2 == "FEET")
      {
         theBorderSizeUnits = OSSIM_FEET;
      }
      else
      {
         theBorderSizeUnits = OSSIM_DEGREES;
      }
      ossimUnitConversionTool unitConvert(theBorderSize,
                                          theBorderSizeUnits);
      
      theBorderSize      = unitConvert.getValue(OSSIM_DEGREES);
      theBorderSizeUnits = OSSIM_DEGREES;
   }
   else
   {
      theBorderSize      = 0.0;
      theBorderSizeUnits = OSSIM_DEGREES;
   }
   
   theFeatureName = featureName;
   
   removeViewProjection();
   
   ossimString newPrefix = prefix;
   newPrefix += "view_proj.";
   
   theViewProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl,
                                                                                    newPrefix.c_str());
   theOwnsViewProjectionFlag = true;
   
   if(filename)
   {
      loadShapeFile(ossimFilename(filename));
   }
   
   checkAndSetDefaultView();

   
   return ossimAnnotationSource::loadState(kwl, prefix);
}

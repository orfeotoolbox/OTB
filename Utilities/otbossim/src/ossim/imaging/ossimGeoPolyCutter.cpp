//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimGeoPolyCutter.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <algorithm>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

static const char* NUMBER_POLYGONS_KW = "number_polygons";

RTTI_DEF2(ossimGeoPolyCutter, "ossimGeoPolyCutter", ossimPolyCutter, ossimViewInterface)

ossimGeoPolyCutter::ossimGeoPolyCutter()
   : ossimPolyCutter(),
     ossimViewInterface(NULL)
{
   ossimViewInterface::theObject = this;
   theGeoPolygonList.push_back(ossimGeoPolygon());
   theViewProjection = NULL;
   theOwnsViewFlag = false;
}

ossimGeoPolyCutter::~ossimGeoPolyCutter()
{
   if(theOwnsViewFlag && theViewProjection)
   {
      delete theViewProjection;
      
      theViewProjection = NULL;
      theOwnsViewFlag = false;
   }
}

bool ossimGeoPolyCutter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
    ossimString newPrefix = prefix;

    for(int i = 0; i < (int)thePolygonList.size();++i)
    {
       newPrefix = ossimString(prefix) + "geo_polygon" + ossimString::toString(i)+".";

       theGeoPolygonList[i].saveState(kwl, newPrefix.c_str());
    }
    kwl.add(prefix,
            NUMBER_POLYGONS_KW,
            static_cast<ossim_uint32>(theGeoPolygonList.size()),
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

    if(theViewProjection)
    {
       ossimString viewPrefix = prefix;
       viewPrefix += "view.";

       theViewProjection->saveState(kwl, viewPrefix.c_str());
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
   int offset = (copyPrefix+"geo_polygon").size();
   
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
      theGeoPolygonList.push_back(ossimGeoPolygon());
      newPrefix = copyPrefix+"geo_polygon"+ossimString::toString(numberList[i])+".";
      theGeoPolygonList[i].loadState(kwl, newPrefix.c_str());
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
   ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl,
                                                                                        viewPrefix);
   if(proj)
   {
      if(theViewProjection&&theOwnsViewFlag)
      {
         delete theViewProjection;
      }
      theViewProjection = proj;
      theOwnsViewFlag = true;
   }
   
   transformVertices();
   return ossimImageSourceFilter::loadState(kwl, prefix);
}


void ossimGeoPolyCutter::setPolygon(const vector<ossimDpt>& polygon,
                                 ossim_uint32 index)
{
   if(theViewProjection)
   {
      ossimPolyCutter::setPolygon(polygon);
      invertPolygon(index);
   }
}

void ossimGeoPolyCutter::setPolygon(const vector<ossimIpt>& polygon,
                                 ossim_uint32 index)
{
   if(theViewProjection)
   {
      ossimPolyCutter::setPolygon(polygon);
      invertPolygon(index);
   }
}

void ossimGeoPolyCutter::setPolygon(const vector<ossimGpt>& polygon,
                                    ossim_uint32 i)
{
   if(i < theGeoPolygonList.size())
   {
      theGeoPolygonList[i] = polygon;

      transformVertices(i);
   }
}

void ossimGeoPolyCutter::setPolygon(const ossimGeoPolygon& polygon,
                                    ossim_uint32 i)
{
   if(i < theGeoPolygonList.size())
   {
      theGeoPolygonList[i] = polygon.getVertexList();

      transformVertices(i);
   }
}


void ossimGeoPolyCutter::addPolygon(const vector<ossimGpt>& polygon)
{
   theGeoPolygonList.push_back(polygon);
   thePolygonList.push_back(ossimPolygon());

   if(theViewProjection)
   {
      transformVertices(((int)theGeoPolygonList.size())-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const vector<ossimIpt>& polygon)
{
   if(theViewProjection)
   {
      ossimPolyCutter::addPolygon(polygon);
      theGeoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon(thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const vector<ossimDpt>& polygon)
{
   if(theViewProjection)
   {
      ossimPolyCutter::addPolygon(polygon);
      theGeoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon(thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const ossimPolygon& polygon)
{
   if(theViewProjection)
   {
      ossimPolyCutter::addPolygon(polygon);
      theGeoPolygonList.push_back(ossimGeoPolygon());
      invertPolygon(thePolygonList.size()-1);
   }
}

void ossimGeoPolyCutter::addPolygon(const ossimGeoPolygon& polygon)
{
   ossimPolyCutter::addPolygon(ossimPolygon());
   theGeoPolygonList.push_back(polygon);
   
   if(theViewProjection)
   {
      transformVertices(((int)theGeoPolygonList.size())-1);
   }
}

void ossimGeoPolyCutter::invertPolygon(int polygonNumber)
{
   if(!theViewProjection) return;
   
   
   ossimPolygon& poly      = thePolygonList[polygonNumber];
   ossimGeoPolygon& gpoly  = theGeoPolygonList[polygonNumber];
   
   gpoly.clear();
   gpoly.resize(poly.getVertexCount());
   int j = 0;
   for(j = 0; j < (int)poly.getVertexCount(); ++j)
   {
      theViewProjection->lineSampleToWorld(poly[j], gpoly[j]);
   }
}

bool ossimGeoPolyCutter::setView(ossimObject* baseObject,
                                 bool ownsTheView)
{
   if(PTR_CAST(ossimProjection, baseObject))
   {
      if(theViewProjection && theOwnsViewFlag)
      {
         delete theViewProjection;
      }
      theViewProjection = PTR_CAST(ossimProjection, baseObject);
      theOwnsViewFlag   = ownsTheView;

      transformVertices();

      return true;
   }

   return false;
}

ossimObject* ossimGeoPolyCutter::getView()
{
   return theViewProjection;
}

const ossimObject* ossimGeoPolyCutter::getView()const
{
   return theViewProjection;
}

void ossimGeoPolyCutter::transformVertices()
{
   if(!theViewProjection) return;
   
   if(theGeoPolygonList.size())
   {
      if(theGeoPolygonList.size() != thePolygonList.size())
      {
         thePolygonList.resize(theGeoPolygonList.size());
      }
      int i = 0;
      int j = 0;
      for(i = 0; i < (int)theGeoPolygonList.size(); ++i)
      {
         ossimGeoPolygon& gpoly = theGeoPolygonList[i];
         ossimPolygon&    poly  = thePolygonList[i];
         int nvert = (int)gpoly.size();
         if((int)poly.getVertexCount() != nvert)
         {
            poly.resize(nvert);
         }
         for(j = 0; j < nvert; ++j)
         {
//	   ossimDpt error;
            theViewProjection->worldToLineSample(gpoly[j], poly[j]);
//	    theViewProjection->getRoundTripError(ossimIpt(poly[j]), error);
//	    poly[j] = poly[j] + error;
            poly[j] = ossimDpt(ossim::round<int>(poly[j].x),
                               ossim::round<int>(poly[j].y));
         }
      }
      computeBoundingRect();
   }
}

void ossimGeoPolyCutter::transformVertices(int i)
{
   if(!theViewProjection) return;
   
   ossimGeoPolygon& gpoly = theGeoPolygonList[i];
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
     theViewProjection->worldToLineSample(gpoly[j], poly[j]);
//     theViewProjection->getRoundTripError(poly[j], error);
//     poly[j] = poly[j] + error;
     poly[j] = ossimDpt(ossim::round<int>(poly[j].x),
			ossim::round<int>(poly[j].y));
   }
   
   computeBoundingRect();
}

//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimTrimFilter.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimTrimFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimDpt.h>

static ossimTrace traceDebug("ossimTrimFilter:debug");

RTTI_DEF1(ossimTrimFilter,
          "ossimTrimFilter",
          ossimImageSourceFilter);

ossimTrimFilter::ossimTrimFilter()
   :ossimImageSourceFilter(),
    theLeftPercent(0.0),
    theRightPercent(0.0),
    theTopPercent(0.0),
    theBottomPercent(0.0)
{
   theCutter = new ossimPolyCutter;
}

ossimTrimFilter::~ossimTrimFilter()
{
   theCutter = 0;
}

ossimRefPtr<ossimImageData> ossimTrimFilter::getTile(
   const ossimIrect& rect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;
   }

   if(!theValidVertices.size())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }
   
   ossimRefPtr<ossimImageData> tile =
      theCutter->getTile(rect, resLevel);

   if(!isSourceEnabled()||!tile.valid())
   {
      return tile;
   }
   if(tile->getDataObjectStatus() == OSSIM_NULL ||
      tile->getDataObjectStatus() == OSSIM_EMPTY)
   {
      return tile;
   }

   return theCutter->getTile(rect, resLevel);
//    theTile->setImageRectangle(rect);
//    theTile->setDataObjectStatus(OSSIM_FULL);

//    theTile->makeBlank();
//    ossimIrect boundingRect = getBoundingRect(resLevel);
//    ossimIrect tileRect     = tile->getImageRectangle();
//    ossimIrect clipRect     = boundingRect.clipToRect(tileRect);

//    theTile->loadTile(tile->getBuf(),
//                      tile->getImageRectangle(),
//                      clipRect,
//                      OSSIM_BSQ);
   
//    theTile->validate();
   
}

void ossimTrimFilter::getValidImageVertices(
   vector<ossimIpt>& validVertices,
   ossimVertexOrdering ordering,
   ossim_uint32 resLevel)const
{
   ossimImageSource::getValidImageVertices(validVertices,
                                           ordering,
                                           resLevel);
   if(validVertices.size()==4)
   {
      ossimDpt averagePt((ossimDpt(validVertices[0])+
                          ossimDpt(validVertices[1])+
                          ossimDpt(validVertices[2])+
                          ossimDpt(validVertices[3]))*.25);
      ossimDpt averageTop( ( ossimDpt(validVertices[0]+
                                      validVertices[1])*.5) );
      ossimDpt averageBottom( ( ossimDpt(validVertices[2]+
                                         validVertices[3])*.5 ) );
      ossimDpt averageLeft( (ossimDpt(validVertices[0]+
                                      validVertices[3])*.5) );
      ossimDpt averageRight( (ossimDpt(validVertices[1]+
                                       validVertices[2])*.5) );

      ossimDpt topAxis    = (averageTop-averagePt);
      ossimDpt rightAxis = (averageRight-averagePt);
      ossimDpt leftAxis  = (averageLeft-averagePt);
      ossimDpt bottomAxis = (averageBottom-averagePt);

      double topLen    = topAxis.length();
      double bottomLen = bottomAxis.length();
      double rightLen  = rightAxis.length();
      double leftLen   = leftAxis.length();

      if((topLen > FLT_EPSILON)&&
         (bottomLen > FLT_EPSILON)&&
         (leftLen > FLT_EPSILON)&&
         (rightLen > FLT_EPSILON))
      {
         topAxis    = topAxis*(1.0/topLen);
         bottomAxis = bottomAxis*(1.0/bottomLen);
         leftAxis   = leftAxis*(1.0/leftLen);
         rightAxis  = rightAxis*(1.0/rightLen);

         ossim_uint32 idx = 0;
         ossim_uint32 count = 0;
         for(idx = 0; idx < validVertices.size(); ++idx)
         {
            ossimDpt axis[2];
            double   axisLen[2];
            ossimDpt diff  = validVertices[idx]-averagePt;
            double testLen = (topAxis.x*diff.x+
                              topAxis.y*diff.y);
            count = 0;
            if(testLen >= -FLT_EPSILON)
            {
               axis[count]    = topAxis;
               axisLen[count] = testLen*(1.0-theTopPercent);
               ++count;
            }

            if(count < 2)
            {
               testLen = (bottomAxis.x*diff.x+
                          bottomAxis.y*diff.y);
               if(testLen >= -FLT_EPSILON)
               {
                  axis[count]    = bottomAxis;
                  axisLen[count] = testLen*(1.0-theBottomPercent);
                  ++count;
               }
            }
            if(count < 2)
            {
               testLen = (leftAxis.x*diff.x+
                          leftAxis.y*diff.y);
               if(testLen >= -FLT_EPSILON)
               {
                  axis[count]    = leftAxis;
                  axisLen[count] = testLen*(1.0-theLeftPercent);
                  ++count;
               }
            }
            if(count < 2)
            {
               testLen = (rightAxis.x*diff.x+
                          rightAxis.y*diff.y);
               if(testLen >= -FLT_EPSILON)
               {
                  axis[count]    = rightAxis;
                  axisLen[count] = testLen*(1.0-theRightPercent);
                  ++count;
               }
            }
            if(count == 2)
            {
               validVertices[idx] = (averagePt + (axis[0]*axisLen[0] +
                                                  axis[1]*axisLen[1]));
            }
         }
      }
   }
//    ossimIrect rect = getBoundingRect();

//    if(ordering == OSSIM_CLOCKWISE_ORDER)
//    {
//       validVertices.push_back(rect.ul());
//       validVertices.push_back(rect.ur());
//       validVertices.push_back(rect.lr());
//       validVertices.push_back(rect.ll());
//    }
//    else
//    {
//       validVertices.push_back(rect.ul());
//       validVertices.push_back(rect.ll());
//       validVertices.push_back(rect.lr());
//       validVertices.push_back(rect.ur());
//    }
}

ossimIrect ossimTrimFilter::getBoundingRect(ossim_uint32 resLevel)const
{

   vector<ossimIpt> validVertices;
   ossimIrect result;
   result.makeNan();
   getValidImageVertices(validVertices, OSSIM_CLOCKWISE_ORDER, resLevel);
   
   if(validVertices.size())
   {
      result = ossimIrect(validVertices);
   }

   return result;
}

void ossimTrimFilter::initialize()
{
   if(theInputConnection)
   {
      theCutter->disconnectMyInput(0, false, false);
      if(getInput())
      {
         theCutter->connectMyInputTo(0, getInput());
      }
      theCutter->initialize();
      getValidImageVertices(theValidVertices);
      theCutter->setNumberOfPolygons(1);
      theCutter->setPolygon(theValidVertices);
   }
}

bool ossimTrimFilter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   kwl.add(prefix,
           "left_percent",
           theLeftPercent,
           true);
   kwl.add(prefix,
           "right_percent",
           theRightPercent,
           true);
   kwl.add(prefix,
           "top_percent",
           theTopPercent,
           true);
   kwl.add(prefix,
           "bottom_percent",
           theBottomPercent,
           true);

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimTrimFilter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char* leftPerc   = kwl.find(prefix, "left_percent");
   const char* rightPerc  = kwl.find(prefix, "right_percent");
   const char* topPerc    = kwl.find(prefix, "top_percent");
   const char* bottomPerc = kwl.find(prefix, "bottom_percent");

   if(leftPerc)
   {
      theLeftPercent = ossimString(leftPerc).toDouble();
   }
   if(rightPerc)
   {
      theRightPercent = ossimString(rightPerc).toDouble();
   }
   if(topPerc)
   {
      theTopPercent = ossimString(topPerc).toDouble();
   }
   if(bottomPerc)
   {
      theBottomPercent = ossimString(bottomPerc).toDouble();
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

ossimRefPtr<ossimProperty> ossimTrimFilter::getProperty(const ossimString& name)const
{
   ossimProperty* prop = 0;
   if(name == "left_percent")
   {
      prop = new ossimNumericProperty(name,
                                      ossimString::toString(theLeftPercent),
                                      0.0, 1.0);
      prop->setFullRefreshBit();
   }
   else if(name == "right_percent")
   {
      prop = new ossimNumericProperty(name,
                                      ossimString::toString(theRightPercent),
                                      0.0, 1.0);
      prop->setFullRefreshBit();
   }
   else if(name == "top_percent")
   {
      prop = new ossimNumericProperty(name,
                                      ossimString::toString(theTopPercent),
                                      0.0, 1.0);
      prop->setFullRefreshBit();
   }
   else if(name == "bottom_percent")
   {
      prop = new ossimNumericProperty(name,
                                      ossimString::toString(theBottomPercent),
                                      0.0, 1.0);
      prop->setFullRefreshBit();
   }

   if(prop) return prop;
   
   return ossimImageSourceFilter::getProperty(name);
}

void ossimTrimFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid())return;
   ossimString name = property->getName();
   
   if(name == "left_percent")
   {
      theLeftPercent = property->valueToString().toDouble();
   }
   else if(name == "right_percent")
   {
      theRightPercent = property->valueToString().toDouble();
   }
   else if(name == "top_percent")
   {
      theTopPercent = property->valueToString().toDouble();
   }
   else if(name == "bottom_percent")
   {
      theBottomPercent = property->valueToString().toDouble();
   }
   else
   {
      return ossimImageSourceFilter::setProperty(property);
   }
}

void ossimTrimFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back("left_percent");
   propertyNames.push_back("right_percent");
   propertyNames.push_back("top_percent");
   propertyNames.push_back("bottom_percent");
}

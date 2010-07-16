//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Abstract class for rendering vector data.  Derived classes should implement
// the "rasterizeVectorData" data method.
//
//*******************************************************************
//  $Id: ossimVectorRenderer.cpp 17195 2010-04-23 17:32:18Z dburken $

#include <iostream>
using namespace std;

#include <ossim/imaging/ossimVectorRenderer.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimImageViewTransformFactory.h>

RTTI_DEF1(ossimVectorRenderer, "ossimVectorRenderer", ossimImageSourceFilter)


ossimVectorRenderer::ossimVectorRenderer()
   :
      ossimImageSourceFilter(),
      theImageViewTransform(NULL),
      theBoundingRect(),
      theAreaOfInterest(),
      theNumberOfAoiObjects(0),
      theNumberOfObjects(0)
{
}

ossimVectorRenderer::ossimVectorRenderer(ossimImageSource* inputSource,
                                         ossimImageViewTransform* transform)
   : ossimImageSourceFilter(inputSource),
     theImageViewTransform(transform),
     theBoundingRect(),
     theAreaOfInterest(),
     theNumberOfAoiObjects(0),
     theNumberOfObjects(0)
{
   
}


ossimVectorRenderer::~ossimVectorRenderer()
{
   if (theImageViewTransform)
   {
      delete theImageViewTransform;
      theImageViewTransform = NULL;
   }
}

bool ossimVectorRenderer::saveState(ossimKeywordlist& /* kwl */,
                                    const char* /* prefix */ ) const
{
   return false;
}

bool ossimVectorRenderer::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   ossimString newPrefix = prefix + ossimString("image_view_trans.");

   if(theImageViewTransform)
   {
      delete theImageViewTransform;
      theImageViewTransform = NULL;
   }

   theImageViewTransform
      = ossimImageViewTransformFactory::instance()->
      createTransform(kwl, newPrefix.c_str());
   
   return ossimImageSource::loadState(kwl, prefix);
}

void
ossimVectorRenderer::setImageViewTransform(ossimImageViewTransform* transform)
{
   if(theImageViewTransform)
   {
      delete theImageViewTransform;
   }
   theImageViewTransform = transform;
}

void ossimVectorRenderer::setAreaOfInterest(const ossimDrect& aoi)
{
   theAreaOfInterest = aoi;
}

ossimDrect ossimVectorRenderer::getAreaOfInterest() const
{
   return theAreaOfInterest;
}

ossimDrect ossimVectorRenderer::getBoundingRect(long /* resLevel */)const
{
   return theBoundingRect;
}

ossim_int32 ossimVectorRenderer::getNumberOfAoiObjects() const
{
   return theNumberOfAoiObjects;
}

ossim_int32 ossimVectorRenderer::getNumberOfObjects() const
{
   return theNumberOfObjects;
}

void ossimVectorRenderer::setBoundingRect(ossimDrect& rect)
{
   theBoundingRect = rect;
}

void ossimVectorRenderer::setNumberOfAoiObjects(ossim_int32 number_of_objects)
{
   theNumberOfAoiObjects = number_of_objects;
}

void ossimVectorRenderer::setNumberOfObjects(ossim_int32 number_of_objects)
{
   theNumberOfObjects = number_of_objects;
}

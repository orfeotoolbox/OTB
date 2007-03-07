//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Abstract class for rendering vector data.  Derived classes should implement
// the "rasterizeVectorData" data method.
//
//*******************************************************************
//  $Id: ossimVectorRenderer.cpp,v 1.7 2003/05/13 11:31:09 dburken Exp $

#include <iostream>
using namespace std;

#include "ossimVectorRenderer.h"
#include "base/common/ossimCommon.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/common/ossimKeywordNames.h"
#include "imaging/filters/ossimFilter.h"
#include "projections/ossimImageViewTransform.h"
#include "projections/factory/ossimImageViewTransformFactory.h"

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

bool ossimVectorRenderer::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
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

ossimDrect ossimVectorRenderer::getBoundingRect(long resLevel)const
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

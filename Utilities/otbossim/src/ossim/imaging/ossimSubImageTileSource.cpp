//*****************************************************************************
// FILE: ossimSubImageTileSource.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimSubImageTileSource.
//   This tile source permits specifying an offset that is to be applied to the
//   tile origin for all getTile() requests. It is intended for converting
//   a full-image space coordinate to a sub-image coordinate.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimSubImageTileSource.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/imaging/ossimSubImageTileSource.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimImageData.h>

RTTI_DEF1(ossimSubImageTileSource,
          "ossimSubImageTileSource",
          ossimImageSourceFilter);

static const char* SUBIMAGE_OFFSET_X = "offset_x";
static const char* SUBIMAGE_OFFSET_Y = "offset_y";

ossimSubImageTileSource::ossimSubImageTileSource()
   : ossimImageSourceFilter(),
     theSubImageOffset(0, 0),
     theTile(0)
{
}

ossimSubImageTileSource::ossimSubImageTileSource(ossimImageSource* inputSource)
   : ossimImageSourceFilter(inputSource), theSubImageOffset(0, 0),
     theTile(0)
{
}

ossimSubImageTileSource::ossimSubImageTileSource(ossimImageSource* inputSource,
                                                 const ossimIpt&   offset)
   : ossimImageSourceFilter(inputSource), theSubImageOffset(offset),
     theTile(0)
{
}

ossimSubImageTileSource::~ossimSubImageTileSource()
{
}

//*****************************************************************************
//  METHOD: ossimSubImageTileSource::getTile()
//  
//*****************************************************************************
ossimRefPtr<ossimImageData> ossimSubImageTileSource::getTile(
   const ossimIrect& rect,
   ossim_uint32  res_level)
{
   ossimIpt offset = theSubImageOffset;

   if(res_level)
   {
      //***
      // Determine the offset for the particular rr level requested:
      //***
      ossimDpt decimation_factor;
      theInputConnection->getDecimationFactor(res_level, decimation_factor);
      
      if(!decimation_factor.hasNans())
      {
         offset = ossimIpt((int)(theSubImageOffset.x*decimation_factor.x + 0.5),
                           (int)(theSubImageOffset.y*decimation_factor.y + 0.5));
      }
      else
      {
         offset = theSubImageOffset;
      }
   }
   //
   // Apply the offset to the requested rect and fetch tile. The tile's origin
   // needs to be adjusted to reflect the requested origin:
   //
   ossimIrect rect_prime (rect - offset);

   ossimRefPtr<ossimImageData> tile =
      ossimImageSourceFilter::getTile(rect_prime, res_level);
   
   if(theTile.valid())
   {
      theTile->setImageRectangle(rect_prime);

      if(tile.valid())
      {
         theTile->loadTile(tile.get());
      }
      else
      {
         theTile->makeBlank();
      }
   }
   else
   {
      if(tile.valid())
      {
         theTile = (ossimImageData*)tile->dup();
      }
   }
   if(theTile.valid())
   {
      theTile->setOrigin(rect.ul());
      theTile->validate();
   }
   
   return theTile;
}

//*****************************************************************************
//  METHOD: ossimSubImageTileSource::getBoundingRect()
//  
//*****************************************************************************
ossimIrect ossimSubImageTileSource::getBoundingRect(ossim_uint32 resLevel) const
{
   ossimDrect result;

   result.makeNan();
   if(!theInputConnection)
      return result;
   ossimDpt offset = theSubImageOffset;
   
   ossimDrect rect (theInputConnection->getBoundingRect(resLevel));
   if(resLevel)
   {
      ossimDpt decimation_factor;
      theInputConnection->getDecimationFactor(resLevel, decimation_factor);

      if(!decimation_factor.hasNans())
      {
         rect*=decimation_factor;
         offset= ossimDpt(theSubImageOffset.x*decimation_factor.x,
                          theSubImageOffset.y*decimation_factor.y);
      }
   }
   ossimDrect rect_prime (rect + offset);
   
   return rect_prime;
}

//*****************************************************************************
//  METHOD: ossimSubImageTileSource::getValidImageVertices()
//  
//*****************************************************************************
void ossimSubImageTileSource::getValidImageVertices(vector<ossimIpt>& vertices,
                                                    ossimVertexOrdering ordering,
                                                    ossim_uint32 /* resLevel */) const
{
   if(!theInputConnection)
   {
      vertices.clear();
      return;
   }

   ossimDpt offset (theSubImageOffset);

   theInputConnection->getValidImageVertices(vertices, ordering);
   vector<ossimIpt>::iterator vertex = vertices.begin();

   while (vertex != vertices.end())
   {
      (*vertex) += offset;
      vertex++;
   }
   
   return;
}

void ossimSubImageTileSource::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile = NULL;
}

bool ossimSubImageTileSource::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   const char* offset_x = kwl.find(prefix, SUBIMAGE_OFFSET_X);
   const char* offset_y = kwl.find(prefix, SUBIMAGE_OFFSET_Y);

   if(offset_x)
   {
      theSubImageOffset.x = ossimString(offset_x).toLong();
      
   }
   if(offset_y)
   {
      theSubImageOffset.y = ossimString(offset_y).toLong();      
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimSubImageTileSource::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   bool result = ossimImageSourceFilter::saveState(kwl, prefix);
   
   kwl.add(prefix,
           SUBIMAGE_OFFSET_X,
           theSubImageOffset.x,
           true);

   kwl.add(prefix,
           SUBIMAGE_OFFSET_Y,
           theSubImageOffset.y,
           true);
   
   return result;        
}

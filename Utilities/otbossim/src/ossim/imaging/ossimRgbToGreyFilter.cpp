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
// $Id: ossimRgbToGreyFilter.cpp 19223 2011-03-24 13:38:21Z dburken $

#include <ossim/imaging/ossimRgbToGreyFilter.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>

RTTI_DEF1(ossimRgbToGreyFilter, "ossimRgbToGreyFilter", ossimImageSourceFilter)

ossimRgbToGreyFilter::ossimRgbToGreyFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theBlankTile(NULL),
    theTile(NULL),
    theC1(1.0/3.0),
    theC2(1.0/3.0),
    theC3(1.0/3.0)
{
}

ossimRgbToGreyFilter::ossimRgbToGreyFilter(ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(NULL, inputSource),
     theBlankTile(NULL),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}

ossimRgbToGreyFilter::ossimRgbToGreyFilter(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(owner, inputSource),
     theBlankTile(NULL),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}
ossimRgbToGreyFilter::~ossimRgbToGreyFilter()
{
}

ossimRefPtr<ossimImageData> ossimRgbToGreyFilter::getTile(const ossimIrect& tileRect,
                                              ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;  // This filter requires an input.
   }

   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(tileRect,
                                                           resLevel);   
   if(!isSourceEnabled() || !inputTile.valid())
   {
      return inputTile;
   }

   if(!theTile)
   {
      allocate(); // First time through...
   }

   // if (!theTile) // throw exeption...

   if( inputTile->getDataObjectStatus() == OSSIM_NULL ||
       inputTile->getDataObjectStatus() == OSSIM_EMPTY )
   {
      theBlankTile->setImageRectangle(tileRect);
      return theBlankTile;
   }

   // Set the origin, resize if needed of the output tile.
   theTile->setImageRectangle(tileRect);

   // Filter the tile.
   runUcharTransformation(inputTile);

   // Always validate to set the status.
   theTile->validate();
   
   return theTile;
}


void ossimRgbToGreyFilter::initialize()
{
   // Base class will recapture "theInputConnection".
   ossimImageSourceFilter::initialize();
}

void ossimRgbToGreyFilter::allocate()
{
   if(theInputConnection)
   {
      theTile      = NULL;
      theBlankTile = NULL;

      if(isSourceEnabled())
      {
         theBlankTile = new ossimU8ImageData(this,
                                             1,
                                             theInputConnection->getTileWidth(),
                                             theInputConnection->getTileHeight());  
         
         
         theTile = (ossimImageData*)theBlankTile->dup();
         theTile->initialize();
      }
   }
}

ossim_uint32 ossimRgbToGreyFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      return 1;
   }

   return ossimImageSourceFilter::getNumberOfOutputBands();
}

bool ossimRgbToGreyFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "c1",
           theC1,
           true);
   kwl.add(prefix,
           "c2",
           theC2,
           true);
   kwl.add(prefix,
           "c3",
           theC2,
           true);
   
   return true;
}

bool ossimRgbToGreyFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "c1");
   if(lookup)
   {
      theC1 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c2");
   if(lookup)
   {
      theC2 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c3");
   if(lookup)
   {
      theC3 = ossimString(lookup).toDouble();
   }
   return true;
}

void ossimRgbToGreyFilter::runUcharTransformation(ossimRefPtr<ossimImageData>& tile)
{   
   unsigned char** bandSrc = new unsigned char*[tile->getNumberOfBands()];
   unsigned char* bandDest;
   
   if(tile->getNumberOfBands() == 1)
   {
      bandSrc[0]  = static_cast<unsigned char*>(tile->getBuf(0));
      bandSrc[1]  = static_cast<unsigned char*>(tile->getBuf(0));
      bandSrc[2]  = static_cast<unsigned char*>(tile->getBuf(0));
   }
   else if(tile->getNumberOfBands() == 2)
   {
      bandSrc[0]  = static_cast<unsigned char*>(tile->getBuf(0));
      bandSrc[1]  = static_cast<unsigned char*>(tile->getBuf(1));
      bandSrc[2]  = static_cast<unsigned char*>(tile->getBuf(1));      
   }
   else if(tile->getNumberOfBands() >= 3)
   {
      bandSrc[0]  = static_cast<unsigned char*>(tile->getBuf(0));
      bandSrc[1]  = static_cast<unsigned char*>(tile->getBuf(1));
      bandSrc[2]  = static_cast<unsigned char*>(tile->getBuf(2));      
   }
   bandDest = static_cast<unsigned char*>(theTile->getBuf());
   
   ossim_int32 offset;

   ossim_int32 upperBound = tile->getWidth()*tile->getHeight();
   for(offset = 0; offset < upperBound; ++offset)
   {
      ossim_int32 value;
      
      value = ossim::round<int>(theC1*(bandSrc[0][offset]) +
                    theC2*(bandSrc[1][offset]) +
                    theC3*(bandSrc[2][offset]));
      
      value = value<255?value:255;
      value = value>0?value:0;

      bandDest[offset] = value;
   }

   delete [] bandSrc;
}

ossimString ossimRgbToGreyFilter::getShortName()const
{
   return ossimString("grey");
}

ossimString ossimRgbToGreyFilter::getLongName()const
{
   return ossimString("rgb to grey scale filter");
}

ossimScalarType ossimRgbToGreyFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return OSSIM_UCHAR;
   }
   
   return ossimImageSourceFilter::getOutputScalarType();
}

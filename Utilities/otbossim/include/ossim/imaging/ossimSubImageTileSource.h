//*****************************************************************************
// FILE: ossimSubImageTileSource.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimSubImageTileSource.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimSubImageTileSource.h 22233 2013-04-15 15:12:54Z gpotts $

#ifndef ossimSubImageTileSource_HEADER
#define ossimSubImageTileSource_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

/*!****************************************************************************
 *
 *  CLASS: ossimSubImageTileSource
 *
 *  This tile source permits specifying an offset that is to be applied to the
 *  tile origin for all getTile() requests. It is intended for converting
 *  a full-image space coordinate to a sub-image coordinate.
 *
 *****************************************************************************/
class OSSIM_DLL ossimSubImageTileSource : public ossimImageSourceFilter
{
public:
   /*!
    * Default constructor initializes the sub-image offset to zero.
    */
   ossimSubImageTileSource();

   /*!
    * Constructor accepts an input source pointer. Offset is defaulted to (0,0).
    */
   ossimSubImageTileSource(ossimImageSource* inputSource);

   /*!
    * Constructs with specified input source and offset.
    */
   ossimSubImageTileSource(ossimImageSource* inputSource,
                           const ossimIpt&   offset);

   /*!
    * Permits setting the image's offset relative to the full image space.
    */
   void setSubImageOffset(const ossimIpt& offset) { theSubImageOffset=offset; }

   ossimIpt getSubImageOffset()const{ return theSubImageOffset;}
 
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32  rr_level=0);

   /*!
    * Fetches the sub-image's bounding rect in FULL-IMAGE SPACE.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;
   
   /*!
    * Fetches the sub-image's valid image vertices in FULL-IMAGE SPACE.
    */
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   virtual void initialize();
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

private:
   virtual ~ossimSubImageTileSource();
   
   ossimIpt                    theSubImageOffset;
   ossimRefPtr<ossimImageData> theTile;
   TYPE_DATA
};

#endif

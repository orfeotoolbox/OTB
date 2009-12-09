//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimTilePatch.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimTilePatch_HEADER
#define ossimTilePatch_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/imaging/ossimDiscreteNearestNeighbor.h>
#include <ossim/imaging/ossimImageData.h>

class ossimImageSource;

class ossimTilePatch : public ossimObject
{
public:
   ossimTilePatch(ossimImageSource* input);
   /*!
    * The rect is assumed up right and all we need
    * to do is compute the even tile boundary rect
    */
   ossimTilePatch(ossimScalarType scalarType = OSSIM_UCHAR,
                  long numberOfComponents = 1,
                  long width=128,
                  long height=128);
   
   /*!
    * return the tile boundary aligned rectangle for this
    * patch.
    */
   virtual ossimIrect getRect()const;

   /*!
    * The padding is for any convolution that must take
    * place.  
    */
   virtual void setRect(const ossimDpt &p1,
                        const ossimDpt &p2,
                        const ossimDpt &p3,
                        const ossimDpt &p4,
                        const ossimDpt &tile_size,
                        const ossimDpt &padding=ossimDpt(0,0));

   virtual void setRect(const ossimDrect &aRect,
                        const ossimDpt &tile_size,
                        const ossimDpt &padding=ossimDpt(0,0));

   virtual void setRect(const ossimDpt &center_of_rect,
                        const ossimDpt &rect_size,
                        const ossimDpt &tile_size,
                        const ossimDpt &padding=ossimDpt(0,0));

   /*!
    * This will fill the tile from the tile patch.
    */
   virtual void fillTile(ossimRefPtr<ossimImageData>& aTile)const;

   /*!
    * This will fill the tile from the tile patch and apply the convolution
    * kernel.  This is optimized for upright rectangles.  Use the bilinear
    * fill tile bellow for arbitrary 4-sided polygons.
    */
   virtual void fillTile(ossimRefPtr<ossimImageData>& result,
                         ossimDiscreteConvolutionKernel* kernel)const;

   /*!
    * We will assume that the patch is already
    * big enough to satisfy the kernel size.
    */
   virtual void fillTile(ossimRefPtr<ossimImageData>& result,
                         const ossimDpt   &ul,
                         const ossimDpt   &ur,
                         const ossimDpt   &deltaUl,
                         const ossimDpt   &deltaUr,
                         const ossimDpt   &length)const;


   virtual void fillPatch(ossimImageSource *imageSource,
                          long resLevel = 0);
   
   
   
//    static ossimIrect alignRectToBoundary(const ossimDrect &aRect,
//                                          const ossimDpt &tile_size);
   static ossimDrect findBoundingRect(const ossimDpt &p1,
                                      const ossimDpt &p2,
                                      const ossimDpt &p3,
                                      const ossimDpt &p4);
   /*!
    * Do not delete this data.  We give raw access to the buffer
    * if there are special things you wish to have done.
    */
   ossimRefPtr<ossimImageData> getData(){return thePatchData;}

   void setData(ossimRefPtr<ossimImageData>& patchData);
   /*!
    *  This will implement an in place convolution.  It will
    *  allocate a buffer pass it to the convolution algorithm
    * and then replace the internal buffer.
    */
   void convolve(ossimDiscreteConvolutionKernel* kernel);
   
protected:
   virtual ~ossimTilePatch();
   
   ossimRefPtr<ossimImageData>         thePatchData;
   static ossimDiscreteNearestNeighbor nearestNeighborKernel;
   


   template<class T>
   void fillTileTemplate(T dummyVariable,
                         ossimRefPtr<ossimImageData>& result,
                         const ossimDpt   &ul,
                         const ossimDpt   &ur,
                         const ossimDpt   &deltaUl,
                         const ossimDpt   &deltaUr,
                         const ossimDpt   &length)const;
   
   template<class T>
   void fillTileTemplate(T dummyVariable,
                         ossimRefPtr<ossimImageData>& result,
                         ossimDiscreteConvolutionKernel* kernel)const;
};

#endif

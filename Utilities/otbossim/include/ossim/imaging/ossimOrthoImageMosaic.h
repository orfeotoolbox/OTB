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
//  $Id: ossimOrthoImageMosaic.h 10777 2007-04-25 14:49:17Z gpotts $
#ifndef ossimOrthoImageMosaic_HEADER
#define ossimOrthoImageMosaic_HEADER
#include <ossim/imaging/ossimImageMosaic.h>

class OSSIMDLLEXPORT ossimOrthoImageMosaic : public ossimImageMosaic
{
public:
   ossimOrthoImageMosaic();
   ossimOrthoImageMosaic(const std::vector<ossimImageSource*>& inputSources);
   virtual ~ossimOrthoImageMosaic();   
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;
   virtual void initialize();
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
  /*!
   * Used to retrieve the number of overlapping images withint the given
   * rect.
   */
   virtual ossim_uint32 getNumberOfOverlappingImages(const ossimIrect& rect, ossim_uint32 resLevel=0)const;

  /*!
   * Used to populate the result with the index of the overlapping images.
   */
   virtual void getOverlappingImages(std::vector<ossim_uint32>& result,
                                     const ossimIrect& rect,
                                     ossim_uint32 resLevel=0)const;
   
   virtual ossimRefPtr<ossimImageData> getNextTile(ossim_uint32& returnedIdx,
                                                   const ossimIrect& origin,
                                                   ossim_uint32 resLevel=0);
   
   ossimIrect getRelativeRect(ossim_uint32 index,
                              ossim_uint32 resLevel = 0)const;
protected:
   std::vector<ossimDpt>  theInputTiePoints;
   
   /*!
    * Holds R0 delta and will be scaled for different r-level requests
    *
    */
   ossimDpt          theDelta;
   
   /*!
    * Will hold the upper left tie of the mosaic.
    */
   ossimDpt          theUpperLeftTie;

   ossimIrect        theBoundingRect;
   ossimString       theUnits;
   
   void computeBoundingRect(ossim_uint32 resLevel=0);
TYPE_DATA
};

#endif

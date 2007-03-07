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
// $Id: ossimImageMosaic.h,v 1.28 2005/02/11 15:07:33 dburken Exp $
#ifndef ossimImageMosaic_HEADER
#define ossimImageMosaic_HEADER
#include <vector>
using namespace std;

#include <imaging/tile_sources/ossimImageCombiner.h>


/**
 * An image mosaic is a simple combiner that will
 * just do a simple mosaic.  It just checks NULL pix values until it finds a
 * pixel that is not empty and copies it out to the output.  The list will
 * have same size tiles and have the same number of bands.
 */
class OSSIMDLLEXPORT ossimImageMosaic : public ossimImageCombiner
{
public:
   ossimImageMosaic();
   ossimImageMosaic(const std::vector<ossimImageSource*>& inputSources);
   virtual ~ossimImageMosaic();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
   ossimRefPtr<ossimImageData> theTile;

   template <class T> ossimRefPtr<ossimImageData> combine(
      T, // dummy template variable not used
      const ossimIrect& tileRect,
      ossim_uint32 resLevel=0);
   template <class T> ossimRefPtr<ossimImageData> combineNorm(
      T, // dummy template variable not used
      const ossimIrect& tileRect,
      ossim_uint32 resLevel=0);

TYPE_DATA
};

#endif /* #ifndef ossimImageMosaic_HEADER */

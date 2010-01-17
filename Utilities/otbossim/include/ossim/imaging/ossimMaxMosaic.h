//*******************************************************************
// Copyright (C) 2005 SANZ Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Kenneth Melero (kmelero@sanz.com)
//
// Description: This combiner is designed to "float" the maximum pixel value
//              of all inputs to the top of the mosaic output. 
//
//*************************************************************************
// $Id: ossimMaxMosaic.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimMaxMosaic_HEADER
#define ossimMaxMosaic_HEADER

#include <vector>
#include <ossim/imaging/ossimImageCombiner.h>


/**
 * An image mosaic is a simple combiner that will
 * just do a simple mosaic.  It just checks NULL pix values until it finds a
 * pixel that is not empty and copies it out to the output.  The list will
 * have same size tiles and have the same number of bands.
 */
class OSSIMDLLEXPORT ossimMaxMosaic : public ossimImageCombiner
{
public:
   ossimMaxMosaic();
   ossimMaxMosaic(ossimConnectableObject::ConnectableObjectList& inputSources);
   
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

   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;
   virtual ossimString getDescription()const;

protected:
   virtual ~ossimMaxMosaic();
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

#endif

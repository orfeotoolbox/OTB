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
// $Id: ossimRLevelFilter.h,v 1.9 2005/02/11 15:07:33 dburken Exp $
#ifndef ossimRLevelFilter_HEADER
#define ossimRLevelFilter_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>

/*!
 * This filter is used to adjust the RLevel.  If you would like to
 * continually change between RLevels or Reslolutions then you can use this
 * filter to do so.  You can also specify whether or not the rLevel
 * passed in from the getTile request is added to this classes current
 * Rlevel.  This is so if you concatenate multiple filters that reduce
 * the RLevels then it will keep reducing.  For example if I have
 * 2 filters that both go to RLevel 1 then it actually will go to
 * 2 since there are 2 decimations happening.  This can be turned off
 * by specifying the Rlevel not to be additive.
 */
class OSSIMDLLEXPORT ossimRLevelFilter : public ossimImageSourceFilter
{
public:

   ossimRLevelFilter(ossimObject* owner,
                     ossimImageSource* inputSource = NULL,
                     ossim_uint32 resLevel         = 0,
                     bool   overrideGeometry       = false);

   ossimRLevelFilter(ossimImageSource* inputSource = NULL,
                     ossim_uint32 resLevel         = 0,
                     bool   overrideGeometry       = false);

   virtual ~ossimRLevelFilter();

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;

   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=NULL);

   virtual void setCurrentRLevel(ossim_uint32 rlevel);

   virtual ossim_uint32 getCurrentRLevel()const;

   /*!
    * Get/set of the data member "theOverrideGeometryFlag".
    * If set this will override the "getImageGeometry" method and adjust
    * the getMetersPerPixel or the getDecimalDegreesPerPixel.
    * Default behavior is to not override method.
    */
   virtual bool getOverrideGeometryFlag() const;
   virtual void setOverrideGeometryFlag(bool override);

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   /*!
    * Initializes result with the sum of decimations from rlevel one to
    * theCurrentRLevel.
    */
   void getSummedDecimation(ossimDpt& result) const;
   
   ossim_uint32 theCurrentRLevel;
   bool         theOverrideGeometryFlag;

TYPE_DATA
};

#endif

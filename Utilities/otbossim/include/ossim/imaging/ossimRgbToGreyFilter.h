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
// $Id: ossimRgbToGreyFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimRgbToGreyFilter_HEADER
#define ossimRgbToGreyFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimRgbToGreyFilter : public ossimImageSourceFilter
{
public:
   ossimRgbToGreyFilter(ossimObject* owner=NULL);
   ossimRgbToGreyFilter(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   ossimRgbToGreyFilter(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   virtual ~ossimRgbToGreyFilter();
   
   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual ossimScalarType getOutputScalarType() const;
   
   ossim_uint32 getNumberOfOutputBands() const;
 
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
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
   
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimRefPtr<ossimImageData>& tile);
   double theC1;
   double theC2;
   double theC3;

TYPE_DATA
};

#endif /* #ifndef ossimRgbToGreyFilter_HEADER */

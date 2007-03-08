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
// $Id: ossimImageSharpenFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageSharpenFilter_HEADER
#define ossimImageSharpenFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimImageSharpenFilter : public ossimImageSourceFilter
{
public:
   ossimImageSharpenFilter(ossimObject* owner=NULL);
   virtual ~ossimImageSharpenFilter();

   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
protected:

   /**
    * Called on first getTile, will initialize all data needed.
    */
   virtual void allocate();
   
   ossimRefPtr<ossimImageData> theTile;

   /*!
    * Convolve full means that the input data is full and has
    * no null data.  We don't have to compare for nulls here
    */
   template<class T>
   void sharpenFull(T,
                    const ossimRefPtr<ossimImageData>& inputData,
                    ossimRefPtr<ossimImageData>& outputData);
   
   /*!
    * Convolve partial means that the input data is has some
    * null data.  We will have to compare nulls
    */
   template<class T>
   void sharpenPartial(T,
                       const ossimRefPtr<ossimImageData>& inputData,
                       ossimRefPtr<ossimImageData>& outputData);

TYPE_DATA
};

#endif /* #ifndef ossimImageSharpenFilter_HEADER */

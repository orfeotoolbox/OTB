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
// $Id: ossimHistogramEqualization.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimHistogramEqualization_HEADER
#define ossimHistogramEqualization_HEADER
#include <ossim/imaging/ossimImageSourceHistogramFilter.h>

class OSSIMDLLEXPORT ossimHistogramEqualization : public ossimImageSourceHistogramFilter
{
public:
   ossimHistogramEqualization();
   ossimHistogramEqualization(ossimImageSource* inputSource,
                              ossimRefPtr<ossimMultiResLevelHistogram> histogram);
   ossimHistogramEqualization(ossimImageSource* inputSource,
                              ossimRefPtr<ossimMultiResLevelHistogram> histogram,
                              bool inverseFlag);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual void setHistogram(ossimRefPtr<ossimMultiResLevelHistogram> histogram);
   virtual bool setHistogram(const ossimFilename& filename);

   virtual bool getInverseFlag()const;
   virtual void initialize();
   virtual void setInverseFlag(bool inverseFlag);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   virtual std::ostream& print(std::ostream& out) const;

protected:
   virtual ~ossimHistogramEqualization();

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
  ossimRefPtr<ossimImageData> theTile;
   /*!
    * This will be used in some of the histogram
    * based operations.
    */
   ossimRefPtr<ossimMultiResLevelHistogram> theAccumulationHistogram;

   /*!
    * Indicates if you should equalize or unequalize an input stream.
    * If the inverse flag is true then it will indicate that we should
    * assume that the input stream is already equalized and we should
    * invert based on the current accumulation histogram.
    *
    *
    * This flag is defaulted to false.
    */
   bool                         theInverseFlag;
   /*!
    * We will create a LUT for each band that directly maps
    * an input value to an equalized output.
    */
   vector<double*> theForwardLut;
   
   /*!
    * We will create a LUT for each band that directly maps
    * an input value to an un-equalized output.
    */
   vector<double*> theInverseLut;

   
  std::vector<ossim_uint32> theBandList;
   virtual void computeAccumulationHistogram();

  template<class T>
  ossimRefPtr<ossimImageData> runEqualizationAlgorithm(T dummyVariable,
                                                       ossimRefPtr<ossimImageData> tile);

   /*!
    * We will pre-compute the luts for the forward and inverse
    * equalization.  Note: the inverse is a little more tricky
    * and since we must fill the empty spots missed by the
    * inverse.  We use a linear interpolation between valid
    * values for this.
    */
   virtual void initializeLuts();
   virtual void deleteLuts();

TYPE_DATA
};
#endif /* #ifndef ossimHistogramEqualization_HEADER */

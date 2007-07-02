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
// $Id: ossimImageToPlaneNormalFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageToPlaneNormalFilter_HEADER
#define ossimImageToPlaneNormalFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimImageToPlaneNormalFilter : public ossimImageSourceFilter
{
public:
   ossimImageToPlaneNormalFilter();
   ossimImageToPlaneNormalFilter(ossimImageSource* inputSource);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32    getNumberOfOutputBands() const;
   
   void setXScale(const double& scale);
   void setYScale(const double& scale);
   
   double getXScale()const;
   double getYScale()const;

   void setTrackScaleFlag(bool flag);
   bool getTrackScaleFlag()const;
   
   void   setSmoothnessFactor(double value);
   double getSmoothnessFactor()const;
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix);
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix)const;
   virtual void initialize();
   
protected:
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theBlankTile;

   bool            theTrackScaleFlag;
   double          theXScale;
   double          theYScale;
   double          theSmoothnessFactor;
   virtual void computeNormals(ossimRefPtr<ossimImageData>& inputTile,
                               ossimRefPtr<ossimImageData>& outputTile);

   template <class T>
   void computeNormalsTemplate(T inputScalarTypeDummy,
                               ossimRefPtr<ossimImageData>& inputTile,
                               ossimRefPtr<ossimImageData>& outputTile);
TYPE_DATA
};

#endif

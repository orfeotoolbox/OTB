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
// $Id: ossimImageToPlaneNormalFilter.h 20078 2011-09-09 12:25:50Z gpotts $
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
   /* ------------------- PROPERTY INTERFACE -------------------- */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /* ------------------ PROPERTY INTERFACE END ------------------- */
   
protected:
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimIrect      theInputBounds;
   bool            theTrackScaleFlag;
   double          theXScale;
   double          theYScale;
   double          theSmoothnessFactor;
   
   void initializeTile();
   virtual void computeNormals(ossimRefPtr<ossimImageData>& inputTile,
                               ossimRefPtr<ossimImageData>& outputTile);

   template <class T>
   void computeNormalsTemplate(T inputScalarTypeDummy,
                               ossimRefPtr<ossimImageData>& inputTile,
                               ossimRefPtr<ossimImageData>& outputTile);
TYPE_DATA
};

#endif

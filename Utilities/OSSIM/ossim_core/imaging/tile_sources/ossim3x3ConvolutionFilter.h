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
// $Id: ossim3x3ConvolutionFilter.h,v 1.9 2005/08/17 19:18:12 gpotts Exp $
#ifndef ossim3x3ConvolutionFilter_HEADER
#define ossim3x3ConvolutionFilter_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>


class ossim3x3ConvolutionFilter : public ossimImageSourceFilter
{
public:
   ossim3x3ConvolutionFilter(ossimObject* owner=NULL);
   virtual ~ossim3x3ConvolutionFilter();

   virtual ossimString getShortName()const{return ossimString("3x3 Convolution");}
   virtual ossimString getLongName()const{return ossimString("Convolves the input image with a 3x3 kernel");}

   virtual void getKernel(double kernel[3][3]);
   
   virtual void setKernel(double kernel[3][3]);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();

   virtual double getNullPixelValue(ossim_uint32 band=0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0)  const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)  const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;


   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   
protected:

   /**
    * Allocates theTile.
    */
   void allocate();

   /**
    * Clears data members theNullPixValue, theMinPixValue, and
    * theMaxPixValue.
    */
   void clearNullMinMax();

   /**
    * Computes null, min, and max considering input connection and theKernel.
    */
   void computeNullMinMax();
   
   ossimRefPtr<ossimImageData> theTile;
   double theKernel[3][3];

   vector<double> theNullPixValue;
   vector<double> theMinPixValue;
   vector<double> theMaxPixValue;   
   
   /*!
    * Convolve full means that the input data is full and has
    * no null data.  We don't have to compare for nulls here
    */
   template<class T>
   void convolveFull(T,
                     ossimRefPtr<ossimImageData> inputData,
                     ossimRefPtr<ossimImageData> outputData);
   
   /*!
    * Convolve partial means that the input data is has some
    * null data.  We will have to compare nulls
    */
   template<class T>
   void convolvePartial(T,
                        ossimRefPtr<ossimImageData> inputData,
                        ossimRefPtr<ossimImageData> outputData);
                       

TYPE_DATA
};

#endif /* #ifndef ossim3x3ConvolutionFilter_HEADER */

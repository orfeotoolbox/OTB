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
// $Id: ossimImageSharpenFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimImageSharpenFilter_HEADER
#define ossimImageSharpenFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimConvolutionSource.h>
class OSSIMDLLEXPORT ossimImageSharpenFilter : public ossimImageSourceFilter
{
public:
   ossimImageSharpenFilter(ossimObject* owner=NULL);

   virtual ossimString getShortName()const;
   virtual ossimString getLongName()const;
   
   ossim_uint32 getWidth()const;
   ossim_float64 getSigma()const;
   void setWidthAndSigma(ossim_uint32 w, ossim_float64 sigma);
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual void connectInputEvent(ossimConnectionEvent &event);
   virtual void disconnectInputEvent(ossimConnectionEvent &event);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   virtual ~ossimImageSharpenFilter();
   inline double laplacianOfGaussian(double x, double y, double sigma)
   {
      double r2 = x*x+y*y;
      double sigma2 = sigma*sigma;
      return ((1.0/(M_PI*sigma2*sigma2))*
              (1.0-r2/(2.0*sigma2))*
              (exp(-r2/(2.0*sigma2))));
      
   }
   
   void buildConvolutionMatrix();
   

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

   ossimRefPtr<ossimConvolutionSource> theConvolutionSource;
   ossim_uint32 theWidth;
   ossim_float64 theSigma;
TYPE_DATA
};

#endif /* #ifndef ossimImageSharpenFilter_HEADER */

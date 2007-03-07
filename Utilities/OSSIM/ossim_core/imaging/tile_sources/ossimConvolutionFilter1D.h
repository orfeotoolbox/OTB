//*******************************************************************
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// class ossimImageGaussianFilter : tile source
// class ossimConvolutionFilter1D
// one dimensional convolution
//
// efficient for gradients & separable convolution kernels (like gaussian)
//*******************************************************************
// $Id: ossimConvolutionFilter1D.h,v 1.2 2005/09/12 13:39:07 gpotts Exp $

#ifndef ossimConvolutionFilter1D_HEADER
#define ossimConvolutionFilter1D_HEADER

#include <imaging/tile_sources/ossimImageSourceFilter.h>

class OSSIM_DLL ossimConvolutionFilter1D : public ossimImageSourceFilter
{
public:
   //! own class methods
   ossimConvolutionFilter1D(ossimObject* owner=NULL);
   virtual ~ossimConvolutionFilter1D();

   virtual inline const std::vector<ossim_float64>& getKernel()const { return theKernel; }
   virtual void setKernel(const std::vector<ossim_float64>& aKernel);

   virtual inline bool isHorizontal()const         { return theIsHz; }
   virtual inline void setIsHorizontal(bool aIsHz) { theIsHz = aIsHz; }

   virtual inline ossim_int32 getCenterOffset()const              { return theCenterOffset; }
   virtual inline void setCenterOffset(ossim_int32 aCenterOffset) { theCenterOffset = aCenterOffset; }
   
   //! inherited methods
   virtual ossimString getShortName()const{return ossimString("1D Convolution");}
   virtual ossimString getLongName()const{return ossimString("Convolves the input image with a one dimensional convolution kernel");}

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,ossim_uint32 resLevel=0);

   virtual void initialize();

   virtual double getNullPixelValue(ossim_uint32 band=0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0)  const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)  const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix = 0)const;
   
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

   //! offset of center point in the Kernel
   ossim_int32                theCenterOffset;
   ossimRefPtr<ossimImageData> theTile;
   std::vector<ossim_float64>  theKernel;
   bool                        theIsHz; //! isHorizontal convolution?

   std::vector<double> theNullPixValue;
   std::vector<double> theMinPixValue;
   std::vector<double> theMaxPixValue;

TYPE_DATA
};

#endif /* #ifndef ossimConvolutionFilter1D_HEADER */

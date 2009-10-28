//*******************************************************************
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// class ossimImageGaussianFilter : tile source
//*******************************************************************
// $Id: ossimImageGaussianFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimImageGaussianFilter_HEADER
#define ossimImageGaussianFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>

/**
 * class for symmetric Gaussian filtering
 * implemented as two separable horizontal/vertical gaussian filters
 *
 * PROPERTIES:
 * -GaussStd is the standard deviation of the gaussian
 *  (filter widths are equal to 2*floor(2.5*GaussStd+0.5)+1)
 * -strictNoData selects the behaviour for NODATA pixels
 *   true  : any NODATA pixels in the convolution will Nullify the center pixel
 *   false : center pixel will be NODATA only if it was NODATA before 
 *     other NODATA pixels are processed as zero in the convolution calculation
 */
class OSSIM_DLL ossimImageGaussianFilter : public ossimImageSourceFilter
{
public:
  /** 
   * own methods
   */
   ossimImageGaussianFilter();

   inline ossim_float64 getGaussStd()const { return theGaussStd; }
   void setGaussStd(const ossim_float64& v);

   inline bool isStrictNoData()const { return theStrictNoData; }
   void setStrictNoData(bool aStrict);

  /** 
   * inherited methods
   */
   virtual void initialize();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect &tileRect,ossim_uint32 resLevel=0);

   virtual void connectInputEvent(ossimConnectionEvent &event);
   virtual void disconnectInputEvent(ossimConnectionEvent &event);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix = 0);   
   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix = 0)const;
   
protected:
   virtual ~ossimImageGaussianFilter();
  /**
   * protected methods
   */
   void initializeProcesses();
   void updateKernels();

  /**
   * parameters
   */
   ossim_float64 theGaussStd;
   bool          theStrictNoData;

  /**
   * subprocesses
   */
   ossimRefPtr<ossimConvolutionFilter1D> theHF; //horizontal filter
   ossimRefPtr<ossimConvolutionFilter1D> theVF; //vertical filter

TYPE_DATA
};

#endif /* #ifndef ossimImageGaussianFilter_HEADER */

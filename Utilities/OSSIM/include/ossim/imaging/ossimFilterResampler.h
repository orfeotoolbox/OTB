//*******************************************************************
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Contributor:
//         David A. Horner (DAH) http://dave.thehorners.com
//
//*************************************************************************
// $Id: ossimFilterResampler.h 9696 2006-10-11 17:51:26Z gpotts $
#ifndef ossimFilterResampler_HEADER
#define ossimFilterResampler_HEADER

#include <vector>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimFilter.h>
#include <ossim/imaging/ossimFilterTable.h>

class OSSIMDLLEXPORT ossimFilterResampler
{
public:
   enum ossimFilterResamplerType
   {
      ossimFilterResampler_NEAREST_NEIGHBOR = 0,
      ossimFilterResampler_BOX              = 1,
      ossimFilterResampler_GAUSSIAN         = 2,
      ossimFilterResampler_CUBIC            = 3,
      ossimFilterResampler_HANNING          = 4,
      ossimFilterResampler_HAMMING          = 5,
      ossimFilterResampler_LANCZOS          = 6,
      ossimFilterResampler_MITCHELL         = 7,
      ossimFilterResampler_CATROM           = 8,
      ossimFilterResampler_BLACKMAN         = 9,
      ossimFilterResampler_BLACKMAN_SINC    = 10,
      ossimFilterResampler_BLACKMAN_BESSEL  = 11,
      ossimFilterResampler_QUADRATIC        = 12,
      ossimFilterResampler_TRIANGLE         = 13,
      ossimFilterResampler_BILINEAR         = 13,
      ossimFilterResampler_HERMITE          = 14,
      ossimFilterResampler_BELL             = 15,
      ossimFilterResampler_BSPLINE          = 16
      
   };
   ossimFilterResampler();
   virtual ~ossimFilterResampler();

  virtual void resample(const ossimRefPtr<ossimImageData>& input,
			ossimRefPtr<ossimImageData>& output,
			const ossimDpt& ul,
			const ossimDpt& ur,
			const ossimDpt& deltaUl,
			const ossimDpt& deltaUr,
			const ossimDpt& length);

  virtual void resample(const ossimRefPtr<ossimImageData>& input,
			ossimRefPtr<ossimImageData>& output,
			const ossimIrect& outputSubRect,
			const ossimDpt& ul,
			const ossimDpt& ur,
			const ossimDpt& deltaUl,
			const ossimDpt& deltaUr,
			const ossimDpt& length);

  void setFilterType(ossimFilterResamplerType filterType);
  void setFilterType(ossimFilterResamplerType minifyFilterType,
		     ossimFilterResamplerType magnifyFilterType);
  void setFilterType(const ossimString& type);
  void setFilterType(const ossimString& minifyType,
		     const ossimString& magnifyType);

  void setMinifyFilterType(const ossimString& minifyType);
  void setMagnifyFilterType(const ossimString& magnifyType);
  void setMinifyFilterType(ossimFilterResamplerType filterType);
  void setMagnifyFilterType(ossimFilterResamplerType filterType);
  ossimString getMinifyFilterTypeAsString()const;
  ossimString getMagnifyFilterTypeAsString()const;
  void setScaleFactor(const ossimDpt& scale);
  void setBlurFactor(ossim_float64 blur);

  void setBoundingInputRect(const ossimIrect& rect)
  {
    theInputRect = rect;
  }
  ossimIrect getBoundingInputRect()const
  {
    return theInputRect;
  }

  ossim_float64 getBlurFactor()const;

  const ossimDpt& getScaleFactor()const
  {
    return theScaleFactor;
  }
  /*!
   * Saves the state of this object.
   */
  virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
  
  /*!
   * Loads the state of this object.
   */
  virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
  
  virtual void getKernelSupport(double& x,
				double & y)const;

  virtual void getFilterTypes(std::vector<ossimString>& filterTypes)const;

private:
  const ossimFilter* getHorizontalFilter()const;
  const ossimFilter* getVerticalFilter()const;

  template <class T>
  void resampleBilinearTile(T dummy,
			    const ossimRefPtr<ossimImageData>& input,
			    ossimRefPtr<ossimImageData>& output,
			    const ossimIrect& outputSubRect,
			    const ossimDpt& inputUl,
			    const ossimDpt& inputUr,
			    const ossimDpt& deltaUl,
			    const ossimDpt& deltaUr,
			    const ossimDpt& outLength);
  
   void computeTable();
   ossimString getFilterTypeAsString(ossimFilterResamplerType type)const;
   ossimFilterResamplerType getFilterType(const ossimString& type)const;
   ossimFilter* createNewFilter(ossimFilterResamplerType filterType,
                                ossimFilterResamplerType& result);


   ossimFilter*             theMinifyFilter;
   ossimFilter*             theMagnifyFilter;
   ossimFilterTable         theFilterTable;
   
   ossimFilterResamplerType theMinifyFilterType;
   ossimFilterResamplerType theMagnifyFilterType;
   ossimDpt                 theScaleFactor;
   ossimDpt                 theInverseScaleFactor;
   
   ossimIrect               theInputRect;
   ossim_float64            theBlurFactor;
};

#endif

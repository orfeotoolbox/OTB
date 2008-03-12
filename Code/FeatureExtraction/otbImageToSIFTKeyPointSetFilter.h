/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbImageToSIFTKeyPointSetFilter_h
#define _otbImageToSIFTKeyPointSetFilter_h

#include "otbImageToPointSetFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkMultiplyImageFilter.h" 

namespace otb
{
  namespace Functor 
    {
      /** \class IndexLexicographicCompare
       * \brief Order point instances lexicographically.
       *
       * This is a comparison functor suitable for storing Point instances
       * in an STL container.  The ordering is total and unique but has
       * little geometric meaning.
       */
      template <class TCoordRep, unsigned int NPointDimension>
	class PointLexicographicCompare
	{
	public:
	  bool operator()(itk::Point<TCoordRep,NPointDimension> const & l,itk::Point<TCoordRep,NPointDimension> const & r) const
	    {
	      for(unsigned int i=0; i < NPointDimension; ++i)
		{
		  if(l[i] < r[i])
		    {
		      return true;
		    }
		  else if(l[i] > r[i])
		    {
		      return false;
		    }
		}
	      return false;
	    }
	};
      /** \class MagnitudeFunctor
       *  \brief This functor computes the magnitude of a covariant vector.
       */
      template <class TInputPixel,class TOutputPixel>
	class MagnitudeFunctor
	{
	public:
	
	  inline TOutputPixel operator()(const TInputPixel& input)
	    {
	      return vcl_sqrt(input[0]*input[0]+input[1]*input[1]);
	    }
	};
      
      /** \class OrientationFunctor
       *  \brief This functor computes the orientation of a cavariant vector<br>
       *  Orientation values lies between 0 and 2*Pi.
       */
      template <class TInputPixel,class TOutputPixel>
	class OrientationFunctor
	{
	public:
	
	  inline TOutputPixel operator()(const TInputPixel& input)
	    {
	      TOutputPixel resp = vcl_atan2(input[0],input[1]);
	      if(resp<0)
		{
		  resp+=2*M_PI;
		}

	      return resp;
	    }
	};
    }// end namespace Functor

  /** \class ImageToSIFTKeyPointSetFilter
   *  \brief This class extracts key points from an input image, trough a pyramidal decomposition.
   *
   * This class implements the SIFT key point detector proposed by David G. Lowe in proceedings of International
   * Conference on Computer Vision, Corfu, september 1999.
   *
   * Selected key points are stored in a itk::PointSet structure.
   * Data points contains a list of (magnitude, main orientation) for each level
   * of the pyramidal decomposition.
   * 
   * Orientation is expressed in degree in the range [0,360] with a precision of 10 degrees.
   *
   */
  template <class TInputImage, class TOutputPointSet>
    class ITK_EXPORT ImageToSIFTKeyPointSetFilter
    : public ImageToPointSetFilter<TInputImage,TOutputPointSet>
    {
    public:
      /** Standard typedefs */
      typedef ImageToSIFTKeyPointSetFilter                       Self;
      typedef ImageToPointSetFilter<TInputImage,TOutputPointSet> Superclass;
      typedef itk::SmartPointer<Self>                            Pointer;
      typedef itk::SmartPointer<const Self>                      ConstPointer;
  
      /** Creation through object factory macro */
      itkNewMacro(Self);  

      /** Type macro */
      itkTypeMacro(ImageToSIFTKeyPointSetFilter,ImageToPointSetFilter);
  
      /** Template parameters typedefs */
      typedef TInputImage InputImageType;
      typedef typename InputImageType::Pointer InputImagePointerType;
      typedef typename InputImageType::SpacingType SpacingType;
      typedef typename InputImageType::PointType PointType;
      typedef typename InputImageType::SizeType SizeType;
      typedef typename InputImageType::IndexType IndexType;
      typedef typename InputImageType::OffsetType OffsetType;
      typedef typename InputImageType::RegionType RegionType;
      typedef typename InputImageType::PixelType PixelType;
      typedef typename InputImageType::ConstPointer InputImageConstPointerType;
      typedef TOutputPointSet OutputPointSetType;
      typedef typename OutputPointSetType::Pointer OutputPointSetPointerType;
      typedef typename OutputPointSetType::PixelType OutputPixelType;
      typedef typename OutputPointSetType::PointType OutputPointType;

      /** Internal filters typedefs */
      typedef itk::RecursiveGaussianImageFilter<InputImageType,InputImageType> GaussianFilterType;
      typedef typename GaussianFilterType::Pointer GaussianFilterPointerType;
      typedef itk::SubtractImageFilter<InputImageType,InputImageType,InputImageType> SubtractFilterType;
      typedef typename SubtractFilterType::Pointer SubtractFilterPointerType;
      typedef itk::ResampleImageFilter<InputImageType,InputImageType> ResampleFilterType;
      typedef typename ResampleFilterType::Pointer ResampleFilterPointerType;
      typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
      typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;
      typedef itk::GradientImageFilter<InputImageType,PixelType,PixelType> GradientFilterType;
      typedef typename GradientFilterType::Pointer GradientFilterPointerType;
      typedef typename GradientFilterType::OutputImageType GradientOutputImageType;
      typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
	Functor::MagnitudeFunctor<typename GradientOutputImageType::PixelType,typename InputImageType::PixelType> > MagnitudeFilterType;
      typedef typename MagnitudeFilterType::Pointer MagnitudeFilterPointerType;
      typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
	Functor::OrientationFunctor<typename GradientOutputImageType::PixelType,typename InputImageType::PixelType> > OrientationFilterType;
      typedef typename OrientationFilterType::Pointer OrientationFilterPointerType;
      typedef itk::RescaleIntensityImageFilter<InputImageType, InputImageType> RescaleFilterType;
      typedef itk::MultiplyImageFilter<InputImageType, InputImageType, InputImageType> MultiplyFilterType;
      typedef itk::MinimumMaximumImageCalculator<InputImageType> MinimumMaximumCalculatorType;

      typedef typename RescaleFilterType::Pointer RescaleFilterPointerType;
      typedef typename MultiplyFilterType::Pointer MultiplyFilterPointerType;
      typedef typename MinimumMaximumCalculatorType::Pointer MinimumMaximumCalculatorPointerType;
  
      /** Internal structure to store the temporary keys */
      typedef  Functor::PointLexicographicCompare<typename OutputPointType::CoordRepType,InputImageType::ImageDimension> PointLexicographicCompareType;
      typedef std::map<OutputPointType,OutputPixelType,PointLexicographicCompareType> ResultMapType;
      typedef typename ResultMapType::iterator ResultMapIteratorType;

      /** Set/Get the number of iterations */
      itkSetMacro(NumberOfIterations,unsigned int);
      itkGetMacro(NumberOfIterations,unsigned int);


      /**
       * Set the input image.
       * \param image The input image.
       */
      virtual void SetInput(const InputImageType * image);

    protected:
      /** Actually process the input */
      virtual void GenerateData();
      /** Constructor */
      ImageToSIFTKeyPointSetFilter();
      /** Destructor */
      virtual ~ImageToSIFTKeyPointSetFilter() {}
      /**PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

      /**
       * Reset the internal filters for next iteration
       */
      virtual void ResetFilters();

      /**
       * Compute a local region centered on idex with a radius of 1/
       * \param index The index of the region center.
       * \return
       */
      virtual RegionType GetLocalRegion(const IndexType & index) const;
      
      /**
       * Setup up key features filters for the given iteration.
       * \param iteration The current iteration.
       */
      virtual void KeyFeaturesFilterSetup(unsigned int iteration);

      /**
       * Add a new key to the results.
       * \param point The key to add.
       */
      virtual void AddKey(const OutputPointType& point);

      /**
       * Check if the current key is valid at the current iteration
       * \param it An iterator pointing to the key in the internal result map.
       * \return 
       */
      virtual bool CheckKey(ResultMapIteratorType it);

      /**
       * Update the key features at the current iteration.
       * \param it An iterator pointing to the key in the internal result map.
       * \param iteration The current iteration.
       */
      virtual void UpdateKey(ResultMapIteratorType it, const unsigned int iteration);

      /**
       * Check if the given neighborhood is a local extremum.
       * \param neigh The neighborhood to check.
       * \return 
       */
      virtual bool IsLocalExtremum(const NeighborhoodType& neigh) const;

      /**
       * Convert the internal result map to the output pointset.
       */
      virtual void ExportSolution();
  
  
    private:
      ImageToSIFTKeyPointSetFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      /** Number of iterations of the pyramidal decomposition */
      unsigned int m_NumberOfIterations;

      /** Gaussian filters */
      GaussianFilterPointerType m_XGaussianFilter1;
      GaussianFilterPointerType m_YGaussianFilter1;
      GaussianFilterPointerType m_XGaussianFilter2;
      GaussianFilterPointerType m_YGaussianFilter2;
      GaussianFilterPointerType m_XGaussianFilter3;
      GaussianFilterPointerType m_YGaussianFilter3;
  
      /** Subtract filter */
      SubtractFilterPointerType m_SubtractFilter;

      /** Resample filter */
      ResampleFilterPointerType m_ResampleFilter;

      /** Gradient filter */
      GradientFilterPointerType m_GradientFilter;

      /** Magnitude filter */
      MagnitudeFilterPointerType m_MagnitudeFilter;

      /** Orientation filter */
      OrientationFilterPointerType m_OrientationFilter;

      /** Rescale Intensity filter */
      RescaleFilterPointerType m_RescaleFilter;
  
      /** Min/Max Calculator */
      MinimumMaximumCalculatorPointerType m_MinimumMaximumCalculator;
  
      /** Multiply Filter */
      MultiplyFilterPointerType m_MultiplyFilter;
  
      /** Temporary results */
      ResultMapType m_ResultMap;

      /** Gaussian sigma for histogram smoothing */
      static const double m_HistogramGaussianWeights[73];
    };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToSIFTKeyPointSetFilter.txx"
#endif

#endif

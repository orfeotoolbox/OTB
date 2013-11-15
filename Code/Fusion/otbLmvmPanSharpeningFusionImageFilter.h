/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLmvmPanSharpeningFusionImageFilter_h
#define __otbLmvmPanSharpeningFusionImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbConvolutionImageFilter.h"
#include "itkTernaryFunctorImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "itkNoiseImageFilter.h"
#include "otbVectorImage.h"

#include "itkProgressAccumulator.h"


namespace otb
{
/**
 * \class LmvmPanSharpeningFusionImageFilter
 * \brief This class performs a Local Mean and Variance Matching (LMVM) Pan sharpening operation
 *
 * Given a Pan image and the corresponding Xs image (oversampled to have the
 * same number of pixels), this filter realizes an Local Mean and Variance
 * Matching Pan sharpening operation as described in:
 *
 * de Bethune, S. and Muller, F. and Donnay, J.P. 1998. Fusion of multispectral and panchromatic images
 * by local mean and variance matching filtering techniques. Fusion of Earth Data, 28-30 August 1998.
 *
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup Fusion
 *
 *
 **/

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision = float>
class ITK_EXPORT LmvmPanSharpeningFusionImageFilter :
  public itk::ImageToImageFilter<TXsImageType, TOutputImageType>
{
public:
  /** Standard class typedefs */
  typedef LmvmPanSharpeningFusionImageFilter                       Self;
  typedef itk::ImageToImageFilter<TXsImageType, TOutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Internal image type used as Pan smoothing and local standard deviation filter output */
  typedef otb::Image<TInternalPrecision,
    TPanImageType::ImageDimension>                InternalImageType;

  /** Single band Xs image type */
  typedef typename TXsImageType::InternalPixelType  XsPixelType;
  typedef otb::Image<XsPixelType,
    TXsImageType::ImageDimension>                XsBandImageType;

  /** Internal image type used as Xs smoothing and local standard deviation filter output */
  typedef otb::VectorImage<TInternalPrecision,
    TXsImageType::ImageDimension>                InternalVectorImageType;

  /** Typedef for the radius of the smoothing and local standard deviation filter */
  typedef typename itk::Array<TInternalPrecision> ArrayType;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(LmvmPanSharpeningFusionImageFilter,
               itk::ImageToImageFilter);

  /** Define the radius type for the smoothing and local standard deviation operation */
  typedef typename InternalImageType::SizeType RadiusType;

  /** Set the smoothing and local standard deviation filter radius  */
  itkGetMacro(Radius, RadiusType);
  itkSetMacro(Radius, RadiusType);

  /** Set the kernel used for the smoothing and local standard deviation filter */
  itkSetMacro(Filter, ArrayType);
  itkGetConstReferenceMacro(Filter, ArrayType);

  virtual void SetPanInput(const TPanImageType * image);
  const TPanImageType * GetPanInput(void) const;

  virtual void SetXsInput(const TXsImageType * path);
  const TXsImageType * GetXsInput(void) const;

protected:
  /** Constructor */
  LmvmPanSharpeningFusionImageFilter();

  /** Destructor */
  virtual ~LmvmPanSharpeningFusionImageFilter() {};

  /** Call to generate data, wiring composite internal minipipeline */
  void GenerateData();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LmvmPanSharpeningFusionImageFilter(Self &);   // intentionally not implemented
  void operator =(const Self&);                 // intentionally not implemented

  /** \class FusionFunctor
   * This functor applies the LMVM
   * operation. It is intended for internal use only.
   */
  class FusionFunctor1
  {
  public:
    // Implement the fusion as a six arguments operator
    typename TOutputImageType::PixelType operator()(
      const typename InternalVectorImageType::PixelType& stdXsPixel,
      const TInternalPrecision& smoothPanchroPixel,
      const typename TPanImageType::PixelType& sharpPanchroPixel) const
    {
      // Build output pixel
      typename TOutputImageType::PixelType output(stdXsPixel.Size());

      // Perform fusion for each band with appropriate casting
      for(unsigned int i = 0; i < stdXsPixel.Size(); ++i)
        {
        output[i] = static_cast<typename TOutputImageType::InternalPixelType>(
          ((sharpPanchroPixel - smoothPanchroPixel) * stdXsPixel[i]));
        }
      // Returns the output pixel
      return output;
    }
  };

  class FusionFunctor2
  {
  public:
    // Implement the fusion as a six arguments operator
    typename TOutputImageType::PixelType operator()(const typename TOutputImageType::PixelType& functor1Pixel,
                                                    const typename InternalVectorImageType::PixelType& smoothXsPixel,
                                              const TInternalPrecision& stdPanchroPixel) const
    {
      // Build output pixel
      typename TOutputImageType::PixelType output(smoothXsPixel.Size());

      TInternalPrecision scale = 1.;

      if(vcl_abs(stdPanchroPixel) > 1e-10)
      {
        scale = 1.0/stdPanchroPixel;
      }

      // Perform fusion for each band with appropriate casting
      for(unsigned int i = 0; i < smoothXsPixel.Size(); ++i)
        {
        output[i] = static_cast<typename TOutputImageType::InternalPixelType>(
          ((functor1Pixel[i] * scale) + smoothXsPixel[i]));
        }
      // Returns the output pixel
      return output;
    }
  };

  /**
   *  Typedef of the TernaryFunctorImageFilter applying the fusion functor to
   *  p, p_smooth, p_std, xs_smooth, xs_std and xs.
   */
  typedef itk::TernaryFunctorImageFilter<InternalVectorImageType,
                                         InternalImageType,
                                         TPanImageType,
                                         TOutputImageType,
                                         FusionFunctor1>     FusionStep1FilterType;

  /** Pointer to the fusion filter */
  typename FusionStep1FilterType::Pointer      m_FusionStep1Filter;


  typedef itk::TernaryFunctorImageFilter<TOutputImageType,
                                         InternalVectorImageType,
                                    InternalImageType,
                                    TOutputImageType,
                                         FusionFunctor2>     FusionStep2FilterType;

  /** Pointer to the fusion filter */
  typename FusionStep2FilterType::Pointer      m_FusionStep2Filter;


  /** Typedef of the convolution filter performing Pan smoothing */
  typedef otb::ConvolutionImageFilter
      <TPanImageType,
       InternalImageType,
       itk::ZeroFluxNeumannBoundaryCondition<TPanImageType>,
       TInternalPrecision>                                  PanConvolutionFilterType;

  /** Pointer to the internal Pan convolution filter */
  typename PanConvolutionFilterType::Pointer m_PanConvolutionFilter;

  /** Typedef of the convolution filter performing Xs smoothing */
  typedef otb::ConvolutionImageFilter
      <XsBandImageType,
       InternalImageType,
       itk::ZeroFluxNeumannBoundaryCondition<TPanImageType>,
       TInternalPrecision>                                  XsConvolutionFilterType;

  /** Pointer to the internal Xs convolution filter */
  typename XsConvolutionFilterType::Pointer m_XsConvolutionFilter;

  /** Typedef of the Pan local standard deviation filter*/
  typedef itk::NoiseImageFilter
      <TPanImageType, InternalImageType>               PanNoiseFilterType;

  /** Pointer to the Pan local standard deviation filter */
  typename PanNoiseFilterType::Pointer m_PanNoiseFilter;

  /** Typedef of the Xs local standard deviation filter*/
  typedef itk::NoiseImageFilter
      <XsBandImageType, InternalImageType>               XsNoiseFilterType;

  /** Pointer to the Xs local standard deviation filter */
  typename XsNoiseFilterType::Pointer m_XsNoiseFilter;

  /** Typedef of a helper filter to apply to a vector image*/
  typedef otb::PerBandVectorImageFilter
      < TXsImageType, InternalVectorImageType, XsConvolutionFilterType> XsVectorConvolutionFilterType;

  /** Pointer to the helper vector image filter */
  typename XsVectorConvolutionFilterType::Pointer m_XsVectorConvolutionFilter;

  /** Typedef of a helper filter to apply to a vector image*/
  typedef otb::PerBandVectorImageFilter
      < TXsImageType, InternalVectorImageType, XsNoiseFilterType> XsVectorNoiseFilterType;

  /** Pointer to the helper vector image filter */
  typename XsVectorNoiseFilterType::Pointer m_XsVectorNoiseFilter;

  /** Radius used for the smoothing filter */
  RadiusType m_Radius;

  /** Kernel used for the smoothing filter */
  ArrayType  m_Filter;

  /** The internal progress accumulator */
  typename itk::ProgressAccumulator::Pointer m_ProgressAccumulator;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLmvmPanSharpeningFusionImageFilter.txx"
#endif

#endif

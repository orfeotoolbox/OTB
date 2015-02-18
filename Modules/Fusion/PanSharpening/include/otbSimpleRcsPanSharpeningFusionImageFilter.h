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
#ifndef __otbSimpleRcsPanSharpeningFusionImageFilter_h
#define __otbSimpleRcsPanSharpeningFusionImageFilter_h

#include "otbConvolutionImageFilter.h"
#include "otbImage.h"
#include "itkTernaryFunctorImageFilter.h"

#include "itkProgressAccumulator.h"


namespace otb
{
/**
 * \class SimpleRcsPanSharpeningFusionImageFilter
 * \brief This class performs a simple Pan sharpening operation
 *
 * Given a Pan image and the corresponding Xs image (oversampled to have the
 * same number of pixels), this filter realizes a simple Pan sharpening
 * operation:
 *
 * \f[ \frac{XS}{\mathrm{Filtered}(PAN)} PAN  \f]
 *
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup Fusion
 *
 * \example Fusion/PanSharpeningExample.cxx
 *
 **/

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision = float>
class ITK_EXPORT SimpleRcsPanSharpeningFusionImageFilter :
  public itk::ImageToImageFilter<TXsImageType, TOutputImageType>
{
public:
  /** Standard class typedefs */
  typedef SimpleRcsPanSharpeningFusionImageFilter                 Self;
  typedef itk::ImageToImageFilter<TXsImageType, TOutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Internal image type used as the smoothing filter output */
  typedef otb::Image<TInternalPrecision,
    TPanImageType::ImageDimension>                InternalImageType;

  /** Typedef for the radius of the smoothing filter */
  typedef typename itk::Array<TInternalPrecision> ArrayType;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(SimpleRcsPanSharpeningFusionImageFilter,
               itk::ImageToImageFilter);

  /** Define the radius type for the smoothing operation */
  typedef typename InternalImageType::SizeType RadiusType;

  /** Set the smoothing filter radius  */
  itkGetMacro(Radius, RadiusType);
  itkSetMacro(Radius, RadiusType);

  /** Set the kernel used for the smoothing filter */
  itkSetMacro(Filter, ArrayType);
  itkGetConstReferenceMacro(Filter, ArrayType);

  virtual void SetPanInput(const TPanImageType * image);
  const TPanImageType * GetPanInput(void) const;

  virtual void SetXsInput(const TXsImageType * path);
  const TXsImageType * GetXsInput(void) const;

protected:
  /** Constructor */
  SimpleRcsPanSharpeningFusionImageFilter();

  /** Destructor */
  virtual ~SimpleRcsPanSharpeningFusionImageFilter() {};

  /** Call to generate data, wiring composite internal minipipeline */
  void GenerateData();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SimpleRcsPanSharpeningFusionImageFilter(Self &);   // intentionally not implemented
  void operator =(const Self&);          // intentionally not implemented

  /** \class FusionFunctor
   * This functor applies the
   *  \f[ \frac{XS}{\mathrm{Filtered}(PAN)}PAN  \f]
   * operation. It is intended for internal use only.
   */
  class FusionFunctor
  {
  public:
    // Implement the fusion as a three arguments operator
    typename TOutputImageType::PixelType operator()(const typename TXsImageType::PixelType& xsPixel,
                                                    const TInternalPrecision& smoothPanchroPixel,
                                                    const typename TPanImageType::PixelType& sharpPanchroPixel) const
    {
      // Build output pixel
      typename TOutputImageType::PixelType output(xsPixel.Size());

      TInternalPrecision scale = 1.;

      if(vcl_abs(smoothPanchroPixel) > 1e-10)
        {
        scale = sharpPanchroPixel/smoothPanchroPixel;
        }

      // Perform fusion for each band with appropriate casting
      for(unsigned int i = 0; i < xsPixel.Size(); ++i)
        {
        output[i] = static_cast<typename TOutputImageType::InternalPixelType>(
          xsPixel[i] * scale);
        }
      // Returns the output pixel
      return output;
    }
  };

  /**
   *  Typedef of the TernaryFunctorImageFilter applying the fusion functor to
   *  p, p_smooth and xs.
   */
  typedef itk::TernaryFunctorImageFilter<TXsImageType,
                                         InternalImageType,
                                         TPanImageType,
                                         TOutputImageType,
                                         FusionFunctor>     FusionFilterType;

  /** Typedef of the convolution filter performing smoothing */
  typedef otb::ConvolutionImageFilter
      <TPanImageType,
       InternalImageType,
       itk::ZeroFluxNeumannBoundaryCondition<TPanImageType>,
       TInternalPrecision>                                  ConvolutionFilterType;

  /** Pointer to the internal convolution filter */
  typename ConvolutionFilterType::Pointer m_ConvolutionFilter;

  /** Pointer to the fusion filter */
  typename FusionFilterType::Pointer      m_FusionFilter;

  /** Radius used for the smoothing filter */
  RadiusType m_Radius;

  /** Kernel used for the smoothing filter */
  ArrayType  m_Filter;

  /** The internal progress accumulator */
  typename itk::ProgressAccumulator::Pointer m_ProgressAccumulator;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleRcsPanSharpeningFusionImageFilter.txx"
#endif

#endif

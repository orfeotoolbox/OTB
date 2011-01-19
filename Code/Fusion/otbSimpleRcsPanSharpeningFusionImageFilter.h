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

#include "itkImageToImageFilter.h"
#include "otbConvolutionImageFilter.h"
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
 *
 * \example Fusion/PanSharpeningExample.cxx
 *
 **/

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision = float>
class ITK_EXPORT SimpleRcsPanSharpeningFusionImageFilter :
  public itk::ImageToImageFilter<TXsImageType, TOutputImageType>
{
public:
  typedef SimpleRcsPanSharpeningFusionImageFilter Self;
  typedef itk::ImageToImageFilter
  <TXsImageType, TOutputImageType>                Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  typedef otb::Image<TInternalPrecision, 
    TPanImageType::ImageDimension>                InternalImageType;

  class FusionFunctor
  {
  public:
    typename TOutputImageType::PixelType operator()(const typename TXsImageType::PixelType & xsPixel, const TInternalPrecision & smoothPanchroPixel, const typename TPanImageType::PixelType& sharpPanchroPixel)
    {
      // Output pixel
      typename TOutputImageType::PixelType output(xsPixel.Size());

      for(unsigned int i = 0; i < xsPixel.Size(); ++i)
        {
        output[i] = static_cast<typename TOutputImageType::InternalPixelType>(xsPixel[i] *
                                                                              static_cast<TInternalPrecision>(sharpPanchroPixel/smoothPanchroPixel));
        }

      return output;
    }
  };

  typedef itk::TernaryFunctorImageFilter<TXsImageType, InternalImageType, TPanImageType,TOutputImageType, FusionFunctor> FusionFilterType;

    typedef otb::ConvolutionImageFilter
      <TPanImageType, InternalImageType> ConvolutionFilterType;
    typedef typename  ConvolutionFilterType::ArrayType                   ArrayType;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(SimpleRcsPanSharpeningFusionImageFilter,
               itk::ImageToImageFilter);

  /** Display */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  typedef typename InternalImageType::SizeType RadiusType;

  /** Set the filter radius  */
  itkGetMacro(Radius, RadiusType);
  itkSetMacro(Radius, RadiusType);

  /** Set the input filter */
  itkSetMacro(Filter, ArrayType);
  itkGetConstReferenceMacro(Filter, ArrayType);

  virtual void SetPanInput(const TPanImageType * image);
  const TPanImageType * GetPanInput(void) const;

  virtual void SetXsInput(const TXsImageType * path);
  const TXsImageType * GetXsInput(void) const;

protected:

  SimpleRcsPanSharpeningFusionImageFilter();

  void GenerateData();

private:

  SimpleRcsPanSharpeningFusionImageFilter(Self &);   // intentionally not implemented
  void operator =(const Self&);          // intentionally not implemented

  typename ConvolutionFilterType::Pointer m_ConvolutionFilter;
  typename FusionFilterType::Pointer      m_FusionFilter;

  RadiusType m_Radius;
  ArrayType  m_Filter;
  typename itk::ProgressAccumulator::Pointer m_ProgressAccumulator;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleRcsPanSharpeningFusionImageFilter.txx"
#endif

#endif

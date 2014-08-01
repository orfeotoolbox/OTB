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
#ifndef __otbForwardFourierMellinTransformImageFilter_txx
#define __otbForwardFourierMellinTransformImageFilter_txx

#include "otbForwardFourierMellinTransformImageFilter.h"
#include "itkImageDuplicator.h"

namespace otb
{
template <class TPixel, class TInterpol, unsigned int Dimension>
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension>
::ForwardFourierMellinTransformImageFilter()
{

  m_Sigma = 1.0;
  m_OutputSize.Fill(512);
  m_FFTFilter = FourierImageFilterType::New();
  m_Interpolator = InterpolatorType::New();
  m_Transform = LogPolarTransformType::New();
  m_ResampleFilter = ResampleFilterType::New();
  m_ResampleFilter->SetInterpolator(m_Interpolator);
  m_ResampleFilter->SetTransform(m_Transform);
  m_DefaultPixelValue = 0;
}

template <class TPixel, class TInterpol, unsigned int Dimension>
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();

  OutputImagePointer outputPtr = this->GetOutput();

  if (!outputPtr)
    {
    return;
    }
  typename OutputImageType::RegionType largestPossibleRegion;
  typename OutputImageType::IndexType  index;
  index.Fill(0);
  largestPossibleRegion.SetIndex(index);
  largestPossibleRegion.SetSize(m_OutputSize);
  outputPtr->SetLargestPossibleRegion(largestPossibleRegion);
}

template <class TPixel, class TInterpol, unsigned int Dimension>
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension>
::GenerateInputRequestedRegion(void)
{
  ImagePointer input = const_cast<InputImageType*>(this->GetInput());
  input->SetRequestedRegion(input->GetLargestPossibleRegion());
}

template <class TPixel, class TInterpol, unsigned int Dimension>
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension>
::GenerateData()
{
  typename LogPolarTransformType::ParametersType params(4);

  // Compute centre of the transform
  SizeType inputSize = this->GetInput()->GetLargestPossibleRegion().GetSize();
  SpacingType inputSpacing = this->GetInput()->GetSpacing();
  itk::ContinuousIndex<double,2> centre;
  centre[0] = -0.5 + 0.5 * static_cast<double>(inputSize[0]);
  centre[1] = -0.5 + 0.5 * static_cast<double>(inputSize[1]);
  PointType centrePt;
  this->GetInput()->TransformContinuousIndexToPhysicalPoint(centre,centrePt);

  // Compute physical radius in the input image
  double radius = vcl_log(vcl_sqrt(
    vcl_pow(static_cast<double>(inputSize[0])*inputSpacing[0],2.0) +
    vcl_pow(static_cast<double>(inputSize[1])*inputSpacing[1],2.0)) / 2.0);

  params[0] = centrePt[0];
  params[1] = centrePt[1];
  params[2] = 360. / m_OutputSize[0];
  params[3] = radius  / m_OutputSize[1];
  m_Transform->SetParameters(params);

  // Compute rho scaling parameter in index space
  double rhoScaleIndex = vcl_log(vcl_sqrt(
    vcl_pow(static_cast<double>(inputSize[0]),2.0) +
    vcl_pow(static_cast<double>(inputSize[1]),2.0)) / 2.0) / m_OutputSize[1];

  // log polar resampling
  m_ResampleFilter->SetInput(this->GetInput());
  m_ResampleFilter->SetDefaultPixelValue(m_DefaultPixelValue);
  m_ResampleFilter->SetSize(m_OutputSize);

  PointType outOrigin;
  outOrigin.Fill(0.5);
  m_ResampleFilter->SetOutputOrigin(outOrigin);
  SpacingType outSpacing;
  outSpacing.Fill(1.0);
  m_ResampleFilter->SetOutputSpacing(outSpacing);

  m_ResampleFilter->Update();

  typename InputImageType::Pointer tempImage = m_ResampleFilter->GetOutput();
  IteratorType iter(tempImage, tempImage->GetRequestedRegion());

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const PixelType minOutputValue =  itk::NumericTraits<PixelType>::NonpositiveMin();
  const PixelType maxOutputValue =  itk::NumericTraits<PixelType>::max();

  // Normalization is specific to FourierMellin convergence conditions, and
  // thus should be implemented here instead of in the resample filter.
  for (iter.GoToBegin(); !iter.IsAtEnd(); ++iter)
    {
    // 0.5 shift in order to follow output image physical space
    double    Rho   = (0.5 + static_cast<double>(iter.GetIndex()[1])) * rhoScaleIndex;
    PixelType pixval;
    double    valueTemp = static_cast<double>(iter.Get());
    valueTemp *= vcl_exp(m_Sigma * Rho);
    valueTemp *= rhoScaleIndex;
    PixelType value = static_cast<PixelType>(valueTemp);

    if (value < minOutputValue)
      {
      pixval = minOutputValue;
      }
    else if (value > maxOutputValue)
      {
      pixval = maxOutputValue;
      }
    else
      {
      pixval = static_cast<PixelType>(value);
      }
    iter.Set(pixval);
    }
  m_FFTFilter->SetInput(tempImage);

  m_FFTFilter->GraftOutput(this->GetOutput());
  m_FFTFilter->Update();
  this->GraftOutput(m_FFTFilter->GetOutput());
}
/**
 * Standard "PrintSelf" method
 */
template <class TPixel, class TInterpol, unsigned int Dimension>
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif

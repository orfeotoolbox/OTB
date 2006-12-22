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
#ifndef _otbForwardFourierMellinTransformImageFilter_txx
#define _otbForwardFourierMellinTransformImageFilter_txx

#include "otbForwardFourierMellinTransformImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbLogPolarTransform.h"
#include "itkResampleImageFilter.h"

namespace otb
{
  template < class TPixel,class  TInterpol,unsigned int   Dimension >
  ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
  ::ForwardFourierMellinTransformImageFilter()
  {
    m_FourierTransform = FourierImageFilterType::New();
    m_Interpolator = InterpolatorType::New();
    m_Sigma = 1.0;
    m_OutputSize.Fill(512); 
  }
  template < class TPixel,class  TInterpol,unsigned int   Dimension >
  void
  ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
  ::GenerateData()
  {
    // some typedefs
    typedef otb::LogPolarTransform<double> LogPolarTransformType;
    typedef itk::ResampleImageFilter<InputImageType,InputImageType,double> ResampleFilterType;
    typedef itk::ImageRegionIteratorWithIndex<InputImageType> IteratorType;

    typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    typename LogPolarTransformType::Pointer transform  = LogPolarTransformType::New();
    
    typename LogPolarTransformType::ParametersType params(4);
    // Center the transform
    params[0]=0.5*static_cast<double>(this->GetInput()->GetLargestPossibleRegion().GetSize()[0]);
    params[1]=0.5*static_cast<double>(this->GetInput()->GetLargestPossibleRegion().GetSize()[1]);
    params[2]=360./m_OutputSize[0];
    params[3]=log(sqrt(pow(this->GetInput()->GetLargestPossibleRegion().GetSize()[0],2)
		       +pow(this->GetInput()->GetLargestPossibleRegion().GetSize()[1],2))/2)/m_OutputSize[1];
  transform->SetParameters(params);
  
  // log polar resampling
  resampler->SetInput(this->GetInput());
  resampler->SetTransform(transform);
  resampler->SetInterpolator(m_Interpolator);
  resampler->SetDefaultPixelValue(m_DefaultPixelValue);
  resampler->SetSize(m_OutputSize);
  resampler->Update();

  typename InputImageType::Pointer tempImage = resampler->GetOutput();
  IteratorType it(tempImage,tempImage->GetLargestPossibleRegion());

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const PixelType minOutputValue =  itk::NumericTraits<PixelType >::NonpositiveMin();
  const PixelType maxOutputValue =  itk::NumericTraits<PixelType >::max();

  // Normalization is specific to FourierMellin convergence conditions, and
  // thus should be implemented here instead of in the resample filter.
  for(it.GoToBegin();!it.IsAtEnd();++it)
  {
    double Rho   = it.GetIndex()[1]*params[3];
    PixelType pixval;
    double valueTemp = static_cast<double>(it.Get());
    valueTemp *= exp(m_Sigma * Rho);
    valueTemp *=params[3];
    PixelType value = static_cast<PixelType>(valueTemp);
	    
    if( value < minOutputValue )
      {
	pixval = minOutputValue;
      }
    else if( value > maxOutputValue )
      {
	pixval = maxOutputValue;
      }
    else 
      {
	pixval = static_cast<PixelType>(value);
      }
    it.Set(pixval);      
  }
  // Fourrier transform of the output
  m_FourierTransform->SetInput(tempImage );
  m_FourierTransform->GraftOutput( this->GetOutput() );

  m_FourierTransform->Update();
  this->GraftOutput( m_FourierTransform->GetOutput() );
}
/**
 * Standard "PrintSelf" method
 */
template < class TPixel,class  TInterpol,unsigned int   Dimension >
void 
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}
} // end namespace otb
#endif

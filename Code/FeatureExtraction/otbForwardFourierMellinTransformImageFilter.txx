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


namespace otb
{

template < class TPixel,class  TInterpol,unsigned int   Dimension >
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::ForwardFourierMellinTransformImageFilter()
{
  m_LogPolarResample = LogPolarResampleImageFilterType::New();  
  m_FourierTransform = FourierImageFilterType::New();
}

template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GenerateData()
{
	m_LogPolarResample->SetInput( this->GetInput() );
 	m_FourierTransform->SetInput( m_LogPolarResample->GetOutput() );
 	m_FourierTransform->GraftOutput( this->GetOutput() );

	m_FourierTransform->Update();
 	this->GraftOutput( m_FourierTransform->GetOutput() );
}


/** 
 * Inform pipeline of required output region
 */
template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  ImagePointer inputPtr  =  const_cast<InputImageType *>( this->GetInput() );
  OutputImagePointer   outputPtr = this->GetOutput();;
  if ( !inputPtr )
    {
    return;
    }

  m_LogPolarResample->GenerateOutputInformation();
    
  OutputImageRegionType    outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( m_LogPolarResample->GetOutput()->GetLargestPossibleRegion().GetSize() );
  outputLargestPossibleRegion.SetIndex( m_LogPolarResample->GetOutput()->GetLargestPossibleRegion().GetIndex() );
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  return;
}



template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetAngularStep(double angularStep )
{
	m_LogPolarResample->SetAngularStep(angularStep);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const double
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetAngularStep()
{
	return static_cast<const double>(m_LogPolarResample->GetAngularStep() );
}


template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetRadialStep(double radialStep )
{
	m_LogPolarResample->GetRadialStep(radialStep);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const double
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetRadialStep( )
{
	return m_LogPolarResample->GetRadialStep();
}

template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetAngularNumberOfSamples(double angularNumberOfSamples )
{
	m_LogPolarResample->SetAngularNumberOfSamples(angularNumberOfSamples);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const double
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetAngularNumberOfSamples()
{
	return m_LogPolarResample->GetAngularNumberOfSamples();
}


template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetRadialNumberOfSamples(double radialNumberOfSamples )
{
	m_LogPolarResample->SetRadialNumberOfSamples(radialNumberOfSamples);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const double
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetRadialNumberOfSamples( )
{
	return static_cast<const double>(m_LogPolarResample->GetRadialNumberOfSamples() );
}

template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetSigma(double sigma )
{
	m_LogPolarResample->SetSimga(sigma);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const double
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetSigma( )
{
	return m_LogPolarResample->GetSigma();
}

template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetOriginIsAtCenter(bool isAtCenter)
{
	m_LogPolarResample->SetOriginIsAtCenter(isAtCenter);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
const bool
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetOriginIsAtCenter()
{
	return m_LogPolarResample->GetOriginIsAtCenter();
}


template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetDefaultPixelValue(PixelType pixelValue)
{
	m_LogPolarResample->SetDefaultPixelValue(pixelValue);
}


template < class TPixel,class  TInterpol,unsigned int   Dimension >
const TPixel
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetDefaultPixelValue( )
{
	return static_cast<const PixelType>(m_LogPolarResample->GetDefaultPixelValue());
}


template < class TPixel,class  TInterpol,unsigned int   Dimension >
void
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::SetInterpolator(InterpolatorPointer interpolator )
{
	m_LogPolarResample->SetInterpolator(interpolator);
}
template < class TPixel,class  TInterpol,unsigned int   Dimension >
typename TInterpol::ConstPointer 
ForwardFourierMellinTransformImageFilter<TPixel, TInterpol, Dimension >
::GetInterpolator( )
{
	return static_cast<InterpolatorConstPointer>(m_LogPolarResample->GetInterpolator());
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
  os << indent << "m_LogPolarResample : " << m_LogPolarResample.GetPointer() << std::endl;
}


} // end namespace otb


#endif

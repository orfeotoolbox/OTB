/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbTwoNRIBandsImageToNComplexBandsImage_txx
#define otbTwoNRIBandsImageToNComplexBandsImage_txx

#include "otbTwoNRIBandsImageToNComplexBandsImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkVariableLengthVector.h"
#include <limits>



namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>::TwoNRIBandsImageToNComplexBandsImage()
{
  m_LowestB = std::numeric_limits < OutputRealType > ::lowest();
  m_HighestB = std::numeric_limits < OutputRealType > ::max();
  m_LowestBD = static_cast < double > ( m_LowestB );
  m_HighestBD = static_cast < double > ( m_HighestB );
}

/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  
  unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
  
 //  if ( (nbCompo % 2) != 0 )
 //  {
	// itkExceptionMacro("Number of bands of the input images must be an even number");
 //  }
 //  else
	this->GetOutput()->SetNumberOfComponentsPerPixel( ( nbCompo + 1 ) / 2 );  
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
	// unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
			
	// if ( (nbCompo % 2) != 0 )
	//   itkExceptionMacro("Number of bands of the input images must be an even number");
		
}

/**
 * ThreadedGenerateData
 */
template<class TInputImage, class TOutputImage>
void TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
 
  unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();
  
  itk::VariableLengthVector < std::complex < OutputRealType > > 
    vlv( ( nbCompo+1 ) / 2 );
  
 
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());	

  auto inIt = itk::ImageRegionConstIterator < InputImageType > (
    this->GetInput() , outputRegionForThread );	
	
  auto outIt = itk::ImageRegionIterator < OutputImageType > (
    this->GetOutput() , outputRegionForThread );
  

  inIt.GoToBegin();
  outIt.GoToBegin();
  OutputRealType reOut , imOut ;
  InputPixelType reIn , imIn ;
  while (!outIt.IsAtEnd())
  {
	  for (unsigned int i = 0 ; i < nbCompo / 2 ; ++i )
	  { 
      reIn = inIt.Get()[ 2 * i ];
      imIn = inIt.Get()[ 2 * i + 1 ];
      reOut = InBoundValue( reIn );
      imOut = InBoundValue( imIn );
		  vlv[i] = std::complex < OutputRealType > ( reOut , imOut );
	  }
    if ( nbCompo % 2 == 1 )
      {
      reIn = inIt.Get()[ nbCompo - 1 ];
      reOut = InBoundValue( reIn );
      vlv[ ( nbCompo + 1 ) / 2 ] = 
        std::complex < OutputRealType > ( reOut , 0 );
      }

	  outIt.Set(vlv);

	  ++inIt;
	  ++outIt;

	  progress.CompletedPixel();
  }
  
}

template <class TInputImage, class TOutput>
typename TwoNRIBandsImageToNComplexBandsImage < TInputImage , TOutput > 
::OutputRealType
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutput>
::InBoundValue( InputPixelType in ) const
{
  OutputRealType out ;
  if ( static_cast < double > ( in ) >= m_HighestBD )
    out = m_HighestB;
  else
    out = static_cast < OutputRealType > ( in ) ;
  if ( static_cast < double > ( in ) <= m_LowestBD )
    out = m_LowestB;
  else
    out = static_cast < OutputRealType > ( in ) ;
  return out ;
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif

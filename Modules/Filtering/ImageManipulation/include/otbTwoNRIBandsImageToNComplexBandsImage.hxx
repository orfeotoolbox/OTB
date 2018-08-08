/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTwoNRIBandsImageToNComplexBandsImage_hxx
#define otbTwoNRIBandsImageToNComplexBandsImage_hxx

#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;
#include "otbTwoNRIBandsImageToNComplexBandsImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkVariableLengthVector.h"




namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>::TwoNRIBandsImageToNComplexBandsImage()
{
  OTB_DISABLE_DYNAMIC_MT;
	//this->SetNumberOfThreads(1);
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

  if ( (nbCompo % 2) != 0 )
    {
    itkExceptionMacro("Number of bands of the input images must be an even number");
    }
  else
    {
    this->GetOutput()->SetNumberOfComponentsPerPixel(nbCompo/2);
    }
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
TwoNRIBandsImageToNComplexBandsImage<TInputImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
	unsigned int nbCompo = this->GetInput()->GetNumberOfComponentsPerPixel();

	if ( (nbCompo % 2) != 0 )
	  itkExceptionMacro("Number of bands of the input images must be an even number");

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
  
  itk::VariableLengthVector< std::complex< typename InputPixelType::ValueType > > vlv(nbCompo/2);
  
 
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());	

  itk::ImageRegionConstIterator<TInputImage>  inIt;
  inIt = itk::ImageRegionConstIterator<TInputImage>(this->GetInput(), outputRegionForThread);	
	
  itk::ImageRegionIterator<OutputImageType>      outIt;
  outIt = itk::ImageRegionIterator<OutputImageType>(this->GetOutput(), outputRegionForThread);
  

  inIt.GoToBegin();
  outIt.GoToBegin();

  while (!outIt.IsAtEnd())
  {
	  
	  unsigned int k=0;
	  for (unsigned int i=0; i<nbCompo-1; i=i+2)
	  {
		  vlv[k] = std::complex< typename InputPixelType::ValueType >(inIt.Get()[i],inIt.Get()[i+1]);
		  k++;
	  }

	  
	  outIt.Set(vlv);
	  

	  ++inIt;
	  ++outIt;
	  
	  progress.CompletedPixel();
  }
  
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

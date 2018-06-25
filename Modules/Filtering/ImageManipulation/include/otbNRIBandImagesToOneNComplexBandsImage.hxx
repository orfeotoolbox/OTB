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

#ifndef otbNRIBandImagesToOneNComplexBandsImage_hxx
#define otbNRIBandImagesToOneNComplexBandsImage_hxx

#include "otbNRIBandImagesToOneNComplexBandsImage.h"

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
NRIBandImagesToOneNComplexBandsImage<TInputImage, TOutputImage>::NRIBandImagesToOneNComplexBandsImage()
{
	//this->SetNumberOfThreads(1);
}

/**
 * GenerateOutputInformation
 */
template<class TInputImage, class TOutputImage>
void
NRIBandImagesToOneNComplexBandsImage<TInputImage, TOutputImage>
::GenerateOutputInformation(void)
{
  Superclass::GenerateOutputInformation();
  
  unsigned int nbInputs = this->GetNumberOfInputs();
  this->GetOutput()->SetNumberOfComponentsPerPixel(nbInputs);
  
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
NRIBandImagesToOneNComplexBandsImage<TInputImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
	unsigned int nbInputs = this->GetNumberOfInputs();
	
	for (unsigned int i=0; i<nbInputs; i++)
		if (this->GetInput(i)->GetNumberOfComponentsPerPixel() != 2 )
			itkExceptionMacro("Input images must be made of two bands and only two (see input #" << i << ").");
		
}

/**
 * ThreadedGenerateData
 */
template<class TInputImage, class TOutputImage>
void NRIBandImagesToOneNComplexBandsImage<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
 
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());	
	
  unsigned int nbInputs = this->GetNumberOfInputs();	  
  
  itk::VariableLengthVector< std::complex< typename InputPixelType::ValueType > > vlv(nbInputs);

  std::vector< typename itk::ImageRegionConstIterator<TInputImage> > vInIt;
  for (unsigned int i=0; i<nbInputs; i++)
	vInIt.push_back( itk::ImageRegionConstIterator<TInputImage>(this->GetInput(i), outputRegionForThread) );

	
  itk::ImageRegionIterator<OutputImageType>      outIt;
  outIt = itk::ImageRegionIterator<OutputImageType>(this->GetOutput(), outputRegionForThread);
  
  for (unsigned int i=0; i<nbInputs; i++)
	vInIt[i].GoToBegin();
  outIt.GoToBegin();

  while (!outIt.IsAtEnd())
  {
	  
	  
	  for (unsigned int i=0; i<nbInputs; i++)
	  {
		  vlv[i] = std::complex< typename InputPixelType::ValueType >(vInIt[i].Get()[0],vInIt[i].Get()[1]);
		//std::cout << "i = " << i << " " << vInIt[i].Get()[0] << " " << vInIt[i].Get()[1] << std::endl;
	  }

	  
	  outIt.Set(vlv);
	  
	  //std::cout << "outIt.Get() = " << outIt.Get() << std::endl;
	  
	  for (unsigned int i=0; i<nbInputs; i++)
		++vInIt[i];
	  ++outIt;
	  
	  progress.CompletedPixel();
  }
  
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
NRIBandImagesToOneNComplexBandsImage<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif

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

#ifndef otbOneRIBandImageToOneComplexBandImage_hxx
#define otbOneRIBandImageToOneComplexBandImage_hxx

#include "otbOneRIBandImageToOneComplexBandImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
OneRIBandImageToOneComplexBandImage<TInputImage, TOutputImage>::OneRIBandImageToOneComplexBandImage()
{
  //this->SetNumberOfThreads(1);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage>
void
OneRIBandImageToOneComplexBandImage<TInputImage, TOutputImage>
::BeforeThreadedGenerateData(void)
{
	if (this->GetInput()->GetNumberOfComponentsPerPixel() != 2 )
			itkExceptionMacro("Input image must be made of two bands and only two.");
		
}

/**
 * ThreadedGenerateData
 */
template<class TInputImage, class TOutputImage>
void OneRIBandImageToOneComplexBandImage<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
	
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());	
	
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
  
  itk::ImageRegionIterator<OutputImageType>      it;
   itk::ImageRegionConstIterator<TInputImage>      itIn;
  
  itIn = itk::ImageRegionConstIterator<TInputImage>(input, outputRegionForThread);
  it = itk::ImageRegionIterator<OutputImageType>(output, outputRegionForThread);
  
  it.GoToBegin();
  itIn.GoToBegin();
  while (!it.IsAtEnd())
  {
	  
	  it.Set(static_cast<OutputPixelType>(   std::complex< typename InputPixelType::ValueType >(itIn.Get()[0],itIn.Get()[1])   ));
	  
	  ++it;
	  ++itIn;
	  progress.CompletedPixel();
  }
  
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
OneRIBandImageToOneComplexBandImage<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif

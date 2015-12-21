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
#ifndef __otbOneRIBandImageToOneComplexBandImage_txx
#define __otbOneRIBandImageToOneComplexBandImage_txx

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

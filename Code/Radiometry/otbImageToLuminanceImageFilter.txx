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
#include "otbImageToLuminanceImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
  
  template <class TInputImage, class TOutputImage>
  ImageToLuminanceImageFilter<TInputImage, TOutputImage>
  ::ImageToLuminanceImageFilter()
  {
    m_Alpha.Fill(0);
    m_Beta.Fill(0);
  }
  
  
  template <class TInputImage, class TOutputImage>
  ImageToLuminanceImageFilter<TInputImage, TOutputImage>
  ::~ImageToLuminanceImageFilter()
  {
  }
  
  /**
   * ThreadedGenerateData Performs the pixel-wise addition
   */
  template <class TInputImage, class TOutputImage>
  void
  ImageToLuminanceImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread, int threadId )
  {
    typename InputImageType::Pointer  inputPtr  = const_cast<InputImageType *>(this->GetInput());
    typename OutputImageType::Pointer outputPtr = static_cast<OutputImageType *>(this->GetOutput());
  
    // Define the portion of the input to walk for this thread, using
    // the CallCopyOutputRegionToInputRegion method allows for the input
    // and output images to be different dimensions
    
    InputImageRegionType inputRegionForThread;
    this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
    
    // Define the iterators
    itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
    itk::ImageRegionIterator<OutputImageType>      outputIt(outputPtr, outputRegionForThread);
    
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
  
    while( !inputIt.IsAtEnd() ) 
      {
	InputPixelType inPixel = inputIt.Get();
	OutputPixelType outPixel = outputIt.Get();
	
	for (unsigned int j=0; j<inPixel.GetSize(); j++)
	  {
	    this->GetFunctor().SetAlpha(static_cast<double>(m_Alpha[j]));
	    this->GetFunctor().SetBeta(static_cast<double>(m_Beta[j]));
	    outPixel[j] = this->GetFunctor()( inPixel[j] );
	  }	
	outputIt.Set(outPixel);
	++inputIt;
	++outputIt;
	progress.CompletedPixel();  // potential exception thrown here    
      }
    
  }
} // end namespace otb

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
#ifndef __otbUnaryImageFunctorWithVectorImageFilter_txx
#define __otbUnaryImageFunctorWithVectorImageFilter_txx

#include "otbUnaryImageFunctorWithVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
UnaryImageFunctorWithVectorImageFilter<TInputImage,TOutputImage,TFunction>
::UnaryImageFunctorWithVectorImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();
}

/** 
 * UnaryImageFunctorWithVectorImageFilter can produce an image which is a different resolution
 * than its input image. As such, UnaryImageFunctorWithVectorImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model. The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformation() 
 */
template <class TInputImage, class TOutputImage, class TFunction>
void 
UnaryImageFunctorWithVectorImageFilter<TInputImage,TOutputImage,TFunction>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  // // do not call the superclass' implementation of this method since
//   // this filter allows the input the output to be of different dimensions
 
  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }
    outputPtr->SetNumberOfComponentsPerPixel( // propagate vector length info
        inputPtr->GetNumberOfComponentsPerPixel());

}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryImageFunctorWithVectorImageFilter<TInputImage,TOutputImage,TFunction>
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

  // Null piuxel construction
  InputPixelType nullPixel;
  nullPixel.SetSize( outputIt.Get().GetSize() );
  nullPixel.Fill(itk::NumericTraits<OutputInternalPixelType>::Zero);


  while( !inputIt.IsAtEnd() ) 
    {
      InputPixelType inPixel = inputIt.Get();
      OutputPixelType outPixel = nullPixel;//outputIt.Get();
      // if the input pixel in null, the output is considered as null ( no sensor informations )
      if( inPixel!= nullPixel)
	{      
      for (unsigned int j=0; j<inPixel.GetSize(); j++)
	{
	  outPixel[j] = m_FunctorVector[j]( inPixel[j] );
	}	
	}
      outputIt.Set(outPixel);
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel();  // potential exception thrown here    
    }
}

template <class TInputImage, class TOutputImage, class TFunction>
void
UnaryImageFunctorWithVectorImageFilter<TInputImage,TOutputImage,TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif

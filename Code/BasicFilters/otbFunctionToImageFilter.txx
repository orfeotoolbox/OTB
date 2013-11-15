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
#ifndef __otbFunctionToImageFilter_txx
#define __otbFunctionToImageFilter_txx

#include "otbFunctionToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "otbImageFileWriter.h"

namespace otb
{

/**
 * Constructor
 */
template<class TInputImage, class TOutputImage, class TFunction>
FunctionToImageFilter<TInputImage, TOutputImage, TFunction>
::FunctionToImageFilter()
{
  this->InPlaceOff();
  m_PixelFunction = FunctionType::New();
}

/**
 * Connect one of the operands for pixel-wise addition
 */
// template<class TInputImage, class TOutputImage, class TFunction >
// void
// FunctionToImageFilter<TInputImage, TOutputImage, TFunction>
// ::SetInput( const TInputImage *image )
// {
//   // The ProcessObject is not const-correct so the const_cast is required here
//   SetNthInput( 0, const_cast<TInputImage *>( image ) );
// }

/**
 * BeforeThreadedGenerateData function. Validate inputs
 */
template<class TInputImage, class TOutputImage, class TFunction>
void
FunctionToImageFilter<TInputImage, TOutputImage, TFunction>
::BeforeThreadedGenerateData()
{
  InputImagePointer inputPtr
    = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));
  if (inputPtr.IsNull())
    {
    itkExceptionMacro(<< "At least one input is missing."
                      << " Input is missing :" << inputPtr.GetPointer(); )

    }
  m_PixelFunction->SetInputImage(inputPtr);
}

/**
 * ThreadedGenerateData function. Performs the pixel-wise addition
 */
template<class TInputImage, class TOutputImage, class TFunction>
void
FunctionToImageFilter<TInputImage, TOutputImage, TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{

  // We use dynamic_cast since inputs are stored as DataObjects.
  InputImagePointer inputPtr
    = dynamic_cast<const TInputImage*>((itk::ProcessObject::GetInput(0)));

  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImage> inputIt(inputPtr, outputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();


  while (!inputIt.IsAtEnd())
    {
      outputIt.Set(static_cast<OutputImagePixelType>(m_PixelFunction->EvaluateAtIndex(inputIt.GetIndex())));
      ++inputIt;
      ++outputIt;
      progress.CompletedPixel(); // potential exception thrown here
    }
}
} // end namespace otb

#endif

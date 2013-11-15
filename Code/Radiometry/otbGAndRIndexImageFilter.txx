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
#ifndef __otbGAndRIndexImageFilter_txx
#define __otbGAndRIndexImageFilter_txx

#include "otbGAndRIndexImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageG, class TInputImageR,
    class TOutputImage, class TFunction>
GAndRIndexImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction>
::GAndRIndexImageFilter()
{
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageG, class TInputImageR,
    class TOutputImage, class TFunction>
void
GAndRIndexImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction>
::SetInputG(const TInputImageG * image1)
{
  // Process object is not const-correct so the const casting is required.
  this->SetInput1(image1);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageG, class TInputImageR,
    class TOutputImage, class TFunction>
void
GAndRIndexImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction>
::SetInputR(const TInputImageR * image2)
{
  // Process object is not const-correct so the const casting is required.
//  SetNthInput(1, const_cast<TInputImageR *>( image2 ));
  this->SetInput2(image2);
}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
/*
template <class TInputImageG, class TInputImageR, class TOutputImage, class TFunction  >
void
GAndRIndexImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        itk::ThreadIdType threadId)
{

  // We use dynamic_cast since inputs are stored as DataObjects.  The
  // ImageToImageFilter::GetInput(int) always returns a pointer to a
  // TInputImageG so it cannot be used for the second input.
  RInputImagePointer inputPtr1
    = dynamic_cast<const TInputImageG*>(itk::ProcessObject::GetInput(0));
  NIRInputImagePointer inputPtr2
    = dynamic_cast<const TInputImageR*>(itk::ProcessObject::GetInput(1));
  OutputImagePointer outputPtr = this->GetOutput(0);

  itk::ImageRegionConstIterator<TInputImageG> inputItR(inputPtr1, outputRegionForThread);
  itk::ImageRegionConstIterator<TInputImageR> inputItNIR(inputPtr2, outputRegionForThread);

  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputItR.GoToBegin();
  inputItNIR.GoToBegin();
  outputIt.GoToBegin();

  while( !inputItR.IsAtEnd() )
    {
    outputIt.Set( this->GetFunctor()( inputItR.Get(), inputItNIR.Get() ) );
    ++inputItNIR;
    ++inputItR;
    ++outputIt;
    progress.CompletedPixel(); // potential exception thrown here
    }

}
*/
template <class TInputImageG, class TInputImageR, class TOutputImage, class TFunction>
void
GAndRIndexImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif

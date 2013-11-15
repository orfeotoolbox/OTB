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
#ifndef __otbVectorImageToIntensityImageFilter_txx
#define __otbVectorImageToIntensityImageFilter_txx

#include "otbVectorImageToIntensityImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::VectorImageToIntensityImageFilter()
{

}

template <class TInputImage, class TOutputImage>
void
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  InputImageConstPointerType inputPtr = this->GetInput();
  OutputImagePointerType     outputPtr = this->GetOutput();

  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<InputImageType>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  while (!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
    {
    double         sum = 0.0;
    InputPixelType pixel = inputIt.Get();
    for (unsigned int i = 0; i < pixel.Size(); ++i)
      {
      sum += pixel[i];
      }
    sum /= pixel.Size();

    outputIt.Set(static_cast<OutputPixelType>(sum));
    ++inputIt;
    ++outputIt;
    progress.CompletedPixel();  // potential exception thrown here
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
VectorImageToIntensityImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif

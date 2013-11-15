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
#ifndef __otbExtractROI_txx
#define __otbExtractROI_txx

#include "otbExtractROI.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputPixel, class TOutputPixel>
ExtractROI<TInputPixel, TOutputPixel>
::ExtractROI() //: ExtractROIBase< itk::Image<TInputPixel, VImageDimension> , itk::Image<TOutputPixel, VImageDimension> >()
{
}

/**
 *
 */
template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

/**
 * ExtractROI can produce an image which is a different resolution
 * than its input image.  As such, ExtractROI needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton()
 */
template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::GenerateOutputInformation()
{
  // Call to the base class method
  Superclass::GenerateOutputInformation();
}

template <class TInputPixel, class TOutputPixel>
void
ExtractROI<TInputPixel, TOutputPixel>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");

  // Get the input and output pointers
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define the portion of the input to walk for this thread
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators.
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIterator;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);
  InputIterator inIt(inputPtr, inputRegionForThread);

  // walk the output region, and sample the input image
  while (!outIt.IsAtEnd())
    {
    // copy the input pixel to the output
    outIt.Set(inIt.Get());
    ++outIt;
    ++inIt;
    progress.CompletedPixel();
    }
}

} // end namespace otb

#endif

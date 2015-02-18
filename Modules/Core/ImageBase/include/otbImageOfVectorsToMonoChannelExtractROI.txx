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
#ifndef __otbImageOfVectorsToMonoChannelExtractROI_txx
#define __otbImageOfVectorsToMonoChannelExtractROI_txx

#include "otbImageOfVectorsToMonoChannelExtractROI.h"

namespace otb
{

/**
 *
 */
template<class TInputImage, class TOutputImage>
ImageOfVectorsToMonoChannelExtractROI<TInputImage, TOutputImage>
::ImageOfVectorsToMonoChannelExtractROI() : m_Channel(1)
{

}

/**
 *
 */
template<class TInputImage, class TOutputImage>
void
ImageOfVectorsToMonoChannelExtractROI<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

/**
 * ExtractImageFilter can produce an image which is a different resolution
 * than its input image.  As such, ExtractImageFilter needs to provide an
 * implementation for GenerateOutputInformation() in order to inform
 * the pipeline execution model.  The original documentation of this
 * method is below.
 *
 * \sa ProcessObject::GenerateOutputInformaton()
 */
template<class TInputImage, class TOutputImage>
void
ImageOfVectorsToMonoChannelExtractROI<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  // Analysis of processed channel
  if ((m_Channel <= 0) || (m_Channel > InputImagePixelType::Dimension))
    {
    itkExceptionMacro(<< "otb::ExtractImImageOfVectorsToMonoChannelExtractROIageFilter::GenerateOutputInformation "
                      << " Channel must be in the following range: [1;" << InputImagePixelType::Dimension << "] "
                      << typeid(itk::ImageBase<InputImageDimension>*).name());
    }

  // Call base class implementation
  Superclass::GenerateOutputInformation();
}

template<class TInputImage, class TOutputImage>
void
ImageOfVectorsToMonoChannelExtractROI<TInputImage, TOutputImage>
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

  // Loop through the processed channels
  unsigned int channelIn(m_Channel - 1);

  InputImagePixelType pixelInput;
  while (!outIt.IsAtEnd())
    {
    OutputImagePixelType pixelOutput;
    pixelInput = inIt.Get();
    pixelOutput = static_cast<OutputValueType>(pixelInput[channelIn]);
    outIt.Set(pixelOutput);
    ++outIt;
    ++inIt;
    progress.CompletedPixel();
    }

}

} // end namespace otb

#endif

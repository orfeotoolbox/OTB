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

#ifndef otbConcatenateVectorImageFilter_hxx
#define otbConcatenateVectorImageFilter_hxx

#include "otbConcatenateVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::ConcatenateVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
}
/**
 * Destructor.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::~ConcatenateVectorImageFilter()
{}
/**
 * Set The first input image.
 * \param image The first input image.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::SetInput1(const TInputImage1 * image)
{
  this->SetNthInput(0, const_cast<TInputImage1 *>(image));
}
/**
 * Set The second input image.
 * \param image The second input image.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::SetInput2(const TInputImage2 * image)
{
  this->SetNthInput(1, const_cast<TInputImage2 *>(image));
}
/**
 * Get the first input image.
 * \return The first input image.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
typename ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::InputImage1Type *
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::GetInput1(void)
{
  return const_cast<InputImage1Type *>(this->GetInput(0));
}
/**
 * Get the second input image.
 * \return The second input image.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
typename ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::InputImage2Type *
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::GetInput2(void)
{
  return const_cast<InputImage2Type *>(this->GetInput(1));
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::GenerateOutputInformation()
{
  // Call to the superclass implementation
  Superclass::GenerateOutputInformation();

  typename Superclass::InputImageConstPointer inputPtr1 = this->GetInput1();
  typename Superclass::InputImageConstPointer inputPtr2 = this->GetInput2();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  unsigned int nbComponentsPerPixel = inputPtr1->GetNumberOfComponentsPerPixel() +
                                      inputPtr2->GetNumberOfComponentsPerPixel();

  // initialize the number of channels of the output image
  outputPtr->SetNumberOfComponentsPerPixel(nbComponentsPerPixel);
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  typename Superclass::InputImageConstPointer inputPtr1 = this->GetInput1();
  typename Superclass::InputImageConstPointer inputPtr2 = this->GetInput2();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  if (inputPtr1->GetLargestPossibleRegion() != inputPtr2->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<< "InputImage1 and InputImage2 have different requested regions.");
    }
}

/**
 * Main computation method.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  // retrieves inputs and output pointer
  InputImage1PointerType input1 = this->GetInput1();
  InputImage2PointerType input2 = this->GetInput2();
  OutputImagePointerType output = this->GetOutput();

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define the portion of the input to walk for this thread
  typename InputImage1Type::RegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Iterators typedefs
  typedef itk::ImageRegionIterator<InputImage1Type> Input1IteratorType;
  typedef itk::ImageRegionIterator<InputImage2Type> Input2IteratorType;
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

  // Iterators declaration
  Input1IteratorType input1It(input1, inputRegionForThread);
  Input2IteratorType input2It(input2, inputRegionForThread);
  OutputIteratorType outputIt(output, outputRegionForThread);

  input1It.GoToBegin();
  input2It.GoToBegin();
  outputIt.GoToBegin();

  typename OutputImageType::PixelType outputPix(outputIt.Get().GetSize());
  // Retrieve the size of each input pixel
  const unsigned int l1 = input1It.Get().GetSize();
  const unsigned int l2 = input2It.Get().GetSize();

  // Iterate through the pixel
  while (!outputIt.IsAtEnd())
    {

    // Check the size of each input pixel
    assert(l1 == input1It.Get().GetSize());
    assert(l2 == input2It.Get().GetSize());
    // Check the size of the output pixel
    assert(l1 + l2 == outputPix.GetSize());
    // References to the input pixels
    InputPixel1Type const& pix1 = input1It.Get();
    InputPixel2Type const& pix2 = input2It.Get();

    // Loop through each band of the first image
    for (unsigned int i = 0; i < l1; ++i)
      {
      // Fill the output pixel
      outputPix[i] = static_cast<typename OutputImageType::InternalPixelType>(pix1[i]);
      }
    // Loop though each band of the second image
    for (unsigned int i = 0; i < l2; ++i)
      {
      // Fill the output pixel
      outputPix[i + l1] = static_cast<typename OutputImageType::InternalPixelType>(pix2[i]);
      }
    // Set the output pixel
    outputIt.Set(outputPix);
    // Increment the iterator
    ++input1It;
    ++input2It;
    ++outputIt;
    }

}
/**
 * PrintSelf method.
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
void
ConcatenateVectorImageFilter<TInputImage1, TInputImage2, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif

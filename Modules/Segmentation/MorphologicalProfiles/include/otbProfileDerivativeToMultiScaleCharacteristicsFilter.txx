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
#ifndef __otbProfileDerivativeToMultiScaleCharacteristicsFilter_txx
#define __otbProfileDerivativeToMultiScaleCharacteristicsFilter_txx

#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "itkImageRegionIterator.h"
#include <vector>
#include "otbMacro.h"

namespace otb
{
template <class TInputImage, class TOutputImage, class TLabeledImage>
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::ProfileDerivativeToMultiScaleCharacteristicsFilter(void)
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, LabeledImageType::New());
  m_InitialValue = 0;
  m_Step = 1;
}

template <class TInputImage, class TOutputImage, class TLabeledImage>
TLabeledImage *
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::GetOutputCharacteristics(void)
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<LabeledImageType *>
           (this->itk::ProcessObject::GetOutput(1));
}
/**
 * GenerateOutputInformation
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
void
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::GenerateOutputInformation(void)
{
  InputImageListPointerType inputPtr = this->GetInput();
  OutputImagePointerType    outputPtr = this->GetOutput();
  LabeledImagePointerType   outputLabeledPtr = this->GetOutputCharacteristics();
  if (inputPtr->Size() > 0)
    {
    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
    outputPtr->CopyInformation(inputListIt.Get());
    outputLabeledPtr->CopyInformation(inputListIt.Get());
    }
}
/**
 * GenerateInputRequestedRegion
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
void
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::GenerateInputRequestedRegion(void)
{
  InputImageListPointerType                  inputPtr = this->GetInput();
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();

  RegionType region1 = this->GetOutput()->GetRequestedRegion();
  RegionType region2 = this->GetOutputCharacteristics()->GetRequestedRegion();
  RegionType region;
  /**
   * Compute the appropriate requested region:
   * This is the smaller region containing the two output regions
   */
  if (region1 == this->GetOutput()->GetLargestPossibleRegion()
      && region2 != this->GetOutputCharacteristics()->GetLargestPossibleRegion())
    {
    region = region2;
    }
  else if (region1 != this->GetOutput()->GetLargestPossibleRegion()
           && region2 == this->GetOutputCharacteristics()->GetLargestPossibleRegion())
    {
    region = region1;
    }
  else
    {
    int xul1 = region1.GetIndex()[0];
    int xul2 = region2.GetIndex()[0];
    int yul1 = region1.GetIndex()[1];
    int yul2 = region2.GetIndex()[1];
    int xlr1 = region1.GetIndex()[0] + region1.GetSize()[0];
    int xlr2 = region2.GetIndex()[0] + region2.GetSize()[0];
    int ylr1 = region1.GetIndex()[1] + region1.GetSize()[1];
    int ylr2 = region2.GetIndex()[1] + region2.GetSize()[1];

    int xul = std::min(xul1, xul2);
    int yul = std::min(yul1, yul2);
    int xlr = std::max(xlr1, xlr2);
    int ylr = std::max(ylr1, ylr2);

    typename RegionType::IndexType index;
    index[0] = xul;
    index[1] = yul;

    typename RegionType::SizeType size;
    size[0] = static_cast<unsigned int>(xlr - xul);
    size[1] = static_cast<unsigned int>(ylr - yul);

    region.SetIndex(index);
    region.SetSize(size);
    }

  while (inputListIt != inputPtr->End())
    {
    inputListIt.Get()->SetRequestedRegion(region);
    ++inputListIt;
    }
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
void
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::GenerateData(void)
{
  OutputImagePointerType    outputPtr = this->GetOutput();
  LabeledImagePointerType   outputLabeledPtr = this->GetOutputCharacteristics();
  InputImageListPointerType inputPtr = this->GetInput();

  // Output image initializations
  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();
  outputPtr->FillBuffer(0);

  outputLabeledPtr->SetBufferedRegion(outputLabeledPtr->GetRequestedRegion());
  outputLabeledPtr->Allocate();
  outputLabeledPtr->FillBuffer(0);

  // defines input and output iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionIterator<LabeledImageType>    LabeledIteratorType;

  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();

  // defines a vector of input iterators
  typedef std::vector<InputIteratorType> InputIteratorListType;
  InputIteratorListType inputIteratorList;

  // fills the vector of input iterators
  for (; inputListIt != inputPtr->End(); ++inputListIt)
    {
    inputIteratorList.push_back(InputIteratorType(inputListIt.Get(), inputListIt.Get()->GetRequestedRegion()));
    inputIteratorList.back().GoToBegin();
    }

  // walk through the output images
  OutputIteratorType outputIt(outputPtr, outputPtr->GetRequestedRegion());
  outputIt.GoToBegin();
  LabeledIteratorType labeledIt(outputLabeledPtr, outputLabeledPtr->GetRequestedRegion());
  labeledIt.GoToBegin();

  bool inputIteratorsAtEnd = false;
  for (typename InputIteratorListType::iterator it = inputIteratorList.begin();
       it != inputIteratorList.end(); ++it)
    {
    inputIteratorsAtEnd = inputIteratorsAtEnd || it->IsAtEnd();
    }

  while (!outputIt.IsAtEnd() && !labeledIt.IsAtEnd() && !inputIteratorsAtEnd)
    {
    unsigned int     index = 0;
    OutputPixelType  outputPixel = 0;
    LabeledPixelType outputChar = 0;

    // for each input iterator, check the output and characteristics
    for (typename InputIteratorListType::iterator it = inputIteratorList.begin();
         it != inputIteratorList.end(); ++it)
      {
      if (it->Get() > outputPixel)
        {
        outputPixel = it->Get();
        outputChar = m_InitialValue + m_Step * (static_cast<LabeledPixelType>(index));
        }
      ++index;
      }
    outputIt.Set(outputPixel);
    labeledIt.Set(outputChar);
    ++outputIt;
    ++labeledIt;
    for (typename InputIteratorListType::iterator it = inputIteratorList.begin();
         it != inputIteratorList.end(); ++it)
      {
      ++(*it);
      inputIteratorsAtEnd = inputIteratorsAtEnd || it->IsAtEnd();
      }
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
void
ProfileDerivativeToMultiScaleCharacteristicsFilter<TInputImage, TOutputImage, TLabeledImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif

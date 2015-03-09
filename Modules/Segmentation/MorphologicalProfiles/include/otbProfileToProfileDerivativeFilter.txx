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
#ifndef __otbProfileToProfileDerivativeFilter_txx
#define __otbProfileToProfileDerivativeFilter_txx

#include "otbProfileToProfileDerivativeFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImageList, class TOutputImageList>
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::ProfileToProfileDerivativeFilter()
{
  m_SubtractFilter = SubtractFilterType::New();
  m_AbsFilter = AbsFilterType::New();
}
/** Generate output information for the ImageList and for each image
      in the List. */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateOutputInformation()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != inputPtr->Size() - 1)
      {
      // in this case, clear the list
      outputPtr->Clear();
      typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
      ++inputListIt;
      while (inputListIt != inputPtr->End())
        {
        outputPtr->PushBack(OutputImageType::New());
        ++inputListIt;
        }
      }

    // For each input image
    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
    typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
    ++inputListIt;
    while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
      {
      // Create the output image and set its information
      outputListIt.Get()->CopyInformation(inputListIt.Get());
      outputListIt.Get()->SetLargestPossibleRegion(inputListIt.Get()->GetLargestPossibleRegion());
      ++inputListIt;
      ++outputListIt;
      }
    }
}
/** Generate input requested region for each image in the List. */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateInputRequestedRegion()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();

  // Use the filter to generate input requested region
  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    inputListIt.Get()->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    ++inputListIt;
    ++outputListIt;
    }
}
/** Main computation method */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::GenerateData()
{
  m_AbsFilter->SetInput(m_SubtractFilter->GetOutput());

  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
  unsigned int                               counter = 0;

  InputImagePointerType lastImage = inputListIt.Get();
  ++inputListIt;

  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    m_SubtractFilter->SetInput1(inputListIt.Get());
    m_SubtractFilter->SetInput2(lastImage);
    m_AbsFilter->GetOutput()->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_AbsFilter->Update();
    outputPtr->SetNthElement(counter, static_cast<OutputImageType *>(m_AbsFilter->GetOutput()));
    outputListIt.Get()->DisconnectPipeline();
    lastImage = inputListIt.Get();
    ++inputListIt;
    ++outputListIt;
    ++counter;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImageList, class TOutputImageList>
void
ProfileToProfileDerivativeFilter<TInputImageList, TOutputImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif

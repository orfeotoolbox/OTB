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
#ifndef otbImageListToImageListApplyFilter_txx
#define otbImageListToImageListApplyFilter_txx

#include "otbImageListToImageListApplyFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::ImageListToImageListApplyFilter()
{
  m_Filter = FilterType::New();
  m_OutputIndex = 0;
}
/** Generate output information for the ImageList and for each image
      in the List. */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::GenerateOutputInformation()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != inputPtr->Size())
      {
      // in this case, clear the list
      outputPtr->Clear();
      typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
      while (inputListIt != inputPtr->End())
        {
        outputPtr->PushBack(OutputImageType::New());
        ++inputListIt;
        }
      }

    // For each input image
    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
    typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();

    while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
      {
      // Create the output image and set its information

      m_Filter->SetInput(inputListIt.Get());
      m_Filter->UpdateOutputInformation();
      outputListIt.Get()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
      outputListIt.Get()->SetLargestPossibleRegion(m_Filter->GetOutput(m_OutputIndex)->GetLargestPossibleRegion());
      ++inputListIt;
      ++outputListIt;
      }
    }
}
/** Generate input requested region for each image in the List. */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
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
    m_Filter->SetInput(inputListIt.Get());
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_Filter->PropagateRequestedRegion(outputListIt.Get());
    ++inputListIt;
    ++outputListIt;
    }
}
/** Main computation method */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::GenerateData()
{
  // Retrieving input/output pointers
  InputImageListPointerType  inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each input image and corresponding output image
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  typename OutputImageListType::Iterator     outputListIt = outputPtr->Begin();
  unsigned int                               counter = 0;

  while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
    {
    m_Filter->SetInput(inputListIt.Get());
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_Filter->Update();
    outputPtr->SetNthElement(counter, static_cast<OutputImageType *>(m_Filter->GetOutput(m_OutputIndex)));
    outputListIt.Get()->DisconnectPipeline();
    ++inputListIt;
    ++outputListIt;
    ++counter;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImageList, class TOutputImageList, class TFilter>
void
ImageListToImageListApplyFilter<TInputImageList, TOutputImageList, TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif

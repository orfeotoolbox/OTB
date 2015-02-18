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
#ifndef __otbImageToProfileFilter_txx
#define __otbImageToProfileFilter_txx

#include "otbImageToProfileFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter>
ImageToProfileFilter<TInputImage, TOutputImage, TFilter, TParameter>
::ImageToProfileFilter()
{
  m_InitialValue = 0;
  m_Step = 1;
  m_ProfileSize = 10;
  m_OutputIndex = 0;
  m_Filter = FilterType::New();
}
/**
 * GenerateOutputInformation method
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter>
void
ImageToProfileFilter<TInputImage, TOutputImage, TFilter, TParameter>
::GenerateOutputInformation(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  if (outputPtr)
    {
    if (outputPtr->Size() != m_ProfileSize)
      {
      // in this case, clear the list
      outputPtr->Clear();
      for (unsigned int i = 0; i < m_ProfileSize; ++i)
        {
        //Create the output image
        outputPtr->PushBack(OutputImageType::New());
        }
      }
    // For each output image
    typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
    m_Filter->SetInput(inputPtr);
    m_Filter->UpdateOutputInformation();
    while (outputListIt != outputPtr->End())
      {
      //Set the image information
      outputListIt.Get()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
      outputListIt.Get()->SetLargestPossibleRegion(m_Filter->GetOutput(m_OutputIndex)->GetLargestPossibleRegion());
      ++outputListIt;
      }
    }
}
/**
 * Generate input requested region
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter>
void
ImageToProfileFilter<TInputImage, TOutputImage, TFilter, TParameter>
::GenerateInputRequestedRegion(void)
{
  // Retrieving input/output pointers
  InputImagePointerType      inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();

  // For each output image
  typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();

  m_Filter->SetInput(inputPtr);

  // Use the filter to generate input requested region
  while (outputListIt != outputPtr->End())
    {
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
    m_Filter->PropagateRequestedRegion(outputListIt.Get());
    ++outputListIt;
    }
}
/**
 * GenerateData method
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter>
void
ImageToProfileFilter<TInputImage, TOutputImage, TFilter, TParameter>
::GenerateData(void)
{
// Retrieving input/output pointers
  InputImagePointerType      inputPtr = this->GetInput();
  OutputImageListPointerType outputPtr = this->GetOutput();
  m_Filter->SetInput(inputPtr);

  for (unsigned int i = 0; i < m_ProfileSize; ++i)
    {
    ParameterType profileParameter = m_InitialValue + static_cast<ParameterType>(i) * m_Step;
    this->SetProfileParameter(profileParameter);
    m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputPtr->GetNthElement(i)->GetRequestedRegion());
    m_Filter->Update();
    outputPtr->SetNthElement(i, static_cast<OutputImageType *>(m_Filter->GetOutput(m_OutputIndex)));
    outputPtr->GetNthElement(i)->DisconnectPipeline();
    }
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TFilter, class TParameter>
void
ImageToProfileFilter<TInputImage, TOutputImage, TFilter, TParameter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Filter: "      << m_Filter->GetNameOfClass() << std::endl;
  os << indent << "OutputIndex: " << m_OutputIndex             << std::endl;
  os << indent << "ProfileSize: " << m_ProfileSize             << std::endl;
  os << indent << "InitialValue: " << m_InitialValue            << std::endl;
  os << indent << "Step: "        << m_Step                    << std::endl;

}
} // End namespace otb
#endif

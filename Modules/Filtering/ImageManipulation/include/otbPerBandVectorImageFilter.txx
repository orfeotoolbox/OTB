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
#ifndef __otbPerBandVectorImageFilter_txx
#define __otbPerBandVectorImageFilter_txx

#include "otbPerBandVectorImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFilter>
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>
::PerBandVectorImageFilter()
{
  m_Filter = FilterType::New();
  m_OutputIndex = 0;
}

template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateOutputInformation()
{
  if (this->GetInput())
    {
    // Create a false monoband image
    typename InputImageType::Pointer dummyInputImage = InputImageType::New();
    dummyInputImage->CopyInformation(this->GetInput());
    dummyInputImage->SetNumberOfComponentsPerPixel(1);
    m_Filter->SetInput(dummyInputImage);
    m_Filter->UpdateOutputInformation();
    this->GetOutput()->CopyInformation(m_Filter->GetOutput(m_OutputIndex));
    this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());
    }
}

template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateInputRequestedRegion()
{
  InputVectorImageType *           inputPtr = const_cast<InputVectorImageType *>(this->GetInput());
  typename InputImageType::Pointer dummyInputImage = InputImageType::New();
  dummyInputImage->CopyInformation(this->GetInput());
  dummyInputImage->SetNumberOfComponentsPerPixel(1);
  m_Filter->SetInput(dummyInputImage);
  m_Filter->GetOutput(m_OutputIndex)->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_Filter->PropagateRequestedRegion(this->GetOutput());
  inputPtr->SetRequestedRegion(m_Filter->GetInput()->GetRequestedRegion());
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>
::GenerateData()
{
  InputVectorImageType *         inputPtr = const_cast<InputVectorImageType *>(this->GetInput());
  OutputVectorImagePointerType   outputPtr =  this->GetOutput();
  DecompositionFilterPointerType decomposer = DecompositionFilterType::New();
  ProcessingFilterPointerType    processor = ProcessingFilterType::New();
  RecompositionFilterPointerType recomposer = RecompositionFilterType::New();

  inputPtr->UpdateOutputData();

  decomposer->SetInput(this->GetInput());
  processor->SetInput(decomposer->GetOutput());
  processor->SetFilter(m_Filter);
  processor->SetOutputIndex(m_OutputIndex);
  recomposer->SetInput(processor->GetOutput());
  recomposer->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());

  recomposer->GraftOutput(this->GetOutput());
  recomposer->Update();
  this->GraftOutput(recomposer->GetOutput());
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage, TOutputImage, TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif

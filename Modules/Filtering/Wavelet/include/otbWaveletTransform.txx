/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletTransform_txx
#define __otbWaveletTransform_txx
#include "otbWaveletTransform.h"

#include "itkImageRegionIterator.h"
#include "itkProgressAccumulator.h"
#include "otbMacro.h"

namespace otb {

/**
 * Template Specialization for the Wavelet::FORWARD case
 */

template <class TInputImage, class TOutputImage, class TFilter>
WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD>
::WaveletTransform ()
  : m_NumberOfDecompositions(1), m_SubsampleImageFactor(2)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageListType::New());

  m_FilterList = FilterListType::New();
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD>
::GenerateData()
{
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  GetFilterList()->Resize(GetNumberOfDecompositions());

  this->GetFilterList()->SetNthElement(0, FilterType::New());
  FilterPointerType filter = this->GetFilterList()->GetNthElement(0); //GetNthFilter( 0 );
  filter->SetInput(this->GetInput());
  filter->SetSubsampleImageFactor(GetSubsampleImageFactor());

  otbMsgDevMacro(<<"Allocating " << (1 + GetNumberOfDecompositions() * (filter->GetNumberOfOutputs() - 1)) << " output\n");
  this->GetOutput()->Resize(
    1 + GetNumberOfDecompositions() * (filter->GetNumberOfOutputs() - 1));

  otbMsgDevMacro(<<"Using " << this->GetOutput()->Size() << " outputs...");
  for (unsigned int idx = 0; idx < this->GetOutput()->Size(); ++idx)
    {
    this->GetOutput()->SetNthElement(idx, OutputImageType::New());
    }

  progress->RegisterInternalFilter(filter,
                                   1.f / static_cast<float>(GetNumberOfDecompositions()));

  filter->Update();

  for (unsigned int idx = 1; idx < filter->GetNumberOfOutputs(); ++idx)
    {
    this->GetOutput()->SetNthElement(
      this->GetOutput()->Size() - filter->GetNumberOfOutputs() + idx,
      filter->GetOutput(idx));
    }

  for (unsigned int nbDecomp = 1; nbDecomp < GetNumberOfDecompositions(); nbDecomp++)
    {
    this->GetFilterList()->SetNthElement(nbDecomp, FilterType::New());
    filter = this->GetFilterList()->GetNthElement(nbDecomp); //this->GetNthFilter( nbDecomp );
    filter->SetInput(this->GetNthFilter(nbDecomp - 1)->GetOutput(0));
    filter->SetSubsampleImageFactor(GetSubsampleImageFactor());
    if (GetSubsampleImageFactor() == 1) filter->SetUpSampleFilterFactor(nbDecomp + 1);

    progress->RegisterInternalFilter(filter,
                                     1.f / static_cast<float>(GetNumberOfDecompositions()));
    filter->Update();

    for (unsigned int idx = 1; idx < filter->GetNumberOfOutputs(); ++idx)
      {
      this->GetOutput()->SetNthElement(
        this->GetOutput()->Size() - 1 - (nbDecomp + 1) * (filter->GetNumberOfOutputs() - 1) + idx,
        filter->GetOutput(idx));
      }
    }

  this->GetOutput()->SetNthElement(0, filter->GetOutput(0));
}

/**
 * Template Specialization for the Wavelet::INVERSE case
 */

template <class TInputImage, class TOutputImage, class TFilter>
WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE>
::WaveletTransform ()
  : m_NumberOfDecompositions(1), m_SubsampleImageFactor(2)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageType::New());

  m_FilterList = FilterListType::New();
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE>
::GenerateOutputInformation()
{
  this->GetOutput()->CopyInformation(this->GetInput()->GetNthElement(0));

  if (GetSubsampleImageFactor() != 1)
    {
    InputImageRegionType inputRegion = this->GetInput()->GetNthElement(0)->GetLargestPossibleRegion();
    SizeType             inputSize = inputRegion.GetSize();
    IndexType            inputIndex = inputRegion.GetIndex();

    OutputImageSizeType  outputSize;
    OutputImageIndexType outputIndex;

    for (unsigned int i = 0; i < InputImageDimension; ++i)
      {
      outputIndex[i] = inputIndex[i] * GetSubsampleImageFactor() * GetNumberOfDecompositions();
      outputSize[i] = inputSize[i] * GetSubsampleImageFactor() * GetNumberOfDecompositions();
      }

    OutputImageRegionType outputRegion;
    outputRegion.SetIndex(outputIndex);
    outputRegion.SetSize(outputSize);

    this->GetOutput()->SetRegions(outputRegion);
    }
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE>
::GenerateData()
{
  FilterPointerType filter = FilterType::New();

  const unsigned int filterbankInputSize = 1 << InputImageDimension;

  m_NumberOfDecompositions = (this->GetInput()->Size() - 1)/(filterbankInputSize - 1);

  otbMsgDevMacro(<< "Found " << m_NumberOfDecompositions << " decompositions");

  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  this->GetFilterList()->Resize(GetNumberOfDecompositions());

  InputImageIterator inputIterator = this->GetInput()->Begin();

  this->GetFilterList()->SetNthElement(0, FilterType::New());
  filter = this->GetNthFilter(0);
  for (unsigned int i = 0; i < filterbankInputSize; ++i)
    {
    filter->SetInput(i, inputIterator.Get());
    ++inputIterator;
    }

  filter->SetSubsampleImageFactor(GetSubsampleImageFactor());
  if (GetSubsampleImageFactor() == 1)
    {
    filter->SetUpSampleFilterFactor(GetNumberOfDecompositions());
    }

  progress->RegisterInternalFilter(filter,
                                   1.f / static_cast<float>(GetNumberOfDecompositions()));
  filter->Update();

  for (unsigned int idx = 1; idx < GetNumberOfDecompositions(); ++idx)
    {
    this->GetFilterList()->SetNthElement(idx, FilterType::New());
    filter = this->GetNthFilter(idx);
    filter->SetInput(0, this->GetNthFilter(idx - 1)->GetOutput());

    for (unsigned int i = 1; i < filterbankInputSize; ++i)
      {
      filter->SetInput(i, inputIterator.Get());
      ++inputIterator;
      }

    filter->SetSubsampleImageFactor(GetSubsampleImageFactor());
    if (GetSubsampleImageFactor() == 1)
      {
      filter->SetUpSampleFilterFactor(GetNumberOfDecompositions() - idx);
      }

    progress->RegisterInternalFilter(filter,
                                     1.f / static_cast<float>(GetNumberOfDecompositions()));
    filter->Update();
    }

  // This step is necessary to transtype output image to the wanted format
  this->GetOutput()->CopyInformation(filter->GetOutput());
  this->GetOutput()->SetRegions(filter->GetOutput()->GetLargestPossibleRegion());
  this->GetOutput()->Allocate();
  this->GetOutput()->FillBuffer(0);

  itk::ImageRegionIterator<OutputImageType> outputIter
    (this->GetOutput(), this->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<typename FilterType::OutputImageType> outFltIter
    (filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion());

  for (outputIter.GoToBegin(), outFltIter.GoToBegin(); !outputIter.IsAtEnd(); ++outputIter, ++outFltIter)
    {
    outputIter.Set(static_cast<OutputValueType>(outFltIter.Get()));
    }
}

} // end of namespace otb

#endif

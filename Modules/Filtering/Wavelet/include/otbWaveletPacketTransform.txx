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

#ifndef __otbWaveletPacketTransform_txx
#define __otbWaveletPacketTransform_txx
#include "otbWaveletPacketTransform.h"
#include "otbMacro.h"

namespace otb {

/**
 * Template specialization for the Wavelet::FORWARD transformation
 */

template <class TInputImage, class TOutputImage, class TFilter, class TCost>
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD, TCost>
::WaveletPacketTransform ()
  : m_SubsampleImageFactor(2), m_NumberOfFilters(0), m_DepthOfDecomposition(0)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageListType::New());

  m_FilterList = FilterListType::New();
  m_Cost = CostType::New();
}

template <class TInputImage, class TOutputImage, class TFilter, class TCost>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD, TCost>
::GenerateData()
{
  /*
   * Start with a decomposition
   */

  m_WaveletPacketRule.clear();

  m_NumberOfFilters = 0;
  m_DepthOfDecomposition = 0;

  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  GenerateData(0, this->GetInput(), progress);

}

template <class TInputImage, class TOutputImage, class TFilter, class TCost>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD, TCost>
::GenerateData
  (unsigned int depth,
  OutputImageType * outputPtr,
  itk::ProgressAccumulator * progress)
{
  // We cannot know in advance the nomber of filters in this mini-pipeline
  // So we decrease the weigth of each filter in order to tend to 1... slowly...
  static float accumulatorWeight = 1.;

  if (this->GetCost()->Evaluate(depth, outputPtr))
    {
    if (m_DepthOfDecomposition < depth) m_DepthOfDecomposition = depth;

    m_WaveletPacketRule.push_back(true);

    this->GetFilterList()->PushBack(FilterType::New());
    FilterPointerType filter = this->GetFilterList()->GetNthElement(m_NumberOfFilters);

    m_NumberOfFilters++;

    filter->SetSubsampleImageFactor(GetSubsampleImageFactor());
    if (GetSubsampleImageFactor() == 1) filter->SetUpSampleFilterFactor(depth);

    accumulatorWeight /= 2.;
    progress->RegisterInternalFilter(filter, accumulatorWeight);
    filter->SetInput(outputPtr);

    filter->Update();

    for (unsigned int idx = 0; idx < filter->GetNumberOfOutputs(); ++idx)
      {
      GenerateData(depth + 1, filter->GetOutput(idx), progress);
      }
    }
  else
    {
    m_WaveletPacketRule.push_back(false);
    this->GetOutput()->PushBack(outputPtr);
    }
}

/**
 * Template specialozation for the Wavelet::INVERSE transformation
 */

template <class TInputImage, class TOutputImage, class TFilter>
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::WaveletPacketTransform ()
  : m_SubsampleImageFactor(2), m_NumberOfFilters(0), m_DepthOfDecomposition(0)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageType::New());

  m_FilterList = FilterListType::New();
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::GenerateOutputInformation()
{
  if (m_NumberOfFilters == 0) InterpretRule();

  this->GetOutput()->CopyInformation(this->GetInput()->GetNthElement(0));

  InputImageRegionType inputRegion = this->GetInput()->GetNthElement(0)->GetLargestPossibleRegion();
  SizeType             inputSize = inputRegion.GetSize();
  IndexType            inputIndex = inputRegion.GetIndex();

  OutputImageSizeType  outputSize;
  OutputImageIndexType outputIndex;

  for (unsigned int i = 0; i < InputImageDimension; ++i)
    {
    outputIndex[i] = inputIndex[i] * GetSubsampleImageFactor() * GetDepthOfDecomposition();
    outputSize[i] = inputSize[i] * GetSubsampleImageFactor() * GetDepthOfDecomposition();
    }

  otbMsgDevMacro(<< "Output Size [" << outputSize[0] << "," << outputSize[1] << "]");

  OutputImageRegionType outputRegion;
  outputRegion.SetIndex(outputIndex);
  outputRegion.SetSize(outputSize);
  this->GetOutput()->SetRegions(outputRegion);
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::GenerateData()
{
  if (m_WaveletPacketRule[0] != true)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
                               "No decomposition to perform in Generic data... Check WaveletPacketRule tab",
                               ITK_LOCATION);
    }

  if (m_NumberOfFilters == 0) InterpretRule();

  otbMsgDevMacro(<< "nbFilter  = " << m_NumberOfFilters);
  otbMsgDevMacro(<< "depth     = " << m_DepthOfDecomposition);
  otbMsgDevMacro(<< "rule size = " << m_WaveletPacketRule.size());

  if (m_NumberOfFilters == 0)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
                               "No filter found in the decomposition tree... Check WaveletPacketRule tab",
                               ITK_LOCATION);
    }

  InputImageIterator inputIterator = this->GetInput()->Begin();

  unsigned int pos = 1;
  SetInputFilters(pos, inputIterator, 0);

  if (pos != m_WaveletPacketRule.size() || inputIterator != this->GetInput()->End())
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
                               "Bad decomposition tree implementation...",
                               ITK_LOCATION);
    }

  m_FilterList->GetNthElement(0)->GraftOutput(this->GetOutput());

  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  for (pos = m_NumberOfFilters; pos > 0; pos--)
    {
    FilterPointerType filter = m_FilterList->GetNthElement(pos - 1);
    progress->RegisterInternalFilter(filter, 1.f / static_cast<float>(m_NumberOfFilters));
    filter->Update();
    }

  this->GraftOutput(m_FilterList->GetNthElement(0)->GetOutput());
}

template <class TInputImage, class TOutputImage, class TFilter>
unsigned int
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::SetInputFilters
  (unsigned int& ruleID, InputImageIterator& imgIt, unsigned int filterID)
{
  unsigned int nextFilterID = filterID + 1;

  if (ruleID == m_WaveletPacketRule.size()) return m_FilterList->Size();

  const unsigned int filterBankInputSize=1 << InputImageDimension;

  for (unsigned int i = 0; i < filterBankInputSize; ++i)
    {
    if (m_WaveletPacketRule[ruleID++] == true)
      {
      m_FilterList->GetNthElement(filterID)->SetInput(i,
                                                      m_FilterList->GetNthElement(nextFilterID)->GetOutput());
      nextFilterID = SetInputFilters(ruleID, imgIt, nextFilterID);
      }
    else
      {
      m_FilterList->GetNthElement(filterID)->SetInput(i, imgIt.Get());
      ++imgIt;
      }
    }

  return nextFilterID;
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::InterpretRule()
{
  if (m_FilterList && m_FilterList->Size() != 0)
    {
    if (m_NumberOfFilters != 0) itkExceptionMacro(<< "Incoherency between member value");
    }

  m_NumberOfFilters = 0;
  m_DepthOfDecomposition = 0;

  for (unsigned int posRule = 0; posRule < m_WaveletPacketRule.size(); posRule++)
    InterpretRule(posRule, 0);
}

template <class TInputImage, class TOutputImage, class TFilter>
void
WaveletPacketTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE,
    FullyDecomposedWaveletPacketCost<TInputImage> >
::InterpretRule
  (unsigned int& ruleID, unsigned int curDepth)
{
  if (curDepth > m_DepthOfDecomposition) m_DepthOfDecomposition = curDepth;

  if (m_WaveletPacketRule[ruleID] == true)
    {
    m_FilterList->PushBack(FilterType::New());

    FilterPointerType filter = m_FilterList->GetNthElement(m_NumberOfFilters);
    filter->SetSubsampleImageFactor(GetSubsampleImageFactor());
    if (GetSubsampleImageFactor() == 1) filter->SetUpSampleFilterFactor(curDepth);

    m_NumberOfFilters++;

    const unsigned int filterBankInputSize = 1 << InputImageDimension;

    for (unsigned int i = 0; i < filterBankInputSize; ++i)
      {
      ruleID++;
      InterpretRule(ruleID, curDepth + 1);
      }
    }
}

} // end of namespace otb

#endif

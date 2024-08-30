/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSimpleRcsPanSharpeningFusionImageFilter_hxx
#define otbSimpleRcsPanSharpeningFusionImageFilter_hxx

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "otbNoDataHelper.h"

namespace otb
{
template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::SimpleRcsPanSharpeningFusionImageFilter()
{
  // Fix number of required inputs
  this->SetNumberOfRequiredInputs(2);
  this->m_UseNoData = false;

  // Instantiate convolution filter
  m_ConvolutionFilter = ConvolutionFilterType::New();
  m_ConvolutionFilter->NormalizeFilterOn();

  // Set-up default parameters
  m_Radius.Fill(9);
  m_Filter.SetSize((2 * m_Radius[0] + 1) * (2 * m_Radius[1] + 1));
  m_Filter.Fill(1);
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::SetPanInput(const TPanImageType* image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, const_cast<TPanImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TPanImageType* SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::GetPanInput(void) const
{
  if (this->GetNumberOfInputs() < 2)
  {
    return nullptr;
  }

  return static_cast<const TPanImageType*>(this->itk::ProcessObject::GetInput(1));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::SetXsInput(const TXsImageType* image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0, const_cast<TXsImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TXsImageType* SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::GetXsInput(void) const
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const TXsImageType*>(this->itk::ProcessObject::GetInput(0));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::GenerateData()
{
  // Check if size is correct
  typename TPanImageType::SizeType sizePan;
  typename TXsImageType::SizeType  sizeXs;
  sizePan = this->GetPanInput()->GetLargestPossibleRegion().GetSize();
  sizeXs  = this->GetXsInput()->GetLargestPossibleRegion().GetSize();
  if ((sizePan[0] != sizeXs[0]) || (sizePan[1] != sizeXs[1]))
  {
    itkExceptionMacro(<< "SimpleRcsPanSharpeningFusionImageFilter: Wrong Pan/Xs size");
  }

  // Set-up progress reporting
  m_ProgressAccumulator = itk::ProgressAccumulator::New();
  m_ProgressAccumulator->SetMiniPipelineFilter(this);
  m_ProgressAccumulator->RegisterInternalFilter(m_ConvolutionFilter, 0.9);

  m_ConvolutionFilter->SetInput(this->GetPanInput());
  m_ConvolutionFilter->SetRadius(this->m_Radius);
  m_ConvolutionFilter->SetFilter(this->m_Filter);

  typedef typename TPanImageType::InternalPixelType PanPixelType;
  typedef typename TXsImageType::InternalPixelType  XsPixelType;

  // Write no-data flags for Pan image
  std::vector<bool>   tmpNoDataValuePanAvailable;
  std::vector<double> tmpNoDataValuePan;
  bool                noDataValuePanAvailable = false;
  PanPixelType        noDataValuePan          = 0;

  bool retPan = ReadNoDataFlags(this->GetPanInput()->GetImageMetadata(), tmpNoDataValuePanAvailable, tmpNoDataValuePan);

  if (retPan && tmpNoDataValuePanAvailable.size() > 0 && tmpNoDataValuePan.size() > 0)
  {
    noDataValuePanAvailable = tmpNoDataValuePanAvailable[0] && retPan;
    noDataValuePan          = static_cast<PanPixelType>(tmpNoDataValuePan[0]);
  }

  // Write no-data flags for Xs image
  std::vector<bool>        noDataValuesXsAvailable;
  std::vector<double>      tmpNoDataValuesXs;
  std::vector<XsPixelType> noDataValuesXs;

  bool retXs = ReadNoDataFlags(this->GetXsInput()->GetImageMetadata(), noDataValuesXsAvailable, tmpNoDataValuesXs);

  // Check if noData is needed and update noDataValuesAvailable with return function value
  if (retPan || retXs)
  {
    m_UseNoData = noDataValuePanAvailable;
    for (unsigned int i = 0; i < tmpNoDataValuesXs.size() && i < noDataValuesXsAvailable.size(); ++i)
    {
      noDataValuesXs.push_back(static_cast<XsPixelType>(tmpNoDataValuesXs[i]));
      m_UseNoData |= (noDataValuesXsAvailable[i] = (noDataValuesXsAvailable[i] && retXs));
    }
  }

  // Instantiate fusion filter
  if (m_UseNoData)
  {
    m_NoDataFusionFilter = NoDataFusionFilterType::New();
    m_ProgressAccumulator->RegisterInternalFilter(m_NoDataFusionFilter, 0.1);

    m_NoDataFusionFilter->SetInput2(m_ConvolutionFilter->GetOutput());
    m_NoDataFusionFilter->GetModifiableFunctor().SetNoDataValuesXsAvailable(noDataValuesXsAvailable);
    m_NoDataFusionFilter->GetModifiableFunctor().SetNoDataValuePanAvailable(noDataValuePanAvailable);
    m_NoDataFusionFilter->GetModifiableFunctor().SetNoDataValuePan(noDataValuePan);
    m_NoDataFusionFilter->GetModifiableFunctor().SetNoDataValuesXs(noDataValuesXs);

    m_NoDataFusionFilter->SetInput1(this->GetXsInput());
    m_NoDataFusionFilter->SetInput2(m_ConvolutionFilter->GetOutput());
    m_NoDataFusionFilter->SetInput3(this->GetPanInput());

    // Wire composite filter
    m_NoDataFusionFilter->GraftOutput(this->GetOutput());
    m_NoDataFusionFilter->Update();
    this->GraftOutput(m_NoDataFusionFilter->GetOutput());
  }
  else
  {
    m_FusionFilter = FusionFilterType::New();
    m_ProgressAccumulator->RegisterInternalFilter(m_FusionFilter, 0.1);

    m_FusionFilter->SetInput1(this->GetXsInput());
    m_FusionFilter->SetInput2(m_ConvolutionFilter->GetOutput());
    m_FusionFilter->SetInput3(this->GetPanInput());

    // Wire composite filter
    m_FusionFilter->GraftOutput(this->GetOutput());
    m_FusionFilter->Update();
    this->GraftOutput(m_FusionFilter->GetOutput());
  }
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void SimpleRcsPanSharpeningFusionImageFilter<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>::PrintSelf(std::ostream& os,
                                                                                                                           itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius:" << this->m_Radius << std::endl;
}

} // end namespace otb

#endif

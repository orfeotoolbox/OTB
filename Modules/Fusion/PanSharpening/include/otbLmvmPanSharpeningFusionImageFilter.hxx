/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbLmvmPanSharpeningFusionImageFilter_hxx
#define otbLmvmPanSharpeningFusionImageFilter_hxx

#include "otbLmvmPanSharpeningFusionImageFilter.h"

namespace otb
{
template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::LmvmPanSharpeningFusionImageFilter()
{
  // Fix number of required inputs
  this->SetNumberOfRequiredInputs(2);

  // Instantiate convolution filters
  m_PanConvolutionFilter = PanConvolutionFilterType::New();
  m_PanConvolutionFilter->NormalizeFilterOn();
  m_XsConvolutionFilter = XsConvolutionFilterType::New();
  m_XsConvolutionFilter->NormalizeFilterOn();
  m_PanNoiseFilter = PanNoiseFilterType::New();
  m_XsNoiseFilter = XsNoiseFilterType::New();
  m_XsVectorConvolutionFilter = XsVectorConvolutionFilterType::New();
  m_XsVectorNoiseFilter = XsVectorNoiseFilterType::New();


  // Set-up default parameters
  m_Radius.Fill(3);
  m_Filter.SetSize(7 * 7);
  m_Filter.Fill(1);

  // Instantiate fusion filter
  m_FusionStep1Filter = FusionStep1FilterType::New();
  m_FusionStep2Filter = FusionStep2FilterType::New();

  // Set-up progress reporting
  m_ProgressAccumulator = itk::ProgressAccumulator::New();
  m_ProgressAccumulator->SetMiniPipelineFilter(this);
  m_ProgressAccumulator->RegisterInternalFilter(m_PanConvolutionFilter, 0.2);
  m_ProgressAccumulator->RegisterInternalFilter(m_PanNoiseFilter, 0.2);
  m_ProgressAccumulator->RegisterInternalFilter(m_XsVectorConvolutionFilter, 0.2);
  m_ProgressAccumulator->RegisterInternalFilter(m_XsVectorNoiseFilter, 0.2);
  m_ProgressAccumulator->RegisterInternalFilter(m_FusionStep1Filter, 0.1);
  m_ProgressAccumulator->RegisterInternalFilter(m_FusionStep2Filter, 0.1);
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetPanInput(const TPanImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<TPanImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TPanImageType *
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetPanInput(void) const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }

  return static_cast<const TPanImageType *>
           (this->itk::ProcessObject::GetInput(1));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetXsInput(const TXsImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<TXsImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TXsImageType *
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetXsInput(void) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const TXsImageType *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GenerateData()
{
  //Check if size is correct
  typename TPanImageType::SizeType       sizePan;
  typename TXsImageType::SizeType        sizeXs;
  sizePan = this->GetPanInput()->GetLargestPossibleRegion().GetSize();
  sizeXs = this->GetXsInput()->GetLargestPossibleRegion().GetSize();
  if ((sizePan[0] != sizeXs[0]) || (sizePan[1] != sizeXs[1]))
  {
    itkExceptionMacro(<< "LmvmPanSharpeningFusionImageFilter: Wrong Pan/Xs size");
  }

  //Process the fusion
  m_PanConvolutionFilter->SetInput(this->GetPanInput());
  m_PanConvolutionFilter->SetRadius(this->m_Radius);
  m_PanConvolutionFilter->SetFilter(this->m_Filter);
  m_PanNoiseFilter->SetRadius(this->m_Radius);
  m_PanNoiseFilter->SetInput(this->GetPanInput());
  m_XsConvolutionFilter->SetRadius(this->m_Radius);
  m_XsConvolutionFilter->SetFilter(this->m_Filter);
  m_XsVectorConvolutionFilter->SetInput(this->GetXsInput());
  m_XsVectorConvolutionFilter->SetFilter(m_XsConvolutionFilter);
  m_XsNoiseFilter->SetRadius(this->m_Radius);
  m_XsVectorNoiseFilter->SetInput(this->GetXsInput());
  m_XsVectorNoiseFilter->SetFilter(m_XsNoiseFilter);


  m_FusionStep1Filter->SetInput1(m_XsVectorNoiseFilter->GetOutput());
  m_FusionStep1Filter->SetInput2(m_PanConvolutionFilter->GetOutput());
  m_FusionStep1Filter->SetInput3(this->GetPanInput());

  m_FusionStep2Filter->SetInput1(m_FusionStep1Filter->GetOutput());
  m_FusionStep2Filter->SetInput3(m_PanNoiseFilter->GetOutput());
  m_FusionStep2Filter->SetInput2(m_XsVectorConvolutionFilter->GetOutput());

  m_FusionStep2Filter->UpdateOutputInformation();

  // Wire composite filter
  m_FusionStep2Filter->GraftOutput(this->GetOutput());
  m_FusionStep2Filter->Update();
  this->GraftOutput(m_FusionStep2Filter->GetOutput());
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
LmvmPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os
  << indent << "Radius:" << this->m_Radius
  << std::endl;
}

} // end namespace otb

#endif

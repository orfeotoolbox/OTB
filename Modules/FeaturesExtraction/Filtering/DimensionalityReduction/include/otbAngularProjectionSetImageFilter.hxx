/*
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

#ifndef otbAngularProjectionSetImageFilter_hxx
#define otbAngularProjectionSetImageFilter_hxx
#include "otbAngularProjectionSetImageFilter.h"

#include <vnl/vnl_math.h>

#include <itkImageRegionIterator.h>

namespace otb
{

template <class TInputImage, class TOutputImage, class TAngleList, class TPrecision>
AngularProjectionSetImageFilter<TInputImage, TOutputImage, TAngleList, TPrecision>::AngularProjectionSetImageFilter()
{
  // this->SetNumberOfRequiredInputs( NumberOfInputImages );
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageListType::New());

  m_FilterList = FilterListType::New();
  m_AngleList  = AngleListType::New();
}

template <class TInputImage, class TOutputImage, class TAngleList, class TPrecision>
void AngularProjectionSetImageFilter<TInputImage, TOutputImage, TAngleList, TPrecision>::SetInput(unsigned int i, const InputImageType* img)
{
  this->itk::ProcessObject::SetNthInput(i, const_cast<InputImageType*>(img));
}

template <class TInputImage, class TOutputImage, class TAngleList, class TPrecision>
const TInputImage* AngularProjectionSetImageFilter<TInputImage, TOutputImage, TAngleList, TPrecision>::GetInput(unsigned int i) const
{
  if (i >= this->GetNumberOfInputs())
  {
    return nullptr;
  }

  return static_cast<const InputImageType*>(this->itk::ProcessObject::GetInput(i));
}


template <class TInputImage, class TOutputImage, class TAngleList, class TPrecision>
void AngularProjectionSetImageFilter<TInputImage, TOutputImage, TAngleList, TPrecision>::GenerateData()
{
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  std::cerr << "Using " << GetAngleList()->Size() << " outputs...\n";
  GetFilterList()->Resize(GetAngleList()->Size());
  this->GetOutput()->Resize(GetAngleList()->Size());

  for (unsigned int idx = 0; idx < GetFilterList()->Size(); ++idx)
  {
    GetFilterList()->SetNthElement(0, FilterType::New());
    FilterPointerType filter = GetFilterList()->GetNthElement(0);
    for (unsigned int i = 0; i < this->GetNumberOfInputs(); ++i)
      filter->SetInput(i, this->GetInput(i));
    filter->SetAngleArray(GetAngleList()->GetMeasurementVector(idx));

    progress->RegisterInternalFilter(filter, 1.f / static_cast<float>(GetAngleList()->Size()));

    filter->Update();

    this->GetOutput()->SetNthElement(idx, filter->GetOutput());
  }
}

} // end of namespace otb

#endif

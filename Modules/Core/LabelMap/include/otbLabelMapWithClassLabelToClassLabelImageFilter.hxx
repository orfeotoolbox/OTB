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

#ifndef otbLabelMapWithClassLabelToClassLabelImageFilter_hxx
#define otbLabelMapWithClassLabelToClassLabelImageFilter_hxx

#include "otbLabelMapWithClassLabelToClassLabelImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>::LabelMapWithClassLabelToClassLabelImageFilter()
{
}


template <class TInputImage, class TOutputImage>
void LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
{
  OutputImageType*      output = this->GetOutput();
  const InputImageType* input  = this->GetInput();
  output->FillBuffer(input->GetBackgroundValue());
  Superclass::BeforeThreadedGenerateData();
}


template <class TInputImage, class TOutputImage>
void LabelMapWithClassLabelToClassLabelImageFilter<TInputImage, TOutputImage>::ThreadedProcessLabelObject(LabelObjectType* labelObject)
{
  typename LabelObjectType::ClassLabelType label = itk::NumericTraits<typename LabelObjectType::ClassLabelType>::max();
  if (labelObject->HasClassLabel())
  {
    label = labelObject->GetClassLabel();
  }

  ConstLineIteratorType lit = ConstLineIteratorType(labelObject);

  while (!lit.IsAtEnd())
  {
    IndexType     idx    = lit.GetLine().GetIndex();
    unsigned long length = lit.GetLine().GetLength();
    for (unsigned int i = 0; i < length; ++i)
    {
      this->GetOutput()->SetPixel(idx, label);
      idx[0]++;
    }
    ++lit;
  }
}

} // end namespace otb
#endif

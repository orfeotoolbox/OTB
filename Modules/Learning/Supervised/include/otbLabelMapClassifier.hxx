/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelMapClassifier_hxx
#define otbLabelMapClassifier_hxx

#include "otbLabelMapClassifier.h"


namespace otb
{

template <class TInputImage>
LabelMapClassifier<TInputImage>::LabelMapClassifier()
{
  // Force to single-threaded in case the learning model is not thread safe
  // This way, we benefit of the LabelMapFilter design and only need
  // to implement ThreadedProcessLabelObject
  this->SetNumberOfThreads(1); // TODO : check if still needed
}

template <class TInputImage>
void LabelMapClassifier<TInputImage>::ReleaseInputs()
{
  // by pass itk::InPlaceLabelMapFilter::ReleaseInputs() implementation,
  // which caused memory issue when ran inplace
  this->itk::LabelMapFilter<TInputImage, TInputImage>::ReleaseInputs();
}

template <class TInputImage>
void LabelMapClassifier<TInputImage>::ThreadedProcessLabelObject(LabelObjectType* labelObject)
{
  ClassLabelType classLabel = (m_Model->Predict(m_MeasurementFunctor(labelObject)))[0];
  labelObject->SetClassLabel(classLabel);
}

} // end namespace otb
#endif

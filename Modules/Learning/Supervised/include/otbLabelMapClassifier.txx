/*
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

#ifndef otbLabelMapSVMClassifier_txx
#define otbLabelMapSVMClassifier_txx

#include "otbLabelMapSVMClassifier.h"


namespace otb {

template <class TInputImage>
LabelMapSVMClassifier<TInputImage>
::LabelMapSVMClassifier()
{
  // Force to single-threaded (SVMModel is not thread-safe)
  // This way, we benefit of the LabelMapFilter design and only need
  // to implement ThreadedProcessLabelObject
  this->SetNumberOfThreads(1);
}

template<class TInputImage>
void
LabelMapSVMClassifier<TInputImage>
::ReleaseInputs( )
{
  // by pass itk::InPlaceLabelMapFilter::ReleaseInputs() implementation,
  // which caused memory issue when ran inplace
  this->itk::LabelMapFilter<TInputImage, TInputImage>::ReleaseInputs();
}

template<class TInputImage>
void
LabelMapSVMClassifier<TInputImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  ClassLabelType classLabel = m_Model->EvaluateLabel(m_MeasurementFunctor(labelObject));
  labelObject->SetClassLabel(classLabel);
}

}// end namespace otb
#endif

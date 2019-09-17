/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelToProSailParameters_hxx
#define otbLabelToProSailParameters_hxx

#include "itkNumericTraits.h"

#include "otbMacro.h"
#include "otbLabelToProSailParameters.h"

namespace otb
{

template <class TLabel>
LabelToProSailParameters<TLabel>::LabelToProSailParameters()
{
  // default parameters
  ParametersType Step1Parameters;
  ParametersType Step2Parameters;

  Step1Parameters.SetSize(6);
  Step2Parameters.SetSize(8);

  Step1Parameters[0] = 30.0;
  Step1Parameters[1] = 10.0;
  Step1Parameters[2] = 0.0;
  Step1Parameters[3] = 0.015;
  Step1Parameters[4] = 0.009;
  Step1Parameters[5] = 1.2;

  Step2Parameters[0] = 2;
  Step2Parameters[1] = 50;
  Step2Parameters[2] = 1;
  Step2Parameters[3] = 70;
  Step2Parameters[4] = 0.2;
  Step2Parameters[5] = 30;
  Step2Parameters[6] = 0;
  Step2Parameters[7] = 0;

  this->SetStep1Parameters(Step1Parameters);
  this->SetStep2Parameters(Step2Parameters);
}

// template <class TLabel>
// const typename LabelToProSailParameters<TLabel>::ParametersType &
// LabelToProSailParameters<TLabel>
//::GetStep1Parameters()
//{
//   return this->m_Step1Parameters;
//}

// template <class TLabel>
// const typename LabelToProSailParameters<TLabel>::ParametersType &
// LabelToProSailParameters<TLabel>
//::GetStep2Parameters()
//{
//  return this->m_Step2Parameters;
//}


template <class TLabel>
void LabelToProSailParameters<TLabel>::GenerateData()
{
  ParametersType Step1Parameters = this->GetStep1Parameters();
  ParametersType Step2Parameters = this->GetStep2Parameters();
  otbMsgDevMacro(<< " Step1Parameters" << Step1Parameters << " Step2Parameters" << Step2Parameters);
}


} // end namespace otb

#endif

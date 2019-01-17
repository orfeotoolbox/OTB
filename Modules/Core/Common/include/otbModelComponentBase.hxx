/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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

#ifndef otbModelComponentBase_hxx
#define otbModelComponentBase_hxx

#include <iostream>

#include "otbMacro.h"
#include "otbModelComponentBase.h"

namespace otb
{
namespace Statistics
{

template<class TSample>
ModelComponentBase<TSample>
::ModelComponentBase()
{
  m_Sample = nullptr;
  m_PdfFunction = nullptr;
  m_CdfFunction = nullptr;
  m_SampleModified = 0;
}

template<class TSample>
void
ModelComponentBase<TSample>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Sample: ";
  if (m_Sample != nullptr)
    {
    os << m_Sample << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }

  os << indent << "Membership Function for pdf: ";
  if (m_PdfFunction != nullptr)
    {
    os << m_PdfFunction << std::endl;
    }
  else
    {
    os << "not instantiated yet." << std::endl;
    }

  os << indent << "Membership Function for cdf: ";
  if (m_CdfFunction != nullptr)
    {
    os << m_CdfFunction << std::endl;
    }
  else
    {
    os << "not instantiated yet." << std::endl;
    }

  os << indent << "Sample are modified and parameters updated: ";
  os << m_SampleModified << std::endl;
}

template <class TSample>
void
ModelComponentBase<TSample>
::ShowParameters(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Generic class of model-component. Parameters :\n";
  for (unsigned int i = 0; i < m_Parameters.Size(); ++i)
    os << indent << m_Parameters[i] << "\n";
}

template<class TSample>
void
ModelComponentBase<TSample>
::SetSample(const TSample* sample)
{
  m_Sample = sample;
  m_SampleModified = 1;
}

template<class TSample>
const TSample*
ModelComponentBase<TSample>
::GetSample() const
{
  return m_Sample;
}

template<class TSample>
void
ModelComponentBase<TSample>
::SetParameters(const ParametersType& parameters)
{
  if (m_Parameters != parameters) m_Parameters = parameters;
}

template<class TSample>
void
ModelComponentBase<TSample>
::SetPdfMembershipFunction(MembershipFunctionType* function)
{
  m_PdfFunction = function;
}

template<class TSample>
void
ModelComponentBase<TSample>
::SetCdfMembershipFunction(MembershipFunctionType* function)
{
  m_CdfFunction = function;
}

template<class TSample>
typename ModelComponentBase<TSample>::MembershipFunctionType*
ModelComponentBase<TSample>
::GetPdfMembershipFunction()
{
  return m_PdfFunction;
}

template<class TSample>
typename ModelComponentBase<TSample>::MembershipFunctionType*
ModelComponentBase<TSample>
::GetCdfMembershipFunction()
{
  return m_CdfFunction;
}

template<class TSample>
inline double
ModelComponentBase<TSample>
::Pdf(MeasurementVectorType& measurements)
{
  return this->m_PdfFunction->Evaluate(measurements);
}

template<class TSample>
inline double
ModelComponentBase<TSample>
::Cdf(MeasurementVectorType& measurements)
{
  return m_CdfFunction->Evaluate(measurements);
}

template <class TSample>
int
ModelComponentBase<TSample>
::IsSampleModified()
{
  return m_SampleModified;
}

template<class TSample>
void
ModelComponentBase<TSample>
::Update()
{
  if (m_SampleModified) this->GenerateData();
  m_SampleModified = 0;
}

template <class TSample>
void
ModelComponentBase<TSample>
::GenerateData()
{
  /** subclasses should override this function to perform
   *  parameter estimation. But it allows switching m_SampleModified
   *  when necessary.
   */
  m_SampleModified = 0;
}

} // end of namespace Statistics
} // end of namespace otb

#endif

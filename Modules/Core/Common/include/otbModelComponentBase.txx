/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are covered by the IMT copyright.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbModelComponentBase_txx
#define __otbModelComponentBase_txx

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
  m_Sample = 0;
  m_PdfFunction = 0;
  m_CdfFunction = 0;
  m_SampleModified = 0;
}

template<class TSample>
void
ModelComponentBase<TSample>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Sample: ";
  if (m_Sample != 0)
    {
    os << m_Sample << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }

  os << indent << "Membership Function for pdf: ";
  if (m_PdfFunction != 0)
    {
    os << m_PdfFunction << std::endl;
    }
  else
    {
    os << "not instantiated yet." << std::endl;
    }

  os << indent << "Membership Function for cdf: ";
  if (m_CdfFunction != 0)
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

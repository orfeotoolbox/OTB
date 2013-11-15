/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are covered by the GET copyright.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGaussianModelComponent_txx
#define __otbGaussianModelComponent_txx

#include <iostream>

#include "itkNumericTraits.h"
#include "otbMacro.h"
#include "otbGaussianModelComponent.h"

namespace otb
{
namespace Statistics
{

template<class TSample>
GaussianModelComponent<TSample>
::GaussianModelComponent()
{
  m_CovarianceEstimator = 0;
  m_GaussianMembershipFunction = 0;
}

template<class TSample>
void
GaussianModelComponent<TSample>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Mean Estimator: " << m_CovarianceEstimator << std::endl;
  os << indent << "Covariance Estimator: " << m_CovarianceEstimator << std::endl;
  os << indent << "GaussianMembershipFunction: " << m_GaussianMembershipFunction << std::endl;
}

template <class TSample>
void
GaussianModelComponent<TSample>
::ShowParameters(std::ostream& os, itk::Indent indent) const
{
  unsigned int i, j;
  os << indent << "Gaussian model component : \n";
  os << indent << "Mean : ";
  for (i = 0; i < m_Mean.Size(); ++i)
    os << m_Mean[i] << "\t";
  os << "\n" << indent << "Covariance : ";
  for (i = 0; i < m_Mean.Size(); ++i)
    {
    for (j = 0; j < m_Mean.Size(); ++j)
      os << m_Covariance(i, j) << "\t";
    os << "\n" << indent << "              ";
    }
  os << "\n";
}

template<class TSample>
void
GaussianModelComponent<TSample>
::SetSample(const TSample* sample)
{
  Superclass::SetSample(sample);
  const MeasurementVectorSizeType measurementVectorLength = sample->GetMeasurementVectorSize();
  this->m_Parameters.SetSize(measurementVectorLength * (1 + measurementVectorLength));

  // Set the size of the mean vector 
  m_Mean.SetSize(measurementVectorLength);


  // Set the parameters of the mean (internally) and the covariance estimator
  m_Covariance.SetSize(measurementVectorLength,
                       measurementVectorLength);

  m_CovarianceEstimator = CovarianceEstimatorType::New();
  m_CovarianceEstimator->SetInput(sample);
  m_CovarianceEstimator->Update();
  
  m_GaussianMembershipFunction = NativeMembershipFunctionType::New();
  this->m_PdfFunction = (MembershipFunctionType *) m_GaussianMembershipFunction;
  m_GaussianMembershipFunction->SetMeasurementVectorSize(
    measurementVectorLength);
  this->SetPdfMembershipFunction((MembershipFunctionType *)
                                 m_GaussianMembershipFunction.GetPointer());
}

template<class TSample>
void
GaussianModelComponent<TSample>
::SetParameters(const ParametersType& parameters)
{
  Superclass::SetParameters(parameters);

  unsigned int paramIndex = 0;
  unsigned int i, j;

  MeasurementVectorSizeType measurementVectorSize
    = this->GetSample()->GetMeasurementVectorSize();

  m_Mean.SetSize (measurementVectorSize);
  for (i = 0; i < measurementVectorSize; i++)
    {
    m_Mean[i] = parameters[paramIndex];
    paramIndex++;
    }
  
  m_Covariance.SetSize(measurementVectorSize, measurementVectorSize);
  for (i = 0; i < measurementVectorSize; i++)
    for (j = 0; j < measurementVectorSize; j++)
      {
      m_Covariance(i, j) = parameters[paramIndex];
      paramIndex++;
      }

  this->m_GaussianMembershipFunction->SetMean(m_Mean);
  this->m_GaussianMembershipFunction->SetCovariance(&m_Covariance);

}

template<class TSample>
void
GaussianModelComponent<TSample>
::GenerateData()
{
  if (this->IsSampleModified() == 0) return;

  MeasurementVectorSizeType measurementVectorSize = this->GetSample()->GetMeasurementVectorSize();

  unsigned int i, j;
  int          paramIndex  = 0;

  // Get the mean using the convariance estimator (computed internally)
  typename CovarianceEstimatorType::MeasurementVectorType meanOutput = m_CovarianceEstimator->GetMean();
  
  for (i = 0; i < measurementVectorSize; i++)
    {
    m_Mean.SetElement(i,meanOutput.GetElement(i));
    this->m_Parameters[paramIndex] = meanOutput.GetElement(i);
    ++paramIndex;
    }

  // Get the covariance matrix and fill the parameters vector
  const typename CovarianceEstimatorType::MatrixType covariance = m_CovarianceEstimator->GetCovarianceMatrix();
  
  for (i = 0; i < measurementVectorSize; i++)
    for (j = 0; j < measurementVectorSize; j++)
      {
      this->m_Parameters[paramIndex] = covariance.GetVnlMatrix().get(i, j);
      m_Covariance(i, j)             = covariance.GetVnlMatrix().get(i, j);
      paramIndex++;
      }
  
  this->m_GaussianMembershipFunction->SetMean(meanOutput);
  this->m_GaussianMembershipFunction->SetCovariance(m_Covariance);

  Superclass::GenerateData();
}

} // end of namespace Statistics
} // end of namesapce otb

#endif

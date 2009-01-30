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

template< class TSample >
GaussianModelComponent< TSample >
::GaussianModelComponent()
{
  m_MeanEstimator = 0;
  m_CovarianceEstimator = 0;
  m_GaussianDensityFunction = 0;
}

template< class TSample >
void
GaussianModelComponent< TSample >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Mean Estimator: " << m_MeanEstimator << std::endl;
  os << indent << "Covariance Estimator: " << m_CovarianceEstimator << std::endl;
  os << indent << "GaussianDensityFunction: " << m_GaussianDensityFunction << std::endl;
}

template < class TSample >
void
GaussianModelComponent< TSample >
::ShowParameters ( std::ostream& os, itk::Indent indent) const
{
  unsigned int i,j;
  os << indent << "Gaussian model component : \n";
  os << indent << "Mean : ";
  for ( i = 0; i < m_Mean.Size(); i++ )
    os << m_Mean[i] << "\t";
  os << "\n" << indent << "Covariance : ";
  for ( i = 0; i < m_Mean.Size(); i++ )
  {
    for ( j = 0; j < m_Mean.Size(); j++ )
      os << m_Covariance(i,j) << "\t";
    os << "\n" << indent << "              ";
  }
  os << "\n";
}

template< class TSample >
void
GaussianModelComponent< TSample >
::SetSample(const TSample* sample)
{
  Superclass::SetSample(sample);

  const MeasurementVectorSizeType measurementVectorLength
  = sample->GetMeasurementVectorSize();

  this->m_Parameters.SetSize( measurementVectorLength
                              * ( 1 + measurementVectorLength ) );

  m_Mean.SetSize( measurementVectorLength );

  m_MeanEstimator = MeanEstimatorType::New();
  m_MeanEstimator->SetInputSample(sample);
  //m_MeanEstimator->Update();

  m_Covariance.SetSize( measurementVectorLength,
                        measurementVectorLength );

  m_CovarianceEstimator = CovarianceEstimatorType::New();
  m_CovarianceEstimator->SetInputSample(sample);
  //m_CovarianceEstimator->SetMean( &m_Mean );
  //m_CovarianceEstimator->Update();

  m_GaussianDensityFunction = NativeMembershipFunctionType::New();
  this->m_PdfFunction = (MembershipFunctionType *) m_GaussianDensityFunction;
  m_GaussianDensityFunction->SetMeasurementVectorSize(
    measurementVectorLength );
  this->SetPdfMembershipFunction( (MembershipFunctionType *)
                                  m_GaussianDensityFunction.GetPointer() );

}

template< class TSample >
void
GaussianModelComponent< TSample >
::SetParameters(const ParametersType &parameters)
{
  Superclass::SetParameters(parameters);

  unsigned int paramIndex = 0;
  unsigned int i, j;

  MeasurementVectorSizeType measurementVectorSize
  = this->GetSample()->GetMeasurementVectorSize();

  m_Mean.SetSize ( measurementVectorSize );
  for ( i = 0; i < measurementVectorSize; i++)
  {
    m_Mean[i] = parameters[paramIndex];
    ++paramIndex;
  }

  m_Covariance.SetSize( measurementVectorSize, measurementVectorSize );
  for ( i = 0; i < measurementVectorSize; i++ )
    for ( j = 0; j < measurementVectorSize; j++ )
    {
      m_Covariance(i, j) = parameters[paramIndex];
      ++paramIndex;
    }

  this->m_GaussianDensityFunction->SetMean(&m_Mean);
  this->m_GaussianDensityFunction->SetCovariance(&m_Covariance);

}

template< class TSample >
void
GaussianModelComponent< TSample >
::GenerateData ()
{
  if ( this->IsSampleModified() == 0 )
    return;

  MeasurementVectorSizeType measurementVectorSize
  = this->GetSample()->GetMeasurementVectorSize();

  unsigned int i, j;
  int paramIndex  = 0;

  m_MeanEstimator->Update();
  // m_Mean.SetSize( m_MeanEstimator->GetOutput()->GetSize() );

  MeanType * mean = m_MeanEstimator->GetOutput();
  for ( i = 0; i < measurementVectorSize; i++)
  {
    this->m_Parameters[paramIndex] = m_Mean[i] = mean->GetElement(i);
    ++paramIndex;
  }

  m_CovarianceEstimator->SetMean(&m_Mean);
  m_CovarianceEstimator->Update();
  // m_Covariance.SetSize( m_CovarianceEstimator->GetOutput()->Rows(),
  //     m_CovarianceEstimator->GetOutput()->Cols() );

  const CovarianceType * covariance = m_CovarianceEstimator->GetOutput();

  for ( i = 0; i < measurementVectorSize; i++ )
    for ( j = 0; j < measurementVectorSize; j++ )
    {
      this->m_Parameters[paramIndex]
      = m_Covariance(i,j)
        = covariance->GetVnlMatrix().get(i, j);
      ++paramIndex;
    }

  this->m_GaussianDensityFunction->SetMean(&m_Mean);
  this->m_GaussianDensityFunction->SetCovariance(&m_Covariance);

  Superclass::GenerateData();
}

} // end of namespace Statistics
} // end of namesapce otb

#endif




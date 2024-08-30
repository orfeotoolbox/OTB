/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbGaussianModelComponent_h
#define otbGaussianModelComponent_h

#include "itkCovarianceSampleFilter.h"
#include "itkGaussianMembershipFunction.h"

#include "otbModelComponentBase.h"

namespace otb
{
namespace Statistics
{

/** \class GaussianModelComponent
 * \brief is a component (derived from ModelComponentBase) for
 * Gaussian class. This class is used in SEMClassifier
 *
 * <b>Recent API changes:</b>
 * N/A
 *
 * \sa ModelComponentBase, SEMClassifier
 *
 * \ingroup OTBLearningBase
 */

template <class TSample>
class ITK_EXPORT GaussianModelComponent : public ModelComponentBase<TSample>
{
public:
  /**Standard class typedefs. */
  typedef GaussianModelComponent        Self;
  typedef ModelComponentBase<TSample>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /**Standard Macros */
  itkNewMacro(Self);
  itkTypeMacro(GaussianModelComponent, ModelComponentBase);

  /** Typedefs from the superclass */
  typedef typename Superclass::MeasurementVectorType     MeasurementVectorType;
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;
  typedef typename Superclass::MembershipFunctionType    MembershipFunctionType;
  typedef typename Superclass::ParametersType            ParametersType;

  /** Type of the membership function. Gaussian density function */
  typedef itk::Statistics::GaussianMembershipFunction<MeasurementVectorType> NativeMembershipFunctionType;
  typedef typename NativeMembershipFunctionType::MeanVectorType              MeanVectorType;

  /** Types of the mean and the covariance calculator that will update
   *  this component's distribution parameters */
  typedef itk::Statistics::CovarianceSampleFilter<TSample> CovarianceEstimatorType;

  /** types of the covariance to be used by the
   *  NativeMembershipFunctionType */
  typedef typename CovarianceEstimatorType::MatrixType CovarianceType;

  /** Sets the input sample */
  void SetSample(const TSample* sample) override;

  /** Sets the component's distribution parameters.
   *  e.g. Then user can call directly Pdf( MeasurementVectorType & )  */
  void SetParameters(const ParametersType& parameters);

  /** Show the parameters in a minimal form in comparison to PrintSelf */
  void ShowParameters(std::ostream& os, itk::Indent indent) const override;

protected:
  GaussianModelComponent();
  ~GaussianModelComponent() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData() override;

private:
  GaussianModelComponent(const Self&) = delete;
  void operator=(const Self&) = delete;

  typename NativeMembershipFunctionType::Pointer m_GaussianMembershipFunction;
  // TODO add a m_GaussianCumulativeFunction
  typename CovarianceEstimatorType::Pointer m_CovarianceEstimator;

  MeanVectorType m_Mean;
  CovarianceType m_Covariance;

}; // end of class

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGaussianModelComponent.hxx"
#endif

#endif

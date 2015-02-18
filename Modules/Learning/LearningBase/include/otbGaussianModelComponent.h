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
#ifndef __otbGaussianModelComponent_h
#define __otbGaussianModelComponent_h

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
 */

template<class TSample>
class ITK_EXPORT GaussianModelComponent :
  public ModelComponentBase<TSample>
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
  typedef typename Superclass::MeasurementVectorType MeasurementVectorType;
  typedef typename Superclass::MeasurementVectorSizeType
  MeasurementVectorSizeType;
  typedef typename Superclass::MembershipFunctionType MembershipFunctionType;
  typedef typename Superclass::ParametersType         ParametersType;

  /** Type of the membership function. Gaussian density function */
  typedef itk::Statistics::GaussianMembershipFunction<MeasurementVectorType>
  NativeMembershipFunctionType;
  typedef typename NativeMembershipFunctionType::MeanVectorType  MeanVectorType;

  /** Types of the mean and the covariance calculator that will update
   *  this component's distribution parameters */
  typedef itk::Statistics::CovarianceSampleFilter<TSample> CovarianceEstimatorType;

  /** types of the covariance to be used by the
   *  NativeMembershipFunctionType */
  typedef typename CovarianceEstimatorType::MatrixType CovarianceType;

  /** Sets the input sample */
  virtual void SetSample(const TSample* sample);

  /** Sets the component's distribution parameters.
   *  e.g. Then user can call directly Pdf( MeasurementVectorType & )  */
  void SetParameters(const ParametersType& parameters);

  /** Show the parameters in a minimal form in comparison to PrintSelf */
  virtual void ShowParameters(std::ostream& os, itk::Indent indent) const;

protected:
  GaussianModelComponent();
  virtual ~GaussianModelComponent() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void GenerateData();

private:
  GaussianModelComponent(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename NativeMembershipFunctionType::Pointer m_GaussianMembershipFunction;
  // TODO ajouter un m_GaussianCumulativeFunction
  typename CovarianceEstimatorType::Pointer m_CovarianceEstimator;

  MeanVectorType  m_Mean;
  CovarianceType  m_Covariance;

}; // end of class

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGaussianModelComponent.txx"
#endif

#endif

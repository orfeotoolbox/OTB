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

#ifndef otbSVMCrossValidationCostFunction_h
#define otbSVMCrossValidationCostFunction_h

#include "itkSingleValuedCostFunction.h"

namespace otb
{

/** \class SVMCrossValidationCostFunction
 * \brief This function returns the cross validation accuracy of a SVM model.
 *
 * It allows performing parameters selection to maximize the
 * accuracy using ITK optimizers.
 *
 * Given an input SVM model, the number of parameters depends on the
 * type of kernel:
 * - Linear kernel : C
 * - Polynomial kernel : C, gamma, coef0
 * - RBF kernel : C, Gamma
 * - Gaussian kernel : C, Gamma, coef0
 * - Other kernels : C
 *
 * Please note that this function is only defined on \f$ R_{+}^{*} \f$.
 *
 * The GetDerivative() uses the GetValue() function to
 * compute the partial derivatives. as such, it can be quite intensive.
 *
 * \ingroup ClassificationFilters
 *
 * \ingroup OTBSupervised
 */
template <class TModel>
class ITK_EXPORT SVMCrossValidationCostFunction : public itk::SingleValuedCostFunction
{
public:
  /** Standard class typedefs. */
  typedef SVMCrossValidationCostFunction Self;
  typedef itk::SingleValuedCostFunction  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type definitions for the SVM Model. */
  typedef TModel                         SVMModelType;
  typedef typename SVMModelType::Pointer SVMModelPointer;

  /** Superclass typedefs */
  typedef typename Superclass::MeasureType         MeasurementType;
  typedef typename Superclass::ParametersType      ParametersType;
  typedef typename Superclass::ParametersValueType ParametersValueType;
  typedef typename Superclass::DerivativeType      DerivativeType;

  /** Set the model */
  itkSetObjectMacro(Model, SVMModelType);
  itkGetObjectMacro(Model, SVMModelType);

  /** Set/Get the derivative step */
  itkSetMacro(DerivativeStep, ParametersValueType);
  itkGetMacro(DerivativeStep, ParametersValueType);

  /** \return The accuracy value corresponding the parameters */
  MeasureType GetValue(const ParametersType& parameters) const override;

  /** \return The accuracy derivative corresponding to the parameters */
  void GetDerivative(const ParametersType& parameters, DerivativeType& derivative) const override;

  /** \return the number of parameters to optimize */
  unsigned int GetNumberOfParameters(void) const override;

protected:
  /// Constructor
  SVMCrossValidationCostFunction();
  /// Destructor
  ~SVMCrossValidationCostFunction() override;

  /** Update svm parameters struct according to the input parameters
    */
  void UpdateParameters(const ParametersType& parameters) const;

private:
  SVMCrossValidationCostFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /**Pointer to the SVM model to optimize */
  SVMModelPointer m_Model;

  /** Step used to compute the derivatives */
  ParametersValueType m_DerivativeStep;

}; // class SVMCrossValidationCostFunction

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMCrossValidationCostFunction.hxx"
#endif

#endif

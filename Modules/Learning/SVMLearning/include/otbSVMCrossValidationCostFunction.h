/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSVMCrossValidationCostFunction_h
#define __otbSVMCrossValidationCostFunction_h

#include "otbSVMModel.h"
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
 * \ingroup OTBSVMLearning
 */
template <class TModel>
class ITK_EXPORT SVMCrossValidationCostFunction
  : public itk::SingleValuedCostFunction
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

  /** Set/Get the number of cross validation folders */
  itkSetMacro(NumberOfCrossValidationFolders, unsigned int);
  itkGetMacro(NumberOfCrossValidationFolders, unsigned int);

  /** Set/Get the derivative step */
  itkSetMacro(DerivativeStep, ParametersValueType);
  itkGetMacro(DerivativeStep, ParametersValueType);

  /** \return The accuracy value corresponding the parameters */
  virtual MeasureType GetValue(const ParametersType& parameters) const;

  /** \return The accuracy derivative corresponding to the parameters */
  virtual void GetDerivative(const ParametersType& parameters, DerivativeType& derivative) const;

  /** \return the number of parameters to optimize */
  virtual unsigned int GetNumberOfParameters(void) const;

protected:
  /// Constructor
  SVMCrossValidationCostFunction();
  /// Destructor
  virtual ~SVMCrossValidationCostFunction();

  /** Update svm parameters struct according to the input parameters
    */
  virtual void UpdateParameters(struct svm_parameter& svm_parameters, const ParametersType& parameters) const;

private:
  SVMCrossValidationCostFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /**Pointer to the SVM model to optimize */
  SVMModelPointer m_Model;

  /** Number of cross validation folders */
  unsigned int m_NumberOfCrossValidationFolders;

  /** Step used to compute the derivatives */
  ParametersValueType m_DerivativeStep;

}; // class SVMCrossValidationCostFunction

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMCrossValidationCostFunction.txx"
#endif

#endif

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
#ifndef __otbSVMCrossValidationCostFunction_txx
#define __otbSVMCrossValidationCostFunction_txx

#include "otbSVMCrossValidationCostFunction.h"

namespace otb
{
  template<class TModel>
  SVMCrossValidationCostFunction<TModel>
  ::SVMCrossValidationCostFunction() : m_Model(), m_NumberOfCrossValidationFolders(10), m_DerivativeStep(0.001)
  {}
  template<class TModel>
  SVMCrossValidationCostFunction<TModel>
  ::~SVMCrossValidationCostFunction()
  {}
  template<class TModel>
  typename SVMCrossValidationCostFunction<TModel>
  ::MeasureType 
  SVMCrossValidationCostFunction<TModel>
  ::GetValue(const ParametersType &parameters) const
  {
    // Check the input model
    if(!m_Model)
      {
      itkExceptionMacro(<<"Model is null, can not evaluate accuracy.");
      }
    
    // Check for a positive and non-null C
    if(parameters[0]<=0)
      {
      return 0;
      }
    
    // Get a copy of the problem
    struct svm_problem svm_problem = m_Model->GetProblem();
    
    // Get a copy of the parameters
    struct svm_parameter svm_parameters = m_Model->GetParameters();

    // Update svm_parameters according to current parameters
    this->UpdateParameters(svm_parameters,parameters);

    // check problem and parameters consistancy
    const char* error_msg = svm_check_parameter(&svm_problem,&svm_parameters);

    // handle errors
    if (error_msg)
      {
      throw itk::ExceptionObject(__FILE__, __LINE__,error_msg,ITK_LOCATION);
      }

    // Get the length of the problem
    int length = m_Model->GetProblem().l;
    
    // Temporary memory to store cross validation results
    double *target = Malloc(double,length);

    // Do cross validation
    svm_cross_validation(&svm_problem,&svm_parameters,m_NumberOfCrossValidationFolders,target);

    // Evaluate accuracy
    int i;
    double total_correct = 0.;
    
    for(i=0;i<length;i++)
      { 
      if(target[i] == m_Model->GetProblem().y[i])
	{
	++total_correct;
	}
      }
    MeasureType accuracy = total_correct/length;
    
    // Free temporary memory
    free(target);

    // return accuracy value
    return accuracy;
  }

  template<class TModel>
  void 
  SVMCrossValidationCostFunction<TModel>
  ::GetDerivative(const ParametersType &parameters, DerivativeType &derivative) const
  {
    // Set derivative size
    derivative.SetSize(parameters.Size());
    derivative.Fill(itk::NumericTraits<ParametersValueType>::Zero);

    for(unsigned int i = 0; i < parameters.Size();++i)
      {
      MeasureType y1, y2;
      ParametersType x1, x2;
      
      x1 = parameters;
      x1[i]-=m_DerivativeStep;
      y1 = this->GetValue(x1);

      x2 = parameters;
      x2[i]+=m_DerivativeStep;
      y2 = this->GetValue(x2);

      derivative[i]=(y2-y1)/(2*m_DerivativeStep);
      std::cout<<"x1= "<<x1<<" x2= "<<x2<<", y1= "<<y1<<", y2= "<<y2<<std::endl;
      }
    std::cout<<"Position: "<<parameters<<", Value: "<<this->GetValue(parameters)<<", Derivatives: "<<derivative<<std::endl;
  }

  template<class TModel>
  unsigned int 
  SVMCrossValidationCostFunction<TModel>
  ::GetNumberOfParameters (void) const
  {
  if(!m_Model)
    {
     itkExceptionMacro(<<"Model is null, can not evaluate number of parameters.");
    }

  switch(m_Model->GetKernelType())
    {
    case LINEAR:
      // C
      return 1;

    case POLY:
      // C, gamma and coef0
      return 3;

    case RBF:
      // C and gamma
      return 2;

    case SIGMOID:
      // C, gamma and coef0
      return 3;

    default:
      // C
      return 1;   
    }
  }

  template<class TModel>
  void 
  SVMCrossValidationCostFunction<TModel>
  ::UpdateParameters(struct svm_parameter& svm_parameters, const ParametersType & parameters) const
  {
    switch(m_Model->GetKernelType())
      {
      case LINEAR:
	// C
	svm_parameters.C = parameters[0];
	break;

      case POLY:
	// C, gamma and coef0
	svm_parameters.C = parameters[0];
	svm_parameters.gamma = parameters[1];
	svm_parameters.coef0 = parameters[2];
	break;

      case RBF:
	// C and gamma
	svm_parameters.C = parameters[0];
	svm_parameters.gamma = parameters[1];
	break;

      case SIGMOID:
	// C, gamma and coef0
	svm_parameters.C = parameters[0];
	svm_parameters.gamma = parameters[1];
	svm_parameters.coef0 = parameters[2];
	break;

      default:
	svm_parameters.C = parameters[0];
	break;
      }
  }

} // namespace otb

#endif


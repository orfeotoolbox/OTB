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
#include <iostream>
#include <fstream>

#include "otbExhaustiveExponentialOptimizer.h"

#include "itkArray.h"

/** class Quadratic2DCostFunction
 * brief: Class for 2D quadratic function evaluation.
 * This class represent a 2 quadratic function which is defined by
 * a * x^2 + b * y^2 + c * x * y + d * x + e * y + f
 * a, b, c, d, e, f are the model internal parameters
 * x, y are the parameters of this function
 *  */

class Quadratic2DCostFunction
 : public itk::SingleValuedCostFunction
{
public:
 /** Standard class typedefs. */
   typedef Quadratic2DCostFunction        Self;
   typedef itk::SingleValuedCostFunction  Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

  /** Type definitions for the quadratic Model. */
  typedef Superclass::MeasureType         MeasureType;
  typedef Superclass::ParametersType      ParametersType;
  typedef Superclass::ParametersValueType ParametersValueType;

  /** Type definitions for the quadratic Model. */
  typedef ParametersValueType ModelValueType;
  typedef ParametersType      ModelType;

  /** Set the parameters of the quadratic function */
  void SetFunctionInternalParameters(ModelValueType aCoef, ModelValueType bCoef, ModelValueType cCoef, ModelValueType dCoef, ModelValueType eCoef, ModelValueType fCoef)
  {
    m_FunctionInternalParameters[0] =  aCoef;
    m_FunctionInternalParameters[1] =  bCoef;
    m_FunctionInternalParameters[2] =  cCoef;
    m_FunctionInternalParameters[3] =  dCoef;
    m_FunctionInternalParameters[4] =  eCoef;
    m_FunctionInternalParameters[5] =  fCoef;
  };

  void SetFunctionInternalParameters(ModelType quadraticModel2D)
  {
    this->SetFunctionInternalParameters(quadraticModel2D[0], quadraticModel2D[1], quadraticModel2D[2], quadraticModel2D[3], quadraticModel2D[4], quadraticModel2D[5]);
  };

  /** \return The accuracy value corresponding the parameters */
  virtual MeasureType GetValue(const ParametersType& parameters) const
  {
    return static_cast<MeasureType> ( m_FunctionInternalParameters.GetElement(0) * parameters.GetElement(0) * parameters.GetElement(0) +
                                 m_FunctionInternalParameters.GetElement(1) * parameters.GetElement(1) * parameters.GetElement(1) +
                                 m_FunctionInternalParameters.GetElement(2) * parameters.GetElement(0) * parameters.GetElement(1) +
                                 m_FunctionInternalParameters.GetElement(3) * parameters.GetElement(0) +
                                 m_FunctionInternalParameters.GetElement(4) * parameters.GetElement(1) +
                                 m_FunctionInternalParameters.GetElement(5) );
  };

  /** \return the number of parameters to optimize */
  virtual unsigned int GetNumberOfParameters(void) const {return 2; };

  /** \return The accuracy derivative corresponding to the parameters */
  virtual void GetDerivative(const ParametersType& parameters, DerivativeType& derivative) const {};

  /// Constructor
  Quadratic2DCostFunction()
  {
    m_FunctionInternalParameters = ModelType(6);
    m_FunctionInternalParameters.Fill(0.0);
  };
  /// Destructor
  virtual ~Quadratic2DCostFunction(){};

private:
  Quadratic2DCostFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ModelType m_FunctionInternalParameters;

}; // class Quadratic2DCostFunction


int otbExhaustiveExponentialOptimizerTest(int argc, char* argv[])
{
  Quadratic2DCostFunction::Pointer costFunction = Quadratic2DCostFunction::New();

  costFunction->SetFunctionInternalParameters(1.0, 1.0, 0.0, -6.0, 4.0, 13.0); // (x-3)^2 + (y+2)^2 => solution: x=3 and y=-2

  typedef Quadratic2DCostFunction::ParametersType  ParametersType;
  ParametersType initialPosition = ParametersType(2);
  initialPosition[0] = 4.5;
  initialPosition[1] = -5.3;
  std::cout << "initial position: " << costFunction->GetValue(initialPosition) << std::endl;

  otb::ExhaustiveExponentialOptimizer::Pointer optimizer = otb::ExhaustiveExponentialOptimizer::New();

  typedef otb::ExhaustiveExponentialOptimizer::StepsType StepOptimizerType;
  StepOptimizerType    nbSteps(initialPosition.Size());
  nbSteps.Fill(5);

  optimizer->SetNumberOfSteps(nbSteps);
  optimizer->SetCostFunction(costFunction);
  optimizer->SetInitialPosition(initialPosition);
  optimizer->StartOptimization();

  std::cout << "Minimum cost function founded: " << optimizer->GetMinimumMetricValue() << " " <<
    optimizer->GetMinimumMetricValuePosition() << std::endl;
  std::cout << "Maximum cost function founded: " << optimizer->GetMaximumMetricValue() << " " <<
    optimizer->GetMaximumMetricValuePosition() << std::endl;

  otb::ExhaustiveExponentialOptimizer::Pointer fineoptimizer = otb::ExhaustiveExponentialOptimizer::New();

  double stepLength = 1. / static_cast<double>(5);

  fineoptimizer->SetNumberOfSteps(nbSteps);
  fineoptimizer->SetStepLength(stepLength);
  fineoptimizer->SetCostFunction(costFunction);
  fineoptimizer->SetInitialPosition(optimizer->GetMinimumMetricValuePosition());
  fineoptimizer->StartOptimization();

  std::cout << "fine minimum cost function founded: " << fineoptimizer->GetMinimumMetricValue() << " " <<
    fineoptimizer->GetMinimumMetricValuePosition() << std::endl;
  std::cout << "fine maximum cost function founded: " << fineoptimizer->GetMaximumMetricValue() << " " <<
    fineoptimizer->GetMaximumMetricValuePosition() << std::endl;

  std::ofstream file;
  file.open(argv[1]);

  file << "initial value: " << costFunction->GetValue(initialPosition) << ", position: "
       << initialPosition <<"\n";
  file << "minimum: " << fineoptimizer->GetMinimumMetricValue() << ", position: " <<
      fineoptimizer->GetMinimumMetricValuePosition() << "\n";
  file << "maximum: " << fineoptimizer->GetMaximumMetricValue() << ", position: " <<
        fineoptimizer->GetMaximumMetricValuePosition() << "\n";

  file.close();

  return EXIT_SUCCESS;
}

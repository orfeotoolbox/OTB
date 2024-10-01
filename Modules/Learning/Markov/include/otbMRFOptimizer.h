/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMRFOptimizer_h
#define otbMRFOptimizer_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkArray.h"

namespace otb
{
/**
 * \class MRFOptimizer
 * \brief This is the base class for optimizer used in the MRF framework.
 *
 * Derived class must reimplement Compute() method.
 *
 * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
 *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

class ITK_EXPORT MRFOptimizer : public itk::Object
{
public:
  typedef MRFOptimizer                  Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Array<double>            ParametersType;

  itkTypeMacro(MRFOptimizer, itk::Object);

  itkGetConstMacro(NumberOfParameters, unsigned int);

  // Get the parameters
  const ParametersType& GetParameters(void) const
  {
    return this->m_Parameters;
  }

  virtual void SetParameters(const ParametersType& parameters)
  {
    if (parameters.GetSize() != m_NumberOfParameters)
    {
      itkExceptionMacro(<< "Invalid number of parameters (" << parameters.GetSize() << " , " << m_NumberOfParameters << ")");
    }
    m_Parameters = parameters;
    this->Modified();
  }

  virtual bool Compute(double deltaEnergy) = 0;

protected:
  MRFOptimizer() : m_NumberOfParameters(1), m_Parameters(1)
  {
  }
  ~MRFOptimizer() override
  {
  }
  unsigned int   m_NumberOfParameters;
  ParametersType m_Parameters;
};
}

#endif

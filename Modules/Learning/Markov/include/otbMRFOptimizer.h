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
#ifndef __otbMRFOptimizer_h
#define __otbMRFOptimizer_h

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
      itkExceptionMacro(
        << "Invalid number of parameters (" << parameters.GetSize() << " , " << m_NumberOfParameters << ")");
      }
    m_Parameters = parameters;
    this->Modified();
  }

  virtual bool Compute(double deltaEnergy) = 0;

protected:
  MRFOptimizer() :
    m_NumberOfParameters(1),
    m_Parameters(1) {}
  virtual ~MRFOptimizer() {}
  unsigned int   m_NumberOfParameters;
  ParametersType m_Parameters;

};
}

#endif

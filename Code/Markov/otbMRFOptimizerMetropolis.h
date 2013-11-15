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
#ifndef __otbMRFOptimizerMetropolis_h
#define __otbMRFOptimizerMetropolis_h

#include "otbMRFOptimizer.h"
#include "otbMath.h"
#include "itkNumericTraits.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
/**
 * \class MRFOptimizerMetropolis
 * \brief This is the optimizer class implementing the Metropolis algorithm
 *
 * This is one optimizer to be used in the MRF framework. This optimizer
 * follows the metropolis algorithm to accept of reject the value proposed by the sampler.
 *
 * The MRFOptimizerMetropolis has one parameter corresponding to the temperature T used
 * to accept or reject proposed values. The proposed value is accepted with a probability:
 *
 *  \f[ e^{\frac{-\Delta E}{T}} \f]
 *
 *
 * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
 *
 * \ingroup Markov
 */

class ITK_EXPORT MRFOptimizerMetropolis : public MRFOptimizer
{
public:

  typedef MRFOptimizerMetropolis        Self;
  typedef MRFOptimizer                  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef Superclass::ParametersType    ParametersType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  itkNewMacro(Self);

  itkTypeMacro(MRFOptimizerMetropolis, MRFOptimizer);

  /** Set parameter to a one array filled with paramVal.*/
  void SetSingleParameter(double parameterVal)
  {
    this->m_Parameters.SetSize(1);
    this->m_Parameters.Fill(parameterVal);
    this->Modified();
  }

  inline bool Compute(double deltaEnergy)
  {
    if (deltaEnergy < 0)
      {
      return true;
      }
    if (deltaEnergy == 0)
      {
      return false;
      }
    else
      {
      double proba = vcl_exp(-(deltaEnergy) / this->m_Parameters[0]);
      if ((m_Generator->GetIntegerVariate() % 10000) < proba * 10000)
        {
        return true;
        }
      }
    return false;
  }

  /** Methods to cancel random effects.*/
  void InitializeSeed(int seed)
  {
    m_Generator->SetSeed(seed);
  }
  void InitializeSeed()
  {
    m_Generator->SetSeed();
  }

protected:
  MRFOptimizerMetropolis()
    {
    this->m_NumberOfParameters = 1;
    this->m_Parameters.SetSize(1);
    this->m_Parameters[0] = 1.0;
    m_Generator = RandomGeneratorType::GetInstance();
    m_Generator->SetSeed();
    }
  virtual ~MRFOptimizerMetropolis() {}
  RandomGeneratorType::Pointer m_Generator;
};

}

#endif

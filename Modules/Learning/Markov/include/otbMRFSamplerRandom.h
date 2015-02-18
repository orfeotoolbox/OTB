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
#ifndef __otbMRFSamplerRandom_h
#define __otbMRFSamplerRandom_h

#include "otbMRFSampler.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
   * \class MRFSamplerRandom
   * \brief This is the base class for sampler methods used in the MRF framework.
   *
   * This is one sampler to be used int he MRF framework. This sampler select the
   * value randomly according to a uniform probability.
   *
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
   * \ingroup Markov
 */

template<class TInput1, class TInput2>
class ITK_EXPORT MRFSamplerRandom : public MRFSampler<TInput1, TInput2>
{
public:
  typedef MRFSamplerRandom                  Self;
  typedef otb::MRFSampler<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
  typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
  typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
  typedef typename Superclass::InputImagePixelType               InputImagePixelType;
  typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
  typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
  typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
  typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  itkNewMacro(Self);

  itkTypeMacro(MRFSamplerRandom, MRFSampler);

  inline int Compute(const InputImageNeighborhoodIterator& itData, const LabelledImageNeighborhoodIterator& itRegul)
  {
    this->m_EnergyBefore = this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
    this->m_EnergyBefore += this->m_Lambda
                            * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());

    this->m_Value = static_cast<LabelledImagePixelType>(m_Generator->GetIntegerVariate() % this->m_NumberOfClasses);
    this->m_EnergyAfter = this->m_EnergyFidelity->GetValue(itData, this->m_Value);
    this->m_EnergyAfter +=  this->m_Lambda * this->m_EnergyRegularization->GetValue(itRegul, this->m_Value);
    this->m_DeltaEnergy =  this->m_EnergyAfter - this->m_EnergyBefore;

    return 0;
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
  // The constructor and destructor.
  MRFSamplerRandom()
    {
    m_Generator = RandomGeneratorType::GetInstance();
    m_Generator->SetSeed();
    }
  virtual ~MRFSamplerRandom() {}

private:
  RandomGeneratorType::Pointer m_Generator;
};
}

#endif

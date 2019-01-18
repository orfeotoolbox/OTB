/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbMRFSamplerMAP_h
#define otbMRFSamplerMAP_h

#include "otbMRFSampler.h"

namespace otb
{
/**
   * \class MRFSamplerMAP
   * \brief This is the base class for sampler methods used in the MRF framework.
   *
   * This is one sampler to be used int he MRF framework. This sampler select the
   * value which maximizes the apriori probability (minimum energy).
   *
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
   * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template<class TInput1, class TInput2>
class ITK_EXPORT MRFSamplerMAP : public MRFSampler<TInput1, TInput2>
{
public:

  typedef MRFSamplerMAP                 Self;
  typedef MRFSampler<TInput1, TInput2>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
  typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
  typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
  typedef typename Superclass::InputImagePixelType               InputImagePixelType;
  typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
  typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
  typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
  typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;

  itkNewMacro(Self);

  itkTypeMacro(MRFSamplerMAP, MRFSampler);

  inline int Compute(const InputImageNeighborhoodIterator& itData,
                     const LabelledImageNeighborhoodIterator& itRegul) override
  {
    if (this->m_NumberOfClasses == 0)
      {
      itkExceptionMacro(<< "NumberOfClasse has to be greater than 0.");
      }

    this->m_EnergyBefore = this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
    this->m_EnergyBefore += this->m_Lambda
                            * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());

    //Try all possible value (how to be generic ?)
    this->m_EnergyAfter = this->m_EnergyBefore; //default values to current one
    this->m_Value = itRegul.GetCenterPixel();

    LabelledImagePixelType valueCurrent = 0;
    while (valueCurrent < static_cast<LabelledImagePixelType>(this->GetNumberOfClasses()) && valueCurrent !=
           itk::NumericTraits<LabelledImagePixelType>::max())
      {
      this->m_EnergyCurrent = this->m_EnergyFidelity->GetValue(itData, valueCurrent);
      this->m_EnergyCurrent += this->m_Lambda
                               * this->m_EnergyRegularization->GetValue(itRegul, valueCurrent);
      if (this->m_EnergyCurrent < this->m_EnergyAfter)
        {
        this->m_EnergyAfter = this->m_EnergyCurrent;
        this->m_Value = valueCurrent;
        }
      valueCurrent++;
      }

    this->m_DeltaEnergy =  this->m_EnergyAfter - this->m_EnergyBefore;

    return 0;
  }

protected:
  // The constructor and destructor.
  MRFSamplerMAP() {};
  ~MRFSamplerMAP() override {}

};

}

#endif

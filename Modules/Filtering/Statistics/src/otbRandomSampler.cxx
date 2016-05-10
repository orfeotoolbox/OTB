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

#include "otbRandomSampler.h"
#include "otbMath.h"

namespace otb
{

bool
RandomSampler::ParameterType::operator!=(const RandomSampler::ParameterType & param) const
{
  return bool(Adaptative != param.Adaptative);
}

void
RandomSampler::Reset(void)
{
  Superclass::Reset();
  if (this->m_Parameters.Adaptative && this->GetTotalElements() == 0UL)
    {
    itkExceptionMacro("Adaptative mode needs the total number of elements to be set!");
    }
}

bool
RandomSampler::TakeSample(void)
{
  bool ret = false;
  this->m_ProcessedElements += 1UL;
  if (this->m_ProcessedElements >= this->GetTotalElements() ||
        this->m_ChosenElements >= this->GetNeededElements())
        return false;
        
  double rate = this->GetRate();
  if (this->m_Parameters.Adaptative)
    {
      rate = double(this->GetNeededElements() - this->m_ChosenElements) /
            double(this->GetTotalElements() - this->m_ProcessedElements);
    }

  if (rate > m_Generator->GetUniformVariate(0.0, 1.0))
    {
    ret = true;
    this->m_ChosenElements += 1UL;
    }
  return ret;
}

RandomSampler::RandomSampler()
{
  this->m_Parameters.Adaptative = false;
  m_Generator = itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance();
}

}

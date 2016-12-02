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

#include "otbPeriodicSampler.h"
#include "otbMath.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

bool
PeriodicSampler::ParameterType::operator!=(const PeriodicSampler::ParameterType & param) const
{
  return bool((Offset != param.Offset)||
              (MaxJitter != param.MaxJitter) ||
              (MaxBufferSize != param.MaxBufferSize));
}

void
PeriodicSampler::Reset(void)
{
  Superclass::Reset();
  // Detect maximum jitter size, clamp with (T/N - 1)
  m_JitterSize = std::min((double) this->m_Parameters.MaxJitter, 1.0/this->GetRate() - 1.0);
  if (m_JitterSize > 0.0)
    {
    // Using jitter : compute random offset value
    m_JitterValues.resize(std::min(this->GetNeededElements(), this->m_Parameters.MaxBufferSize));
    for (unsigned long i=0UL ; i<m_JitterValues.size() ; i++)
      {
      m_JitterValues[i] = itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()
        ->GetUniformVariate(0.0,m_JitterSize);
      }
    if (m_JitterValues.empty())
      {
      m_OffsetValue = 0.0;
      }
    else
      {
      m_OffsetValue = m_JitterValues[0];
      }
    }
  else
    {
    // Using a constant offset value, between 0 and T/N
    m_OffsetValue = (double)this->m_Parameters.Offset;
    while (m_OffsetValue >= 1.0/this->GetRate())
      {
      m_OffsetValue -= 1.0/this->GetRate();
      }
    }
}

bool
PeriodicSampler::TakeSample(void)
{
  bool ret = false;
  // Take p elements among N, while iterating on the full set (current index i)
  // where k elements have already been selected. The offset is in the intervall
  // [0, N/p]
  //   val = (i - offset)*p/N  - k
  //  if ceil(val) > 0; then take the sample
  this->m_ProcessedElements += 1UL;
  double val = ((double)(this->m_ProcessedElements) - m_OffsetValue )*this->GetRate() - (double)(this->m_ChosenElements);
  if (0 < (int)(vcl_ceil(val)))
    {
    this->m_ChosenElements += 1UL;
    if (m_JitterSize > 0.0)
      {
      // Using jitter : compute random offset value
      m_OffsetValue = m_JitterValues[this->m_ChosenElements%m_JitterValues.size()];
      }
    ret = true;
    }
  return ret;
}

PeriodicSampler::PeriodicSampler()
{
  this->m_Parameters.Offset = 0UL;
  this->m_Parameters.MaxJitter = 0UL;
  this->m_Parameters.MaxBufferSize = 100000000UL;
  m_JitterSize = 0.0;
  m_OffsetValue = 0.0;
}

}

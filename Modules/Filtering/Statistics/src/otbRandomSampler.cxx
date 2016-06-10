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
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "otbMath.h"
#include <algorithm>

namespace otb
{

bool
RandomSampler::ParameterType::operator!=(const RandomSampler::ParameterType & param) const
{
  return bool(MaxBufferSize != param.MaxBufferSize);
}

void
RandomSampler::Reset(void)
{
  Superclass::Reset();

  // Generate list of positions
  m_Positions.clear();
  unsigned int seed =
    itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->GetSeed();
  std::srand(seed);
  unsigned long currentOffset = 0UL;
  unsigned long bufferSize, needed;
  std::vector<bool> buffer;
  while (currentOffset < this->GetTotalElements())
    {
    // shuffle a buffer
    bufferSize = std::min(
      this->GetTotalElements() - currentOffset,
      m_Parameters.MaxBufferSize);
    if (bufferSize == this->GetTotalElements())
      {
      needed = this->GetNeededElements();
      }
    else
      {
      // try to handle cases where there is less than 1 needed sample per buffer
      double sizeRatio1 = static_cast<double>(currentOffset)/
        static_cast<double>(this->GetTotalElements());
      double sizeRatio2 = static_cast<double>(currentOffset+bufferSize)/
        static_cast<double>(this->GetTotalElements());
      needed =
        static_cast<unsigned long>( sizeRatio2 * (double)this->GetNeededElements()) -
        static_cast<unsigned long>( sizeRatio1 * (double)this->GetNeededElements());
      }
    buffer.clear();
    buffer.resize(bufferSize,0);
    // switch ON the first 'needed' booleans
    for (unsigned long k=0UL ; k<needed ; k++)
      {
      buffer[k] = 1;
      }
    // shuffle boolean buffer
    std::random_shuffle ( buffer.begin(), buffer.end() );
    // compute positions
    for (unsigned long k=0UL ; k<bufferSize ; k++)
      {
      if (buffer[k])
        {
        m_Positions.push_back(k + currentOffset);
        }
      }
    currentOffset += bufferSize;
    }
  m_CurrentPosition = m_Positions.begin();
}

bool
RandomSampler::TakeSample(void)
{
  bool ret = false;
  if (this->m_ProcessedElements >= this->GetTotalElements() ||
        this->m_ChosenElements >= this->GetNeededElements())
    {
    this->m_ProcessedElements += 1UL;
    return false;
    }

  if (this->m_ProcessedElements == *m_CurrentPosition)
    {
    ret = true;
    this->m_ChosenElements += 1UL;
    ++m_CurrentPosition;
    }

  this->m_ProcessedElements += 1UL;
  return ret;
}

RandomSampler::RandomSampler()
{
  this->m_Parameters.MaxBufferSize = 100000000;
}

}

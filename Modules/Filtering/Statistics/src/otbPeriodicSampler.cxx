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

namespace otb
{

bool
PeriodicSampler::ParameterType::operator!=(const PeriodicSampler::ParameterType & param) const
{
  return bool(Offset != param.Offset);
}

void
PeriodicSampler::Reset(void)
{
  Superclass::Reset();
  m_Value = double(this->m_Parameters.Offset) * this->GetRate();
}

bool
PeriodicSampler::TakeSample(void)
{
  bool ret = false;
  // take p elements among N, while iterating on the full set (current index i)
  //   v1 = int( ceil( float(i*p) / float(N) ) )
  //   v2 = int( ceil( float((i+1)*p) / float(N) ) )
  //   if (v1 < v2) then select sample
  this->m_ProcessedElements += 1UL;
  double val = double(
    (this->m_ProcessedElements+this->m_Parameters.Offset))*this->GetRate();
  
  if ((unsigned long)(vcl_ceil(m_Value)) < (unsigned long)(vcl_ceil(val)))
    {
    this->m_ChosenElements += 1UL;
    ret = true;
    }
  m_Value = val;
  return ret;
}

PeriodicSampler::PeriodicSampler()
{
  this->m_Parameters.Offset = 0UL;
}

}

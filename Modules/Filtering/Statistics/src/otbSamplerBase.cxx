/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSamplerBase.h"
#include "otbMath.h"
#include "itkMath.h"
#include <cmath>

namespace otb
{

void SamplerBase::Reset(void)
{
  this->m_ChosenElements    = 0UL;
  this->m_ProcessedElements = 0UL;
}

void SamplerBase::SetNumberOfElements(unsigned long needed, unsigned long total)
{
  bool          modified      = false;
  unsigned long neededChecked = needed;
  if (needed > total)
  {
    itkWarningMacro(<< "Needed elements (" << needed << ") will be clamped to total elements (" << total << ")" << std::endl);
    neededChecked = total;
  }
  if (m_NeededElements != neededChecked)
  {
    m_NeededElements = neededChecked;
    modified         = true;
  }
  if (m_TotalElements != total)
  {
    m_TotalElements = total;
    modified        = true;
  }
  if (modified)
  {
    if (m_TotalElements > 0)
    {
      m_Rate = (double)(m_NeededElements) / (double)(m_TotalElements);
    }
    else
    {
      m_Rate = 0.0;
    }
    this->Modified();
  }
}

void SamplerBase::SetRate(double rate, unsigned long total)
{
  bool   modified    = false;
  double rateChecked = rate;
  if (rate > 1.0)
  {
    itkWarningMacro(<< "Rate (" << rate << ") will be clamped to 1.0" << std::endl);
    rateChecked = 1.0;
  }
  if (rate < 0.0)
  {
    itkWarningMacro(<< "Rate (" << rate << ") will be clamped to 0.0" << std::endl);
    rateChecked = 0.0;
  }
  if (std::fabs(m_Rate - rateChecked) > 1e-12)
  {
    m_Rate   = rateChecked;
    modified = true;
  }
  if (m_TotalElements != total)
  {
    m_TotalElements = total;
    modified        = true;
  }
  if (modified)
  {
    m_NeededElements = (unsigned long)(std::floor(0.5 + m_Rate * (double)(m_TotalElements)));
    this->Modified();
  }
}

SamplerBase::SamplerBase() : m_ChosenElements(0UL), m_ProcessedElements(0UL), m_TotalElements(0UL), m_NeededElements(0UL), m_Rate(0.0)
{
}
}

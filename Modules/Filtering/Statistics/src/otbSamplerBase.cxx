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

#include "otbSamplerBase.h"
#include "otbMath.h"
#include "itkMath.h"

namespace otb
{

void
SamplerBase::Reset(void)
{
  this->m_ChosenElements = 0UL;
  this->m_ProcessedElements = 0UL;
}

void
SamplerBase::SetNumberOfElements(unsigned long needed, unsigned long total)
{
  bool modified = false;
  unsigned long neededChecked = needed;
  if (needed > total)
    {
    itkWarningMacro(<< "Needed elements (" << needed <<
      ") will be clamped to total elements (" << total << ")" << std::endl);
    neededChecked = total;
    }
  if (m_NeededElements != neededChecked)
    {
    m_NeededElements = neededChecked;
    modified = true;
    }
  if (m_TotalElements != total)
    {
    m_TotalElements = total;
    modified = true;
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

void
SamplerBase::SetRate(double rate, unsigned long total)
{
  bool modified = false;
  double rateChecked = rate;
  if (rate > 1.0)
    {
    itkWarningMacro(<< "Rate (" << rate <<
      ") will be clamped to 1.0" << std::endl);
    rateChecked = 1.0;
    }
  if (rate < 0.0)
    {
    itkWarningMacro(<< "Rate (" << rate <<
      ") will be clamped to 0.0" << std::endl);
    rateChecked = 0.0;
    }
  if (!itk::Math::AlmostEquals(m_Rate, rateChecked))
    {
    m_Rate = rateChecked;
    modified = true;
    }
  if (m_TotalElements != total)
    {
    m_TotalElements = total;
    modified = true;
    }
  if (modified)
    {
    m_NeededElements = (unsigned long)(vcl_floor(0.5 + m_Rate * (double)(m_TotalElements) ));
    this->Modified();
    }
}

SamplerBase::SamplerBase()
  : m_ChosenElements(0UL)
  , m_ProcessedElements(0UL)
  , m_TotalElements(0UL)
  , m_NeededElements(0UL)
  , m_Rate(0.0)
{
}

}

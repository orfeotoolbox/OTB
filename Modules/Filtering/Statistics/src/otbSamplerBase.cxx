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
  if (needed > total)
    {
    itkExceptionMacro(<< "Needed elements (" << needed << 
      ") greater than total elements" << total << ")." << std::endl);
    }
  if (m_NeededElements != needed)
    {
    m_NeededElements = needed;
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
  if (m_Rate != rate)
    {
    m_Rate = rate;
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

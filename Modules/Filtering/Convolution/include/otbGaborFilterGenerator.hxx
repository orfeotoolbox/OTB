/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbGaborFilterGenerator_hxx
#define otbGaborFilterGenerator_hxx

#include "otbGaborFilterGenerator.h"
#include "otbMath.h"

namespace otb
{
template <class TPrecision>
GaborFilterGenerator<TPrecision>::GaborFilterGenerator()
{
  m_Radius.Fill(2);
  m_A                      = 0;
  m_B                      = 0;
  m_U0                     = 0;
  m_V0                     = 0;
  m_Theta                  = 0;
  m_Phi                    = 0;
  m_NeedToRegenerateFilter = true;
}
template <class TPrecision>
const typename GaborFilterGenerator<TPrecision>::ArrayType& GaborFilterGenerator<TPrecision>::GetFilter()
{
  if (m_NeedToRegenerateFilter)
  {
    this->GenerateFilter();
  }
  return m_Filter;
}

template <class TPrecision>
void GaborFilterGenerator<TPrecision>::GenerateFilter()
{
  m_Filter.SetSize((2 * m_Radius[0] + 1) * (2 * m_Radius[1] + 1));
  m_Filter.Fill(0);

  PrecisionType coef, xr, yr, costheta, sintheta;
  unsigned int  k = 0;

  costheta = std::cos(m_Theta * CONST_PI_180);
  sintheta = std::sin(m_Theta * CONST_PI_180);

  for (PrecisionType y = -static_cast<PrecisionType>(m_Radius[1]); y <= static_cast<PrecisionType>(m_Radius[1]); y += 1)
  {
    for (PrecisionType x = -static_cast<PrecisionType>(m_Radius[0]); x <= static_cast<PrecisionType>(m_Radius[0]); x += 1)
    {
      xr   = x * costheta + y * sintheta;
      yr   = y * costheta - x * sintheta;
      coef = std::exp(-CONST_PI * (std::pow(m_A * xr, 2) + std::pow(m_B * yr, 2))) * cos(CONST_2PI * (m_U0 * x + m_V0 * y) + m_Phi);
      m_Filter.SetElement(k, coef);
      ++k;
    }
  }
}

template <class TPrecision>
void GaborFilterGenerator<TPrecision>::Modified() const
{
  this->Superclass::Modified();
  m_NeedToRegenerateFilter = true;
}

template <class TPrecision>
void GaborFilterGenerator<TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
  os << indent << "A: " << m_A << std::endl;
  os << indent << "B: " << m_B << std::endl;
  os << indent << "Theta: " << m_Theta << std::endl;
  os << indent << "U0: " << m_U0 << std::endl;
  os << indent << "V0: " << m_V0 << std::endl;
  os << indent << "Phi: " << m_Phi << std::endl;
  os << "Need to regenerate filter: " << m_NeedToRegenerateFilter << std::endl;
}
}
#endif

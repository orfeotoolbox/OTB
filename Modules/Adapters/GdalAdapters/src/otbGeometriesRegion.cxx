/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "otbGeometriesRegion.h"

namespace otb
{

GeometriesRegion::GeometriesRegion()
  : m_Mode(RANGE) // Default mode : range type
{
  m_R.Range.Start = 0;
  m_R.Range.Count = 0;
}

GeometriesRegion::~GeometriesRegion()
{
}

void
GeometriesRegion::SetMode(ModeType m)
{
  if (m_Mode != m)
    {
    m_Mode = m;
    }
}

const GeometriesRegion::ModeType &
GeometriesRegion::GetMode()
{
  return m_Mode;
}

bool
GeometriesRegion::IsRange() const
{
  return bool(m_Mode==GeometriesRegion::RANGE);
}

bool
GeometriesRegion::IsSpatial() const
{
  return bool(m_Mode==GeometriesRegion::SPATIAL);
}

const unsigned long &
GeometriesRegion::GetStartId(void) const
{
  return m_R.Range.Start;
}

void
GeometriesRegion::SetStartId(const unsigned long & id)
{
  m_R.Range.Start = id;
}

const unsigned long &
GeometriesRegion::GetCount() const
{
  return m_R.Range.Count;
}

void
GeometriesRegion::SetCount(const unsigned long & count)
{
  m_R.Range.Count = count;
}

const double &
GeometriesRegion::GetStartX() const
{
  return m_R.Spatial.Start[0];
}

void
GeometriesRegion::SetStartX(const double& x)
{
  m_R.Spatial.Start[0] = x;
}

const double &
GeometriesRegion::GetStartY() const
{
  return m_R.Spatial.Start[1];
}

void
GeometriesRegion::SetStartY(const double& y)
{
  m_R.Spatial.Start[1] = y;
}

const double &
GeometriesRegion::GetEndX() const
{
  return m_R.Spatial.End[0];
}

void
GeometriesRegion::SetEndX(const double& x)
{
  m_R.Spatial.End[0] = x;
}

const double &
GeometriesRegion::GetEndY() const
{
  return m_R.Spatial.End[1];
}

void
GeometriesRegion::SetEndY(const double& y)
{
  m_R.Spatial.End[1] = y;
}

double*
GeometriesRegion::GetStartPoint()
{
  return m_R.Spatial.Start;
}

double*
GeometriesRegion::GetEndPoint()
{
  return m_R.Spatial.End;
}

bool
GeometriesRegion::operator==(const GeometriesRegion & region) const
{
  if (m_Mode != region.m_Mode)
    return false;
  switch(m_Mode)
    {
    case RANGE:
      return bool(m_R.Range.Start == region.m_R.Range.Start &&
                  m_R.Range.Count == region.m_R.Range.Count);
    case SPATIAL:
      return bool(m_R.Spatial.Start[0] == region.m_R.Spatial.Start[0] &&
                  m_R.Spatial.Start[1] == region.m_R.Spatial.Start[1] &&
                  m_R.Spatial.End[0] == region.m_R.Spatial.End[0] &&
                  m_R.Spatial.End[1] == region.m_R.Spatial.End[1]);
    default:
      break;
    }
  return false;
}

bool
GeometriesRegion::operator!=(const GeometriesRegion & region) const
{
  if (m_Mode != region.m_Mode)
    return true;
  switch(m_Mode)
    {
    case RANGE:
      return bool(m_R.Range.Start != region.m_R.Range.Start ||
                  m_R.Range.Count != region.m_R.Range.Count);
    case SPATIAL:
      return bool(m_R.Spatial.Start[0] != region.m_R.Spatial.Start[0] ||
                  m_R.Spatial.Start[1] != region.m_R.Spatial.Start[1] ||
                  m_R.Spatial.End[0] != region.m_R.Spatial.End[0] ||
                  m_R.Spatial.End[1] != region.m_R.Spatial.End[1]);
    default:
      break;
    }
  return true;
}

void
GeometriesRegion::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  switch(m_Mode)
    {
    case RANGE:
      os << indent << "Mode  : RANGE"  << std::endl;
      os << indent << "Start : " << this->m_R.Range.Start << std::endl;
      os << indent << "Count : " << this->m_R.Range.Count << std::endl;
      break;
    case SPATIAL:
      os << indent << "Mode  : SPATIAL"  << std::endl;
      os << indent << "Start : [" << this->m_R.Spatial.Start[0]
        << " ; "<< this->m_R.Spatial.Start[1] << "]" << std::endl;
      os << indent << "End   : [" << this->m_R.Spatial.End[0]
        << " ; "<< this->m_R.Spatial.End[1] << "]" << std::endl;
      break;
    default:
      break;
    }
}

std::ostream & operator<<(std::ostream & os, const GeometriesRegion & region)
{
  region.Print(os);
  return os;
}

}

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
  : m_Modes(1) // Default mode : range type
  , m_StartId(0)
  , m_Count(0)
  , m_StartPoint{0.,0.}
  , m_EndPoint{0.,0.}
{
}

GeometriesRegion::~GeometriesRegion()
{
}

void
GeometriesRegion::SetRangeMode(bool state)
{
  if (state)
    {
    m_Modes = m_Modes | 0x01;
    }
  else
    {
    m_Modes = m_Modes & 0xFE;
    }
}

void
GeometriesRegion::SetSpatialMode(bool state)
{
  if (state)
    {
    m_Modes = m_Modes | 0x02;
    }
  else
    {
    m_Modes = m_Modes & 0xFD;
    }
}

bool
GeometriesRegion::IsRange() const
{
  return bool(m_Modes & 0x01);
}

bool
GeometriesRegion::IsSpatial() const
{
  return bool((m_Modes & 0x02)>>1);
}

const unsigned long &
GeometriesRegion::GetStartId(void) const
{
  return m_StartId;
}

void
GeometriesRegion::SetStartId(const unsigned long & id)
{
  m_StartId = id;
}

const unsigned long &
GeometriesRegion::GetCount() const
{
  return m_Count;
}

void
GeometriesRegion::SetCount(const unsigned long & count)
{
  m_Count = count;
}

const double &
GeometriesRegion::GetStartX() const
{
  return m_StartPoint[0];
}

void
GeometriesRegion::SetStartX(const double& x)
{
  m_StartPoint[0] = x;
}

const double &
GeometriesRegion::GetStartY() const
{
  return m_StartPoint[1];
}

void
GeometriesRegion::SetStartY(const double& y)
{
  m_StartPoint[1] = y;
}

const double &
GeometriesRegion::GetEndX() const
{
  return m_EndPoint[0];
}

void
GeometriesRegion::SetEndX(const double& x)
{
  m_EndPoint[0] = x;
}

const double &
GeometriesRegion::GetEndY() const
{
  return m_EndPoint[1];
}

void
GeometriesRegion::SetEndY(const double& y)
{
  m_EndPoint[1] = y;
}

double*
GeometriesRegion::GetStartPoint()
{
  return m_StartPoint;
}

double*
GeometriesRegion::GetEndPoint()
{
  return m_EndPoint;
}

bool
GeometriesRegion::operator==(const Self & region) const
{
  if (m_Modes != region.m_Modes)
    return false;
  bool same = true;
  if (this->IsRange())
    same = same && (m_StartId == region.m_StartId) && (m_Count == region.m_Count);
  if (this->IsSpatial())
    same = same && ( m_StartPoint[0] == region.m_StartPoint[0] )
                && ( m_StartPoint[1] == region.m_StartPoint[1] )
                && ( m_EndPoint[0] == region.m_EndPoint[0] )
                && ( m_EndPoint[1] == region.m_EndPoint[1] );
  return same;
}

bool
GeometriesRegion::operator!=(const Self & region) const
{
  if (m_Modes != region.m_Modes)
    return true;
  bool same = false;
  if (this->IsRange())
    same = same && (m_StartId == region.m_StartId) && (m_Count == region.m_Count);
  if (this->IsSpatial())
    same = same && ( m_StartPoint[0] == region.m_StartPoint[0] )
                && ( m_StartPoint[1] == region.m_StartPoint[1] )
                && ( m_EndPoint[0] == region.m_EndPoint[0] )
                && ( m_EndPoint[1] == region.m_EndPoint[1] );
  return !same;
}

void
GeometriesRegion::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Modes     : " << (this->IsSpatial()?"S":"_") << (this->IsRange()?"R":"_") << std::endl;
  os << indent << "StartId   : " << this->m_StartId << std::endl;
  os << indent << "Count     : " << this->m_Count << std::endl;
  os << indent << "StartPoint: " << this->m_Count << std::endl;
  os << indent << "EndPoint  : " << this->m_Count << std::endl;
}

std::ostream & operator<<(std::ostream & os, const GeometriesRegion & region)
{
  region.Print(os);
  return os;
}

}

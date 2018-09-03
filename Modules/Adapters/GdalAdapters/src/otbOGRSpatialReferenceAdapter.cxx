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

#include "otbOGRSpatialReferenceAdapter.h"

#include "ogr_spatialref.h"

#include <sstream>

namespace otb {

std::ostream & operator << (std::ostream& o, const OGRSpatialReferenceAdapter & i)
{
  return o << i.ToWkt();
}

InvalidSRDescriptionException::InvalidSRDescriptionException(const std::string & description) : m_Description(description) {};

InvalidSRDescriptionException::~InvalidSRDescriptionException()
{}

const char * InvalidSRDescriptionException::what() const noexcept
{
  return m_Description.c_str();
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const std::string & description)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code = tmpSR->SetFromUserInput(description.c_str());

  if(code!=OGRERR_NONE)
    {
    throw InvalidSRDescriptionException(description);
    }

  m_SR.swap(tmpSR);
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const unsigned int & epsg)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());  
  OGRErr code = tmpSR->importFromEPSGA(epsg);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss<<"EPSG:"<<epsg;
    throw InvalidSRDescriptionException(oss.str());
    }

  m_SR.swap(tmpSR);
}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const unsigned int & zone, bool north)
{
  std::unique_ptr<OGRSpatialReference> tmpSR(new OGRSpatialReference());
  OGRErr code = tmpSR->SetUTM(zone,north);

  if(code!=OGRERR_NONE)
    {
    std::ostringstream oss;
    oss<<"UTM"<<zone<<(north?"N":"S");
    throw InvalidSRDescriptionException(oss.str());
    }

  m_SR.swap(tmpSR);

}

OGRSpatialReferenceAdapter::~OGRSpatialReferenceAdapter() noexcept
{}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const OGRSpatialReferenceAdapter & other) noexcept
  : m_SR(other.m_SR->Clone())
{}

OGRSpatialReferenceAdapter::OGRSpatialReferenceAdapter(const OGRSpatialReference * ref) noexcept
: m_SR(ref->Clone())
{}

OGRSpatialReferenceAdapter & OGRSpatialReferenceAdapter::operator=(const OGRSpatialReferenceAdapter& other) noexcept
{
  m_SR = std::unique_ptr<OGRSpatialReference>(other.m_SR->Clone());
  return *this;
}

bool OGRSpatialReferenceAdapter::operator==(const OGRSpatialReferenceAdapter& other) noexcept
{
  return m_SR->IsSame(other.m_SR.get());
}

bool OGRSpatialReferenceAdapter::operator!=(const OGRSpatialReferenceAdapter& other) noexcept
{
  return !(*this==other);
}

std::string OGRSpatialReferenceAdapter::ToWkt() const
{
  char * cwkt;
  m_SR->exportToWkt(&cwkt);
  std::string wkt(cwkt);
  delete cwkt;

  return wkt;
}
}

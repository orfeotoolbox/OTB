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

#include "otbOGRCoordinateTransformationAdapter.h"

#include "ogr_spatialref.h"

#include <sstream>


namespace otb
{
InvalidCoordinateTransfromationException::InvalidCoordinateTransfromationException(const std::string & description)
  : m_Description(description)
{}

InvalidCoordinateTransfromationException::~InvalidCoordinateTransfromationException()
{}

const char * InvalidCoordinateTransfromationException::what() const noexcept
{
  return m_Description.c_str();
}

// Constructor
OGRCoordinateTransformationAdapter::OGRCoordinateTransformationAdapter(const OGRSpatialReferenceAdapter & source, const OGRSpatialReferenceAdapter & target)
{
  std::unique_ptr<OGRCoordinateTransformation> tmpTransform(OGRCreateCoordinateTransformation(source.m_SR.get(),target.m_SR.get()));

// If construction failed, throw
  if(!tmpTransform)
    {
    std::ostringstream oss;
    oss<<"Source: "<<source<<", target: "<<target;
    throw InvalidCoordinateTransfromationException(oss.str());
    }
  
  // Swap safely
  m_Transform.swap(tmpTransform);
}

// Default destructor
OGRCoordinateTransformationAdapter::~OGRCoordinateTransformationAdapter() noexcept {}
  
// Copy constructor
OGRCoordinateTransformationAdapter::OGRCoordinateTransformationAdapter(const OGRCoordinateTransformationAdapter& other) noexcept
{
  // Mimic a clone operator
  std::unique_ptr<OGRCoordinateTransformation> newTransform (OGRCreateCoordinateTransformation(other.GetSourceSpatialReference().m_SR.get(),other.GetTargetSpatialReference().m_SR.get()));
  
  // Only update transfrom if newTransform is valid
  if(newTransform)
    m_Transform.swap(newTransform);
}

// Asignment operator
OGRCoordinateTransformationAdapter & OGRCoordinateTransformationAdapter::operator=(const OGRCoordinateTransformationAdapter& other) noexcept
{
  // Mimic a clone operator
  std::unique_ptr<OGRCoordinateTransformation> newTransform (OGRCreateCoordinateTransformation(other.GetSourceSpatialReference().m_SR.get(),other.GetTargetSpatialReference().m_SR.get()));

  // Only update transfrom if newTransform is valid
  if(newTransform)
    m_Transform.swap(newTransform);
  
  return *this;
}

// equal operator
bool OGRCoordinateTransformationAdapter::operator==(const OGRCoordinateTransformationAdapter& other) noexcept
{
  auto thisSourceCS = OGRSpatialReferenceAdapter(m_Transform->GetSourceCS());
  auto thisTargetCS = OGRSpatialReferenceAdapter(m_Transform->GetTargetCS());
  auto otherSourceCS = OGRSpatialReferenceAdapter(other.m_Transform->GetSourceCS());
  auto otherTargetCS = OGRSpatialReferenceAdapter(other.m_Transform->GetTargetCS());

  return thisSourceCS == otherSourceCS && thisTargetCS == otherTargetCS;
}

bool OGRCoordinateTransformationAdapter::operator!=(const OGRCoordinateTransformationAdapter& other) noexcept
{
  return !(*this == other);
}
    
OGRSpatialReferenceAdapter OGRCoordinateTransformationAdapter::GetSourceSpatialReference() const
{
  return OGRSpatialReferenceAdapter(m_Transform->GetSourceCS());
}

OGRSpatialReferenceAdapter OGRCoordinateTransformationAdapter::GetTargetSpatialReference() const
{
  return OGRSpatialReferenceAdapter(m_Transform->GetTargetCS());
}


std::ostream & operator << (std::ostream& o, const OGRCoordinateTransformationAdapter & i)
{
  o << "Source: "<<i.GetSourceSpatialReference()<<", Target: "<<i.GetTargetSpatialReference();
  return o;
}


  // 3D Transfrom of points
bool OGRCoordinateTransformationAdapter::Transform(const double& inX, const double& inY, const double& inZ, double & outX, double & outY, double & outZ) const
{
  outX = inX;
  outY = inY;
  outZ = inZ;

  bool success = m_Transform->Transform(1,&outX,&outY,&outZ);

  return success;
}

  // 2D transform of points
bool OGRCoordinateTransformationAdapter::Transform(const double& inX, const double& inY, double & outX, double & outY) const
{
  outX = inX;
  outY = inY;

  bool success = m_Transform->Transform(1,&outX,&outY,nullptr);

  return success;
}

  
}

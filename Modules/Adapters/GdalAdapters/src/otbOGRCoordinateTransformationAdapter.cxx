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
  : std::runtime_error(description)
{}

TransformFailureException::TransformFailureException(const std::string & description)
  : std::runtime_error(description)
{}

std::ostream & operator << (std::ostream& o, const OGRCoordinateTransformationAdapter & i)
{
  o << "Source: " << i.GetSourceSpatialReference() << ", Target: " << i.GetTargetSpatialReference();
  return o;
}

// equal operator
bool operator==(const OGRCoordinateTransformationAdapter& ct1, const OGRCoordinateTransformationAdapter& ct2) noexcept
{
  auto thisSourceCS  = ct1.GetSourceSpatialReference();
  auto thisTargetCS  = ct1.GetTargetSpatialReference();
  auto otherSourceCS = ct2.GetSourceSpatialReference();
  auto otherTargetCS = ct2.GetTargetSpatialReference();

  return thisSourceCS == otherSourceCS && thisTargetCS == otherTargetCS;
}

bool operator!=(const OGRCoordinateTransformationAdapter& ct1, const OGRCoordinateTransformationAdapter & ct2) noexcept
{
  return !(ct1==ct2);
}

// Constructor
OGRCoordinateTransformationAdapter::OGRCoordinateTransformationAdapter(const OGRSpatialReferenceAdapter & source, const OGRSpatialReferenceAdapter & target)
{
  std::unique_ptr<OGRCoordinateTransformation> tmpTransform(OGRCreateCoordinateTransformation(source.m_SR.get(),target.m_SR.get()));

// If construction failed, throw
  if(!tmpTransform)
    {
    std::ostringstream oss;
    oss << "Source: " << source << ", target: " << target;
    throw InvalidCoordinateTransfromationException(oss.str());
    }
  
  // Swap safely
  m_Transform.swap(tmpTransform);
}

// Destructor
OGRCoordinateTransformationAdapter::~OGRCoordinateTransformationAdapter() noexcept {};

// Copy constructor
OGRCoordinateTransformationAdapter::OGRCoordinateTransformationAdapter(const OGRCoordinateTransformationAdapter& other)
{
  // Mimic a clone operator
  std::unique_ptr<OGRCoordinateTransformation> newTransform (OGRCreateCoordinateTransformation(other.GetSourceSpatialReference().m_SR.get(),other.GetTargetSpatialReference().m_SR.get()));

  // Ensure that newTransform is valid
  if(!newTransform)
    {
    std::ostringstream oss;
    oss << "Source: " << other.GetSourceSpatialReference() << ", target: " << other.GetTargetSpatialReference();
    throw InvalidCoordinateTransfromationException(oss.str());
    }

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
    
OGRSpatialReferenceAdapter OGRCoordinateTransformationAdapter::GetSourceSpatialReference() const
{
  return OGRSpatialReferenceAdapter(m_Transform->GetSourceCS());
}

OGRSpatialReferenceAdapter OGRCoordinateTransformationAdapter::GetTargetSpatialReference() const
{
  return OGRSpatialReferenceAdapter(m_Transform->GetTargetCS());
}

// 3D Transfrom of points
std::tuple<double,double,double> OGRCoordinateTransformationAdapter::Transform(const std::tuple<double,double,double> & in) const
{
  double outX, outY, outZ;
  std::tie(outX, outY,outZ) = in;
  
  bool success = m_Transform->Transform(1,&outX,&outY,&outZ);

  if(!success)
    {
    std::ostringstream oss;
    oss<<"Transform: "<<this<<", Parameters: "<<std::get<0>(in)<<", "<<std::get<1>(in)<<", "<<std::get<2>(in);
    throw new TransformFailureException(oss.str());
    }

  return std::make_tuple(outX,outY,outZ);
}

// 2D Transfrom of points
std::tuple<double,double> OGRCoordinateTransformationAdapter::Transform(const std::tuple<double,double> & in) const
{
  double outX, outY;
  std::tie(outX, outY) = in;
  
  bool success = m_Transform->Transform(1,&outX,&outY,nullptr);

  if(!success)
    {
    std::ostringstream oss;
    oss<<"Transform: "<<this<<", Parameters: "<<std::get<0>(in)<<", "<<std::get<1>(in);
    throw new TransformFailureException(oss.str());
    }

  return std::make_tuple(outX,outY);
}  
}

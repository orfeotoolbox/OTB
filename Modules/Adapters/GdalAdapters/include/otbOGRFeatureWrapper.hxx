/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRFeatureWrapper_hxx
#define otbOGRFeatureWrapper_hxx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRFeatureWrapper.h"
#include <cassert>
#include <utility>

/*===========================================================================*/
/*================================[ Feature ]================================*/
/*===========================================================================*/
inline void otb::ogr::Feature::CheckInvariants() const
{
  assert(m_Feature && "OGRFeature can't be null");
}

inline otb::ogr::Feature otb::ogr::Feature::Clone() const
{
  CheckInvariants();
  return UncheckedClone();
}

inline void otb::ogr::Feature::SetFrom(Feature const& rhs, bool mustForgive)
{
  CheckInvariants();
  UncheckedSetFrom(rhs, mustForgive);
}

inline void otb::ogr::Feature::SetFrom(Feature const& rhs, int* map, bool mustForgive)
{
  CheckInvariants();
  UncheckedSetFrom(rhs, map, mustForgive);
}

/*===========================================================================*/
/*================================[ Fields ]=================================*/
/*===========================================================================*/
inline otb::ogr::Field otb::ogr::Feature::operator[](int index)
{
  assert(index < GetSize() && "out of range field-index."); // also calls CheckInvariants();
  return UncheckedGetElement(index);
}

inline otb::ogr::Field const otb::ogr::Feature::operator[](int index) const
{
  return const_cast<Feature*>(this)->operator[](index);
}

inline otb::ogr::Field otb::ogr::Feature::operator[](std::string const& name)
{
  CheckInvariants();
  return UncheckedGetElement(name);
}

inline otb::ogr::Field const otb::ogr::Feature::operator[](std::string const& name) const
{
  return const_cast<Feature*>(this)->operator[](name);
}

inline otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(int index) const
{
  assert(index < GetSize() && "out of range field-index."); // also calls CheckInvariants();
  return UncheckedGetFieldDefn(index);
}

inline otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(std::string const& name) const
{
  CheckInvariants();
  return UncheckedGetFieldDefn(name);
}

inline int otb::ogr::Feature::GetFieldIndex(std::string const& name) const
{
  CheckInvariants();
  return UncheckedGetFieldIndex(name);
}

/*===========================================================================*/
/*==============================[ Properties ]===============================*/
/*===========================================================================*/
inline long otb::ogr::Feature::GetFID() const
{
  CheckInvariants();
  return UncheckedGetFID();
}

inline void otb::ogr::Feature::SetFID(long fid)
{
  CheckInvariants();
  UncheckedSetFID(fid);
}

inline OGRFeatureDefn& otb::ogr::Feature::GetDefn() const
{
  CheckInvariants();
  return UncheckedGetDefn();
}

/*===========================================================================*/
/*==============================[ Geometries ]===============================*/
/*===========================================================================*/
inline void otb::ogr::Feature::SetGeometryDirectly(UniqueGeometryPtr geometry)
{
  CheckInvariants();
#if !defined(NDEBUG)
  OGRGeometry* g = geometry.get();
#endif
  UncheckedSetGeometryDirectly(std::move(geometry));
  assert((m_Feature->GetGeometryRef() == g) && "The new geometry hasn't been set as expected");
  assert(!geometry && "UniqueGeometryPtr hasn't released its pointer");
}

inline otb::ogr::UniqueGeometryPtr otb::ogr::Feature::StealGeometry()
{
  CheckInvariants();
  UniqueGeometryPtr res = UncheckedStealGeometry();
  itkAssertOrThrowMacro(!m_Feature->GetGeometryRef(), "Geometry hasn't been properly stolen");
  return res;
}

inline void otb::ogr::Feature::SetGeometry(OGRGeometry const* geometry)
{
  CheckInvariants();
  UncheckedSetGeometry(geometry);
}

inline OGRGeometry const* otb::ogr::Feature::GetGeometry() const
{
  CheckInvariants();
  return UncheckedGetGeometry();
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
inline OGRFeature& otb::ogr::Feature::ogr() const
{ // not returning a OGRFeature const& because OGR is not const-correct
  CheckInvariants();
  return *m_Feature;
}

inline OGRFeature& otb::ogr::Feature::ogr()
{
  CheckInvariants();
  return *m_Feature;
}

inline void otb::ogr::Feature::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  CheckInvariants();
  UncheckedPrintSelf(os, indent);
}

#endif // otbOGRFeatureWrapper_hxx

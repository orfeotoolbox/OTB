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
#ifndef __otbOGRFeatureWrapper_txx
#define __otbOGRFeatureWrapper_txx

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include <cassert>
#include <boost/version.hpp>
#if BOOST_VERSION >= 104800
#   include <boost/move/move.hpp> // since 1.48
#else
#   include <boost/interprocess/detail/move.hpp>
#endif

namespace otb
{
#if BOOST_VERSION >= 104800
using boost::move;
#else
using boost::interprocess::move;
#endif
}

/*===========================================================================*/
/*================================[ Feature ]================================*/
/*===========================================================================*/
inline
otb::ogr::Feature otb::ogr::Feature::Clone() const
{
  CheckInvariants();
  return UncheckedClone();
}

inline
void otb::ogr::Feature::SetFrom(Feature const& rhs, bool mustForgive)
{
  CheckInvariants();
  UncheckedSetFrom(rhs, mustForgive);
}

inline
void otb::ogr::Feature::SetFrom(Feature const& rhs, int * map, bool mustForgive)
{
  CheckInvariants();
  UncheckedSetFrom(rhs, map, mustForgive);
}

/*===========================================================================*/
/*================================[ Fields ]=================================*/
/*===========================================================================*/
inline
otb::ogr::Field otb::ogr::Feature::operator[](size_t index)
{
  assert(index < GetSize() && "out of range field-index."); // also calls CheckInvariants();
  return UncheckedGetElement(index);
}

inline
otb::ogr::Field const otb::ogr::Feature::operator[](size_t index) const
{
  return const_cast<Feature*>(this)->operator[](index);
}

inline
otb::ogr::Field otb::ogr::Feature::operator[](std::string const& name)
{
  CheckInvariants();
  return UncheckedGetElement(name);
}

inline
otb::ogr::Field const otb::ogr::Feature::operator[](std::string const& name) const
{
  return const_cast<Feature*>(this)->operator[](name);
}

inline
otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(size_t index) const
{
  assert(index < GetSize() && "out of range field-index."); // also calls CheckInvariants();
  return UncheckedGetFieldDefn(index);
}

inline
otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(std::string const& name) const
{
  CheckInvariants();
  return UncheckedGetFieldDefn(name);
}

inline
int otb::ogr::Feature::GetFieldIndex(std::string const& name) const
{
  CheckInvariants();
  return UncheckedGetFieldIndex(name);
}

/*===========================================================================*/
/*==============================[ Properties ]===============================*/
/*===========================================================================*/
inline
long otb::ogr::Feature::GetFID() const
{
  CheckInvariants();
  return UncheckedGetFID();
}

inline
void otb::ogr::Feature::SetFID(long fid)
{
  CheckInvariants();
  UncheckedSetFID(fid);
}

inline
OGRFeatureDefn&  otb::ogr::Feature::GetDefn() const
{
  CheckInvariants();
  return UncheckedGetDefn();
}

/*===========================================================================*/
/*==============================[ Geometries ]===============================*/
/*===========================================================================*/
inline
void otb::ogr::Feature::SetGeometryDirectly(UniqueGeometryPtr geometry)
{
  CheckInvariants();
  //OGRGeometry * g = geometry.get();
  UncheckedSetGeometryDirectly(otb::move(geometry));
  assert(m_Feature->GetGeometryRef() == geometry.get() && "The new geometry hasn't been set as expected");
  assert(! geometry && "UniqueGeometryPtr hasn't released its pointer");
}

inline
otb::ogr::UniqueGeometryPtr otb::ogr::Feature::StealGeometry()
{
  CheckInvariants();
  UniqueGeometryPtr res = UncheckedStealGeometry();
  assert(! m_Feature->GetGeometryRef() && "Geometry hasn't been properly stolen");
  return otb::move(res);
}

inline
void otb::ogr::Feature::SetGeometry(OGRGeometry const* geometry)
{
  CheckInvariants();
  UncheckedSetGeometry(geometry);
}

inline
OGRGeometry const* otb::ogr::Feature::GetGeometry() const
{
  CheckInvariants();
  return UncheckedGetGeometry();
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
inline
OGRFeature & otb::ogr::Feature::ogr() const
{ // not returning a OGRFeature const& because OGR is not const-correct
  CheckInvariants();
  return *m_Feature;
}

inline
OGRFeature & otb::ogr::Feature::ogr()
{
  CheckInvariants();
  return *m_Feature;
}

inline
void otb::ogr::Feature::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  CheckInvariants();
  UncheckedPrintSelf(os, indent);
}


inline
void otb::ogr::Feature::CheckInvariants() const
{
  assert(m_Feature && "OGRFeature can't be null");
}

#endif // __otbOGRFeatureWrapper_txx

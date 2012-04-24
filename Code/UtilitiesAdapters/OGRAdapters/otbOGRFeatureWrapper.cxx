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

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRFeatureWrapper.h"
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include "ogr_feature.h"
#include "itkMacro.h"

#include "otbOGRFieldWrapper.h"
/*===========================================================================*/
/*======================[ Construction / Destruction ]=======================*/
/*===========================================================================*/
otb::ogr::Feature::Feature(OGRFeatureDefn & definition)
: m_Feature(
  OGRFeature::CreateFeature(&definition),
  boost::bind(&OGRFeature::DestroyFeature, _1))
{
  CheckInvariants();
}

otb::ogr::Feature::Feature(OGRFeature * feature)
{
  if (feature)
    {
    m_Feature.reset(feature, boost::bind(&OGRFeature::DestroyFeature, _1));
    }
  // else default is perfect -> delete null
}

otb::ogr::Feature::~Feature()
{
}

otb::ogr::Feature otb::ogr::Feature::clone() const
{
  CheckInvariants();
  const Feature res(m_Feature->Clone());
  return res;
}

void otb::ogr::Feature::SetFrom(Feature const& rhs, bool mustForgive)
{
  CheckInvariants();
  m_Feature->SetFrom(&rhs.ogr(), mustForgive);
}

void otb::ogr::Feature::SetFrom(Feature const& rhs, int * map, bool mustForgive)
{
  CheckInvariants();
  m_Feature->SetFrom(&rhs.ogr(), map, mustForgive);
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
void otb::ogr::Feature::PrintSelf(std::ostream & os, itk::Indent indent) const
{
  CheckInvariants();
  const size_t nbFields = m_Feature->GetFieldCount();
  os << indent << "+";
  os << " " << nbFields << " fields\n";
  indent = indent.GetNextIndent();
  for (size_t i=0; i!=nbFields; ++i)
    {
    assert(ogr().GetFieldDefnRef(i));
    Field const& field = (*this)[i];
    field.PrintSelf(os, indent);
    }
}

bool otb::ogr::operator==(otb::ogr::Feature const& lhs, otb::ogr::Feature const& rhs)
{
  // special case: they may be null (end() mark)

  // OGR is not const correct ...
  OGRFeature * l = const_cast<OGRFeature*>(lhs.m_Feature.get());
  OGRFeature * r = const_cast<OGRFeature*>(rhs.m_Feature.get());
  return
    (l == r ) // incl. ==0
    ||
    (l && r && l->Equal(r)) // must be non-null to compare them with Equal
;
}

/*===========================================================================*/
/*================================[ Fields ]=================================*/
/*===========================================================================*/
size_t otb::ogr::Feature::GetSize() const {
  return ogr().GetFieldCount();
}

otb::ogr::Field otb::ogr::Feature::operator[](size_t index)
{
  CheckInvariants();
  Field field(*this, index);
  return field;
}

otb::ogr::Field const otb::ogr::Feature::operator[](size_t index) const
{
  return const_cast<Feature*>(this)->operator[](index);
}

otb::ogr::Field otb::ogr::Feature::operator[](std::string const& name)
{
  CheckInvariants();
  int index = m_Feature->GetFieldIndex(name.c_str());
  if (index < 0)
    {
    itkGenericExceptionMacro(<<"No field named <"<<name<<"> in feature");
    }
  return this->operator[](index);
}

otb::ogr::Field const otb::ogr::Feature::operator[](std::string const& name) const
{
  return const_cast<Feature*>(this)->operator[](name);
}

otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(size_t index) const
{
  CheckInvariants();
  return FieldDefn(*m_Feature->GetFieldDefnRef(index));
}

otb::ogr::FieldDefn otb::ogr::Feature::GetFieldDefn(std::string const& name) const
{
  CheckInvariants();
  int index = m_Feature->GetFieldIndex(name.c_str());
  if (index < 0)
    {
    itkGenericExceptionMacro(<<"No field named <"<<name<<"> in feature");
    }
  return this->GetFieldDefn(index);
}

/*===========================================================================*/
/*==============================[ Properties ]===============================*/
/*===========================================================================*/
long otb::ogr::Feature::GetFID() const
{
  CheckInvariants();
  return m_Feature->GetFID();
}

void otb::ogr::Feature::SetFID(long fid)
{
  CheckInvariants();
  const OGRErr res = m_Feature->SetFID(fid);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot Set FID to "<<fid<<" for feature: " << res);
    }
}

OGRFeatureDefn&  otb::ogr::Feature::GetDefn() const
{
  CheckInvariants();
  return *m_Feature->GetDefnRef();
}

/*===========================================================================*/
/*==============================[ Geometries ]===============================*/
/*===========================================================================*/

void otb::ogr::Feature::SetGeometryDirectly(UniqueGeometryPtr geometry)
{
  CheckInvariants();
  OGRGeometry * g = geometry.release();
  m_Feature->SetGeometryDirectly(g);
  assert(! geometry);
}

otb::ogr::Feature::UniqueGeometryPtr otb::ogr::Feature::StealGeometry()
{
  CheckInvariants();
  OGRGeometry * g = m_Feature->StealGeometry();
  UniqueGeometryPtr res(g);
  assert(! m_Feature->GetGeometryRef());
  return UniqueGeometryPtr(g);
}

void otb::ogr::Feature::SetGeometry(OGRGeometry const* geometry)
{
  CheckInvariants();
  // OGR copies the input geometry => should have been const
  m_Feature->SetGeometryDirectly(const_cast <OGRGeometry*>(geometry));
  assert(m_Feature->GetGeometryRef() == geometry);
}

OGRGeometry const* otb::ogr::Feature::GetGeometry() const
{
  CheckInvariants();
  return m_Feature->GetGeometryRef();
}

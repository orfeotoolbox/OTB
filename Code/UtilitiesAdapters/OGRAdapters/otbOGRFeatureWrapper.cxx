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
#include "cpl_error.h"
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

otb::ogr::Feature otb::ogr::Feature::UncheckedClone() const
{
  const Feature res(m_Feature->Clone());
  return res;
}

void otb::ogr::Feature::UncheckedSetFrom(Feature const& rhs, bool mustForgive)
{
  const OGRErr res = m_Feature->SetFrom(&rhs.ogr(), mustForgive);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot assign from another feature: " << CPLGetLastErrorMsg());
    }
}

void otb::ogr::Feature::UncheckedSetFrom(Feature const& rhs, int * map, bool mustForgive)
{
#if GDAL_VERSION_NUM >= 1900
  const OGRErr res = m_Feature->SetFrom(&rhs.ogr(), map, mustForgive);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot assign from another feature: " << CPLGetLastErrorMsg());
    }
#else
  itkGenericExceptionMacro("OGRLayer::SetFrom(feature, fieldmap, forgive) is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.")
#endif
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
void otb::ogr::Feature::UncheckedPrintSelf(std::ostream & os, itk::Indent indent) const
{
  const size_t nbFields = m_Feature->GetFieldCount();
  os << indent << "+";
  os << " " << nbFields << " fields\n";
  indent = indent.GetNextIndent();
  for (size_t i=0; i!=nbFields; ++i)
    {
    assert(ogr().GetFieldDefnRef(i) && "No definition associated to the i-th field");
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

otb::ogr::Field otb::ogr::Feature::UncheckedGetElement(size_t index)
{
  Field field(*this, index);
  return field;
}

otb::ogr::Field otb::ogr::Feature::UncheckedGetElement(std::string const& name)
{
  const int index = GetFieldIndex(name);
  return this->operator[](index);
}

otb::ogr::FieldDefn otb::ogr::Feature::UncheckedGetFieldDefn(size_t index) const
{
  return FieldDefn(*m_Feature->GetFieldDefnRef(index));
}

otb::ogr::FieldDefn otb::ogr::Feature::UncheckedGetFieldDefn(std::string const& name) const
{
  const int index = GetFieldIndex(name);
  if (index < 0)
    {
    itkGenericExceptionMacro(<<"no field named <"<<name<<">");
    }
  return this->GetFieldDefn(index);
}

int otb::ogr::Feature::UncheckedGetFieldIndex(std::string const& name) const
{
  const int index = m_Feature->GetFieldIndex(name.c_str());
  if (index < 0)
    {
    itkGenericExceptionMacro(<<"No field named <"<<name<<"> in feature");
    }
  return index;
}

/*===========================================================================*/
/*==============================[ Properties ]===============================*/
/*===========================================================================*/
long otb::ogr::Feature::UncheckedGetFID() const
{
  return m_Feature->GetFID();
}

void otb::ogr::Feature::UncheckedSetFID(long fid)
{
  const OGRErr res = m_Feature->SetFID(fid);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot Set FID to "<<fid<<" for feature: " << CPLGetLastErrorMsg());
    }
}

OGRFeatureDefn&  otb::ogr::Feature::UncheckedGetDefn() const
{
  return *m_Feature->GetDefnRef();
}

/*===========================================================================*/
/*==============================[ Geometries ]===============================*/
/*===========================================================================*/

void otb::ogr::Feature::UncheckedSetGeometryDirectly(UniqueGeometryPtr geometry)
{
  OGRGeometry * g = geometry.get();
  const OGRErr res = m_Feature->SetGeometryDirectly(g);
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot set (directly) the geometry: " << CPLGetLastErrorMsg());
    }
  geometry.release(); // success => commit the transaction (after any exception thrown)
}

otb::ogr::UniqueGeometryPtr otb::ogr::Feature::UncheckedStealGeometry()
{
  OGRGeometry * g = m_Feature->StealGeometry();
  return UniqueGeometryPtr(g);
}

void otb::ogr::Feature::UncheckedSetGeometry(OGRGeometry const* geometry)
{
  // OGR copies the input geometry => should have been const
  const OGRErr res = m_Feature->SetGeometry(const_cast <OGRGeometry*>(geometry));
  if (res != OGRERR_NONE)
    {
    itkGenericExceptionMacro(<<"Cannot set the geometry: " << CPLGetLastErrorMsg());
    }
}

OGRGeometry const* otb::ogr::Feature::UncheckedGetGeometry() const
{
  return m_Feature->GetGeometryRef();
}

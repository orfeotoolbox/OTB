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
    Field field(*this, index);
    return field;
}

otb::ogr::Field const otb::ogr::Feature::operator[](size_t index) const
{
  return const_cast<Feature*>(this)->operator[](index);
}

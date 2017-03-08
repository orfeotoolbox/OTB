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


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "otbOGRGeometryWrapper.h"
#include "ogr_geometry.h"

#if GDAL_VERSION_NUM < 1900
#  include "itkMacro.h"
#endif

#ifdef _MSC_VER
// warning conversion int -> bool
#pragma warning ( disable : 4800 )
#endif
/*===========================================================================*/
/*================================[ Deleter ]================================*/
/*===========================================================================*/
void otb::ogr::internal::GeometryDeleter::operator()(OGRGeometry* p)
{
  if (p) { // OGR refuses delete 0...
    OGRGeometryFactory::destroyGeometry (p);
  }
}

/*===========================================================================*/
/*======================[ Double dispatched functions ]======================*/
/*===========================================================================*/
bool otb::ogr::Intersects(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  // OGRGeometry::Intersects is not const-correct ...
  return lhs.Intersects(const_cast <OGRGeometry*>(&rhs));
}

bool otb::ogr::Equals(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  // OGRGeometry::Equals is not const-correct ...
  return lhs.Equals(const_cast <OGRGeometry*>(&rhs));
}

bool otb::ogr::Disjoint(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Disjoint(&rhs);
}

bool otb::ogr::Touches(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Touches(&rhs);
}

bool otb::ogr::Crosses(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Crosses(&rhs);
}

bool otb::ogr::Within(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Within(&rhs);
}

bool otb::ogr::Contains(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Contains(&rhs);
}

bool otb::ogr::Overlaps(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Overlaps(&rhs);
}

double otb::ogr::Distance(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return lhs.Distance(&rhs);
}

otb::ogr::UniqueGeometryPtr otb::ogr::Intersection(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return UniqueGeometryPtr(lhs.Intersection(&rhs));
}

otb::ogr::UniqueGeometryPtr otb::ogr::Union(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return UniqueGeometryPtr(lhs.Union(&rhs));
}

otb::ogr::UniqueGeometryPtr otb::ogr::UnionCascaded(OGRGeometry const& this_)
{
#if GDAL_VERSION_NUM >= 1800
  return UniqueGeometryPtr(this_.UnionCascaded());
#else
  itkGenericExceptionMacro("OGRGeometry::UnionCascaded is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.8.0, and recompile OTB.");
#endif
}

otb::ogr::UniqueGeometryPtr otb::ogr::Difference(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
  return UniqueGeometryPtr(lhs.Difference(&rhs));
}

otb::ogr::UniqueGeometryPtr otb::ogr::SymDifference(OGRGeometry const& lhs, OGRGeometry const& rhs)
{
#if GDAL_VERSION_NUM >= 1800
  return UniqueGeometryPtr(lhs.SymDifference(&rhs));
#else
  return UniqueGeometryPtr(lhs.SymmetricDifference(&rhs));
#endif
}

otb::ogr::UniqueGeometryPtr otb::ogr::SimplifyDontPreserveTopology(OGRGeometry const& g, double tolerance)
{
#if GDAL_VERSION_NUM >= 1800
  return UniqueGeometryPtr(g.Simplify(tolerance));
#else
  itkGenericExceptionMacro("OGRGeometry::Simplify is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.8.0, and recompile OTB.");
#endif
}

otb::ogr::UniqueGeometryPtr otb::ogr::SimplifyPreserveTopology(OGRGeometry const& g, double tolerance)
{
#if GDAL_VERSION_NUM >= 1900
  return UniqueGeometryPtr(g.SimplifyPreserveTopology(tolerance));
#else
  itkGenericExceptionMacro("OGRGeometry::Simplify is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.");
#endif
}

otb::ogr::UniqueGeometryPtr otb::ogr::Simplify(OGRGeometry const& g, double tolerance)
{
#if   GDAL_VERSION_NUM >= 1900
  return UniqueGeometryPtr(g.SimplifyPreserveTopology(tolerance));
#elif GDAL_VERSION_NUM >= 1800
  return UniqueGeometryPtr(g.Simplify(tolerance));
#else
  itkGenericExceptionMacro("OGRGeometry::Simplify(PreserveTopology) is not supported by OGR v"
    << GDAL_VERSION_NUM << ". Upgrade to a version >= 1.9.0, and recompile OTB.");
#endif
}

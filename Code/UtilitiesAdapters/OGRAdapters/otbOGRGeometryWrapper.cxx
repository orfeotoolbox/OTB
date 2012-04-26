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
#include "otbOGRGeometryWrapper.h"
#include "ogr_geometry.h"

#ifdef _MSC_VER
// warning convertion int -> bool
#pragma warning ( disable : 4800 )
#endif
/*===========================================================================*/
/*================================[ Deleter ]================================*/
/*===========================================================================*/
void otb::ogr::internal::GeometryDeleter::operator()(OGRGeometry* p)
{
  OGRGeometryFactory::destroyGeometry (p);
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
  return UniqueGeometryPtr(this_.UnionCascaded());
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

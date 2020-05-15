/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include <cassert>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "gdal_priv.h" // GDALDataset
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#include "gdal_priv.h" // GDALDataset
#pragma warning(pop)
#else
#include "gdal_priv.h" // GDALDataset
#endif


#include "otbOGRDataSourceWrapper.h"

/*===========================================================================*/
/*======================[ Construction & Destruction ]=======================*/
/*===========================================================================*/
namespace
{ // Anonymous namespace
  /**\ingroup GeometryInternals
   * Deleter for \c boost::shared_ptr<> that doesn't delete.
   * This is required for \c OGRLayer s that belong to \c OGRDataSource.
   * \internal Unlike OGR, works as a no-op on null geometries.
   */
struct LeaveAloneDeleter
{
  void operator()(OGRLayer*) const
  {
  }
};
} // Anonymous namespace


otb::ogr::Layer::Layer(OGRLayer* layer, bool modifiable) : m_Layer(layer, LeaveAloneDeleter()), m_Modifiable(modifiable)
#if 0
  , m_DataSource(datasource)
#endif
{
}

otb::ogr::Layer::Layer(OGRLayer* layer, GDALDataset& sourceInChargeOfLifeTime, bool modifiable)
  : m_Layer(layer, [&](auto const& x) { return sourceInChargeOfLifeTime.ReleaseResultSet(x); }), m_Modifiable(modifiable)
{
  assert(layer && "A null OGRlayer cannot belong to an OGRDataSource");
  // OGR always refuses "delete 0". *sigh*
}

/*===========================================================================*/
/*===============================[ Features ]================================*/
/*===========================================================================*/
int otb::ogr::Layer::GetFeatureCount(bool doForceComputation) const
{
  assert(m_Layer && "Can't ask the features count on an invalid layer");
  return m_Layer->GetFeatureCount(doForceComputation);
}

otb::ogr::Feature otb::ogr::Layer::GetNextFeature()
{
  assert(m_Layer && "OGRLayer not initialized");
  OGRFeature* f = m_Layer->GetNextFeature();
  return f;
}

otb::ogr::Layer::iterator otb::ogr::Layer::begin()
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->ResetReading();
  return iterator(*this);
}

otb::ogr::Layer::const_iterator otb::ogr::Layer::cbegin() const
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->ResetReading();
  return const_iterator(*const_cast<Layer*>(this));
}

otb::ogr::Layer::iterator otb::ogr::Layer::start_at(GIntBig index)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetNextByIndex(index);
  return iterator(*this);
}

otb::ogr::Layer::const_iterator otb::ogr::Layer::cstart_at(GIntBig index) const
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetNextByIndex(index);
  return const_iterator(*const_cast<Layer*>(this));
}

void otb::ogr::Layer::CreateFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot create a new feature in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->CreateFeature(&feature.ogr());
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot create a new feature in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::DeleteFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot create a new feature in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->DeleteFeature(nFID);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot delete the feature <" << nFID << "> in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

otb::ogr::Feature otb::ogr::Layer::GetFeature(long nFID)
{
  assert(m_Layer && "OGRLayer not initialized");
  if (nFID == OGRNullFID)
  {
    itkGenericExceptionMacro(<< "Invalid feature null id GetFeature() in the layer <" << GetName() << ">.");
  }
  const Feature feat = m_Layer->GetFeature(nFID);
  return feat;
}

void otb::ogr::Layer::SetFeature(Feature feature)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot create a new feature in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->SetFeature(&feature.ogr());
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot update a feature in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

/*===========================================================================*/
/*=================================[ Misc ]==================================*/
/*===========================================================================*/
std::string otb::ogr::Layer::GetName() const
{
  assert(m_Layer && "null layer");
  return m_Layer->GetName();
}

OGREnvelope otb::ogr::Layer::GetExtent(bool force /* = false */) const
{
  assert(m_Layer && "OGRLayer not initialized");
  OGREnvelope  sExtent;
  const OGRErr res = m_Layer->GetExtent(&sExtent, force);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot retrieve extent of layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
  return sExtent;
}

void otb::ogr::Layer::GetExtent(double& ulx, double& uly, double& lrx, double& lry, bool force) const
{
  const OGREnvelope sExtent = GetExtent(force);
  ulx                       = sExtent.MinX;
  uly                       = sExtent.MinY;
  lrx                       = sExtent.MaxX;
  lry                       = sExtent.MaxY;
}

OGRLayer& otb::ogr::Layer::ogr()
{
  assert(m_Layer && "OGRLayer not initialized");
  return *m_Layer;
}

void otb::ogr::Layer::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "+";
  if (m_Layer) // in case for odd reason the layer that should exist can't be found
  {
    os << "Layer <" << GetName() << "> of " << OGRGeometryTypeToName(GetGeomType()) << "\n";
    indent = indent.GetNextIndent();
    BOOST_FOREACH (Feature f, *this)
    {
      f.PrintSelf(os, indent);
    }
  }
  else
  {
    os << "null Layer\n";
  }
}

/*===========================================================================*/
/*============================[ Spatial Filter ]=============================*/
/*===========================================================================*/

OGRGeometry const* otb::ogr::Layer::GetSpatialFilter() const
{
  assert(m_Layer && "OGRLayer not initialized");
  OGRGeometry* spatialFilter = m_Layer->GetSpatialFilter();
  return spatialFilter;
}

void otb::ogr::Layer::SetSpatialFilter(OGRGeometry const* spatialFilter)
{
  assert(m_Layer && "OGRLayer not initialized");
  // const_cast because OGR is not 100% const-correct
  m_Layer->SetSpatialFilter(const_cast<OGRGeometry*>(spatialFilter));
}

void otb::ogr::Layer::SetSpatialFilterRect(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY)
{
  assert(m_Layer && "OGRLayer not initialized");
  m_Layer->SetSpatialFilterRect(dfMinX, dfMinY, dfMaxX, dfMaxY);
}

OGRSpatialReference const* otb::ogr::Layer::GetSpatialRef() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return m_Layer->GetSpatialRef();
}

std::string otb::ogr::Layer::GetProjectionRef() const
{
  assert(m_Layer && "OGRLayer not initialized");
  char*                      wkt = nullptr;
  OGRSpatialReference const* srs = GetSpatialRef();
  if (srs)
  {
    const OGRErr res = srs->exportToWkt(&wkt);

    if (res != OGRERR_NONE)
    {
      itkGenericExceptionMacro(<< "Cannot convert spatial reference to wkt string for layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
    }

    assert(wkt);
    const std::string stringWkt(wkt);
    // According to documentation, argument of exportToWkt() should be freed
    CPLFree(wkt);
    return stringWkt;
  }

  return "";
}

/*===========================================================================*/
/*==========================[ Feature Definition ]===========================*/
/*===========================================================================*/
OGRFeatureDefn& otb::ogr::Layer::GetLayerDefn() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return *const_cast<OGRLayer*>(m_Layer.get())->GetLayerDefn();
}

void otb::ogr::Layer::CreateField(FieldDefn const& field, bool bApproxOK /* = true */)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot create a new field in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->CreateField(&field.ogr(), bApproxOK);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot create a field in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::DeleteField(int fieldIndex)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot delete field in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->DeleteField(fieldIndex);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot delete the " << fieldIndex << "th field in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::AlterFieldDefn(size_t fieldIndex, FieldDefn const& newFieldDefn, int nFlags)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot alter field definition in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->AlterFieldDefn(int(fieldIndex), &newFieldDefn.ogr(), nFlags);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot alter the " << fieldIndex << "th field in the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::ReorderField(size_t oldPos, size_t newPos)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot reorder fields in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->ReorderField(int(oldPos), int(newPos));
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot move the " << oldPos << "th field to the " << newPos << "th position in the layer <" << GetName()
                             << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::ReorderFields(int* map)
{
  assert(m_Layer && "OGRLayer not initialized");

  if (!m_Modifiable)
  {
    itkGenericExceptionMacro(<< "Cannot reorder fields in the layer <" << GetName() << ">: layer is not modifiable");
  }

  const OGRErr res = m_Layer->ReorderFields(map);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot reorder the fields of the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

void otb::ogr::Layer::SetIgnoredFields(char const** fieldNames)
{
  assert(m_Layer && "OGRLayer not initialized");
  const OGRErr res = m_Layer->SetIgnoredFields(fieldNames);
  if (res != OGRERR_NONE)
  {
    itkGenericExceptionMacro(<< "Cannot set fields to ignore on the layer <" << GetName() << ">: " << CPLGetLastErrorMsg());
  }
}

OGRwkbGeometryType otb::ogr::Layer::GetGeomType() const
{
  assert(m_Layer && "OGRLayer not initialized");
  return m_Layer->GetGeomType();
}
